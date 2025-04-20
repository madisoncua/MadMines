// Sound.cpp
// Runs on MSPM0
// Sound assets in sounds/sounds.h
// your name
// your data 
#include <ti/devices/msp/msp.h>
#include "../inc/LaunchPad.h"
#include "../inc/Clock.h"
#include "../inc/UART.h"
#include "../inc/Timer.h"
#include "../inc/DAC5.h"  // student's Lab 5
#include <stdio.h>
#include <string.h>
#include "Sound.h"

enum Flags{NOTHING, CLANG, CORRECT, WRONG};
uint8_t sound_flag;
int32_t soundIndex;
const uint8_t* soundArr;
uint16_t soundLength;
uint16_t wrong_size = 3971;
uint16_t correct_size = 17616;
uint16_t clang_size = 7936;

// void SysTick_IntArm(uint32_t period, uint32_t priority){
//   // write this
// }
// initialize a 11kHz SysTick, however no sound should be started
// initialize any global variables
// Initialize the 5 bit DAC
void Sound_Init(void){//priority = 0, period = 8370
  SysTick->CTRL = 0x0; //turn off all interrupts during initialization
  SysTick->LOAD = 7273-1; //sets the timer to our period for given note
  SysTick->VAL = 0;
  SCB->SHP[1] = SCB->SHP[1]&(~0xC0000000);
  SCB->SHP[1] |= (1<<30);
  SysTick->CTRL = 7;
  // write this
}
extern "C" void SysTick_Handler(void);

//******* Sound_Start ************
// This function does not output to the DAC. 
// Rather, it sets a pointer and counter, and then enables the SysTick interrupt.
// It starts the sound, and the SysTick ISR does the output
// feel free to change the parameters
// Sound should play once and stop
// Input: pt is a pointer to an array of DAC outputs
//        count is the length of the array
// Output: none
// special cases: as you wish to implement
void Sound_Start(uint8_t s){
// write this
  sound_flag = 1;
  soundIndex = 0;
  if(s==CLANG){
     soundArr = clang;
     soundLength = clang_size;
  }else if(s==CORRECT){
    soundArr = correct;
     soundLength = correct_size;
   }else 
  if(s==WRONG){
    soundArr = wrong;
    soundLength = wrong_size;
  }
};

void SysTick_Handler(void){ //place holder until sound set up
  // if(songOn && songIndex < 20112){
  //   DAC_Out(adventuring_song[songIndex++]);
  // //}else if(songIndex < 23000){
  // //  songIndex++;
  // }else{
  //   songIndex = 0;
  // }
  if(sound_flag==0){
    return;//don't output a sound effect if the mode is zero
  }else{
    if(soundIndex < soundLength){
      DAC5_Out(soundArr[soundIndex]);
      soundIndex++;
    }else{
      sound_flag = 0;
    }
  }
};

void Sound_Correct(void){
  Sound_Start(CORRECT);
}
void Sound_Wrong(void){
  Sound_Start(WRONG);
}
void Sound_Clang(void){
  Sound_Start(CLANG);
}
