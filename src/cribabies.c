/*
 * example to use libbabies + CAENVMELib
 * H.B. RIKEN
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "bbpid.h"
#include "libbabies.h"
#include "libbbcaenvme.h"
#include "segidlist.h"

#include "modules/madc32.h"
#include "modules/sis3820.h"
#include "modules/v1190.h"
#include "modules/v7XX.h"

int efn = 0;
int mode = STAT_RUN_IDLE;
unsigned int scrbuff[32] = {0};
char pidpath[] = "./run/babies";

// address of VME board
const unsigned int scraddr = 0x38000000;
const unsigned int adc0addr = 0x10000000;
const unsigned int adc1addr = 0x10010000;
const unsigned int adc2addr = 0x10020000;
const unsigned int tdc0addr = 0x20000000;
const unsigned int tdc1addr = 0x20010000;
const unsigned int tdc2addr = 0x20020000;
const unsigned int mhtdcaddr = 0x40000000;
const unsigned int madc1addr = 0x50000000;
const unsigned int madc2addr = 0x50010000;
const unsigned int qdcaddr = 0x11010000;
const int intlevel = 1;

const unsigned short output0 = 0x40;
const unsigned short output1 = 0x80;
const unsigned short output2 = 0x100;
const unsigned short output3 = 0x200;
const unsigned short output4 = 0x400;

void clear(void);

void quit(void) {
    release_caen();
    rmpid(pidpath);
    printf("Exit\n");
}

void start(void) {

    /*
    // Initialization for V1290 is in ../init/v1290.sh
    v1290_clear(tdcaddr);
    v7XX_clear(qdcaddr);
    v7XX_noberr(qdcaddr);
    v7XX_multievtberr(qdcaddr);
    v7XX_intlevel(qdcaddr, intlevel);
    */

    printf("debuginfo: start() clearing modules.\n");

    v7XX_clear(adc0addr);
    //  v7XX_noberr(adc0addr);
    v7XX_multievtberr(adc0addr);
    v7XX_intlevel(adc0addr, intlevel);
    v7XX_mcstctrl(adc0addr, 2);
    v7XX_mcstaddr(adc0addr, 0xab);

    v7XX_clear(adc1addr);
    // v7XX_noberr(adc1addr);
    v7XX_multievtberr(adc1addr);
    v7XX_mcstctrl(adc1addr, 1);
    v7XX_mcstaddr(adc1addr, 0xab);
    // v7XX_intlevel(adc1addr, intlevel);

    v7XX_clear(tdc0addr);
    // v7XX_noberr(tdc0addr);
    v7XX_multievtberr(tdc0addr);
    v7XX_mcstctrl(tdc0addr, 2);
    v7XX_mcstaddr(tdc0addr, 0xac);
    // v7XX_singleevtberr(tdc0addr);
    // v7XX_intlevel(tdc0addr, intlevel);

    // v7XX_clear(tdc1addr);
    ////  v7XX_noberr(tdc1addr);
    // v7XX_multievtberr(tdc1addr);
    // v7XX_mcstctrl(tdc1addr, 1);
    // v7XX_mcstaddr(tdc1addr, 0xac);
    ////v7XX_intlevel(tdc0addr, intlevel);
    //
    madc32_stop_acq(madc1addr);
    madc32_FIFO_reset(madc1addr); // HY added, but no effect?
    madc32_clear(madc1addr);
    madc32_start_acq(madc1addr);

    madc32_stop_acq(madc2addr);
    madc32_FIFO_reset(madc2addr); // HY added, but no effect?
    madc32_clear(madc2addr);
    madc32_start_acq(madc2addr);

    v1190_clear(mhtdcaddr);

    ///* -- initialize scaler -- */
    sis3820_initialize_nc(scraddr);

    // printf("debuginfo: start() define intlevel.\n");
    vme_define_intlevel(intlevel);

    vme_enable_interrupt();
    printf("Start\n");

    vme_v2718_output_pulse(output1);

    printf("start(): vme_output_pulse\n");
    vme_v2718_chk_output_register(); // for debug
    printf("start(): debug1\n");

    printf("DAQ start\n");
}

void stop(void) {
    vme_v2718_output_pulse(output2);

    madc32_stop_acq(madc1addr);
    madc32_stop_acq(madc2addr);
    clear();

    printf("debuginfo: stop() disable interrupt.\n");
    vme_disable_interrupt();
}

void reload(void) {
    printf("debuginfo: reload() Reloading.\n");
    printf("Reload\n");
}

void sca(void) {
    //  int i;
    // printf("debuginfo: sca() called.\n");
    // printf("Sca\n");

    //  for(i=0;i<32;i++){
    //    scrbuff[i] = scrbuff[i] + 1;
    //  }
    babies_init_ncscaler(efn);
    sis3820_ridf_scaler(scraddr, scrbuff); // modified with Baba-san
    babies_scrdata((char *)scrbuff, sizeof(scrbuff));
    babies_end_ncscaler32();
}

void clear(void) {
    // write clear function i.e. send end-of-busy pulse
    // printf("Clear\n");

    vme_v2718_output_pulse(output4); // clear of V785, V775 by ECL connectors
    v7XX_clear(adc0addr);
    v7XX_clear(adc1addr);
    v7XX_clear(tdc0addr);
    v7XX_clear(tdc1addr);

    madc32_clear(madc1addr);
    madc32_clear(madc2addr);
    v1190_clear(mhtdcaddr);

    vme_v2718_output_pulse(output0); // latch reset output0 eob pulse width ~5us
}

