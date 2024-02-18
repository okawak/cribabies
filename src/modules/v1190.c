#include "modules/v1190.h"
#include "libbbcaenvme.h"
// void v1190_clear(unsigned long maddr);
// int v1190_segdata(unsigned long maddr);
// int v1190_dmasegdata(unsigned long maddr);

/* Segment data */
int v1190_segdata(unsigned long maddr) {
    int wordcnt;

    wordcnt = 0;
    /* Global Header */
    vread32(maddr + V1190_OUTBUFF, (int *)(babies_segpt32()));
    wordcnt++;

    if ((*(int *)(babies_pt() - 4) & V1190_TYPE_MASK_S) == V1190_GLOBAL_HEADER_BIT_S) {
        while (wordcnt < 256) {
            /* TDC Header */
            vread32(maddr + V1190_OUTBUFF, (int *)(babies_segpt32()));
            wordcnt++;
            if ((*(int *)(babies_pt() - 4) & V1190_TYPE_MASK_S) == V1190_TDC_HEADER_BIT_S) {
                while (wordcnt < 256) {
                    vread32(maddr + V1190_OUTBUFF, (int *)(babies_segpt32()));
                    wordcnt++;
                    if ((*(int *)(babies_pt() - 4) & V1190_TYPE_MASK_S) != V1190_TDC_DATA_BIT_S) {
                        /* TDC Trailer or TDC Error */
                        break;
                    }
                }
            } else {
                break;
                /* Global Trailer */
            }
        }
    }

    return 0;
}

int v1190_get_word_count(unsigned long maddr) {
    int i;
    vread32(maddr + V1190_EVENT_FIFO, &i);
    // HY Oct 29, 2021 always 0?
    //   printf("%x %d", maddr + V1190_EVENT_FIFO, i);
    return i & 0xffff;
}

int v1190_dmasegdata(unsigned long maddr, int cnt) {
    int sz, rsz;
    sz = cnt * 4;
    rsz = dma_vread32(maddr, babies_pt(), sz);
    babies_segptfx(rsz);
    return rsz / 2;
}

/* Software clear */
void v1190_clear(unsigned long maddr) {
    short sval;

    sval = V1190_SOFT_CLEAR_BIT;
    vwrite16(maddr + V1190_SOFT_CLEAR, &sval);
}

/* Set Almost full register */
void v1190_tdcfull(unsigned long maddr, short fl) {
    short sval;

    sval = fl;
    vwrite16(maddr + V1190_ALMOST_FULL, &sval);
}
