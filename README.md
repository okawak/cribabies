## configuration
- V2718 + A3818

developed by H. Shimizu, updated by K. Okawa

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

### bbcaenvme and modification
It use "bbcaenvme150709" files, and they can download from RIBFDAQ download page.

- bbcaenvme/babies/segidlist.h -> include/segidlist.h (need to add line #define CRIB 12)
- bbcaenvme/babies/bbpid.h -> include/bbpid.h
- bbcaenvme/babies/bbpid.c -> src/bbpid.c

- bbcaenvme/cmdvme/cmdvme.c -> src/cmdvme.c

in the struct definition
```cpp
  // Feb.2024 K.Okawa modified
  //const char mdchar[];
  const char *mdchar;
```

- bbcaenvme/babies/libbabies.h (NOT in module directory) -> include/libbabies.h (modify to debug mode)
- bbcaenvme/babies/libbabies.c -> src/libbabies.c

in "commain" function
```cpp
    //len = sprintf(f, name);
    len = sprintf(f, "%s", name); // K.Okawa Feb.2024
```

- bbcaenvme/lib/libbbcaenvme.h -> include/libbbcaenvme.h


```cpp
#ifndef LIBBBCAENVME_H
#define LIBBBCAENVME_H
#include <CAENVMEtypes.h>
#include <stdint.h> // K.Okawa Feb.2024
#endif

//int init_caen(CVBoardTypes VMEBoard, short Link, short Device);
int init_caen(CVBoardTypes VMEBoard, uint32_t Link, short Device); // K.Okawa Feb.2024

// -- snip --

// add last line
/* shimizu Dec.2019 */
void vme_setoutput_v2718();
void vme_v2718_output_pulse(unsigned short mask);

int vme_v2718_chk_output_register(void);
```

- bbcaenvme/lib/libbbcaenvme.c -> src/libbbcaenvme.c
```cpp
// Feb.2024 K.Okawa add <stdio.h>
#include <stdio.h>

// -- snip --

// Feb.2024 K.Okawa mod: CAENVME_Init -> CAENVME_Init2
//int init_caen(CVBoardTypes VMEBoard, short Link, short Device){
  //if( CAENVME_Init(VMEBoard, Link, Device, &BHandle) != cvSuccess ) {
int init_caen(CVBoardTypes VMEBoard, uint32_t Link, short Device){
  if( CAENVME_Init2(VMEBoard, &Link, Device, &BHandle) != cvSuccess ) {

// -- snip --

/* shimizu Dec.2019 */
void vme_setoutput_v2718(){

  unsigned short val;
  unsigned int width;
  unsigned int period;

  // assignment 
  // ch0: BUSY CLR
  // ch1: START
  // ch2: STOP
  // ch3: Clock (Pulser B)
  // ch4: SCL CLR
  CAENVME_SetOutputConf(BHandle,cvOutput0,cvDirect,cvActiveHigh,cvManualSW);
  CAENVME_SetOutputConf(BHandle,cvOutput1,cvDirect,cvActiveHigh,cvManualSW);
  CAENVME_SetOutputConf(BHandle,cvOutput2,cvDirect,cvActiveHigh,cvManualSW);
  CAENVME_SetOutputConf(BHandle,cvOutput3,cvDirect,cvActiveHigh,cvMiscSignals);
  CAENVME_SetOutputConf(BHandle,cvOutput4,cvDirect,cvActiveHigh,cvManualSW);
  // set ch3 for clock generator
  width=4;// 100ns width
  period=40;// 0.1us period -> 10MHz
  CAENVME_SetPulserConf(BHandle,cvPulserB,period,width,cvUnit25ns, 0,cvManualSW,cvManualSW); 
  val = 0x0004;
  CAENVME_WriteRegister(BHandle,cvOutRegSet,val);// PulserB start 

}

void vme_v2718_output_pulse(unsigned short mask) {
  
  //printf("CAENVME_PulseOutputRegister: %d\n",CAENVME_PulseOutputRegister(BHandle,mask));
  CAENVME_PulseOutputRegister(BHandle,mask);

}

int vme_v2718_chk_output_register(void) {

  unsigned int data;
  CAENVME_ReadRegister(BHandle,cvOutRegSet,&data);
  //printf("CAENVME_ReadRegister: %04x\n",data);
  return (data == 0x0000);

}
```

### babies 
modify these files as your experimental setting
- modulelist.yaml

    set the physical address dial of each module to consist with "haddr" in this file.

- src/cribabies.c

    1. uncomment unused modules or comment out used modules in "start" function
    2. If you don't use MADC, comment out the related function in "stop" function
    3. uncomment or comment out the related function in "clear" depend on the setting
    4. uncomment or comment out the related function in "evtloop" that make event segment
    5. If you don't use scaler, comment out the sca() in "evtloop"

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
# initial python setting
cd init_modules
poetry install
cd ..

./init_vme.sh
cribabies $EFN
```
where EFN is Event Fragment Number, to be shared with babirl.
