#ifndef _UDRV_INTERNAL_H
#define _UDRV_INTERNAL_H

#include "driver.h"
#include "env.h"

extern udrv_env_t *udrv_env;

#define FUNC_CHECK(func,val) if (!udrv_env) return val;\
	if (!udrv_env->func) {\
		udrv_log(UDRV_LOG_WARNING, "non implemented mandatory function '" #func "'");\
		return val;\
	}
#endif
