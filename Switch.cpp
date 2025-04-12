/*
 * Switch.cpp
 *
 *  Created on: Nov 5, 2023
 *      Author:
 *      Edited: Evan Roberts
 */
#include <ti/devices/msp/msp.h>
#include "../inc/LaunchPad.h"
// LaunchPad.h defines all the indices into the PINCM table
void Switch_Init(void){
    // write this
  IOMUX->SECCFG.PINCM[PA24INDEX] = 0x00040081; // input, pick up/put down
  IOMUX->SECCFG.PINCM[PA25INDEX] = 0x00040081; // input, interaction button
}
// return current state of switches
uint8_t Switch_In(void){
    // write this
  uint32_t data = GPIOA->DIN31_0;
  data>>= 19;
  return data&0x60; // return buttons left and right in bits 5 & 6 respectively
}
