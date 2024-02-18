/* madc32 */

#include "modules/madc32.h"

// void madc32_clear_map(int n){
//   short val;
//
//   val = 1;
//   univ_map_write16(MADC32_READOUT_RESET, &val, n);
//
// }

void madc32_clear(unsigned int maddr) {
    short val;
    val = 1;
    vwrite16(maddr + MADC32_READOUT_RESET, &val);
}

void madc32_FIFO_reset(unsigned int maddr) {
    short val;
    val = 1;
    vwrite16(maddr + MADC32_FIFO_RESET, &val);
}

// void madc32_start_acq_map(int n){
//   short val;
//
//   val = 1;
//   univ_map_write16(MADC32_START_ACQ, &val, n);
// }

void madc32_start_acq(unsigned int maddr) {
    short val;

    val = 1;
    vwrite16(maddr + MADC32_START_ACQ, &val);
}

// void madc32_stop_acq_map(int n){
//   short val;
//
//   val = 0;
//   univ_map_write16(MADC32_START_ACQ, &val, n);
// }

void madc32_stop_acq(unsigned int maddr) {
    short val;
    val = 0;
    vwrite16(maddr + MADC32_START_ACQ, &val);
}

// void madc32_irq_level(int n, short val){
//
//   univ_map_write16(MADC32_IRQ_LEVEL, &val, n);
// }
//

void madc32_irq_level(unsigned int maddr, short val) {

    vwrite16(maddr + MADC32_IRQ_LEVEL, &val);
}

// void madc32_resol_8khires(int n){
//   short val;
//
//   val = 4;
//   univ_map_write16(MADC32_ADC_RESOLUTION, &val, n);
// }

/*
 * adc resolution
 *
 * val = 0 : hires, 3.2us 2k, 800ns conv. Time
 * val = 1 : hires, 3.2us 4k, 1.6us conv. Time
 * val = 2 : hires, 3.2us 4k, 1.6us conv. Time
 * val = 3 : hires, 3.2us 8k, 1.6us conv. Time
 * val = 4 : hires, 3.2us 8k, hires, 3.2us conv. Time
 *
 */

void madc32_adc_resolution(unsigned int maddr, short val) {
    vwrite16(maddr + MADC32_ADC_RESOLUTION, &val);
}

// void madc32_input_range(int n, short val){
//   univ_map_write16(MADC32_INPUT_RANGE, &val, n);
// }

void madc32_input_range(unsigned int maddr, short val) {
    vwrite16(maddr + MADC32_INPUT_RANGE, &val);
}

// vvoid madc32_module_id(int n, short val){
// v  univ_map_write16(MADC32_MODULE_ID, &val, n);
// v}

void madc32_module_id(unsigned int maddr, short val) {
    vwrite16(maddr + MADC32_MODULE_ID, &val);
}

// int madc32_segdata_map(int n){
//   volatile short cnt, i;
//
//   univ_map_read16(MADC32_BUFFER_DATA_LENGTH, (short *)&cnt, n);
//   cnt = cnt & 0x3fff;
//
//   for(i=0;i<cnt;i++){
//     univ_map_read32(MADC32_DATA, (long *)(data+mp), n);
//     mp += 2;
//     segmentsize += 2;
//   }
//
//   return segmentsize;
// }

void madc32_segdata(unsigned int maddr) {
    // volatile
    short cnt, i;

    vread16(maddr + MADC32_BUFFER_DATA_LENGTH, &cnt);
    //  vread16(maddr + MADC32_MODULE_ID, (short *)&cnt);
    //  cnt = cnt & 0x3fff;
    //  printf("madc cnt : %d\n", cnt);
    // Why cnt is only 2 by this???
    // By setting fixed cnt:
    cnt = 34;
    for (i = 0; i < cnt; i++) {
        int val;

        //    printf("0x%x\n",maddr);
        vread32(maddr + MADC32_DATA, &val); // Ok
        //    printf("0x%02x\n",val);
        // vread32(maddr + MADC32_DATA,(int *) (babies_segpt32()));//Segmentation fault by char *->int* ?
    }
    //  printf("debug 4 \n");
}

int madc32_dmasegdata(unsigned int maddr, int cnt) {
    int sz, rsz;

    short i;
    vread16(maddr + MADC32_BUFFER_DATA_LENGTH, &i);
    // i = i & 0x3fff;
    // printf("madc cnt : 0x%04x\n", i);

    sz = cnt * 4; // bytes

    //  rsz = dma_vread32(maddr, babies_pt(), sz);
    // babies_segptfx(rsz);

    rsz = dma_vread32(maddr, babies_segptfx(sz), sz);

    return rsz / 2;
}
