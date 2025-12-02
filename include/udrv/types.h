#ifndef _UDRV_TYPES_H
#define _UDRV_TYPES_H

#include <stddef.h>
#include <stdint.h>

typedef ptrdiff_t udrv_ssize_t;

typedef struct udrv_version {
	uint16_t major;
	uint16_t minor;
	uint32_t patch;
} udrv_version_t;

#endif
