#ifndef _UDRV_ENV_H
#define _UDRV_ENV_H

#include "types.h"
#include <stdarg.h>

struct udrv_bus_addr;
struct udrv_device;
struct udrv_device_typedef;

typedef struct udrv_env {
	int (*log)(int level, const char *fmt, va_list args);
	void *(*malloc)(size_t size);
	void (*free)(void *ptr);
} udrv_env_t;

#endif
