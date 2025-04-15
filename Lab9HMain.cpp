// Lab9HMain.cpp
// Runs on MSPM0G3507
// Lab 9 ECE319H
// Evan Roberts and Madison Cua
// Last Modified: 4/11/2025

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

SlidePot Sensor; // copy calibration from Lab 7
uint8_t buttons;
int8_t menuOpen;
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
    // 3) move sprites
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
  ST7735_DrawBitmap(60, 80, rock, 44,34);
  ST7735_DrawBitmap(90, 95, Emerald, 9,12);
  ST7735_DrawBitmap(70, 95, Ruby, 9,12);
  ST7735_DrawBitmap(50, 95, Diamond, 9,12);
  ST7735_SetRotation(1);
  ST7735_DrawBitmap(50, 95, Diamond, 9,12);
  ST7735_SetRotation(2);
  ST7735_DrawBitmap(50, 95, Diamond, 9,12);
  ST7735_SetRotation(3);
  ST7735_DrawBitmap(50, 95, Diamond, 9,12);
  ST7735_SetRotation(0);
  ST7735_DrawBitmap(50, 95, Diamond, 9,12);
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

// use main3 to test switches and LEDs
int main3(void){ // main3
  __disable_irq();
  PLL_Init(); // set bus speed
  LaunchPad_Init();
  Switch_Init(); // initialize switches
  LED_Init(); // initialize LED
  while(1){
    // write code to test switches and LEDs

  }
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

typedef struct itemHeld{
  const uint16_t* image;
  uint8_t w;
  uint8_t h;
}itemHeld;
//{EMPTY, SILVER_ORE, GOLD_ORE, DIAMOND_ORE, RUBY_ORE, EMERALD_ORE, 
//SILVER, GOLD, DIAMOND, RUBY, EMERALD, SWORD, SHIELD, RING, WATCH, KEY, TRASH};
itemHeld sprites[17] = {{0x0, 0, 0}, {rawSilver, 18, 11}, {rawGold, 18, 11}, {rawDiamond, 18, 11}, {rawRuby, 18, 11}, {rawEmerald, 18, 11},
            {silver, 15, 9}, {gold, 15, 9}, {Diamond, 9, 12}, {Ruby, 9, 12}, {Emerald, 9, 12}, {sword, 19, 19},
            {shield, 16, 21}, {ring, 18, 20}, {watch, 20, 21}, {key, 18, 19}, {trash, 20, 20}};


Player p1; //player 1
Machine m_refiner(61, 0, 121, 35); //(top_left_x, top_left_y, bot_right_x, bot_right_y)
Machine m_smelter(34, 111, 94, 159);
uint8_t input = 0;
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
    // initialize interrupts on TimerG12 at 30 Hz
  TimerG12_IntArm(2666666, 2);//2666666
  // initialize all data structures
  __enable_irq();
  p1.setPossession(3);
    ST7735_DrawFastHLine(106, 138, 22, 0x0);   //trying to make a box outline in the corner
  ST7735_DrawFastVLine(106, 138, 22, 0x0);
  ST7735_DrawFastHLine(105, 137, 24, 0x0);   //thickening box
  ST7735_DrawFastVLine(105, 137, 24, 0x0);
  p1.setPossession(1);
  while(1){
    Sensor.Sync(); //checks for semaphore to be set that interrupt has occured
    uint32_t vert = Sensor.DistanceY();
    uint32_t horiz = Sensor.DistanceX();
    uint8_t change = 0;
    //buttons is global variable with PA25 and PA24 in bits 6-5
    //updating player graphics 
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
    if(change){
      ST7735_DrawBitmap(p1.getXPos(), p1.getYPos(), miner, p1.getSize(), p1.getSize());
    }
    ST7735_SetRotation(0); 
    p1.resetCoordinates();
    //refiner
    int8_t machineOut = -1;
    input = p1.getMachineInput(m_refiner); //get the input for refiner
    input |= buttons;
    machineOut = m_refiner.updateRefiner(input);  //update refiner
    if(machineOut > -1){
      p1.setPossession(machineOut);
      ST7735_FillRect(107, 159, 20, 21, 0x630C);
      if(machineOut != 0){
        ST7735_DrawBitmap(117-sprites[machineOut].w/2, 149+sprites[machineOut].h/2, sprites[machineOut].image, sprites[machineOut].w, sprites[machineOut].h);
      }
    }

    input = p1.getMachineInput(m_smelter);
    input|= buttons;
    machineOut = m_smelter.updateSmelter(input);
    if(machineOut > -1){
      p1.setPossession(machineOut);
      ST7735_FillRect(107, 159, 20, 21, 0x630C);
      if(machineOut != 0){
        ST7735_DrawBitmap(117-sprites[machineOut].w/2, 159-sprites[machineOut].h/2, sprites[machineOut].image, sprites[machineOut].w, sprites[machineOut].h);
      }
    }
  }
}


