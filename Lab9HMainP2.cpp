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
#include "Clock.h"
#include "LaunchPad.h"
#include "Timer.h"
#include "DAC5.h"
#include "Switch.h"
#include "Sound.h"
#include "images2.h"
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
extern Player p2;
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
int8_t menuOpen;
uint8_t numCounters = 3;
uint8_t input = 0;
uint8_t deadTimer = 0;

Smelter m_smelter(89, 80, 127, 134, 103, 72);
Anvil m_anvil(31, 130, 98, 159, 25, 136, 70, 90, 130, 159); 
Rock m_rock2(67, 21, 111, 42, 113, 17);
Cart m_cart2(5, 8, 51, 54, 0, 0);
Counter m_counter4(0, 60, 28, 84, 0, 35, 72, 74, 1);
Counter m_counter5(0, 84, 28, 108, 0, 35, 89, 90, 1);
Counter m_counter6(0, 108, 28, 132, 0, 35, 120, 125, 1);
Rock m_rock2Mid(78, 15, 108, 21, 0, 0);
Rock m_rock2Top(87, 8, 106, 15, 0, 0);
Rock m_smelterProgress(103, 72, 125, 78, 0, 0);

Machine* machineArr2[10] = {&m_smelter, &m_anvil, &m_rock2, &m_cart2, &m_counter4, &m_counter5, &m_counter6, &m_rock2Mid, &m_rock2Top, &m_smelterProgress};

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

void movePlayer(){
  //buttons is global variable with PA25 and PA24 in bits 6-5
  //updating player graphics 
  uint32_t vert = Sensor.DistanceY();
  uint32_t horiz = Sensor.DistanceX();
  uint8_t change = 0; //keep track if player position tried to change
  bool canRight = 0, canLeft = 0, canUp = 0, canDown = 0; //keep track of direction player moved in
  if(horiz < 1000){
    int16_t oldX = p2.getXPos();
    int16_t oldY = p2.getYPos();
    change |= p2.moveRight();
    int16_t newX = p2.getXPos();
    int16_t newY = p2.getYPos();
    p2.resetCoordinates(&newX, &newY);
    if(change && p2.inBounds(newX, newY, machineArr2, 9, 2)){
      canRight = 1;
      p2.setXPos(newX);
      p2.setYPos(newY);
    }else{ //put the coordinates back where they were
      p2.setXPos(oldX);
      p2.setYPos(oldY);
    }
  }
  if(horiz > 3000){
    int16_t oldX = p2.getXPos();
    int16_t oldY = p2.getYPos();
    change |= p2.moveLeft();
    int16_t newX = p2.getXPos();
    int16_t newY = p2.getYPos();
    p2.resetCoordinates(&newX, &newY);
    if(change && p2.inBounds(newX, newY, machineArr2, 9, 2)){
      canLeft = 1;
      p2.setXPos(newX);
      p2.setYPos(newY);
    }else{ //put the coordinates back where they were
      p2.setXPos(oldX);
      p2.setYPos(oldY);
    }
  }
  if(vert > 3000){
    int16_t oldX = p2.getXPos();
    int16_t oldY = p2.getYPos();
    change |= p2.moveUp();
    int16_t newX = p2.getXPos();
    int16_t newY = p2.getYPos();
    p2.resetCoordinates(&newX, &newY);
    if(change && p2.inBounds(newX, newY, machineArr2, 9, 2)){
      canUp = 1;
      p2.setXPos(newX);
      p2.setYPos(newY);
    }else{ //put the coordinates back where they were
      p2.setXPos(oldX);
      p2.setYPos(oldY);
    }
  }
  if(vert < 1000){
    int16_t oldX = p2.getXPos();
    int16_t oldY = p2.getYPos();
    change|= p2.moveDown();
    int16_t newX = p2.getXPos();
    int16_t newY = p2.getYPos();
    p2.resetCoordinates(&newX, &newY);
    if(change && p2.inBounds(newX, newY, machineArr2, 9, 2)){
      canDown = 1;
      p2.setXPos(newX);
      p2.setYPos(newY);
    }else{ //put the coordinates back where they were
      p2.setXPos(oldX);
      p2.setYPos(oldY);
    }
  }

  //checks if attempted movement was legal and prints new position
  if(canUp){
    ST7735_SetRotation(0);
    ST7735_DrawBitmap(p2.getXPos(), p2.getYPos(), miner, p2.getSize(), p2.getSize());
  }else if(canDown){
    p2.translateDown();
    ST7735_DrawBitmap(p2.getXPos(), p2.getYPos(), miner, p2.getSize(), p2.getSize());
    int16_t newX = p2.getXPos();
    int16_t newY = p2.getYPos();
    p2.resetCoordinates(&newX, &newY);
    p2.setXPos(newX);
    p2.setYPos(newY);
  }else if(canRight){
    p2.translateRight();
    ST7735_DrawBitmap(p2.getXPos(), p2.getYPos(), miner, p2.getSize(), p2.getSize());
    int16_t newX = p2.getXPos();
    int16_t newY = p2.getYPos();
    p2.resetCoordinates(&newX, &newY);
    p2.setXPos(newX);
    p2.setYPos(newY);
  }else if(canLeft){
    p2.translateLeft();
    ST7735_DrawBitmap(p2.getXPos(), p2.getYPos(), miner, p2.getSize(), p2.getSize());
    int16_t newX = p2.getXPos();
    int16_t newY = p2.getYPos();
    p2.resetCoordinates(&newX, &newY);
    p2.setXPos(newX);
    p2.setYPos(newY);
  }
  ST7735_SetRotation(0);
}

