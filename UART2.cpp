/* UART2.cpp
 * Evan Roberts
 * Data: 04/01/2025
 * PA22 UART2 Rx from other microcontroller PA8 IR output<br>
 */


#include <ti/devices/msp/msp.h>
#include "UART2.h"
#include "../inc/Clock.h"
#include "../inc/LaunchPad.h"
#include "../inc/FIFO2.h"
//extern uint32_t ReceiveCount;
uint32_t LostData;
Queue FIFO2;

// power Domain PD0
// for 80MHz bus clock, UART2 clock is ULPCLK 40MHz
// initialize UART2 for 2375 baud rate
// no transmit, interrupt on receive timeout
void UART2_Init(void){
  LostData = 0;
  UART2->GPRCM.RSTCTL = 0xB1000003;//turn off power
  UART2->GPRCM.PWREN = 0x26000001;//turn on power
  Clock_Delay(24);
  IOMUX->SECCFG.PINCM[PA22INDEX] = 0x00040082; //enable PA22
  UART2->CLKSEL = 0x08; //bus clock
  UART2->CLKDIV = 0; //no division
  UART2->CTL0 &= ~0x01; //disable UART2
  UART2->CTL0 = 0x00020018; //enable fifo 
  //setting baud rate
  UART2->IBRD = 1052; //integer part of cycles/sample of a stable bit
  UART2->FBRD = 40; //decimal part of cycles/sample --> resolution of 2^-6 (0.631579)
  UART2->LCRH = 0x30; // 8 bit, 1 stop, no parity
  UART2->CPU_INT.IMASK = 0x401; 
  UART2->IFLS |= 0x422;//setting bits 8-11

  NVIC->ICPR[0] = 1<<14; //UART2
  NVIC->ISER[0] = 1<<14;
  NVIC->IP[3] = (NVIC->IP[3]&(~(3<<22))) | (2<<22); //priority bits
  UART2->CTL0 |= 0x1; //ENABLE UART2 
    // RSTCLR to GPIOA and UART2 peripherals
   // write this
}

void UART2_Disable(void){
  UART2->CTL0 &= ~(0x1);
}

void UART2_Enable(void){
  UART2->CTL0 |= 0x1;
}

//------------UART2_InChar------------
// Get new serial port receive data from FIFO2
// Input: none
// Output: Return 0 if the FIFO2 is empty
//         Return nonzero data from the FIFO1 if available
char UART2_InChar(void){
// write this
char letter;
return (FIFO2.Get(&letter))? letter : 0;
}

extern "C" void UART2_IRQHandler(void);
void UART2_IRQHandler(void){ uint32_t status; char letter;
  status = UART2->CPU_INT.IIDX; // reading clears bit in RTOUT
  if(status == 0x01){   // 0x01 receive timeout
    GPIOB->DOUTTGL31_0 = BLUE; // toggle PB22 (minimally intrusive debugging)
    GPIOB->DOUTTGL31_0 = BLUE; // toggle PB22 (minimally intrusive debugging)
    char data;
    while((UART2->STAT&(0x4))==0){
      data = (UART2->RXDATA)&(0xFF);
      if(!(FIFO2.Put(data))){
        LostData++;
      }
      //ReceiveCount++;
    }

    GPIOB->DOUTTGL31_0 = BLUE; // toggle PB22 (minimally intrusive debugging)
  }
}