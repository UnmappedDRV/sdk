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
		return UDRV_ERR_FORMAT;
	}

	// TODO : apply relocations

	return 0;
}
