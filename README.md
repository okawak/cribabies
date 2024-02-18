## configuration
- V2718 + A3818


## Implemented modules
- v7XX
- v1190
- MADC32
- SIS3820


## How to use
## need CAEN library
From CAEN page, please install these library first

- CAENVMELib
- A3818-driver

(option)
- CAENComm
- CAEN Upgrader

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

To execute cribabies, say
```shell
./init_vme.sh
cribabies $EFN
```
where EFN is Event Fragment Number, to be shared with babirl.
