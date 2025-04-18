// Lab9HMain.cpp
// Runs on MSPM0G3507
// Lab 9 ECE319H
// Evan Roberts and Madison Cua
// Last Modified: 4/11/2025
#include <cassert>
#include <stdio.h>
#include <stdint.h>
#include <ti/devices/msp/msp.h>
#include "../inc/ST7735.h"
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

extern "C" void __disable_irq(void);
extern "C" void __enable_irq(void);
extern "C" void TIMG12_IRQHandler(void);
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
    // NO LCD OUTPUT IN INTERRUPT SERVICE ROUTINES
    //GPIOB->DOUTTGL31_0 = GREEN; // toggle PB27 (minimally intrusive debugging)
  }
}
uint8_t TExaS_LaunchPadLogicPB27PB26(void){
  return (0x80|((GPIOB->DOUT31_0>>26)&0x03));
}

typedef enum {English, Spanish, Portuguese, French} Language_t;
Language_t myLanguage=English;
typedef enum {HELLO, GOODBYE, LANGUAGE} phrase_t;
const char Hello_English[] ="Hello";
const char Hello_Spanish[] ="\xADHola!";
const char Hello_Portuguese[] = "Ol\xA0";
const char Hello_French[] ="All\x83";
const char Goodbye_English[]="Goodbye";
const char Goodbye_Spanish[]="Adi\xA2s";
const char Goodbye_Portuguese[] = "Tchau";
const char Goodbye_French[] = "Au revoir";
const char Language_English[]="English";
const char Language_Spanish[]="Espa\xA4ol";
const char Language_Portuguese[]="Portugu\x88s";
const char Language_French[]="Fran\x87" "ais";
const char *Phrases[3][4]={
  {Hello_English,Hello_Spanish,Hello_Portuguese,Hello_French},
  {Goodbye_English,Goodbye_Spanish,Goodbye_Portuguese,Goodbye_French},
  {Language_English,Language_Spanish,Language_Portuguese,Language_French}
};
// use main1 to observe special characters
int main1(void){ // main1
    char l;
  __disable_irq();
  PLL_Init(); // set bus speed
  LaunchPad_Init();
  ST7735_InitPrintf();
  ST7735_FillScreen(0x0000);            // set screen to black
  for(int myPhrase=0; myPhrase<= 2; myPhrase++){
    for(int myL=0; myL<= 3; myL++){
         ST7735_OutString((char *)Phrases[LANGUAGE][myL]);
      ST7735_OutChar(' ');
         ST7735_OutString((char *)Phrases[myPhrase][myL]);
      ST7735_OutChar(13);
    }
  }
  Clock_Delay1ms(3000);
  ST7735_FillScreen(0x0000);       // set screen to black
  l = 128;
  while(1){
    Clock_Delay1ms(2000);
    for(int j=0; j < 3; j++){
      for(int i=0;i<16;i++){
        ST7735_SetCursor(7*j+0,i);
        ST7735_OutUDec(l);
        ST7735_OutChar(' ');
        ST7735_OutChar(' ');
        ST7735_SetCursor(7*j+4,i);
        ST7735_OutChar(l);
        l++;
      }
    }
  }
}

// use main2 to observe graphics
int main2(void){ // main2
  __disable_irq();
  PLL_Init(); // set bus speed
  LaunchPad_Init();
  ST7735_InitPrintf();
    //note: if you colors are weird, see different options for
    // ST7735_InitR(INITR_REDTAB); inside ST7735_InitPrintf()
  //ST7735_FillScreen(ST7735_BLACK);
  ST7735_FillScreen(ST7735_Color565(0x96, 0x4B, 0x00));

  //ST7735_DrawBitmap(100, 80, miner_sprite, 30,38);
  //ST7735_DrawBitmap(20, 50, emerald, 18,21);
  /*
  IT READS IN THE HEX VALUES BACKWARDS
  BITS 15-11 BLUE (5)
  BITS 10-5 GREEN (6)
  BITS 4-0 RED (5)  
  */
  while(1){
    Clock_Delay1ms(1000);
  }
  // ST7735_DrawBitmap(22, 159, PlayerShip0, 18,8); // player ship bottom
  // ST7735_DrawBitmap(53, 151, Bunker0, 18,5);
  // ST7735_DrawBitmap(42, 159, PlayerShip1, 18,8); // player ship bottom
  // ST7735_DrawBitmap(62, 159, PlayerShip2, 18,8); // player ship bottom
  // ST7735_DrawBitmap(82, 159, PlayerShip3, 18,8); // player ship bottom
  // ST7735_DrawBitmap(0, 9, SmallEnemy10pointA, 16,10);
  // ST7735_DrawBitmap(20,9, SmallEnemy10pointB, 16,10);
  // ST7735_DrawBitmap(40, 9, SmallEnemy20pointA, 16,10);
  // ST7735_DrawBitmap(60, 9, SmallEnemy20pointB, 16,10);
  // ST7735_DrawBitmap(80, 9, SmallEnemy30pointA, 16,10);

  while(1){
  }
  for(uint32_t t=500;t>0;t=t-5){
    SmallFont_OutVertical(t,104,6); // top left
    Clock_Delay1ms(50);              // delay 50 msec
  }
  ST7735_FillScreen(0x0000);   // set screen to black
  ST7735_SetCursor(1, 1);
  ST7735_OutString((char *)"GAME OVER");
  ST7735_SetCursor(1, 2);
  ST7735_OutString((char *)"Nice try,");
  ST7735_SetCursor(1, 3);
  ST7735_OutString((char *)"Earthling!");
  ST7735_SetCursor(2, 4);
  ST7735_OutUDec(1234);
  
}

