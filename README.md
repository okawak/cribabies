Babian
V2718 + A3818


## Implemented modules
- v7XX
- v1180
- MADC32
- SIS3820


## How to use

### babies
Modify 
```
src/cribabies.c
```
as your experimental setting.


### stop.sh
```
./stop.sh
```
is the shell script to be executed after RUN STOP (not NSSTA).

### build 
just
```
make
```
is fine for building.

To execute cribabies, say
```
./bin/cribabies $EFN
```
where EFN is Event Fragment Number, to be shared with babirl.
