#ifndef _UDRV_DRIVER_H
#define _UDRV_DRIVER_H

#include "types.h"
#include "errno.h"

struct udrv_env;

typedef struct udrv_driver {
	const char *name;
	const char *author;
	const char *lisence;
	udrv_version_t version;
	udrv_version_t udrv_version;
	int (*init)(int argc, const char **argv);
	void (*fini)(void);
} udrv_driver_t;

#define UDRV_LOG_DEBUG   0
#define UDRV_LOG_INFO    1
#define UDRV_LOG_WARNING 2
#define UDRV_LOG_ERROR   3
#define UDRV_LOG_FATAL   4

void udrv_set_env(struct udrv_env *env);
void udrv_log(int level, const char *fmt, ...);
void *udrv_malloc(size_t *size);
void udrv_free(void *ptr);
int udrv_register_device(udrv_device_t *device);
int udrv_unregister_device(udrv_device_t *device);
int udrv_register_device_typedef(udrv_device_typedef_t *device_typedef);
int udrv_unregister_device_typedef(udrv_device_typedef_t *device_typedef);

#endif
