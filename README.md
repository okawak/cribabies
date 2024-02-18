## configuration
- V2718 + A3818


## Implemented modules
- v7XX
- v1190
- MADC32
- SIS3820


## How to use
### CAEN library
From CAEN page, please install these library first

- CAENVMELib
- A3818-driver

(option)
- CAENComm
- CAEN Upgrader

### babirl
From RIBFDAQ page, please install the babirl

If you want, please copy the latest babirl files
- babirl/include/segidlist.h -> include/segidlist.h (need to add line #define CRIB 12)
- babirl/include/bi-config.h -> include/bi-config.h (modify PIDDIR -> "run", comment out BABIESRC)
- babirl/lib/bi-pid.c -> src/bi-pid.c
- babirl/skelton/libbabies.h -> include/libbabies.h (modify to debug mode)
- babirl/skelton/libbabies.c -> include/libbabies.c

### babies 
modufy these files:
- modulelist.yaml
- src/cribabies.c

as your experimental setting.

```shell
mkdir build
cd build
cmake ..
make
make install
cd ..
source thiscribabies.sh 
```
is fine for building.

compiled files are installed here
- install/bin/cribabies
- install/bin/cmdvme
- install/lib/libcribvme.so

To execute cribabies, say
```shell
./init_vme.sh
cribabies $EFN
```
where EFN is Event Fragment Number, to be shared with babirl.
