// Lab9HMain.cpp
// Runs on MSPM0G3507
// Lab 9 ECE319H
// Authors: Evan Roberts and Madison Cua
// Last Modified: 4/24/2025
#include <cassert>
#include <stdio.h>
#include <stdint.h>
#include <ti/devices/msp/msp.h>
#include "ST7735.h"
#include "../inc/Clock.h"
#include "../inc/LaunchPad.h"
//#include "../inc/TExaS.h"
#include "../inc/Timer.h"
#include "../inc/DAC5.h"
#include "Switch.h"
#include "Sound.h"
#include "images1.h"
#include "Player.h"
#include "Machine.h"
#include "SlidePot.h"
#include "IRxmt.h"
#include "UART2.h"
#include "FIFO2.h"
#include "Instructions.h"
#include "ti/devices/msp/m0p/mspm0g350x.h"

extern "C" void __disable_irq(void);
extern "C" void __enable_irq(void);
extern "C" void TIMG12_IRQHandler(void);
extern int ToDoArr[5];
extern Player p1;
extern uint8_t toDoOpen;
// ****note to ECE319K students****
// the data sheet says the ADC does not work when clock is 80 MHz
// however, the ADC seems to work on my boards at 80 MHz
// I suggest you try 80MHz, but if it doesn't work, switch to 40MHz
void PLL_Init(void){ // set phase lock loop (PLL)
  // Clock_Init40MHz(); // run this line for 40MHz
  Clock_Init80MHz(0);   // run this line for 80MHz
}

////////////////////////////////GLOBAL VARIABLES\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\/
SlidePot Sensor; // copy calibration from Lab 7
uint8_t buttons;
int16_t score;
uint8_t numCounters = 3;
uint8_t input = 0;
uint8_t deadTimer = 0;

Refiner m_refiner(67, 10, 121, 45, 61, 15);
TurnInArea m_portal(65, 135, 103, 159, 0, 0);
Rock m_rock1(88, 114, 132, 135, 104, 94, 88, 127, 101, 135);
Rock m_rock1Mid(99, 108, 129, 114, 0, 0);
Rock m_rock1Top(108, 101, 127, 108, 0, 0);
Cart m_cart1(5, 8, 51, 54, 5, 51, 8, 54, 3);
Counter m_todo(0, 0, 32, 159, 0, 32, 90, 159, 3);
Counter m_counter1(0, 75, 28, 99, 0, 30, 85, 89, 1);
Counter m_counter2(0, 99, 28, 123, 0, 30, 109, 110, 1);
Counter m_counter3(0, 123, 28, 147, 0, 30, 133, 135, 1);
Rock m_todoDown(0, 149, 32, 159, 0, 0);
Rock m_rock1Progress(104, 94, 126, 114, 0, 0);

Machine* machineArr1[12] = {&m_refiner, &m_portal, &m_rock1, &m_cart1, &m_todo, &m_counter1, &m_counter2, &m_counter3, &m_todoDown, &m_rock1Mid, &m_rock1Top, &m_rock1Progress};

