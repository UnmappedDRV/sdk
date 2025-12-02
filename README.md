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
