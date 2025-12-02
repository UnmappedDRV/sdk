#include <udrv/env.h>
#include <udrv/device.h>
#include <udrv/bus.h>
#include <udrv/loader.h>

static udrv_env_t *env;
static list_t devices;
static list_t device_typedefs;
static list_t drivers;

#define FUNC_CHECK(func,val) if (!env) return val;\
	if (!env->func) {\
		udrv_log(UDRV_LOG_WARNING, "non implemented mandatory function '" #func "'");\
		return val;\
	}

int udrv_init_device(udrv_bus_addr_t *addr) {
	if (addr->device) {
		// a driver already control this address
		return UDRV_ERR_BUSY;
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
		device->def = device_typedef;
		udrv_register_device(device);
		break;
	}

	return ret;
}

int udrv_destroy_device(udrv_device_t *device){
	if (device->type == UDRV_TYPE_BUS) {
		// TODO : free addresses
	}

	if (device->def->destroy) {
		device->def->destroy(device);
	}

	// release the bus addr
	if (device->addr) {
		device->addr->device = NULL;
	}

	// remove from the devices list
	udrv_list_remove(&devices, device);

	udrv_free(device);
	return 0;
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

int udrv_register_device_typedef(udrv_device_typedef_t *device_typedef) {
	udrv_list_append(&device_typedefs, device_typedef);
	// TODO : try to init already present bus addr with this new driver
	return 0;
}

int udrv_unregister_device_typedef(udrv_device_typedef_t *device_typedef) {
	udrv_list_remove(&device_typedefs, device_typedef);
	return 0;
}

int udrv_hotplug_addr(udrv_bus_addr_t *addr) {
	udrv_list_append(&addr->bus->addresses, addr);
	udrv_init_device(addr);
	return 0;
}

int udrv_hotunplug_addr(udrv_bus_addr_t *addr) {
	if (addr->device) {
		int ret = udrv_destroy_device(addr->device);
		if (ret < 0) return ret;
	}
	udrv_list_remove(&addr->bus->addresses, addr);
	return 0;
}

void udrv_log(int level, const char *fmt, ...) {
	if (!env || !env->log) return;
	va_list args;
	va_start(args, fmt);
	env->log(level, fmt, args);
	va_end(args);
}

void *udrv_malloc(size_t size) {
	FUNC_CHECK(malloc, NULL);
	return env->malloc(size);
}

void udrv_free(void *ptr) {
	FUNC_CHECK(free,);
	return env->free(ptr);
}

int udrv_init_env(udrv_env_t *_env) {
	env = _env;
	return 0;
}

int udrv_load_driver(void *data, size_t size, udrv_driver_t **driver, int argc, const char **argv) {
	int ret = udrv_load_module(data, size, driver);
	if (ret < 0) return ret;

	if ((*driver)->init) {
		ret = (*driver)->init(argc, argv);
		if (ret < 0) {
			// TODO : free allocated sections
			return ret;
		}
	}

	udrv_list_append(&drivers, driver);
	return 0;
}

int udrv_unload_driver(udrv_driver_t *driver) {
	// check if the driver is in use
	foreach (udrv_device_t*, device, devices) {
		if (device->def->driver == driver) {
			return UDRV_ERR_BUSY;
		}
	}
	if (driver->fini) {
		driver->fini();
	}
	udrv_list_remove(&drivers, driver);
	// TODO : free the driver
	return 0;
}
