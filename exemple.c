#include <udrv/driver.h>
#include <udrv/device.h>

// exemple driver

int init(int argc, char *argv){
	udrv_log(UDRV_LOG_INFO, "hello");
}

udrv_driver_t udrv_meta = {
	.name = "exemple",
	.init = init,
};

udrv_device_typedef_t device_type = {
};
