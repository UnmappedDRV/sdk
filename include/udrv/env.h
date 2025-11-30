#ifndef _UDRV_ENV_H
#define _UDRV_ENV_H

#include "types.h"

typedef struct udrv_env {
	int (*log)(int level, const char *fmt, ...);
	void *(*malloc);
	void (*free)(void *ptr);
} udrv_env_t;

#endif