// use main4 to test sound outputs
int main4(void){ uint32_t last=0,now;
  __disable_irq();
  PLL_Init(); // set bus speed
  LaunchPad_Init();
  Switch_Init(); // initialize switches
  LED_Init(); // initialize LED
  Sound_Init();  // initialize sound
  TExaS_Init(ADC0,6,0); // ADC1 channel 6 is PB20, TExaS scope
  __enable_irq();
  while(1){
    now = Switch_In(); // one of your buttons
    if((last == 0)&&(now == 1)){
      Sound_Shoot(); // call one of your sounds
    }
    if((last == 0)&&(now == 2)){
      Sound_Killed(); // call one of your sounds
    }
    if((last == 0)&&(now == 4)){
      Sound_Explosion(); // call one of your sounds
    }
    if((last == 0)&&(now == 8)){
      Sound_Fastinvader1(); // call one of your sounds
    }
    // modify this to test all your sounds
  }
}


Player p1; //player 1
//(top_left_x, top_left_y, bot_right_x, bot_right_y, progX, progY, proXL, proXR, proYT, proYB)
Machine m_refiner(67, 10, 121, 45, 61, 15);
Machine m_portal(104, 45, 128, 95, 0, 0);
Machine m_cart1(5, 8, 36, 50, 5, 36, 8, 50, 3);
Machine m_rock1(58, 125, 102, 159, 52, 134);
//(top_left_x, top_left_y, bot_right_x, bot_right_y, proXL, proXR, proYT, proYB, state)
Machine m_todo(0, 0, 32, 159, 0, 32, 90, 159, 3);

uint8_t numCounters = 3;
//(top_left_x, top_left_y, bot_right_x, bot_right_y, proXL, proXR, proYT, proYB, state)
Machine m_counter1(0, 75, 28, 99, 0, 30, 85, 89, 1);
Machine m_counter2(0, 99, 28, 123, 0, 30, 109, 110, 1);
Machine m_counter3(0, 123, 28, 147, 0, 30, 133, 135, 1);
Machine Counters[4] = {m_todo, m_counter1, m_counter2, m_counter3};
uint8_t input = 0;

