#ifndef _UDRV_DEVICE_H
#define _UDRV_DEVICE_H

#include "types.h"
#include "list.h"

struct udrv_driver;
struct udrv_device;
struct udrv_bus_addr;

typedef int (*udrv_init_device_t)(struct udrv_bus_addr *addr, struct udrv_device **);
typedef void (*udrv_destroy_device_t)(struct udrv_device *device);
typedef int (*udrv_check_addr_t)(struct udrv_bus_addr *addr);
typedef int (*udrv_ioctl_t)(struct udrv_device *device, long request, void *arg);
typedef udrv_ssize_t (*udrv_read_t)(struct udrv_device *device, void *buffer, size_t count);
typedef udrv_ssize_t (*udrv_write_t)(struct udrv_device *device, const void *buffer, size_t count);

typedef struct udrv_device_typedef {
	list_node_t list_node;
	struct udrv_driver *driver;
	udrv_init_device_t    init;
	udrv_destroy_device_t destroy;
	udrv_check_addr_t check;
	udrv_ioctl_t      ioctl;
	udrv_read_t       read;
	udrv_write_t      write;
} udrv_device_typedef_t;

typedef struct udrv_device {
	list_node_t list_node;
	struct udrv_bus_addr       *addr;
	struct udrv_device_typedef *def;
	int type;
	int class;
} udrv_device_t;

#define UDRV_TYPE_CHAR  1
#define UDRV_TYPE_BLOCK 2
#define UDRV_TYPE_BUS   3

#define UDRV_CLASS_BUS      1
#define UDRV_CLASS_DISK     2
#define UDRV_CLASS_KEYBOARD 3
#define UDRV_CLASS_MOUSE    4
#define UDRV_CLASS_GAMEPAD  5
#define UDRV_CLASS_TOUCH    6
#define UDRV_CLASS_GPU      7

int udrv_register_device(udrv_device_t *device);
int udrv_destroy_device(udrv_device_t *device);
int udrv_register_device_typedef(udrv_device_typedef_t *device_typedef);
int udrv_unregister_device_typedef(udrv_device_typedef_t *device_typedef);

#endif
