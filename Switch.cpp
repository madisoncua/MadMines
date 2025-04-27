/*
 * Switch.cpp
 *
 *  Created on: Nov 5, 2023
 *      Author:
 *      Edited: Evan Roberts
 */
#include <ti/devices/msp/msp.h>
#include "LaunchPad.h"
// LaunchPad.h defines all the indices into the PINCM table
void Switch_Init(void){
    // LaunchPad_Init has been called so power should be reset to ports
  IOMUX->SECCFG.PINCM[PA24INDEX] = 0x40081; // input, pick up/put down
  IOMUX->SECCFG.PINCM[PA25INDEX] = 0x40081; // input, interaction button
}
// return current state of switches
uint8_t Switch_In(void){
  uint32_t data = GPIOA->DIN31_0;
  data>>= 19;
  return (uint8_t) (data&0x60); // return buttons left and right in bits 5 & 6 respectively
}
