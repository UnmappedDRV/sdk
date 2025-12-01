#include <udrv/env.h>
#include <udrv/device.h>
#include <udrv/bus.h>

static udrv_env_t *env;

int udrv_init_env(udrv_env_t *_env) {
	env = _env;
}

int udrv_load_module(const char *path){
}