void receiveStart(){
  uint8_t startGame = 0;
  char c1, c2, c3, c4;
  
  while(!startGame){  //continue until it sees message to start
    c1 = UART2_InChar();
    c2 = UART2_InChar();
    c3 = UART2_InChar();
    c4 = UART2_InChar();
    uint8_t count = 0;
    if(c1 == '1')count++; //looks for start messages from player 1
    if(c2 == '2')count++;
    if(c3 == '3')count++;
    if(c4 == '4')count++;
    if(count >= 2){   //message received that player 1 is ready to start the game
      startGame = 1;
    }
  }

  startGame = 0;  //now send acknlowege until transmitter stops sending start to know it also saw acknowledge
  char ackMsg[4] = {'a', 'b', 'c', 'd'};
  while(!startGame){  //loops checking if start messages stopped and then sending receieved message
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
  for (int j = 0; j < 4; j++) { //sends acknowledge message 4 times on 5ms delay
    UART2_Disable();
    for (int i = 0; i < 4; i++) {
      IRxmt_OutChar(ackMsg[i]);
    }
    UART2_Enable();
    Clock_Delay1ms(5);
  }
    
  ST7735_FillScreen(0x630C);  //black screen to start game
}

void receiveScore(){
  char c1, c2, c3, c4, c5, c6, c7;
  int16_t temp = 0;
   while((c2!=c5) || (c4 != c7) || (c3!=c6) || ((temp%100) != 0 && temp != 999)){
    while(UART2_InChar() != 167){}
    c2 = UART2_InChar();
    c3 = UART2_InChar();
    c4 = UART2_InChar();

    while(UART2_InChar() != 167){}
    c5 = UART2_InChar();
    c6 = UART2_InChar();
    c7 = UART2_InChar();
    temp = ((c2<<8)&0xFF00)+c4;
  }
  if((c3&0xF) == 0xF){
    temp*= -1;
  }
 if((c3&0xF0)==0xC0){
  ST7735_FillScreen(0x630C);
  setUpInstructions(1);
 }else{
  ST7735_FillScreen(0x630C);
  setUpInstructions(2);
 }
  printScore(temp, 70, 80, 2);
}

// ALL ST7735 OUTPUT MUST OCCUR IN MAIN
int main(void){ // THIS IS THE PLAYER 2 WITH ROCK, SMELTER, AND ANVIL
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
  //TExaS_Init(0,0,&TExaS_LaunchPadLogicPB27PB26); // PB27 and PB26
    // initialize interrupts on TimerG12 at 30 Hz
  TimerG12_IntArm(2666666, 2);
  // initialize all data structures
  __enable_irq();
 /////////////intro stuff///////////////////////
  setUpInstructions(4); //does the intro screen
   ST7735_FillScreen(0x630C);//set screen grey
   Clock_Delay1ms(200);
  setUpInstructions(0); //does the intro screen
  ST7735_FillScreen(0x630C);

  UART2_Enable();
  ST7735_SetCursor(0, 1);
  setUpInstructions(3);

  //waits for message to start game, sends acknowledge message to player 1
  receiveStart();
  
  ///////////////////////////////////////////////SETUP GAME\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\/
  menuOpen = 0;
  uint8_t cursorStart = 1;
  uint8_t scoreY = 2;
  uint8_t letterOffset = 6;
  uint16_t color = 0xFFFF;
  ST7735_FillScreen(0x630C);
  
  //ST7735_DrawBitmap(67, 42, rock, 44, 34);
  ST7735_DrawFastHLine(106, 138, 22, 0x0);   //trying to make a box outline in the corner
  ST7735_DrawFastVLine(106, 138, 22, 0x0);
  ST7735_DrawFastHLine(105, 137, 24, 0x0);   //thickening box
  ST7735_DrawFastVLine(105, 137, 24, 0x0);
  int16_t timer = 5400;
  ST7735_DrawBitmap(p2.getXPos(), p2.getYPos(), miner, p2.getSize(), p2.getSize());
  Sensor.Sync();

  ///////////////////////////////////////////////MAIN GAME LOOP\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\/
  while(timer>=0){
    
    //changes visual timer
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

    //checks if player is dead
    if(deadTimer > 0){
      if(deadTimer%30 == 0){
        ST7735_DrawChar(61, 74, deadTimer/30+48, 0x1F, 0x630C, 2);
      }
      deadTimer--;
      if(deadTimer == 0){
        ST7735_DrawBitmap(p2.getXPos(), p2.getYPos(), miner, p2.getSize(), p2.getSize());
      }
    }

    Sensor.Sync(); //checks for semaphore to be set that interrupt has occured
    timer--;
    
    //moves player if they aren't dead/frozen
    if(!menuOpen && deadTimer == 0){
      movePlayer();
    }

    for (int i = 0; i < 7; i++) {
      input = p2.getMachineInput(*(machineArr2[i]));
      input |= buttons;
      machineArr2[i]->update(input);
    }
    
  }
  /////////////////////////////////////////////TIMEOUT GAME ENDS\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\/

  //receives score from player 1 and displays it
  receiveScore();

  while(1){}
  return -1;
}
