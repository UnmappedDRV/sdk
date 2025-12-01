#ifndef _UDRV_BUS_H
#define _UDRV_BUS_H

#include "types.h"
#include "device.h"

typedef struct udrv_bus {
	udrv_device_t device;
	int type;
} udrv_bus_t;

#define UDRV_BUS_PCI 1
#define UDRV_BUS_USB 2

typedef struct udrv_bus_addr {
	udrv_bus_t *bus;
	udrv_device_t *device;
	void *data;
} udrv_bus_addr_t;

typedef struct udrv_pci_addr {
	uint16_t vendor_id;
	uint16_t device_id;
	uint8_t class;
	uint8_t subclass;
	uint8_t prog_if;
	uint8_t bus;
	uint8_t device;
	uint8_t fonction;
} udrv_pci_addr_t;

#endif
