#ifndef _UDRV_DRIVER_H
#define _UDRV_DRIVER_H

#include "types.h"

typedef struct udrv_driver {
	const char *name;
	const char *author;
	const char *lisence;
	udrv_version_t version;
	udrv_version_t udrv_version;
 udrv_driver_t;

#endif
