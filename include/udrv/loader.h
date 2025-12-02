#ifndef _UDRV_LOADER_H
#define _UDRV_LOADER_H

#include "types.h"
#include "driver.h"

int udrv_load_module(void *data, size_t size, udrv_driver_t **driver, udrv_entry_t *entry);

#endif
