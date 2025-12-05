#include <udrv/driver.h>
#include <udrv/device.h>
#include <udrv/utils.h>
#include <udrv/bus.h>

// test driver

#define CHECK(func) total++; if (!func) udrv_log(UDRV_LOG_ERROR, "unimplemented func "#func); else succed++;

int test_init(int argc, const char **argv) {
	(void)argc;
	(void)argv;
	udrv_log(UDRV_LOG_INFO, "hello world");
	void *ptr = udrv_malloc(7);
	udrv_free(ptr);

	int total = 0;
	int succed = 0;
	CHECK(udrv_memcmp)
	CHECK(udrv_strcmp)
	CHECK(udrv_memset)
	CHECK(udrv_register_device)
	CHECK(udrv_destroy_device)
	CHECK(udrv_register_device_typedef)
	CHECK(udrv_unregister_device_typedef)
	CHECK(udrv_hotplug_addr)
	CHECK(udrv_hotunplug_addr)
	udrv_log(UDRV_LOG_INFO, "succed %d/%d", succed, total);
	return 0;
}

udrv_driver_t udrv_meta = {
	.init = test_init,
	.author = "tayoky",
	.name = "test",
};
