// Lab9HMain.cpp
// Runs on MSPM0G3507
// Lab 9 ECE319H
// Evan Roberts and Madison Cua
// Last Modified: 4/11/2025
#include <cassert>
#include <stdio.h>
#include <stdint.h>
#include <ti/devices/msp/msp.h>
#include "ST7735.h"
#include "../inc/Clock.h"
#include "../inc/LaunchPad.h"
#include "../inc/TExaS.h"
#include "../inc/Timer.h"
#include "../inc/DAC5.h"
#include "SmallFont.h"
#include "LED.h"
#include "Switch.h"
#include "Sound.h"
#include "images.h"
#include "Player.h"
#include "Machine.h"
#include "SlidePot.h"
#include "IRxmt.h"
#include "UART2.h"
#include "FIFO2.h"
#include "Instructions.h"
#include "ti/devices/msp/m0p/mspm0g350x.h"
#include <string.h>

extern "C" void __disable_irq(void);
extern "C" void __enable_irq(void);
extern "C" void TIMG12_IRQHandler(void);
extern int ToDoArr[5];
// ****note to ECE319K students****
// the data sheet says the ADC does not work when clock is 80 MHz
// however, the ADC seems to work on my boards at 80 MHz
// I suggest you try 80MHz, but if it doesn't work, switch to 40MHz
void PLL_Init(void){ // set phase lock loop (PLL)
  // Clock_Init40MHz(); // run this line for 40MHz
  Clock_Init80MHz(0);   // run this line for 80MHz
}
//Queue FIFO;   //unsure if this is necessary
SlidePot Sensor; // copy calibration from Lab 7
uint8_t buttons;
int8_t menuOpen;
int8_t toDoOpen;

// games  engine runs at 30Hz
void TIMG12_IRQHandler(void){uint32_t pos, msg;
  if((TIMG12->CPU_INT.IIDX) == 1){ // this will acknowledge
    //GPIOB->DOUTTGL31_0 = GREEN; // toggle PB27 (minimally intrusive debugging)
    //GPIOB->DOUTTGL31_0 = GREEN; // toggle PB27 (minimally intrusive debugging)
// game engine goes here
    // 1) sample joystick
    pos = Sensor.In();
    Sensor.Save(pos);
    // 2) read input switches
    buttons = Switch_In();
    // 4) start sounds
    // 5) set semaphore
    //GPIOB->DOUTTGL31_0 = GREEN; // toggle PB27 (minimally intrusive debugging)
  }
}
uint8_t TExaS_LaunchPadLogicPB27PB26(void){
  return (0x80|((GPIOB->DOUT31_0>>26)&0x03));
}

uint8_t checkWin(void){
  for (int i=0; i<5; i++){
    if(ToDoArr[i]!=0){
      return 0;
    }
  }
  return 1;
}


// use main1 to observe special characters
void setUpInstructions(uint8_t mode){ // main1
  static uint8_t currOption = 0;
  switch(mode){
    case 0:
    ST7735_SetCursor(1, 1);
  ST7735_OutString((char *)"Language/Idioma"); //english
  ST7735_SetCursor(1, 3);
  ST7735_OutString((char *)Phrases[2][0]); //english
  ST7735_SetCursor(1, 5);
  ST7735_OutString((char *)Phrases[2][1]); //spanish


  while((buttons&0x20)==0){
    Sensor.Sync(); //checks for semaphore to be set that interrupt has occured
    uint32_t vert = Sensor.DistanceY();
    uint8_t change = 0;
    if(vert > 3000){ //move up
      if(currOption!=0){//option for english 
       //replaces the highlight on the last one
        ST7735_FillRect(3, 58, 45, 2, 0x630C); //highlights this language 
        ST7735_SetCursor(1, 5);
        ST7735_OutString((char *)Phrases[2][1]); //spanish
        //highlights new option
        ST7735_FillRect(3, 38, 45, 2, 0x475F); //highlights this language 
        ST7735_SetCursor(1, 3);
        ST7735_OutString((char *)Phrases[2][0]); //english
        currOption = 0;
      }
    }
    if(vert < 1000){ //mpve down
      if(currOption!=1){
        //replaces the highlight on the last one
        ST7735_FillRect(3, 38, 45, 2, 0x630C); //highlights this language 
        ST7735_SetCursor(1, 3);
        ST7735_OutString((char *)Phrases[2][0]); //english
        //highlights new option
        ST7735_FillRect(3, 58, 45, 2, 0x475F); //highlights this language 
        ST7735_SetCursor(1, 5);
        ST7735_OutString((char *)Phrases[2][1]); //spanish
        currOption = 1;
      }
    }
  }
  for(int i=0; i<5; i++){//4 pages of instructions
    ST7735_FillScreen(0x630C);//set screen grey
    Clock_Delay1ms(10);
    Sensor.Sync();

    ST7735_SetCursor(0, 0);
    ST7735_OutString((char *)Instructions[currOption][i]);
    while(buttons==0){
      Sensor.Sync();
      if((buttons&0x20)>0){
        if(i>0){
          i-=2;
        }else{
          i-=1;
        }
      }
    }
  }
  return;
  case 1: //this is for printing at the end
   ST7735_SetCursor(7, 4);
   ST7735_OutString((char *)Phrases[3][currOption]); //
   return;
   case 2:
    ST7735_SetCursor(7, 4);
    ST7735_OutString((char *)Phrases[4][currOption]);
    return;  
  }
}

