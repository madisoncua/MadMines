// FIFO2.cpp
// Runs on any microcontroller
// Provide functions that initialize a FIFO, put data in, get data out,
// and return the current size.  The file includes a transmit FIFO
// using index implementation and a receive FIFO using pointer
// implementation.  Other index or pointer implementation FIFOs can be
// created using the macros supplied at the end of the file.
// Created: 1/16/2020 
// Student names: Evan Roberts & Madison Cua
// Last modification date: 4/2/2025


/* This example accompanies the book
   "Embedded Systems: Real Time Interfacing to the Arm Cortex M3",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2011
   Programs 3.7, 3.8., 3.9 and 3.10 in Section 3.7

 Copyright 2019 by Jonathan W. Valvano, valvano@mail.utexas.edu
    You may use, edit, run or distribute this file
    as long as the above copyright notice remains
 THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 VALVANO SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 For more information about my classes, my research, and my books, see
 http://users.ece.utexas.edu/~valvano/
 */
#include <stdint.h>

#include "../inc/FIFO2.h"
#include "../inc/ST7735.h"

// A class named Queue that defines a FIFO
Queue::Queue(){
  PutI = 0;
  GetI = 0;
  // Constructor - set PutI and GetI as 0. 
  // We are assuming that for an empty Queue, both PutI and GetI will be equal
    // add code here to initialize on creation
}

// To check whether Queue is empty or not
bool Queue::IsEmpty(void){
    return GetI==PutI;  // replace this with solution
}

  // To check whether Queue is full or not
bool Queue::IsFull(void){
  return (((PutI+1)%FIFOSIZE) == GetI);
}

  // Inserts an element in queue at rear end
bool Queue::Put(char x){
    if(IsFull()){
      return false;
    }
    Buf[PutI++] = x;
    PutI %=FIFOSIZE;
    return true;  // replace this with solution

}

  // Removes an element in Queue from front end. 
bool Queue::Get(char *pt){
    if(IsEmpty()){
      return false;
    }
    *pt = Buf[GetI++];
    GetI %= FIFOSIZE;
    return true;   // replace this with solution

}

  /* 
     Printing the elements in queue from front to rear. 
     This function is only to test the code. 
     This is not a standard function for Queue implementation. 
  */
void Queue::Print(void){
    // Finding number of elements in queue  
    // output to ST7735R
    ST7735_OutChar('[');
    for(int i=GetI; i<PutI-1; i = (i+1)%FIFOSIZE){
      ST7735_OutChar(Buf[i]);
      ST7735_OutChar(' ');
      ST7735_OutChar(',');
    }
    ST7735_OutChar(Buf[PutI-1]);
    ST7735_OutChar(']');
}

