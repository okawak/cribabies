/* V792 for libbabies + CAENLIB */

#include "libbabies.h"
#include "libbbcaenvme.h"

#include "modules/sis3820.h"

void sis3820_initialize_nc(unsigned int maddr) {
    int val = SIS3820_CTRL_USER_LED_ON;
    vwrite32(maddr + SIS3820_CONTROL_STATUS, &val); // turn on user LED
    val = 0x1;
    vwrite32(maddr + SIS3820_KEY_RESET, &val); // reset
    val = SIS3820_NON_CLEARING_MODE;
    vwrite32(maddr + SIS3820_OPERATION_MODE, &val);       // non-clearing mode
    vwrite32(maddr + SIS3820_KEY_OPERATION_ENABLE, &val); // enable channels
}

//-----------------------------------------------------------------------------
int sis3820_ridf_scaler(unsigned int maddr, unsigned int *buff) {
    volatile int i;
    int dummy = 0x1;
    /* clock shadow */
    vwrite32(maddr + SIS3820_KEY_LNE_PULS, &dummy);
    for (i = 0; i < 32; i++) {
        /* Read Scaler data */
        vread32(maddr + SIS3820_COUNTER_SHADOW_CH1 + i * 4, (int *)(buff + i));
    }
    return 1;
}

// void sis3820_map_initialize_nc_inhbitin(int dn) {
//   int val;
//   val = 0x1;
//   univ_map_write32(SIS3820_KEY_RESET, &val, dn);           //reset
//   val = SIS3820_CTRL_USER_LED_ON;
//   univ_map_write32(SIS3820_CONTROL_STATUS, &val,dn);      // turn on user LED
//   val = 0xffffffff;
//   univ_map_write32(SIS3820_COUNTER_CLEAR,&val,dn);
//   val = 0x00000000;
//   univ_map_write32(SIS3820_COUNTER_EXTINHIBIT,&val,dn);
//
//   val = SIS3820_NON_CLEARING_MODE | SIS3820_INPUT_MODE3; // control input4 = inhibit counting
//   univ_map_write32(SIS3820_OPERATION_MODE, &val, dn);   //non-clearing mode
//   univ_map_write32(SIS3820_KEY_OPERATION_ENABLE, &val, dn);//enable channels
// }
//
// void sis3820_map_terminate(int dn) {
//   int val = SIS3820_KEY_OPERATION_DISABLE;
//   univ_map_write32(SIS3820_CONTROL_STATUS, &val, dn); //disable channels
//   val = SIS3820_CTRL_USER_LED_OFF;
//   univ_map_write32(SIS3820_CONTROL_STATUS, &val,dn);  //turn off user LED
// }
//
// int sis3820_map_ridf_scaler(int dn){
//   volatile int i;
//   int dummy = 0x1;
//   /* clock shadow */
//   univ_map_write32(SIS3820_KEY_LNE_PULS, &dummy, dn);
//   for(i=0;i<32;i++){
//     /* read scaler data */
//     univ_map_read32(SIS3820_COUNTER_SHADOW_CH1+i*4, (int *)(babies_segpt32()), dn);
//     //    if(eventsize > 0) eventsize += 2;
//   }
//   return 1;
// }
//
// int sis3820_map_segdata(int dn){
//   volatile int i;
//   int dummy = 0x1;
//   /* clock shadow */
//   univ_map_write32(SIS3820_KEY_LNE_PULS, &dummy, dn);
//   for(i=0;i<32;i++){
//     /* read scaler data */
//     univ_map_read32(SIS3820_COUNTER_SHADOW_CH1+i*4, (int *)(babies_segpt32()), dn);
//     //    eventsize += 2;
//   }
//   //  if(eventsize > 0) eventsize += 2* SIS3820_NCH;
//   return 1;
// }
//
//
// int sis3820_segdata2_map(unsigned int maddr, int dn){
//   int dummy = 0x1;
//
//   // modify only to read from ch30 to ch31
//   /* clock shadow */
//   univ_map_write32(SIS3820_KEY_LNE_PULS, &dummy, dn);
//   univ_map_read32(SIS3820_COUNTER_SHADOW_CH1+2*4, (int *)(babies_segpt32()), dn);
//   univ_map_read32(SIS3820_COUNTER_SHADOW_CH1+31*4, (int *)(babies_segpt32()), dn);
//
//
//   return 1;
// }
//
// int sis3820_map_fiforidf_scaler(int dn, int fn){
//   volatile int i;
//   int dummy = 0x1;
//   /* clock shadow */
//   univ_map_write32(SIS3820_KEY_LNE_PULS, &dummy, dn);
//   for(i=0;i<32;i++){
//     /* read scaler data */
//     univ_map_read32(0, (int *)(babies_segpt32()), fn);
//     //    if(eventsize > 0) eventsize += 2;
//   }
//   return 1;
// }
