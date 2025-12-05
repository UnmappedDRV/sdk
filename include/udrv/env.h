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
	void (*out_byte)(udrv_port_t port, uint8_t data);
	uint8_t (*in_byte)(udrv_port_t port);
	void (*out_word)(udrv_port_t port, uint16_t data);
	uint16_t (*in_word)(udrv_port_t port);
	void (*out_dword)(udrv_port_t port, uint32_t data);
	uint32_t (*in_dword)(udrv_port_t port);
} udrv_env_t;

int udrv_init_env(udrv_env_t *_env);
int udrv_load_driver(void *data, size_t size, udrv_driver_t **driver, int argc, const char **argv) ;

#endif
