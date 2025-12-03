#ifndef _UDRV_IO_H
#define _UDRV_IO_H

#include "types.h"

void udrv_out_byte(udrv_port_t port, uint8_t data);
uint8_t udrv_in_byte(udrv_port_t port);
void udrv_out_word(udrv_port_t port, uint16_t data);
uint16_t udrv_in_word(udrv_port_t port);
void udrv_out_dword(udrv_port_t port, uint32_t data);
uint32_t udrv_in_dword(udrv_port_t port);

#endif
