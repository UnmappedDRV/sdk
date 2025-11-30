#include <udrv/driver.h>
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
	va_start(fmt, args);
	env->log(level, fmt, args);
	va_end(args);
}

void *udrv_malloc(size_t *size) {
	FUNC_CHECK(malloc, NULL);
	return env->malloc(size);
}

void udrv_free(void *ptr) {
	FUNC_CHECK(malloc);
	return env->free(ptr);
}

int udrv_entry(void *env, int argc, const char **argv){
	udrv_set_env(env);
	if (meta->init) {
		meta->init(argc, argv);
	} else {
		return 0;
	}
}
