#include <udrv/loader.h>
#include <udrv/env.h>
#include <udrv/driver.h>
#include <udrv/device.h>
#include <udrv/bus.h>
#include <udrv/utils.h>
#include <elf.h>

#ifdef BITS32
#define Elf_Ehdr Elf32_Ehdr
#define Elf_Shdr Elf32_Shdr
#define Elf_Addr Elf32_Addr
#define Elf_Sym  Elf32_Sym
#define Elf_Rela Elf32_Rela
#define ELF_R_SYM(i)  ELF32_R_SYM(i)
#define ELF_R_TYPE(i) ELF32_R_TYPE(i)
#define ELF_R_INFO(i) ELF32_R_INFO(i)
#define ELFCLASS ELFCLASS32
#else
#define Elf_Ehdr Elf64_Ehdr
#define Elf_Shdr Elf64_Shdr
#define Elf_Addr Elf64_Addr
#define Elf_Sym  Elf64_Sym
#define Elf_Rela Elf64_Rela
#define ELF_R_SYM(i)  ELF64_R_SYM(i)
#define ELF_R_TYPE(i) ELF64_R_TYPE(i)
#define ELF_R_INFO(i) ELF64_R_INFO(i)
#define ELFCLASS ELFCLASS64
#endif

#define get_Shdr(index) \
((Elf_Shdr *)((uintptr_t)data + header->e_shoff + (index * header->e_shentsize)))

typedef struct udrv_mapping {
	const char *name;
	void *ptr;
} udrv_mapping_t;

#define FUNC(name) {#name, name}

static udrv_mapping_t mappings[] = {
	FUNC(udrv_log),
	FUNC(udrv_malloc),
	FUNC(udrv_free),
	FUNC(udrv_register_device),
	FUNC(udrv_destroy_device),
	FUNC(udrv_register_device_typedef),
	FUNC(udrv_unregister_device_typedef),
	FUNC(udrv_hotplug_addr),
	FUNC(udrv_hotunplug_addr),
	FUNC(udrv_strcmp),
	FUNC(udrv_memcmp),
	FUNC(udrv_memset),

	// convinent mappings
	{"malloc", udrv_malloc},
	{"free"  , udrv_free},
	{"strcmp", udrv_strcmp},
	{"memcmp", udrv_memcmp},
	{"memset", udrv_memset},
};

static int check_header(Elf_Ehdr *header){
	if(udrv_memcmp(header->e_ident,ELFMAG,4)){
		return 0;
	}

	if(header->e_ident[EI_VERSION] != EV_CURRENT){
		return 0;
	}

	if(header->e_ident[EI_DATA] != ELFDATA2LSB){
		return 0;
	}
	
	if(header->e_ident[EI_CLASS] != ELFCLASS){
		return 0;
	}

	if(header->e_type != ET_REL){
		return 0;
	}

	return 1;
}

static uintptr_t sym_lookup(const char *name) {
	for (size_t i=0; i<sizeof(mappings)/sizeof(*mappings); i++) {
		if (!udrv_strcmp(name, mappings[i].name)) {
			return (uintptr_t)mappings[i].ptr;
		}
	}
	return 0;
}

static int apply_rela(Elf_Rela *rela, Elf_Shdr *section, Elf_Sym *symtab) {
	// let define some macros
	#define A rela[i].r_addend
	#define B 0 // TODO
	#define P (section->sh_addr + rela->r_offset)
	#define S symtab[ELF_R_SYM(rela->r_info)].st_value
	#define Z symtab[ELF_R_SYM(rela->r_info)].st_size

	uint32_t w32;
#ifndef BITS32
	uint64_t w64;
#endif

	switch(ELF_R_TYPE(rela->r_info)){
#ifdef __x86_64__
	case R_X86_64_NONE:
		break;
	case R_X86_64_64:
		w64 = S + A;
		memcpy((void *)P,&w64,sizeof(uint64_t));
		break;
	case R_X86_64_PC32:
		w32 = S + A - P;
		memcpy((void *)P,&w32,sizeof(uint32_t));
		break;
	case R_X86_64_32:
		w32 = S + A;
		memcpy((void *)P,&w32,sizeof(uint32_t));
		break;
#else
	(void)w32;
#ifndef BITS32
	(void)w64;
#endif
	(void)section;
	(void)symtab;
#warning "unsupported architecture"
#endif
	default :
		udrv_log(UDRV_LOG_WARNING, "unknow relocation type %d\n", ELF_R_TYPE(rela->r_info));
		return -1;
	}
	return 0;
}

int udrv_load_module(void *data, size_t size, udrv_driver_t **driver) {
	if (size < sizeof(Elf_Ehdr)) {
		// too small
		return UDRV_ERR_FORMAT;
	}
	Elf_Ehdr *header = data;
	if (!check_header(header)) {
		return UDRV_ERR_FORMAT;
	}

	if (size < header->e_shoff + header->e_shentsize * header->e_shnum) {
		// too small
		return UDRV_ERR_FORMAT;
	}

	// relocate sections
	for (size_t i=0; i<header->e_shnum; i++) {
		Elf_Shdr *sheader = get_Shdr(i);
		if (sheader->sh_type == SHT_NOBITS && sheader->sh_flags & SHF_ALLOC) {
			//TODO alloc section
		} else {
			sheader->sh_addr = (Elf_Addr)data + sheader->sh_offset;
		}
	}

	*driver = NULL;

	// relocate symbols
	for (size_t i=0; i<header->e_shnum; i++) {
		Elf_Shdr *sheader = get_Shdr(i);
		if (sheader->sh_type != SHT_SYMTAB) continue;

		// get the symbols table and the string table
		Elf_Sym *symtab = (Elf_Sym *)sheader->sh_addr;
		char *strtab = (char *)get_Shdr(sheader->sh_link)->sh_addr;

		// iterate trought each symbol
		// note that we skip the first symbol ( the NULL symbol)
		for (size_t i = 1; i < sheader->sh_size / sizeof(Elf_Sym); i++) {
			if (symtab[i].st_shndx > SHN_UNDEF && symtab[i].st_shndx < SHN_LOPROC) {
				// symbol to relocate
				symtab[i].st_value += get_Shdr(symtab[i].st_shndx)->sh_addr;
			} else if (symtab[i].st_shndx == SHN_UNDEF) {
				// symbols undefined (maybee we can link it)
				uintptr_t value = sym_lookup(strtab + symtab[i].st_name);
				if(value){
					symtab[i].st_value += value;
				} else {
					// unknow sym
					return UDRV_ERR_FORMAT;
				}
			}

			if (udrv_strcmp(strtab + symtab[i].st_name, "udrv_meta")) {
				*driver = (udrv_driver_t *)symtab[i].st_value;
			}
		}
		
	}

	if (!*driver) {
error:
		// TODO : unmap mapped sections
		return UDRV_ERR_FORMAT;
	}

	// apply relocations
	for(size_t i=0; i<header->e_shnum; i++){
		Elf_Shdr *sheader = get_Shdr(i);
		if(sheader->sh_type != SHT_RELA) continue;

		// find the relocation table and the symbol table and the section it apply to
		Elf_Rela *rela = (Elf_Rela *)sheader->sh_addr;
		Elf_Sym *symtab = (Elf_Sym *)get_Shdr(sheader->sh_link)->sh_addr;
		Elf_Shdr *section = get_Shdr(sheader->sh_info);

		// iterate trought each relocation
		for (size_t i=0; i < sheader->sh_size / sizeof(Elf_Rela); i++){
			if (apply_rela(&rela[i], section, symtab) < 0) goto error;
		}
	}
		

	return 0;
}