/*
void machineInit(Machine** m, uint8_t main){
    if(main == 1){  //initializes main 1 machines
        m[0] = new Refiner

        m[1] = new TurnInArea(65, 135, 103, 159, 0, 0);
        
        m[2] = new Rock(88, 114, 132, 135, 104, 94, 88, 127, 101, 135);
        
        m[3] = new Cart(5, 8, 51, 54, 5, 51, 8, 54, 3);
        //this is the todo
        m[4] = new Counter(0, 0, 32, 159, 0, 32, 90, 159, 3);
        //counter 1
        m[5] = new Counter(0, 75, 28, 99, 0, 30, 85, 89, 1);
        //counter 2
        m[6] = new Counter(0, 99, 28, 123, 0, 30, 109, 110, 1);
        //counter 3
        m[7] = new Counter(0, 123, 28, 147, 0, 30, 133, 135, 1);
        //dummy mchine for when todo list is down
        m[8] = new Rock(0, 149, 32, 159, 0, 0);
        //middle of the rock
        m[9] = new Rock(99, 108, 129, 114, 0, 0);
        //top of the rock
        m[10] = new Rock(108, 101, 127, 108, 0, 0);
        //dummy machine for rock's progress bar
        m[11] = new Rock(104, 94, 126, 114, 0, 0);
    }else{  //initializes main 2 machines
        m[0] = new Smelter(89, 80, 127, 134, 103, 72);

        m[1] = new Anvil(31, 130, 98, 159, 25, 136, 70, 90, 130, 159); 
        
        m[2] = new Rock(67, 21, 111, 42, 113, 17);
        
        m[3] =  new Cart(5, 8, 51, 54, 0, 0);
        //counter 4
        m[4] = new Counter(0, 60, 28, 84, 0, 35, 72, 74, 1);
        //counter 5
        m[5] = new Counter(0, 84, 28, 108, 0, 35, 89, 90, 1);
        //counter 6
        m[6] = new Counter(0, 108, 28, 132, 0, 35, 120, 125, 1);
        //rock middle
        m[7] = new Rock(78, 15, 108, 21, 0, 0);
        //rock top
        m[8] = new Rock(87, 8, 106, 15, 0, 0);
        //dummy machine for smelter progress bar
        m[9] = new Rock(103, 72, 125, 78, 0, 0);
    }
}
*/
// games  engine runs at 30Hz
void TIMG12_IRQHandler(void){uint32_t pos, msg;
  if((TIMG12->CPU_INT.IIDX) == 1){ // this will acknowledge
    //GPIOB->DOUTTGL31_0 = GREEN; // toggle PB27 (minimally intrusive debugging)
    //GPIOB->DOUTTGL31_0 = GREEN; // toggle PB27 (minimally intrusive debugging)
  //game engine goes here
    // 1) sample joystick
    pos = Sensor.In();
    Sensor.Save(pos);
    // 2) read input switches
    buttons = Switch_In();
    // 4) start sounds (these are set in FSMs)
    // 5) set semaphore (already done in Sensor.Save)
    //GPIOB->DOUTTGL31_0 = GREEN; // toggle PB27 (minimally intrusive debugging)
  }
}
// uint8_t TExaS_LaunchPadLogicPB27PB26(void){
//   return (0x80|((GPIOB->DOUT31_0>>26)&0x03));
// }

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

  ST7735_FillRect(3, 38, 45, 2, 0x475F); //highlights this language 
  ST7735_SetCursor(1, 3);
  ST7735_OutString((char *)Phrases[2][0]); //english


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
   ST7735_SetCursor(6, 5);
   ST7735_OutString((char *)Phrases[3][currOption]); //
   return;
   case 2:
    ST7735_SetCursor(6, 5);
    ST7735_OutString((char *)Phrases[4][currOption]);
    return;  
   case 3:
   ST7735_SetCursor(0, 1);
   ST7735_OutString((char*)Phrases[5][currOption]);
   return;
   case 4:
    uint8_t madX = 20;
    uint8_t madY = 20;
    uint8_t wordSpace = 20;
    int16_t madcolor = 0xD5E0; 
    ST7735_DrawChar(madX+20, madY, 'M',madcolor, 0x630C, 3);
    ST7735_DrawChar(madX+20+wordSpace, madY, 'A', madcolor, 0x630C, 3);
    ST7735_DrawChar(madX+20+(wordSpace*2), madY, 'D', madcolor, 0x630C, 3);

    ST7735_DrawChar(madX, madY+30, 'M', madcolor, 0x630C, 3);
    ST7735_DrawChar(madX+wordSpace, madY+30, 'I', madcolor, 0x630C, 3);
    ST7735_DrawChar(madX+(wordSpace*2), madY+30, 'N', madcolor, 0x630C, 3);
    ST7735_DrawChar(madX+(wordSpace*3), madY+30, 'E', madcolor, 0x630C, 3);
    ST7735_DrawChar(madX+(wordSpace*4), madY+30, 'S', madcolor, 0x630C, 3);

    ST7735_SetCursor(2, 15);
    ST7735_OutString((char*)"(Click any button)");

    uint8_t gemX = 44;
    uint8_t gemY = 120;
    uint8_t gemSpace = 20;
    ST7735_DrawBitmap(gemX, gemY, Ruby, 9, 12);
    ST7735_DrawBitmap(gemX+15, gemY, Emerald, 9, 12);
    ST7735_DrawBitmap(gemX+30, gemY, Diamond, 9, 12);
   while(buttons==0){}
   return;
  }
}

void randomizeOrders(void){
  switch(SysTick->VAL%5){ //5 different options for order list
    case 0:
      ToDoArr[0] = 1;
      ToDoArr[1] = 0;
      ToDoArr[2] = 2;
      ToDoArr[3] = 1;
      ToDoArr[4] = 0;
      break;
    case 1:
      ToDoArr[0] = 2;
      ToDoArr[1] = 0;
      ToDoArr[2] = 1;
      ToDoArr[3] = 1;
      ToDoArr[4] = 0;
      break;
    case 2:
      ToDoArr[0] = 0;
      ToDoArr[1] = 1;
      ToDoArr[2] = 0;
      ToDoArr[3] = 1;
      ToDoArr[4] = 1;
      break;
    case 3:
      ToDoArr[0] = 0;
      ToDoArr[1] = 2;
      ToDoArr[2] = 0;
      ToDoArr[3] = 1;
      ToDoArr[4] = 1;
      break;
    case 4:
      ToDoArr[0] = 1;
      ToDoArr[1] = 1;
      ToDoArr[2] = 0;
      ToDoArr[3] = 0;
      ToDoArr[4] = 2;
      break;
  };
}