// thread
void evtloop(void) {
    int status;
    mode = 0;

    printf("debuginfo:\x1b[32m evtloop() called.\x1b[39m\n");

    while ((status = babies_status()) != -1) {
        switch (status) {
        case STAT_RUN_IDLE:
            /* noop */
            usleep(1000);
            break;

        case STAT_RUN_START:
        case STAT_RUN_NSSTA:

            // HY: When you want to check deadtime without reading modules:
            // clear();
            // break;

            if (vme_check_interrupt()) {
                /* continue */
                // printf("interrupt ok\n");
            } else {
                //	usleep(1); furu
                break;
            }

            if (vme_v2718_chk_output_register()) {
                printf("[error] V2718 output seems strange: Stopping babies. Restart babies may solve this.\n");
                stop();
            }

            babies_init_event();

            // babies_init_segment(segid)
            // MKSEGID(device, focalplane, detector, module)
            // please see segidlist.h
            // module is important, e.g. C16 is 16bit data
            // device, focalplane, detector, can be defined as you want

            /*
                  // Read data from V1290
                  babies_init_segment(MKSEGID(0, 0, PLAT, V1190));
                  //v1290_segdata(tdcaddr);
                  v1290_dmasegdata(tdcaddr, 256);
                  babies_end_segment();

            */

            // printf("debuginfo:\x1b[32m evtloop() reading data from v785.\x1b[39m\n");
            //  Read data from v785
            babies_init_segment(MKSEGID(CRIB, 0, SSDE, V785));
            // v785_segdata(adc1addr);
            // v7XX_dmasegdata(adc0addr, 34);
            // v7XX_dmasegdata(adc1addr, 34);
            v7XX_dmasegdata(0xab000000, 256);
            babies_end_segment();

            // Read data from v775
            babies_init_segment(MKSEGID(CRIB, 0, SSDT, V775));
            // v7XX_dmasegdata(tdc0addr, 34);
            // v7XX_dmasegdata(tdc1addr, 34);
            v7XX_dmasegdata(0xac000000, 256);
            babies_end_segment();

            // Read data from madc
            babies_init_segment(MKSEGID(CRIB, 0, SSDE, MADC32));
            madc32_dmasegdata(madc1addr, 34);
            madc32_dmasegdata(madc2addr, 34);

            //      madc32_segdata(madc1addr);
            babies_end_segment();

            // Read data from v1190
            babies_init_segment(MKSEGID(CRIB, 0, SSDT, V1190));
            // HY Oct 27,2021
            // Tried to count the number of words by Event FIFO, but always 0

            // int wcount=v1190_get_word_count(mhtdcaddr);
            // printf("V1190 #Words: %d\n", wcount);

            v1190_dmasegdata(mhtdcaddr, 256);
            // v1190_segdata(mhtdcaddr);

            babies_end_segment();

            // End of event
            babies_end_event();

            // babies_chk_block(int maxbuff)
            // if block size is larger than maxbuff,
            //  data should be send to the event builder
            //  i.e., read scaler and flush
            // example : 8000 words = 16kByte (1 word = 16bit)
            // 1 block = 16384B = 8192 words

            mode = status;
            if (babies_chk_block(8000)) {

                sca();
                babies_flush();
            }

            // clear module is here (not in clear())
            // v1290_clear(tdcaddr);

            // v7XX_clear(adc1addr);

            clear();

            break;
        case STAT_RUN_WAITSTOP:
            // for the last sequense of run
            sca();
            babies_flush();
            babies_last_block();
            break;

        default:
            break;
        }
    }

    // write codes to quit safely
}

int main(int argc, char *argv[]) {

    printf("debuginfo: main() babies main function called.\n");
    if (argc != 2) {
        printf("babies EFN\n");
        exit(0);
    } else {
        efn = strtol(argv[1], NULL, 0);
    }

    babies_quit(quit);
    printf("debuginfo: main() quit.\n");
    babies_start(start);
    printf("debuginfo: main() start.\n");
    babies_stop(stop);
    printf("debuginfo: main() stop.\n");
    babies_reload(reload);
    printf("debuginfo: main() reload.\n");
    babies_evtloop(evtloop);
    printf("debuginfo: main() evtloop.\n");
    babies_name("babies");

    printf("debuginfo: intializing babies with EFN = %d.\n", efn);
    babies_init(efn);
    babies_check();

    /* init vme */
    init_caen_v2718();
    printf("main(): init_caenv2718\n");
    vme_v2718_chk_output_register(); // for debug
    vme_amsr(0x09);                  // 09...A32 non previleged data access
    printf("main(): vme_amsr\n");
    vme_v2718_chk_output_register(); // for debug

    /* set output of v2718 */
    vme_setoutput_v2718();
    printf("main(): vme_setoutput_v2718\n");
    vme_v2718_chk_output_register(); // for debug

    // mkpid to use babimo
    // default = /var/run/babies
    // in this example, use /tmp/babies
    mkpid(pidpath);

    printf("debuginfo: main() about to start main loop.\n");
    babies_main();

    return 0;
}
