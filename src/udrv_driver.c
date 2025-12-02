#include <udrv/driver.h>
#include <udrv/device.h>
#include <udrv/bus.h>
#include <udrv/env.h>

extern udrv_driver_t udrv_meta;

static udrv_env_t *env = NULL;

#define FUNC_CHECK(func,val) if (!env) return val;\
	if (!env->func) {\
		udrv_log(UDRV_LOG_WARNING, "non implemented mandatory function '" #func "'");\
		return val;\
	}

void udrv_set_env(udrv_env_t *_env) {
	 env = _env;
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

int udrv_register_device(udrv_device_t *device) {
	FUNC_CHECK(register_device, UDRV_ERR_NOSYS);
	return env->register_device(device);
}

int udrv_destroy_device(udrv_device_t *device) {
	FUNC_CHECK(destroy_device, UDRV_ERR_NOSYS);
	return env->destroy_device(device);
}

int udrv_register_device_typedef(udrv_device_typedef_t *device_typedef) {
	FUNC_CHECK(register_device_typedef, UDRV_ERR_NOSYS);
	return env->register_device_typedef(device_typedef);
}

int udrv_unregister_device_typedef(udrv_device_typedef_t *device_typedef) {
	FUNC_CHECK(unregister_device_typedef, UDRV_ERR_NOSYS);
	return env->unregister_device_typedef(device_typedef);
}

int udrv_hotplug_addr(udrv_bus_addr_t *addr) {
	FUNC_CHECK(hotplug_addr, UDRV_ERR_NOSYS);
	return env->hotplug_addr(addr);
}

int udrv_hotunplug_addr(udrv_bus_addr_t *addr) {
	FUNC_CHECK(hotunplug_addr, UDRV_ERR_NOSYS);
	return env->hotunplug_addr(addr);
}

int udrv_entry(void *env, int argc, const char **argv){
	udrv_set_env(env);
	if (udrv_meta.init) {
		return udrv_meta.init(argc, argv);
	} else {
		return 0;
	}
}
