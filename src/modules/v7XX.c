/* v7XX for libbabies + CAENLIB */

#include "lib/libbabies.h"
#include "lib/libbbcaenvme.h"
#include "modules/v7XX.h"

int v7XX_segdata(unsigned int maddr){
  int wordcnt;

  wordcnt = 0;
  vread32(maddr+v7XX_OUTBUFF,(int *)(babies_segpt32()));
  wordcnt++;

  if((*((int *)(babies_pt()-4)) & v7XX_TYPE_MASK) == v7XX_HEADER_BIT){
    while(wordcnt < 34){
      vread32(maddr+v7XX_OUTBUFF,(int *)(babies_segpt32()));
      wordcnt++;
      if((*(int *)(babies_pt()-4) & (v7XX_TYPE_MASK)) != v7XX_DATA_BIT){
        break;
      }
    }
  }

  return 1;
}

int v7XX_segdata2(unsigned int maddr){
  int wordcnt;
  int tdata;
  
  wordcnt = 0;
  vread32(maddr+v7XX_OUTBUFF,&tdata);

  if((tdata & v7XX_TYPE_MASK) != v7XX_HEADER_BIT){
    return 0;
  }
  
  babies_segdata((char *)&tdata, sizeof(tdata));
  wordcnt++;

  while(wordcnt < 34){
    vread32(maddr+v7XX_OUTBUFF,(int *)(babies_segpt32()));
    wordcnt++;
    if((*(int *)(babies_pt()-4) & (v7XX_TYPE_MASK)) != v7XX_DATA_BIT){
      break;
    }
  }

  return 1;
}


void v7XX_clear(unsigned int maddr){
  short sval;
 
  sval = 0x04;
 
  vwrite16(maddr + v7XX_BIT_SET2, &sval);
  vwrite16(maddr + v7XX_BIT_CLE2, &sval);
 
}

void v7XX_intlevelmulti(unsigned int maddr, short level, short evtn){
  vwrite16(maddr + v7XX_EVT_TRIG_REG, &evtn);
  vwrite16(maddr + v7XX_INT_REG1, &level);
}

void v7XX_intlevel(unsigned int maddr, short level){
  v7XX_intlevelmulti(maddr, level, 1);
}

// cnt = word count in 32bit data
int v7XX_dmasegdata(unsigned int maddr, int cnt){
  int sz, rsz;

  sz = cnt *4;

  rsz = dma_vread32(maddr, babies_pt(), sz);
  babies_segptfx(rsz);

  return rsz/2;
}

void v7XX_multievtberr(unsigned int maddr){
  short sval = 0x20;

  vwrite16(maddr + v7XX_CTRL_REG1, &sval);
}

void v7XX_singleevtberr(unsigned int maddr){
  short sval = 0x24;

  vwrite16(maddr + v7XX_CTRL_REG1, &sval);
}

void v7XX_noberr(unsigned int maddr){
  short sval = 0x00;

  vwrite16(maddr + v7XX_CTRL_REG1, &sval);
}

void v7XX_mcstctrl(unsigned int maddr, short sval){
  // v = 0 : disable
  // v = 1 : last
  // v = 2 : first
  // v = 3 : intermediate

  vwrite16(maddr + v7XX_MCST_CTRL, &sval);

}

void v7XX_mcstaddr(unsigned int maddr, short sval){
 
  vwrite16(maddr + v7XX_MCST_ADDR, &sval);

}