void randomizeOrders(void){
  for (int i = 0; i < 5; i++) {
    ToDoArr[i] = SysTick->VAL%3;//0-2
  }
}

void printScore(int16_t score, uint8_t x_cursor, uint8_t y_cursor, uint8_t fontSize){
  bool isNeg = false;
    if(score<0){
        isNeg = true;
        score*= -1;
    }
    while(score!=0){
      ST7735_DrawChar(x_cursor, y_cursor, (score%10)+48, 0xFFFF, 0x630C, fontSize);
      score /=10;
      x_cursor-=10;
    }
    if(isNeg){
      ST7735_DrawChar(x_cursor, y_cursor, '-', 0xFFFF, 0x630C, fontSize);
    }
}

Player p1; //player 1
//(top_left_x, top_left_y, bot_right_x, bot_right_y, progX, progY, proXL, proXR, proYT, proYB)
Machine m_refiner(67, 10, 121, 45, 61, 15);
Machine m_portal(65, 135, 103, 159, 0, 0);
Machine m_cart1(5, 8, 51, 54, 5, 51, 8, 54, 3);
Machine m_rock1(88, 114, 132, 135, 104, 94, 88, 127, 101, 135);
Machine m_rock1Mid(99, 108, 129, 114, 0, 0);
Machine m_rock1Top(108, 101, 127, 108, 0, 0);
Machine m_rock1Progress(104, 94, 126, 114, 0, 0);
//(top_left_x, top_left_y, bot_right_x, bot_right_y, proXL, proXR, proYT, proYB, state)
Machine m_todo(0, 0, 32, 159, 0, 32, 90, 159, 3);

uint8_t numCounters = 3;
//(top_left_x, top_left_y, bot_right_x, bot_right_y, proXL, proXR, proYT, proYB, state)
Machine m_counter1(0, 75, 28, 99, 0, 30, 85, 89, 1);
Machine m_counter2(0, 99, 28, 123, 0, 30, 109, 110, 1);
Machine m_counter3(0, 123, 28, 147, 0, 30, 133, 135, 1);
Machine Counters[4] = {m_todo, m_counter1, m_counter2, m_counter3};
Machine m_todoDown(0, 149, 32, 159, 0, 0);
uint8_t input = 0;
uint8_t deadTimer = 0;

