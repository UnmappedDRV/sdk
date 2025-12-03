#ifndef _UDRV_UTILS_H
#define _UDRV_UTILS_H

#include "types.h"

int udrv_strcmp(const char *s1, const char *s2);
int udrv_memcmp(const void *buf1,const void *buf2,size_t count);
void *udrv_memset(void *buf,int value,size_t count);

#endif
