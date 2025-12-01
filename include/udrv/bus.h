#ifndef _UDRV_BUS_H
#define _UDRV_BUS_H

#include "types.h"
#include "list.h"
#include "device.h"

typedef struct udrv_bus {
	udrv_device_t device;
	list_t addresses;
	int type;
} udrv_bus_t;

#define UDRV_BUS_PCI 1
#define UDRV_BUS_USB 2

typedef struct udrv_bus_addr {
	list_node_t list_node;
	udrv_bus_t *bus;
	udrv_device_t *device;
} udrv_bus_addr_t;

typedef struct udrv_pci_addr {
	udrv_bus_addr_t addr;
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