Machine* machineArr1[12] = {&m_refiner, &m_portal, &m_rock1, &m_cart1, &m_todo, &m_counter1, &m_counter2, &m_counter3, &m_todoDown, &m_rock1Mid, &m_rock1Top, &m_rock1Progress};
int16_t score;
// ALL ST7735 OUTPUT MUST OCCUR IN MAIN
int mainP1(void){ // THIS IS THE PLAYER 1 WITH REFINER, SMELTER, AND ORDER WINDOW
//initializations
  __disable_irq();
  PLL_Init(); // set bus speed
  LaunchPad_Init();
  ST7735_InitPrintf();
  DAC5_Init();
    //note: if you colors are weird, see different options for
    // ST7735_InitR(INITR_REDTAB); inside ST7735_InitPrintf()
  ST7735_FillScreen(0x630C);//set screen grey
  Sensor.Init(); // PB18 = ADC1 channel 5, slidepot
  Switch_Init(); // initialize switches PA24, PA25  
  //LED_Init();    // initialize LED
  Sound_Init();  // initialize sound
  TExaS_Init(0,0,&TExaS_LaunchPadLogicPB27PB26); // PB27 and PB26
  //Wireless Inits
  IRxmt_Init();   //transmitter PA8
  UART2_Init();   //just receive, PA22, receiver timeout synchronization
    // initialize interrupts on TimerG12 at 30 Hz
  TimerG12_IntArm(2666666, 2);//2666666
  // initialize all data structures
  __enable_irq();
  setUpInstructions(0); //does the intro screen

  ST7735_FillScreen(0x630C);//set screen grey
  ST7735_SetCursor(0, 1);
  ST7735_OutString((char*)"Waiting for Player 2");
  uint8_t acknowledge = 0;
  score = 0;
  char startMsg[4] = {'1', '2', '3' ,'4'};
  while(!acknowledge){
    UART2_Disable();
    for (int i = 0; i < 4; i++) {
      IRxmt_OutChar(startMsg[i]);
    }
    UART2_Enable();
    Clock_Delay1ms(10);
    char c1, c2, c3, c4;
    c1 = UART2_InChar();
    while(c1){
      c2 = UART2_InChar();
      c3 = UART2_InChar();
      c4 = UART2_InChar();
      uint8_t count = 0;
      if(c1 == 'a')count++;
      if(c2 == 'b')count++;
      if(c3 == 'c')count++;
      if(c4 == 'd')count++;
      if(count >= 2){   //acknowledge received that player 2 is ready to start the game
        acknowledge = 1;
        break;
      }
      c1 = UART2_InChar();  //continue if there's more messages
    }
  }
  randomizeOrders();
  ST7735_FillScreen(0x630C);//set screen grey
  Clock_Delay1ms(10); //this is a guess to delay until the other one starts
  /////////////begin game///////////////////
  m_rock1.setRockType(1);//only gives metal (silver or gold)
  m_cart1.setSprite(4);
  m_cart1.printCart();
  ST7735_DrawFastHLine(106, 138, 22, 0x0);   //trying to make a box outline in the corner
  ST7735_DrawFastVLine(106, 138, 22, 0x0);
  ST7735_DrawFastHLine(105, 137, 24, 0x0);   //thickening box
  ST7735_DrawFastVLine(105, 137, 24, 0x0);
  uint8_t cursorStart = 1;
  uint8_t scoreStart = 64;
  uint8_t scoreY = 2;
  uint8_t letterOffset = 6;
  uint16_t color = 0xFFFF;
  ST7735_DrawChar(scoreStart, scoreY, 'S', color, 0x630C, 1);
  ST7735_DrawChar(scoreStart+letterOffset, scoreY, 'C', color, 0x630C, 1);
  ST7735_DrawChar(scoreStart+letterOffset*2, scoreY, 'O', color, 0x630C, 1);
  ST7735_DrawChar(scoreStart+letterOffset*3, scoreY, 'R', color, 0x630C, 1);
  ST7735_DrawChar(scoreStart+letterOffset*4, scoreY, 'E', color, 0x630C, 1);
  ST7735_DrawChar(scoreStart+letterOffset*5, scoreY, ':', color, 0x630C, 1);
  ST7735_DrawChar(scoreStart+letterOffset*6, scoreY, ' ', color, 0x630C, 1);
  ST7735_DrawBitmap(0, 159, todo+4800, 32, 10); //draws the to do button at the bottom
  ST7735_DrawBitmap(p1.getXPos(), p1.getYPos(), miner, p1.getSize(), p1.getSize());
  int16_t timer = 5400;
  Sensor.Sync();
  while(timer>=0){
    if(timer%30 == 0 && toDoOpen == 0){
      ST7735_FillRect(0, 0, 40, 10, 0x630C);
      uint16_t seconds = timer/30;
      uint16_t minute = (seconds/60);
      seconds %= 60;
      ST7735_DrawChar(cursorStart, scoreY, minute+48, color, 0x630C, 1); //gets the minute
      ST7735_DrawChar(cursorStart+letterOffset, scoreY, ':', color, 0x630C, 1);
      ST7735_DrawChar(cursorStart+letterOffset*2, scoreY, (seconds/10)+48, color, 0x630C, 1);
      ST7735_DrawChar(cursorStart+letterOffset*3, scoreY, (seconds%10)+48, color, 0x630C, 1);
    }
    if(deadTimer > 0){
      if(deadTimer%30 == 0){
        ST7735_DrawChar(61, 74, deadTimer/30+48, 0x1F, 0x630C, 2);
      }
      deadTimer--;
      if(deadTimer == 0){
        ST7735_DrawBitmap(p1.getXPos(), p1.getYPos(), miner, p1.getSize(), p1.getSize());
      }
    }
    Sensor.Sync(); //checks for semaphore to be set that interrupt has occured
    uint32_t vert = Sensor.DistanceY();
    uint32_t horiz = Sensor.DistanceX();
    uint8_t change = 0;
    timer--;
    if(deadTimer == 0){
      //buttons is global variable with PA25 and PA24 in bits 6-5
      //updating player graphics 
      bool canRight = 0, canLeft = 0, canUp = 0, canDown = 0;
      if(horiz < 1000){
        int16_t oldX = p1.getXPos();
        int16_t oldY = p1.getYPos();
        change |= p1.moveRight();
        int16_t newX = p1.getXPos();
        int16_t newY = p1.getYPos();
        p1.resetCoordinates(&newX, &newY);
        if(change && p1.inBounds(newX, newY, machineArr1, 11, 1)){
          canRight = 1;
          p1.setXPos(newX);
          p1.setYPos(newY);
        }else{ //put the coordinates back where they were
          p1.setXPos(oldX);
          p1.setYPos(oldY);
        }
      }
      if(horiz > 3000){
        int16_t oldX = p1.getXPos();
        int16_t oldY = p1.getYPos();
        change |= p1.moveLeft();
        int16_t newX = p1.getXPos();
        int16_t newY = p1.getYPos();
        p1.resetCoordinates(&newX, &newY);
        if(change && p1.inBounds(newX, newY, machineArr1, 11, 1)){
          canLeft = 1;
          p1.setXPos(newX);
          p1.setYPos(newY);
        }else{ //put the coordinates back where they were
          p1.setXPos(oldX);
          p1.setYPos(oldY);
        }
      }
      if(vert > 3000){
        int16_t oldX = p1.getXPos();
        int16_t oldY = p1.getYPos();
        change |= p1.moveUp();
        int16_t newX = p1.getXPos();
        int16_t newY = p1.getYPos();
        p1.resetCoordinates(&newX, &newY);
        if(change && p1.inBounds(newX, newY, machineArr1, 11, 1)){
          canUp = 1;
          p1.setXPos(newX);
          p1.setYPos(newY);
        }else{ //put the coordinates back where they were
          p1.setXPos(oldX);
          p1.setYPos(oldY);
        }
      }
      if(vert < 1000){
        int16_t oldX = p1.getXPos();
        int16_t oldY = p1.getYPos();
        change|= p1.moveDown();
        int16_t newX = p1.getXPos();
        int16_t newY = p1.getYPos();
        p1.resetCoordinates(&newX, &newY);
        if(change && p1.inBounds(newX, newY, machineArr1, 11, 1)){
          canDown = 1;
          p1.setXPos(newX);
          p1.setYPos(newY);
        }else{ //put the coordinates back where they were
          p1.setXPos(oldX);
          p1.setYPos(oldY);
        }
      }

      if(canUp){
        ST7735_SetRotation(0);
        ST7735_DrawBitmap(p1.getXPos(), p1.getYPos(), miner, p1.getSize(), p1.getSize());
      }else if(canDown){
        p1.translateDown();
        ST7735_DrawBitmap(p1.getXPos(), p1.getYPos(), miner, p1.getSize(), p1.getSize());
        int16_t newX = p1.getXPos();
        int16_t newY = p1.getYPos();
        p1.resetCoordinates(&newX, &newY);
        p1.setXPos(newX);
        p1.setYPos(newY);
      }else if(canRight){
        p1.translateRight();
        ST7735_DrawBitmap(p1.getXPos(), p1.getYPos(), miner, p1.getSize(), p1.getSize());
        int16_t newX = p1.getXPos();
        int16_t newY = p1.getYPos();
        p1.resetCoordinates(&newX, &newY);
        p1.setXPos(newX);
        p1.setYPos(newY);
      }else if(canLeft){
        p1.translateLeft();
        ST7735_DrawBitmap(p1.getXPos(), p1.getYPos(), miner, p1.getSize(), p1.getSize());
        int16_t newX = p1.getXPos();
        int16_t newY = p1.getYPos();
        p1.resetCoordinates(&newX, &newY);
        p1.setXPos(newX);
        p1.setYPos(newY);
      }
      ST7735_SetRotation(0);
    }
    //refiner
    int8_t machineOut = -1;
    input = p1.getMachineInput(m_refiner); //get the input for refiner
    input |= buttons;
    machineOut = m_refiner.updateRefiner(input);  //update refiner
    if(machineOut > -1){
      if(machineOut == 0){
        if(p1.getYPos() < 62){
          p1.setYPos(62);
          ST7735_FillRect(38, 10, 23, 51, 0x630C);
          ST7735_DrawBitmap(p1.getXPos(), p1.getYPos(), miner, p1.getSize(), p1.getSize());
        }
      }
      p1.setPossession(machineOut);
      p1.printPosession(machineOut);
    }

    input = p1.getMachineInput(m_rock1);
    input |= buttons;
    machineOut = m_rock1.updateRock(input);
    if(machineOut > -1){
      if(machineOut == 80){
        if(p1.getYPos() > 93 && p1.getXPos() > 78){
          if(p1.getYPos() < 102){
            p1.setYPos(93);
            ST7735_FillRect(84, 94, 20, 5, 0x630C);
            ST7735_DrawBitmap(p1.getXPos(), p1.getYPos(), miner, p1.getSize(), p1.getSize());
          }else{
            p1.setXPos(78);
            ST7735_FillRect(104, 84, 4, 10, 0x630C);
            ST7735_DrawBitmap(p1.getXPos(), p1.getYPos(), miner, p1.getSize(), p1.getSize());
          }
        }
      }else{
        p1.setPossession(machineOut);
        p1.printPosession(machineOut);
      }
    }

    //does cart
    input = p1.getMachineInput(m_cart1);
    input |= buttons;
    machineOut = m_cart1.updateCart(input);
    if(machineOut > -1){
      if(machineOut == 80){
        if(p1.getYPos() <= 80 && p1.getXPos() <= 51){
          ST7735_FillRect(p1.getXPos(), p1.getYPos()-p1.getSize(), p1.getSize(), p1.getSize(), 0x630C);
          p1.setXPos(42);
          p1.setYPos(102);
          deadTimer = 90;
        }
      }else{
        p1.setPossession(machineOut);
        p1.printPosession(machineOut);
      }
    }

    input = p1.getMachineInput(m_portal);
    input|= buttons;
    machineOut = m_portal.updateTurnInArea(input);
    if(machineOut > -1){
      p1.setPossession(machineOut);
      p1.printPosession(machineOut);
    }
      input = p1.getMachineInput(Counters[0]);
      input|= buttons;
      machineOut = Counters[0].updateCounters(input, Counters); //this is the todo
      if(machineOut==50){
        if(p1.getXPos() < 34){
          p1.setXPos(34);
          ST7735_DrawBitmap(p1.getXPos(), p1.getYPos(), miner, p1.getSize(), p1.getSize());
        }
        continue;
      }else if(machineOut==22){
        m_cart1.menuPrintCart();
        ST7735_DrawBitmap(p1.getXPos(), p1.getYPos(), miner, p1.getSize(), p1.getSize());
        for(int i=1; i<numCounters+1; i++){
          Counters[i].setSprite(4);
          Counters[i].printCounters(Counters);
        }
      }else{
          //does counters
          for(int i=1; i<numCounters+1; i++){
          input = p1.getMachineInput(Counters[i]);
          input|= buttons;
          machineOut = Counters[i].updateCounters(input, Counters);
          if(machineOut>-1 && machineOut<50){//player grabbed something from the main
            p1.setPossession(machineOut);
            p1.printPosession(machineOut);
          }
        }
      }
  }
  if(checkWin()==1){
  ST7735_FillScreen(0x630C);
  setUpInstructions(1);
  startMsg[2] = 0x0F; //won
  }else{
  ST7735_FillScreen(0x630C);
  setUpInstructions(2);
  startMsg[2] = 0xF0; //lost
 }
 printScore(score, 70, 80, 2);

 startMsg[0] = 167;
 startMsg[1] = (score<<8);
 startMsg[3] = (score&0xFF);
    while(1){///sends score to other player
    UART2_Disable();
    for (int i = 0; i < 4; i++) {
      IRxmt_OutChar(startMsg[i]);
    }
    UART2_Enable();
    Clock_Delay1ms(10);
  }
  while(1){}
  return 0;
}

