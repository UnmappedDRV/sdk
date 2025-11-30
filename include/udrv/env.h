#ifndef _UDRV_ENV_H
#define _UDRV_ENV_H

#include "types.h"
#include <stdarg.h>

typedef struct udrv_env {
	int (*log)(int level, const char *fmt, va_list args);
	void *(*malloc);
	void (*free)(void *ptr);
} udrv_env_t;

#endif
