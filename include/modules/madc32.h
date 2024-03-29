/* header file for madc32 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../libbabies.h"
#include "../libbbcaenvme.h"

void madc32_segdata(unsigned int maddr);
void madc32_clear(unsigned int maddr);
void madc32_FIFO_reset(unsigned int maddr);
void madc32_start_acq(unsigned int maddr);
void madc32_stop_acq(unsigned int maddr);
void madc32_irq_level(unsigned int maddr, short val);
void madc32_adc_resolution(unsigned int maddr, short val);
void madc32_input_range(unsigned int maddr, short val);
void madc32_module_id(unsigned int maddr, short val);
int madc32_dmasegdata(unsigned int maddr, int cnt);

#define MADC32_DATA 0x0000

#define MADC32_ADDRESS_SOURCE 0x6000
#define MADC32_ADDRESS_REG 0x6002
#define MADC32_MODULE_ID 0x6004
#define MADC32_SOFT_RESET 0x6008
#define MADC32_FIRMWARE_REVISION 0x600e

#define MADC32_IRQ_LEVEL 0x6010
#define MADC32_IRQ_VECTOR 0x6012
#define MADC32_IRQ_TEST 0x6014
#define MADC32_IRQ_RESET 0x6016
#define MADC32_IRQ_THRESHOLD 0x6018
#define MADC32_MAX_TRANSFER_DATA 0x601a
#define MADC32_WITHDRAW_IRQ 0x601c

#define MADC32_CBLT_MCST_CONTROL 0x6020
#define MADC32_CBLT_ADDRESS 0x6022
#define MADC32_MCST_ADDRESS 0x6024

#define MADC32_BUFFER_DATA_LENGTH 0x6030
#define MADC32_DATA_LEN_FORMAT 0x6032
#define MADC32_READOUT_RESET 0x6034
#define MADC32_MULTIEVENT 0x6036
#define MADC32_MARKING_TYPE 0x6038
#define MADC32_START_ACQ 0x603a
#define MADC32_FIFO_RESET 0x603c
#define MADC32_DATA_READY 0x603e

#define MADC32_BANK_OPERATION 0x6040
#define MADC32_ADC_RESOLUTION 0x6042
#define MADC32_OUTPUT_FORMAT 0x6044
#define MADC32_ADC_OVERRIDE 0x6046
#define MADC32_SLC_OFF 0x6048
#define MADC32_SKIP_OORANGE 0x604a
#define MADC32_IGNORE_THRESHOLDS 0x604c

#define MADC32_INPUT_RANGE 0x6060
#define MADC32_INPUT_RANGE_4V 0
#define MADC32_INPUT_RANGE_10V 1
#define MADC32_INPUT_RANGE_8V 2
