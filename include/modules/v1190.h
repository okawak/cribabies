/* Header for V1190 Multihit TDC */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../libbabies.h"
#include "../libbbcaenvme.h"

void v1190_clear(unsigned long maddr);
int v1190_segdata(unsigned long maddr);
int v1190_get_word_count(unsigned long maddr);
int v1190_dmasegdata(unsigned long maddr, int cnt);
void v1190_tdcfull(unsigned long maddr, short fl);

/* Register */
#define V1190_OUTBUFF 0x0000
#define V1190_SOFT_CLEAR 0x1016
#define V1190_ALMOST_FULL 0x1022
#define V1190_EVENT_FIFO 0x1038

/* Bit */
#define V1190_SOFT_CLEAR_BIT 1

#define V1190_TYPE_MASK_S 0xf800
#define V1190_GLOBAL_HEADER_BIT_S 0x4000
#define V1190_TDC_HEADER_BIT_S 0x0800
#define V1190_TDC_DATA_BIT_S 0x0000
#define V1190_TDC_TRAILER_BIT_S 0x1800
#define V1190_TDC_ERROR_BIT_S 0x2000
#define V1190_GLOBAL_TRAILER_BIT_S 0x8000