Machine m_anvil(20, 130, 86, 160); //(top_left_x, top_left_y, top_right_x, top_right_y)
Machine m_rock(67, 8, 86, 42);//(top_left_x, top_left_y, top_right_x, top_right_y)
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
  //LED_Init();    // initialize LED
  Sound_Init();  // initialize sound
  TExaS_Init(0,0,&TExaS_LaunchPadLogicPB27PB26); // PB27 and PB26
    // initialize interrupts on TimerG12 at 30 Hz
  TimerG12_IntArm(2666666, 2);
  // initialize all data structures
  __enable_irq();
  menuOpen = 0;
  ST7735_FillScreen(0x630C);
  //ST7735_DrawBitmap(67, 42, rock, 44, 34);
  ST7735_DrawFastHLine(106, 138, 22, 0x0);   //trying to make a box outline in the corner
  ST7735_DrawFastVLine(106, 138, 22, 0x0);
  ST7735_DrawFastHLine(105, 137, 24, 0x0);   //thickening box
  ST7735_DrawFastVLine(105, 137, 24, 0x0);
  while(1){
    Sensor.Sync(); //checks for semaphore to be set that interrupt has occured
    uint32_t vert = Sensor.DistanceY();
    uint32_t horiz = Sensor.DistanceX();
    uint8_t change = 0;
    //buttons is global variable with PA25 and PA24 in bits 6-5
    //updating player graphics 
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
      if(change){
        ST7735_DrawBitmap(p1.getXPos(), p1.getYPos(), miner, p1.getSize(), p1.getSize());
      }
      ST7735_SetRotation(0); 
      p1.resetCoordinates();
    }
    int8_t outMachine = 0;
    //updating anvil
    input = p1.getMachineInput(m_anvil);
    input |= buttons;
    outMachine = m_anvil.updateAnvil(input); 
    if(outMachine > -1){
      if(outMachine == 1){
        ST7735_DrawBitmap(p1.getXPos(), p1.getYPos(), miner, p1.getSize(), p1.getSize());
      }else{
        p1.setPossession(outMachine);
        ST7735_FillRect(107, 159, 20, 21, 0x630C);
        if(outMachine != 0){
          ST7735_DrawBitmap(117-sprites[outMachine].w/2, 149+sprites[outMachine].h/2, sprites[outMachine].image, sprites[outMachine].w, sprites[outMachine].h);
        }
      }
    }
    input = p1.getMachineInput(m_rock);
    input |= buttons;
    outMachine = m_rock.updateRock(input);
    if(outMachine > -1){
      p1.setPossession(outMachine);
      ST7735_FillRect(107, 159, 20, 21, 0x630C);
      if(outMachine != 0){
        ST7735_DrawBitmap(117-sprites[outMachine].w/2, 149+sprites[outMachine].h/2, sprites[outMachine].image, sprites[outMachine].w, sprites[outMachine].h);
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