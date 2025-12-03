#include <udrv/io.h>
#include <udrv/env.h>
#include <udrv/internal.h>

void udrv_out_byte(udrv_port_t port, uint8_t data) {
	FUNC_CHECK(out_byte,);
	return udrv_env->out_byte(port, data);
}

uint8_t udrv_in_byte(udrv_port_t port) {
	FUNC_CHECK(in_byte, 0);
	return udrv_env->in_byte(port);
}

void udrv_out_word(udrv_port_t port, uint16_t data) {
	FUNC_CHECK(out_word,);
	return udrv_env->out_word(port, data);
}

uint16_t udrv_in_word(udrv_port_t port) {
	FUNC_CHECK(in_word, 0);
	return udrv_env->in_word(port);
}

void udrv_out_dword(udrv_port_t port, uint32_t data) {
	FUNC_CHECK(out_dword,);
	return udrv_env->out_dword(port, data);
}

uint32_t udrv_in_dword(udrv_port_t port) {
	FUNC_CHECK(in_dword, 0);
	return udrv_env->in_dword(port);
}
