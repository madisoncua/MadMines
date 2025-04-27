
/* SlidePot.cpp
 * Students Madison Cua Evan Roberts
 * Modified: put the date here
 * 12-bit ADC input on ADC1 channel 5, PB18
 */
#include <ti/devices/msp/msp.h>
#include "Clock.h"
#include "SlidePot.h"
#define ADCVREF_VDDA 0x000
#define ADCVREF_INT  0x200


void SlidePot::Init(void){
// write code to initialize ADC1 channel 5, PB18
// Your measurement will be connected to PB18
// 12-bit mode, 0 to 3.3V, right justified
// software trigger, no averaging
  // write this
  ADC1->ULLMEM.GPRCM.RSTCTL = 0xB1000003; //RESET
  ADC1->ULLMEM.GPRCM.PWREN = 0x26000001; //ACTIVATE : POWER
  Clock_Delay(24);
  ADC1->ULLMEM.GPRCM.CLKCFG = 0xA9000000;//
  ADC1->ULLMEM.CLKFREQ = 7; //Processor Frequency = 40Mhz
  ADC1->ULLMEM.CTL0 = 0x3010000; //ADC frequency = 40/8
  ADC1->ULLMEM.CTL1 = 1<<16; //mode set - no averaging, software trigger, one sample
  ADC1->ULLMEM.CTL2 = 1<<24; //the digital result is put into the mem register
  ADC1->ULLMEM.MEMCTL[0] = 6; //Channel 4
  ADC1->ULLMEM.MEMCTL[1] = 4;
  ADC1->ULLMEM.SCOMP0 = 0;//Observes analog input for 8 clocks
  ADC1->ULLMEM.CPU_INT.IMASK=0;//Disables Interrupts
}

uint32_t SlidePot::In(void){
  // write code to sample ADC1 channel 4, PB17 once
  // return digital result (0 to 4095)  
  ADC1->ULLMEM.CTL0 |= 0x1; //bit 1 enables conversions
  ADC1->ULLMEM.CTL1 |= 0x100; //bit 3 starts ADC
  uint32_t volatile delay=ADC1->ULLMEM.STATUS;//time to let ADC start
  while((ADC1->ULLMEM.STATUS & 0x1)==0x1){} //waits for status to be low (bit 1)
  return ADC1->ULLMEM.MEMRES[0]+ (ADC1->ULLMEM.MEMRES[1]<<12); //returns value in mem register
}


// constructor, invoked on creation of class
// m and b are linear calibration coefficents
SlidePot::SlidePot(void){
   flag = 0;
}

void SlidePot::Save(uint32_t n){
  distanceX = (n&0xFFF); //put raw horiz and vert into mailboxes
  distanceY = (n>>12);
  flag = 1;
}

void SlidePot::Sync(void){
  // write this
  while(flag==0){} //wait until semaphore 
  flag = 0; //clear semaphore
}


uint16_t SlidePot::DistanceX(void){  // return distance value (0 to 2000), 0.001cm
   return distanceX; // replace this with solution
}

uint16_t SlidePot::DistanceY(void){  // return distance value (0 to 2000), 0.001cm
   return distanceY; // replace this with solution
}