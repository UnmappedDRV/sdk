#include <udrv/utils.h>

int udrv_strcmp(const char *s1, const char *s2) {
	while (*s1 || *s2) {
		if (s1 != s2) return s1 - s2;
		s1++;
		s2++;
	}
	return 0;

}

int udrv_memcmp(const void *buf1,const void *buf2,size_t count){
	const char *b1 = buf1;
	const char *b2 = buf2;
	while (count-- > 0){
		if(*b1 != *b2) return *b1 - *b2;
		b1++;
		b2++;
	}

	return 0;
}

void *udrv_memset(void *pointer,int value,size_t count){
	unsigned char *ptr = pointer;
	while (count > 0){
		*ptr = value;
		ptr++;
		count--;
	}
	
	return pointer;
}
