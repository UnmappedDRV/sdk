#include <udrv/env.h>
#include <udrv/device.h>
#include <udrv/bus.h>

static udrv_env_t *env;
static list_t devices;
static list_t device_typedefs;

int udrv_init_device(udrv_bus_addr_t *addr) {
	if (addr->device) {
		// a driver already control this address
		// TODO: proper error code
		return -1;
	}

	int ret = -1;

	foreach (udrv_device_typedef_t*, device_typedef, device_typedefs) {
		if (!device_typedef->check) continue;
		if (!device_typedef->check(addr)) continue;
		// we found a driver typedef compatible with this address
		udrv_device_t *device;
		ret = device_typedef->init_device(addr, &device);
		if (ret < 0) {
			// the init failed retry with another driver
			continue;
		}

		// we succed
		addr->device = device;
		break;
	}

	return ret;
}

int udrv_register_device(udrv_device_t *device) {
	udrv_list_append(&devices, device);
	if (device->type == UDRV_TYPE_BUS) {
	}
}

int udrv_unregister_device(udrv_device_t *device) {
	udrv_list_remove(&devices, device);
}

int udrv_init_env(udrv_env_t *_env) {
	env = _env;
}

int udrv_load_module(const char *path){
}