Machine machineArr1[8] = {m_refiner, m_portal, m_cart1, m_rock1, m_todo, m_counter1, m_counter2, m_counter3};
// ALL ST7735 OUTPUT MUST OCCUR IN MAIN
int main(void){ // THIS IS THE PLAYER 1 WITH REFINER, SMELTER, AND ORDER WINDOW
//initializations
  __disable_irq();
  PLL_Init(); // set bus speed
  LaunchPad_Init();
  ST7735_InitPrintf();
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
    Sensor.Sync(); //checks for semaphore to be set that interrupt has occured
    uint32_t vert = Sensor.DistanceY();
    uint32_t horiz = Sensor.DistanceX();
    uint8_t change = 0;
    timer--;
    //buttons is global variable with PA25 and PA24 in bits 6-5
    //updating player graphics 
    int16_t oldX = p1.getXPos();
    int16_t oldY = p1.getYPos();
    if(horiz < 1000){
      change |= p1.moveRight();
    }
    if(horiz > 3000){
      change |= p1.moveLeft();
    }
    if(vert > 3000){
      change |= p1.moveUp();
    }
    if(vert < 1000){
      change|= p1.moveDown();
    }

    int16_t newX = p1.getXPos();
    int16_t newY = p1.getYPos();
    p1.resetCoordinates(&newX, &newY);

    if(change && p1.inBounds(newX, newY, machineArr1, 8)){
      ST7735_DrawBitmap(p1.getXPos(), p1.getYPos(), miner, p1.getSize(), p1.getSize());
      p1.setXPos(newX);
      p1.setYPos(newY);
    }else{ //put the coordinates back where they were
      p1.setXPos(oldX);
      p1.setYPos(oldY);
    }
    ST7735_SetRotation(0); 
    
    //refiner
    int8_t machineOut = -1;
    input = p1.getMachineInput(m_refiner); //get the input for refiner
    input |= buttons;
    machineOut = m_refiner.updateRefiner(input);  //update refiner
    if(machineOut > -1){
      p1.setPossession(machineOut);
      p1.printPosession(machineOut);
    }

    input = p1.getMachineInput(m_rock1);
    input |= buttons;
    machineOut = m_rock1.updateRock(input);
    if(machineOut > -1){
      p1.setPossession(machineOut);
      p1.printPosession(machineOut);
    }

    //does cart
    input = p1.getMachineInput(m_cart1);
    input |= buttons;
    machineOut = m_cart1.updateCart(input);
    if(machineOut > -1){
      p1.setPossession(machineOut);
      p1.printPosession(machineOut);
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
      machineOut = Counters[0].updateCounters(input, Counters);
      if(machineOut==50){
          continue;
      }else if(machineOut==22){
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
  return 0;
}

//uint8_t TLX, uint8_t TLY, uint8_t BRX, uint8_t BRY, uint8_t PBX, uint8_t PBY, uint8_t XL, uint8_t XR, uint8_t YT, uint8_t YB
Machine m_smelter(89, 76, 127, 130, 99, 68);
Machine m_anvil(35, 130, 101, 159, 28, 136, 43, 101, 130, 159); 
Machine m_rock2(67, 8, 111, 42, 113, 17);
Machine m_cart2(5, 8, 36, 50, 0, 0);

//(top_left_x, top_left_y, bot_right_x, bot_right_y, proXL, proXR, proYT, proYB, state)
Machine m_counter4(0, 60, 28, 84, 0, 30, 65, 74, 1);
Machine m_counter5(0, 84, 28, 108, 0, 30, 89, 100, 1);
Machine m_counter6(0, 108, 28, 132, 0, 30, 111, 125, 1);
Machine machineArr2[7] = {m_smelter, m_anvil, m_rock2, m_cart2, m_counter4, m_counter5, m_counter6};
Machine Counters2[3] = {m_counter4, m_counter5, m_counter6};
int mainP2(void){ // THIS IS THE PLAYER 2 WITH ROCKS AND ANVIL
//initializations
  __disable_irq();
  PLL_Init(); // set bus speed
  LaunchPad_Init();
  ST7735_InitPrintf();
    //note: if you colors are weird, see different options for
    // ST7735_InitR(INITR_REDTAB); inside ST7735_InitPrintf()
  ST7735_FillScreen(ST7735_BLACK);
  Sensor.Init(); // PB18 = ADC1 channel 5, slidepot
  Switch_Init(); // initialize switches PA24, PA25
  //Wireless Inits
  IRxmt_Init();   //transmitter PA8
  UART2_Init();   //just receive, PA22, receiver timeout synchronization

  //LED_Init();    // initialize LED
  Sound_Init();  // initialize sound
  TExaS_Init(0,0,&TExaS_LaunchPadLogicPB27PB26); // PB27 and PB26
    // initialize interrupts on TimerG12 at 30 Hz
  TimerG12_IntArm(2666666, 2);
  // initialize all data structures
  __enable_irq();
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
  //
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

    Sensor.Sync(); //checks for semaphore to be set that interrupt has occured
    uint32_t vert = Sensor.DistanceY();
    uint32_t horiz = Sensor.DistanceX();
    uint8_t change = 0;
    timer--;

    int16_t oldX = p1.getXPos();
    int16_t oldY = p1.getYPos();
    if(!menuOpen){
      if(horiz < 1000){
        change |= p1.moveRight();
      }
      if(horiz > 3000){
        change |= p1.moveLeft();
      }
      if(vert > 3000){
        change |= p1.moveUp();
      }
      if(vert < 1000){
        change|= p1.moveDown();
      }
      int16_t newX = p1.getXPos();
      int16_t newY = p1.getYPos();
      p1.resetCoordinates(&newX, &newY);

    if(change && p1.inBounds(newX, newY, machineArr2, 7)){
      ST7735_DrawBitmap(p1.getXPos(), p1.getYPos(), miner, p1.getSize(), p1.getSize());
      p1.setXPos(newX);
      p1.setYPos(newY);
    }else{ //put the coordinates back where they were
      p1.setXPos(oldX);
      p1.setYPos(oldY);
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
      }else{
        p1.setPossession(machineOut);
        p1.printPosession(machineOut);
      }
    }

    input = p1.getMachineInput(m_smelter);
    input|= buttons;
    machineOut = m_smelter.updateSmelter(input);
    if(machineOut > -1){
      p1.setPossession(machineOut);
      p1.printPosession(machineOut);
    }

    input = p1.getMachineInput(m_rock2);
    input |= buttons;
    machineOut = m_rock2.updateRock(input);
    if(machineOut > -1){
      p1.setPossession(machineOut);
      p1.printPosession(machineOut);
    }
    input = p1.getMachineInput(m_cart2);
    input |= buttons;
    machineOut = m_cart2.updateCart(input);
    if(machineOut > -1){
      p1.setPossession(machineOut);
      p1.printPosession(machineOut);
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
}

void SysTick_Handler(void){ //place holder until sound set up
  // if(songOn && songIndex < 20112){
  //   DAC_Out(adventuring_song[songIndex++]);
  // //}else if(songIndex < 23000){
  // //  songIndex++;
  // }else{
  //   songIndex = 0;
  // }
}