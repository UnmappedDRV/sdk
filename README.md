# UnmappedDRV sdk
this repo contain the UnmappedDRV sdk

## build
```sh
./configure
make
```

## use
To use build the environment (make) and link your kernel with build/udrv_env.a  
In your kernel call udrv_init_env and pass it the pointer to your env with all your functions

ex
```c
#include <udrv/env.h>

void my_log(int level, const char *fmt, va_args args) {
    if (level == UDRV_LOG_DEBUG) my_printf("[debug] : ");
    my_vprintf(fmt, args);
}

udrv_env_t myenv = {
    .malloc = kmalloc,
    .free   = kfree,
    .log    = my_log,
    // ...
}

int load_driver(const char *path, int argc, const char *argv){
    FILE *file = open_file();
    // read file in memory
    udrv_driver_t *driver;
    return udrv_load_driver(data, size, &driver, argc, argv);
}

int kmain(){
    // stuff ...
    udrv_init_env(&myenv);
    load_driver("udrv_pci.o");
    // stuff ...
}
```