//uint8_t TLX, uint8_t TLY, uint8_t BRX, uint8_t BRY, uint8_t PBX, uint8_t PBY, uint8_t XL, uint8_t XR, uint8_t YT, uint8_t YB
Machine m_smelter(89, 80, 127, 134, 103, 72);
Machine m_anvil(31, 130, 98, 159, 25, 136, 70, 90, 130, 159); 
Machine m_rock2(67, 21, 111, 42, 113, 17);
Machine m_rock2Mid(78, 15, 108, 21, 0, 0);
Machine m_rock2Top(87, 8, 106, 15, 0, 0);
Machine m_cart2(5, 8, 51, 54, 0, 0);//51 should be 36 if it's the ladder
Machine m_smelterProgress(103, 72, 125, 78, 0, 0);

//(top_left_x, top_left_y, bot_right_x, bot_right_y, proXL, proXR, proYT, proYB, state)
Machine m_counter4(0, 60, 28, 84, 0, 35, 65, 74, 1);
Machine m_counter5(0, 84, 28, 108, 0, 35, 89, 90, 1);
Machine m_counter6(0, 108, 28, 132, 0, 35, 120, 125, 1);
Machine* machineArr2[10] = {&m_smelter, &m_anvil, &m_rock2, &m_cart2, &m_counter4, &m_counter5, &m_counter6, &m_rock2Mid, &m_rock2Top, &m_smelterProgress};

