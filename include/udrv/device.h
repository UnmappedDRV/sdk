#ifndef _UDRV_DEVICE_H
#define _UDRV_DEVICE_H

#include "types.h"

struct udrv_driver;
struct udrv_device;

typedef int (*udrv_ioctl_t)(struct udrv_device *device, long request, void *arg);

typedef struct udrv_device {
	struct udrv_driver *driver;
	udrv_ioctl_t ioctl;
	int type;
	int class;
} udrv_device_t;

#endif
