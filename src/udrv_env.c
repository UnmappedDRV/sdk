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
		ret = device_typedef->init(addr, &device);
		if (ret < 0) {
			// the init failed retry with another driver
			continue;
		}

		// we succed
		addr->device = device;
		udrv_register_device(device);
		break;
	}

	return ret;
}

int udrv_register_device(udrv_device_t *device) {
	udrv_list_append(&devices, device);
	if (device->type == UDRV_TYPE_BUS) {
		// let try to find driver for devices
		udrv_bus_t *bus = (udrv_bus_t*)device;
		foreach (udrv_bus_addr_t*, addr, bus->addresses) {
			udrv_init_device(addr);
		}
	}
	return 0;
}

int udrv_unregister_device(udrv_device_t *device) {
	if (device->type == UDRV_TYPE_BUS) {
		// TODO : delete all devices on the bus
	}
	udrv_list_remove(&devices, device);
	return 0;
}

int udrv_register_device_typedef(udrv_device_typedef_t *device_typedef) {
	udrv_list_append(&device_typedefs, device_typedef);
	// TODO : try to init already present bus addr with this new driver
	return 0;
}

int udrv_unregister_device_typedef(udrv_device_typedef_t *device_typedef) {
	udrv_list_remove(&device_typedefs, device_typedef);
	return 0;
}

int udrv_init_env(udrv_env_t *_env) {
	env = _env;
	env->register_device   = udrv_register_device;
	env->unregister_device = udrv_register_device;
	env->register_device_typedef     = udrv_register_device_typedef;
	env->unregister_device_typedef   = udrv_register_device_typedef;
	return 0;
}

int udrv_load_module(void *data, size_t size){
	(void)data;
	(void)size;
	// TODO
	return UDRV_ERR_NOSYS;
}