Machine Counters2[3] = {m_counter4, m_counter5, m_counter6};
int main(void){ // THIS IS THE PLAYER 2 WITH ROCKS AND ANVIL
//initializations
  __disable_irq();
  PLL_Init(); // set bus speed
  LaunchPad_Init();
  ST7735_InitPrintf();
  DAC5_Init();
    //note: if you colors are weird, see different options for
    // ST7735_InitR(INITR_REDTAB); inside ST7735_InitPrintf()
  ST7735_FillScreen(0x630C);
  Sensor.Init(); // PB18 = ADC1 channel 5, slidepot
  Switch_Init(); // initialize switches PA24, PA25
  //Wireless Inits
  IRxmt_Init();   //transmitter PA8
  UART2_Init();   //just receive, PA22, receiver timeout synchronization
  UART2_Disable();
  //LED_Init();    // initialize LED
  Sound_Init();  // initialize sound
  TExaS_Init(0,0,&TExaS_LaunchPadLogicPB27PB26); // PB27 and PB26
    // initialize interrupts on TimerG12 at 30 Hz
  TimerG12_IntArm(2666666, 2);
  // initialize all data structures
  __enable_irq();

  setUpInstructions(0); //does the intro screen
  ST7735_FillScreen(0x630C);
  UART2_Enable();
  ST7735_SetCursor(0, 1);
  ST7735_OutString((char*)"Waiting for Player 1");
  uint8_t startGame = 0;

  char c1, c2, c3, c4;
  
  while(!startGame){  //continue until it sees message to start
    c1 = UART2_InChar();
    c2 = UART2_InChar();
    c3 = UART2_InChar();
    c4 = UART2_InChar();
    uint8_t count = 0;
    if(c1 == '1')count++;
    if(c2 == '2')count++;
    if(c3 == '3')count++;
    if(c4 == '4')count++;
    if(count >= 2){   //acknowledge received that player 2 is ready to start the game
      startGame = 1;
    }
  }


  startGame = 0;  //now wait until transmitter stops sending stuff to know it also saw acknowledge
  char ackMsg[4] = {'a', 'b', 'c', 'd'};
  while(!startGame){
    UART2_Disable();
    for (int i = 0; i < 4; i++) {
      IRxmt_OutChar(ackMsg[i]);
    }
    UART2_Enable();
    Clock_Delay1ms(7);
    c1 = UART2_InChar();
    c2 = UART2_InChar();
    if(c1 == 0 && c2 == 0){ //FIFO is empty and transmitter has started game
      startGame = 1;
    }
  }
ST7735_FillScreen(0x630C);
  ///////////begin game//////////////////////////
  menuOpen = 0;
  toDoOpen = 0;
  uint8_t cursorStart = 1;
  uint8_t scoreY = 2;
  uint8_t letterOffset = 6;
  uint16_t color = 0xFFFF;
  ST7735_FillScreen(0x630C);
  
  m_rock2.setRockType(0);//this rock only return gems
  //ST7735_DrawBitmap(67, 42, rock, 44, 34);
  ST7735_DrawFastHLine(106, 138, 22, 0x0);   //trying to make a box outline in the corner
  ST7735_DrawFastVLine(106, 138, 22, 0x0);
  ST7735_DrawFastHLine(105, 137, 24, 0x0);   //thickening box
  ST7735_DrawFastVLine(105, 137, 24, 0x0);
  int16_t timer = 5400;
  ST7735_DrawBitmap(p1.getXPos(), p1.getYPos(), miner, p1.getSize(), p1.getSize());
  Sensor.Sync();
  while(timer>=0){
    if(timer%30 == 0){
      ST7735_FillRect(0, 0, 60, 11, 0x630C);
      uint16_t seconds = timer/30;
      uint16_t minute = (seconds/60);
      seconds %= 60;
      ST7735_DrawChar(cursorStart, scoreY, minute+48, color, 0x630C, 1); //gets the minute
      ST7735_DrawChar(cursorStart+letterOffset, scoreY, ':', color, 0x630C, 1);
      ST7735_DrawChar(cursorStart+letterOffset*2, scoreY, (seconds/10)+48, color, 0x630C, 1);
      ST7735_DrawChar(cursorStart+letterOffset*3, scoreY, (seconds%10)+48, color, 0x630C, 1);
    }
    if(deadTimer > 0){
      if(deadTimer%30 == 0){
        ST7735_DrawChar(61, 74, deadTimer/30+48, 0x1F, 0x630C, 2);
      }
      deadTimer--;
      if(deadTimer == 0){
        ST7735_DrawBitmap(p1.getXPos(), p1.getYPos(), miner, p1.getSize(), p1.getSize());
      }
    }

    Sensor.Sync(); //checks for semaphore to be set that interrupt has occured
    uint32_t vert = Sensor.DistanceY();
    uint32_t horiz = Sensor.DistanceX();
    uint8_t change = 0;
    timer--;
    if(!menuOpen && deadTimer == 0){
      bool canRight = 0, canLeft = 0, canUp = 0, canDown = 0;
      if(horiz < 1000){
        int16_t oldX = p1.getXPos();
        int16_t oldY = p1.getYPos();
        change |= p1.moveRight();
        int16_t newX = p1.getXPos();
        int16_t newY = p1.getYPos();
        p1.resetCoordinates(&newX, &newY);
        if(change && p1.inBounds(newX, newY, machineArr2, 9, 2)){
          canRight = 1;
          p1.setXPos(newX);
          p1.setYPos(newY);
        }else{ //put the coordinates back where they were
          p1.setXPos(oldX);
          p1.setYPos(oldY);
        }
      }
      if(horiz > 3000){
        int16_t oldX = p1.getXPos();
        int16_t oldY = p1.getYPos();
        change |= p1.moveLeft();
        int16_t newX = p1.getXPos();
        int16_t newY = p1.getYPos();
        p1.resetCoordinates(&newX, &newY);
        if(change && p1.inBounds(newX, newY, machineArr2, 9, 2)){
          canLeft = 1;
          p1.setXPos(newX);
          p1.setYPos(newY);
        }else{ //put the coordinates back where they were
          p1.setXPos(oldX);
          p1.setYPos(oldY);
        }
      }
      if(vert > 3000){
        int16_t oldX = p1.getXPos();
        int16_t oldY = p1.getYPos();
        change |= p1.moveUp();
        int16_t newX = p1.getXPos();
        int16_t newY = p1.getYPos();
        p1.resetCoordinates(&newX, &newY);
        if(change && p1.inBounds(newX, newY, machineArr2, 9, 2)){
          canUp = 1;
          p1.setXPos(newX);
          p1.setYPos(newY);
        }else{ //put the coordinates back where they were
          p1.setXPos(oldX);
          p1.setYPos(oldY);
        }
      }
      if(vert < 1000){
        int16_t oldX = p1.getXPos();
        int16_t oldY = p1.getYPos();
        change|= p1.moveDown();
        int16_t newX = p1.getXPos();
        int16_t newY = p1.getYPos();
        p1.resetCoordinates(&newX, &newY);
        if(change && p1.inBounds(newX, newY, machineArr2, 9, 2)){
          canDown = 1;
          p1.setXPos(newX);
          p1.setYPos(newY);
        }else{ //put the coordinates back where they were
          p1.setXPos(oldX);
          p1.setYPos(oldY);
        }
      }

      if(canUp){
        ST7735_SetRotation(0);
        ST7735_DrawBitmap(p1.getXPos(), p1.getYPos(), miner, p1.getSize(), p1.getSize());
      }else if(canDown){
        p1.translateDown();
        ST7735_DrawBitmap(p1.getXPos(), p1.getYPos(), miner, p1.getSize(), p1.getSize());
        int16_t newX = p1.getXPos();
        int16_t newY = p1.getYPos();
        p1.resetCoordinates(&newX, &newY);
        p1.setXPos(newX);
        p1.setYPos(newY);
      }else if(canRight){
        p1.translateRight();
        ST7735_DrawBitmap(p1.getXPos(), p1.getYPos(), miner, p1.getSize(), p1.getSize());
        int16_t newX = p1.getXPos();
        int16_t newY = p1.getYPos();
        p1.resetCoordinates(&newX, &newY);
        p1.setXPos(newX);
        p1.setYPos(newY);
      }else if(canLeft){
        p1.translateLeft();
        ST7735_DrawBitmap(p1.getXPos(), p1.getYPos(), miner, p1.getSize(), p1.getSize());
        int16_t newX = p1.getXPos();
        int16_t newY = p1.getYPos();
        p1.resetCoordinates(&newX, &newY);
        p1.setXPos(newX);
        p1.setYPos(newY);
      }
      ST7735_SetRotation(0);
    }


    int8_t machineOut = 0;
    //updating anvil
    input = p1.getMachineInput(m_anvil);
    input |= buttons;
    machineOut = m_anvil.updateAnvil(input); 
    if(machineOut > -1){
      if(machineOut == 20){
        m_cart2.printCart();
        ST7735_DrawBitmap(p1.getXPos(), p1.getYPos(), miner, p1.getSize(), p1.getSize());
        m_smelter.printSmelter();
      }else{
        p1.setPossession(machineOut);
        p1.printPosession(machineOut);
      }
    }

    input = p1.getMachineInput(m_smelter);
    input|= buttons;
    machineOut = m_smelter.updateSmelter(input);
    if(machineOut > -1){
      if(machineOut == 80){
        if(p1.getXPos() > 73){
          p1.setXPos(73);
          ST7735_FillRect(72, 43, 55, 25, 0x630C);
          ST7735_DrawBitmap(p1.getXPos(), p1.getYPos(), miner, p1.getSize(), p1.getSize());
        }
        p1.setPossession(0);
        p1.printPosession(0);
      }else{
        p1.setPossession(machineOut);
        p1.printPosession(machineOut);
      }
    }

    input = p1.getMachineInput(m_rock2);
    input |= buttons;
    machineOut = m_rock2.updateRock(input);
    if(machineOut > -1 && machineOut < 10){
      p1.setPossession(machineOut);
      p1.printPosession(machineOut);
    }
    //does cart
    input = p1.getMachineInput(m_cart2);
    input |= buttons;
    machineOut = m_cart2.updateCart(input);
    if(machineOut > -1){
      if(machineOut == 80){
        if(p1.getYPos() <= 80 && p1.getXPos() <= 51){
          ST7735_FillRect(p1.getXPos(), p1.getYPos()-p1.getSize(), p1.getSize(), p1.getSize(), 0x630C);
          p1.setXPos(42);
          p1.setYPos(102);
          deadTimer = 90;
        }
      }else{
        p1.setPossession(machineOut);
        p1.printPosession(machineOut);
      }
    }

    //does counters
    for(int i=0; i<numCounters; i++){
      input = p1.getMachineInput(Counters2[i]);
      input|= buttons;
      machineOut = Counters2[i].updateCounters(input, Counters2);
        if(machineOut>-1 && machineOut<50){//player grabbed something from the main
          p1.setPossession(machineOut);
          p1.printPosession(machineOut);
        }
    }
  }
  int16_t temp = 0;
  while(UART2_InChar() != 167){}
  c2 = UART2_InChar();
  c3 = UART2_InChar();
  c4 = UART2_InChar();
  temp = (c2<<8)+c4;
  
 if(c3==0x0F){
  ST7735_FillScreen(0x630C);
  setUpInstructions(1);
 }else{
  ST7735_FillScreen(0x630C);
  setUpInstructions(2);
 }
  printScore(temp, 70, 80, 2);
  while(1){}
  return -1;
}