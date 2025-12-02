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
	int (*register_device)(struct udrv_device *device);
	int (*destroy_device)(struct udrv_device *device);
	int (*register_device_typedef)(struct udrv_device_typedef *device_typedef);
	int (*unregister_device_typedef)(struct udrv_device_typedef *device_typedef);
	int (*hotplug_addr)(struct udrv_bus_addr *addr);
	int (*hotunplug_addr)(struct udrv_bus_addr *addr);
} udrv_env_t;

#endif