void printScore(int16_t score, uint8_t x_cursor, uint8_t y_cursor, uint8_t fontSize){
  bool isNeg = false;
    if(score<0){
        isNeg = true;
        score*= -1;
    }
    do{
      ST7735_DrawChar(x_cursor, y_cursor, (score%10)+48, 0xFFFF, 0x630C, fontSize);
      score /=10;
      x_cursor-=15;
    }while(score!=0);
    if(isNeg){
      ST7735_DrawChar(x_cursor, y_cursor, '-', 0xFFFF, 0x630C, fontSize);
    }
}

void sendStart(){
  char startMsg[4] = {'1', '2', '3' ,'4'};
  uint8_t acknowledge = 0;
  while(!acknowledge){
    UART2_Disable();
    for (int i = 0; i < 4; i++) {
      IRxmt_OutChar(startMsg[i]);
    }
    UART2_Enable();
    Clock_Delay1ms(100);
    char c1, c2, c3, c4;
    c1 = UART2_InChar();
    while(c1 != 'a' && c1 != 0)c1 = UART2_InChar();
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
  }
  ST7735_FillScreen(0x630C);//set screen grey to start game
  Clock_Delay1ms(10); //delay until the other player starts
}

void movePlayer(){
  //buttons is global variable with PA25 and PA24 in bits 6-5
  //updating player graphics 
  uint32_t vert = Sensor.DistanceY();
  uint32_t horiz = Sensor.DistanceX();
  uint8_t change = 0; //keep track if player position tried to change
  bool canRight = 0, canLeft = 0, canUp = 0, canDown = 0; //keep track of direction player moved in
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

  //checks if attempted movement was legal and prints new position
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

//prints the score to the screen then sends score to other player indefinitely
void displayScore(){
  char startMsg[4];
  if(checkWin()==1){
  ST7735_FillScreen(0x630C);
  setUpInstructions(1);
  startMsg[2] += 0xC0; //won
  }else{
  ST7735_FillScreen(0x630C);
  setUpInstructions(2);
  startMsg[2] += 0x30; //lost
  }
  printScore(score, 70, 80, 2);

  //formats score as positive to send it
  if(score < 0){
    startMsg[2] = 0x0F;
    score*= -1;
  }
  startMsg[0] = 167;              //start character for score message
  startMsg[1] = (score>>8)&0xFF;  //puts positive score into two bytes
  startMsg[3] = (score&0xFF);

  //sends score to other player indefinitely
  while(1){
    UART2_Disable();
    for (int i = 0; i < 4; i++) {
      IRxmt_OutChar(startMsg[i]);
    }
    UART2_Enable();
    Clock_Delay1ms(10);
  }
}

// ALL ST7735 OUTPUT MUST OCCUR IN MAIN
int main(void){ // THIS IS THE PLAYER 1 WITH REFINER, ROCK, AND ORDER WINDOW
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
  Sound_Init();  // initialize sound
  //TExaS_Init(0,0,&TExaS_LaunchPadLogicPB27PB26); // PB27 and PB26
  //Wireless Inits
  IRxmt_Init();   //transmitter PA8
  UART2_Init();   //just receive, PA22, receiver timeout synchronization
    // initialize interrupts on TimerG12 at 30 Hz
  TimerG12_IntArm(2666666, 2);//2666666
  // initialize all data structures
  __enable_irq();
  /////////////intro stuff///////////////////////
  setUpInstructions(4); //does the intro screen
  ST7735_FillScreen(0x630C);//set screen grey
  Clock_Delay1ms(200);
  setUpInstructions(0); //does the intro screen

  ST7735_FillScreen(0x630C);//set screen grey
  setUpInstructions(3);
  score = 0;

  //syncs start with other player
  sendStart();

  ///////////////////////////////////////////////SETUP GAME\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\/
  machineArr1[3]->print();
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
  randomizeOrders();
  Sensor.Sync();

  ///////////////////////////////////////////////MAIN GAME LOOP\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\/
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

    //delay of 3 sec if person run over by cart
    if(deadTimer > 0){
      if(deadTimer%30 == 0){
        ST7735_DrawChar(61, 74, deadTimer/30+48, 0x1F, 0x630C, 2);
      }
      deadTimer--;
      if(deadTimer == 0){
        ST7735_DrawBitmap(p1.getXPos(), p1.getYPos(), miner, p1.getSize(), p1.getSize());
      }
    }

    Sensor.Sync(); //checks for semaphore to be set that ADC has been sampled
    
    timer--;
    if(deadTimer == 0){ //only lets player move if they aren't run over
      movePlayer();
    }

    for (int i = 0; i < 8; i++) {
      if(i > 4 && toDoOpen)continue;
      input = p1.getMachineInput(*(machineArr1[i]));
      input |= buttons;
      machineArr1[i]->update(input);
    }

  }
  
  //prints score and if order list was completed (win/lose)
  displayScore();
  
  return 0;
}