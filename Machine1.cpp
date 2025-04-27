// Authors: Evan Roberts and Madison Cua
// Last Modified: 4/24/2025
#include <cstdint>
#include <iostream>
#include "Machine.h"
#include "ST7735.h"
#include "IRxmt.h"
#include "Sound.h"
#include "UART2.h"
#include "images1.h"
#include "../inc/Clock.h"
#include <stdio.h>
#include <stdint.h>
#include <ti/devices/msp/msp.h>
#include "../inc/LaunchPad.h"
#include "../inc/TExaS.h"
#include "../inc/Timer.h"
#include "Sound.h"
#include "Player.h"
#include "ti/devices/msp/m0p/mspm0g350x.h"

extern uint8_t toDoOpen, sound_flag;
extern int16_t score;
extern uint8_t deadTimer;
extern Machine* machineArr1[12];
// //inputs (bits 0-4)
 #define material 0x1F
 enum Materials {EMPTY, SILVER_ORE, GOLD_ORE, DIAMOND_ORE, RUBY_ORE, EMERALD_ORE, 
 SILVER, GOLD, DIAMOND, RUBY, EMERALD, SWORD, SHIELD, WATCH, RING, KEY, BOOT, TURNIP};
// //0-empty
// //1-raw silver
// //2-raw gold
// //3-raw diamond
// //4-raw ruby
// //5-raw emerald
// //6-finished silver
// //7-finished gold
// //8-finished diamond
// //9-finshed ruby
// //10-finished emerald
// //11-sword
// //12-sheild
// //13-ring
// //14-staff
// //15-wand
// //16-boot
// //17-turnip
// //Bit 5: LButtong 
 #define LButton (1<<5)
// //Bit 6: RButton
 #define RButton (1<<6)
// //Bit 7:player proximity
 #define Prox (1<<7)

itemHeld sprites[18] = {{0x0, 0, 0}, {rawSilver, 18, 11}, {rawGold, 18, 11}, {rawDiamond, 18, 11}, {rawRuby, 18, 11}, {rawEmerald, 18, 11},
                        {silver, 15, 9}, {gold, 15, 9}, {Diamond, 9, 12}, {Ruby, 9, 12}, {Emerald, 9, 12}, {sword, 19, 19},
                        {shield, 16, 21}, {watch, 20, 21}, {ring, 16, 17}, {key, 18, 19}, {boot, 20, 20}, {turnip, 13, 19}};
Player p1; //player 1
//for progress bar universal size
const uint8_t progW = 6;
const uint8_t progH = 22;

//the To do list 
int ToDoArr[5]; 

void Machine::print(){}
void Machine::update(uint8_t val){}

Machine::Machine(uint8_t TLX, uint8_t TLY, uint8_t BRX, uint8_t BRY, uint8_t PBX, uint8_t PBY){
    sprite = 100;   //start of game engine prints default
    state = 0;
    top_L_x = TLX;
    top_L_y = TLY;
    bot_R_x = BRX;
    bot_R_y = BRY;
    workTimer = 0;
    wasWorking = 0;
    progX = PBX;
    progY = PBY;
    proXL = TLX;
    proXR =  BRX; //right proximity
    proYT =  TLY; //top proximity
    proYB =  BRY; //bottom proximity
}

Machine::Machine(uint8_t TLX, uint8_t TLY, uint8_t BRX, uint8_t BRY, uint8_t XL, uint8_t XR, uint8_t YT, uint8_t YB, uint8_t s){
    sprite = 100;   //start of game engine prints default
    state = s;
    top_L_x = TLX;
    top_L_y = TLY;
    bot_R_x = BRX;
    bot_R_y = BRY;
    workTimer = 0;
    wasWorking = 0;
    progX = 0;
    progY = 0;
    proXL = XL;
    proXR =  XR; //right proximity
    proYT =  YT; //top proximity
    proYB =  YB; //bottom proximity
}

Machine::Machine(uint8_t TLX, uint8_t TLY, uint8_t BRX, uint8_t BRY, uint8_t PBX, uint8_t PBY, uint8_t XL, uint8_t XR, uint8_t YT, uint8_t YB){
    sprite = 100;   //start of game engine prints default
    state = 0;
    top_L_x = TLX;
    top_L_y = TLY;
    bot_R_x = BRX;
    bot_R_y = BRY;
    workTimer = 0;
    wasWorking = 0;
    progX = PBX;
    progY = PBY;
    proXL = XL;
    proXR =  XR; //right proximity
    proYT =  YT; //top proximity
    proYB =  YB; //bottom proximity
}

uint8_t Machine::getSprite(){
    return sprite;
}

void Machine::setSprite(uint8_t s){
    sprite = s;
}

//progress bar constructor
Refiner::Refiner(uint8_t TLX, uint8_t TLY, uint8_t BRX, uint8_t BRY, uint8_t PBX, uint8_t PBY)
        : Machine(TLX, TLY, BRX, BRY, PBX, PBY){}

void Refiner::print() {
    if(sprite==0){ //default
        ST7735_DrawBitmap(top_L_x, bot_R_y, refiner, 61, 35);
    }else if(sprite==1){ //highlighted refiner
        ST7735_DrawBitmap(top_L_x, bot_R_y, refinerHighlight, 61, 35);
    }else if(sprite==2){ //working refiner
        ST7735_DrawBitmap(top_L_x, bot_R_y, refinerWorking, 61, 35);
    }else if(sprite ==3){//not highlighted in done state
        ST7735_DrawBitmap(top_L_x, bot_R_y, refiner, 61, 35);
        if(holdItem!=0){
            uint32_t size = sprites[holdItem].h * sprites[holdItem].w;
            unsigned short blendedItem[size];
            for(int i=0; i<size; i++){
                if(sprites[holdItem].image[i] == 0x630C){
                   blendedItem[i] = 0xFC47;
                }else{
                  blendedItem[i] = sprites[holdItem].image[i];
                }
            }
            ST7735_DrawBitmap((top_L_x)+((bot_R_x+4-top_L_x)/2)-sprites[holdItem].w/2, ((top_L_y)+(bot_R_y-4-top_L_y)/2)+sprites[holdItem].h/2, blendedItem, sprites[holdItem].w, sprites[holdItem].h);
        }
    }else if(sprite ==4){//highlighted in done state
        ST7735_DrawBitmap(top_L_x, bot_R_y, refinerHighlight, 61, 35);
        if(holdItem!=0){
            uint32_t size = sprites[holdItem].h * sprites[holdItem].w;
            unsigned short blendedItem[size];
            for(int i=0; i<size; i++){
                if(sprites[holdItem].image[i] == 0x630C){
                    blendedItem[i] = 0xFC47;
                }else{
                   blendedItem[i] = sprites[holdItem].image[i];
                }
            }
            ST7735_DrawBitmap((top_L_x)+((bot_R_x+4-top_L_x)/2)-sprites[holdItem].w/2, ((top_L_y)+(bot_R_y-4-top_L_y)/2)+sprites[holdItem].h/2, blendedItem, sprites[holdItem].w, sprites[holdItem].h);
        }

    }
}

int8_t Refiner::refinerFSM(uint8_t input){
    static int8_t debounce = 0;
    switch(state){
      case 0: //wait state
        if(debounce > 0)debounce--;
        if((input&Prox) ==0){      //ser to default state
            if(sprite !=0){ //don't reprint if already default
                sprite = 0;
                print();
            }
            return -1;
        }else{//print highlighted state
            if(sprite!=1){
                sprite = 1;
                print();
            } //don't reprint if already highlighted
        }
        if((input&RButton) == 0X40 && holdItem !=0){ //start working and item to work on
            state++;
            sprite = 2;
            print();
            if(!wasWorking){
                wasWorking = 1;
                workTimer = 150;    //150 updates at 30Hz = 5 sec work time
                //print progress bar
                
            }
            return -1;
        }
        if(debounce > 0)return -1;//wait .03 sec before pickup/put down again
        if((input&LButton) == 0x20 && (input&material) != EMPTY){ //take item in for playing to work on
            holdItem = input&material;
            debounce = 10;
            //outline of progress bar
            ST7735_DrawFastHLine(progX, progY, progW, 0x0);  //top line
            ST7735_DrawFastVLine(progX, progY, progH, 0x0); //left line
            ST7735_DrawFastHLine(progX, progY+progH-1, progW, 0x0);  //bottom
            ST7735_DrawFastVLine(progX+progW-1, progY, progH, 0x0); //right line
            ST7735_FillRect(progX+1, progY+1, progW-2, progH-2, 0x4208); //fills inside of empty progress bar
            return EMPTY;               //tells the main to empty player's hand
        }
        if((input&LButton) == 0x20 && (input&material) == EMPTY && holdItem != EMPTY){ //give item with no work done to player
            int8_t temp = holdItem;
            holdItem = 0;
            debounce = 10;
            ST7735_FillRect(progX, progY, progW, progH, 0x630C); //fills inside of empty progress bar
            return temp;          //tells main item to return
        }
        return -1;
      case 1://working state
        if(((input&Prox) == 0) || (input&RButton) == 0){
            state--;
            return -1;
        }
        workTimer--;
        if(workTimer%15 == 0){
            if(sprite == 2){
                sprite = 0;
                print();
            }else{
                sprite = 2;
                print();
            }
            //update progress bar every 10%
            ST7735_FillRect(progX+1, progY+progH-2*((150-workTimer)/15)-1, progW-2, (progH-2)/10, 0x001F);
        }
        if(workTimer == 0){
            sprite = 0;
            if((holdItem) > 2 && (holdItem) < 6){   //makes sure gem was input
                holdItem +=5;
            }else{
                holdItem = BOOT;
            }
            ST7735_FillRect(progX, progY, progW, progH, 0x630C); //fills inside of empty progress bar
            state++;
            sprite = 0;
            print();
        }
        return -1;
      case 2: //done state 
        wasWorking = 0;
        if((input&Prox)==0){
            if(sprite!=3){
                sprite = 3;
                print();
            }
        }else{
            if(sprite!=4){
                sprite = 4;
                print();
            }
            if(debounce > 0){   //debounce time after printing menu
                debounce--;
                return -1;
            }
            if((input&LButton)==0x20 && ((input&material)==EMPTY)){ //if LButton is pressed, eject from the menu screen (decrement state)
                int temp = holdItem;
                sprite = 0;
                holdItem = 0;
                print();
                state = 0;
                debounce = 10;
                return temp;  //number that isn't an item to indicate reprint player
            }
        }
        return -1; 
    }
    return -1;
}

void Refiner::update(uint8_t input) {
    int8_t machineOut = refinerFSM(input);
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
}

//proximity constructor
Rock::Rock(uint8_t TLX, uint8_t TLY, uint8_t BRX, uint8_t BRY, uint8_t PBX, uint8_t PBY, uint8_t XL, uint8_t XR, uint8_t YT, uint8_t YB)
        : Machine(TLX, TLY, BRX, BRY, PBX, PBY, XL, XR, YT, YB) {
            holdItem = 1;   //returns only metals
        }

//progress bar constructor
Rock::Rock(uint8_t TLX, uint8_t TLY, uint8_t BRX, uint8_t BRY, uint8_t PBX, uint8_t PBY)
        : Machine(TLX, TLY, BRX, BRY, PBX, PBY){
            holdItem = 0;   //returns only gems
        }

void Rock::print() {
    if(sprite == 0){
        ST7735_DrawBitmap(top_L_x, bot_R_y, rock_b, (bot_R_x-top_L_x), (bot_R_y-top_L_y));    //default rock
        ST7735_DrawBitmap(top_L_x+11, bot_R_y-21, rock_m, 30, 6);
        ST7735_DrawBitmap(top_L_x+20, bot_R_y-27, rock_t, 19, 7);
    }else if(sprite == 1){
        ST7735_DrawBitmap(top_L_x, bot_R_y, rockHighlight_b, (bot_R_x-top_L_x), (bot_R_y-top_L_y));
        ST7735_DrawBitmap(top_L_x+11, bot_R_y-21, rockHighlight_m, 30, 6);
        ST7735_DrawBitmap(top_L_x+20, bot_R_y-27, rockHighlight_t, 19, 7);
    }else if(sprite == 2){
        ST7735_DrawBitmap(top_L_x, bot_R_y, rockWorking_b, (bot_R_x-top_L_x), (bot_R_y-top_L_y));
        ST7735_DrawBitmap(top_L_x+11, bot_R_y-21, rockWorking_m, 30, 6);
        ST7735_DrawBitmap(top_L_x+20, bot_R_y-27, rockWorking_t, 19, 7);
    }else if(sprite == 3){ //cracked highlights
        ST7735_DrawBitmap(top_L_x, bot_R_y, rockWorkingHighlight_b, (bot_R_x-top_L_x), (bot_R_y-top_L_y));
        ST7735_DrawBitmap(top_L_x+11, bot_R_y-21, rockWorkingHighlight_m, 30, 6);
        ST7735_DrawBitmap(top_L_x+20, bot_R_y-27, rockWorkingHighlight_t, 19, 7);
    }
}

int8_t Rock::rockFSM(uint8_t input){
    static uint8_t halfway = 0;
    static uint8_t lastOuts[3] = {0, 1, 0};
    switch(state){
      case 0: //wait state
        if((input&Prox) ==0){      //ser to default state
            if(halfway==0){
                if(sprite!=0){
                    sprite = 0;
                    print();
                }
            }else{
                if(sprite!=2){
                    sprite = 2;
                    print();
                }
            }
            return -1;
        }else{//print highlighted state
            if(halfway==0){
                if(sprite!=1){
                    sprite = 1;
                    print();
                }
            }else{
                if(sprite!=3){
                    sprite = 3;
                    print();
                }
            }
        }
        if((input&RButton) == 0x40){
            state++;
            if(!wasWorking){
                Sound_Clang();
                sprite = 0;
                print();
                workTimer = 150;    //5 sec of work time
                wasWorking = 1;
                if(!holdItem){
                    ST7735_DrawFastHLine(progX, progY, progW, 0x0);  //top line
                    ST7735_DrawFastVLine(progX, progY, progH, 0x0); //left line
                    ST7735_DrawFastHLine(progX, progY+progH-1, progW, 0x0);  //bottom
                    ST7735_DrawFastVLine(progX+progW-1, progY, progH, 0x0); //right line
                    ST7735_FillRect(progX+1, progY+1, progW-2, progH-2, 0x4208); //fills inside of empty progress bar
                }else{
                    ST7735_DrawFastHLine(progX, progY, progH, 0x0);  //top line
                    ST7735_DrawFastVLine(progX, progY, progW, 0x0); //left line
                    ST7735_DrawFastHLine(progX, progY+progW-1, progH, 0x0);  //bottom
                    ST7735_DrawFastVLine(progX+progH-1, progY, progW, 0x0); //right line
                    ST7735_FillRect(progX+1, progY+1, progH-2, progW-2, 0x4208); //fills inside of empty progress bar
                }
                return 80+holdItem; //differentiates between which rock
            }
        }
        return -1;
      case 1://mining
        if(((input&RButton) == 0 || (input&Prox) == 0)){
            state--;
            wasWorking = 1;
            return -1;
        }
        workTimer--;
        if(workTimer%15 == 0){
            if(!holdItem){
                ST7735_FillRect(progX+1, progY+progH-2*((150-workTimer)/15)-1, progW-2, (progH-2)/10, 0x001F);
            }else{
                ST7735_FillRect(progX+progH-2*((150-workTimer)/15)-1, progY+1, (progH-2)/10, progW-2, 0x001F);
            }
            Sound_Clang();
        }
        if(workTimer == 75){    //print cracked halfway through
            sprite = 2;
            print();
            halfway = 1;
        }
        if(workTimer == 0){
            halfway = 0;
            wasWorking = 0;
            sprite = 0;
            state++;
            workTimer = 15;
            print();
            if(!holdItem){
                ST7735_FillRect(progX, progY, progW, progH, 0x630C); //fills inside of empty progress bar
            }else{
                ST7735_FillRect(progX, progY, progH, progW, 0x630C); //fills inside of empty progress bar
            }
            if((input&material) != EMPTY){
                Sound_Wrong();
                return -1;
            }
            uint8_t randOre;
            do{
                uint32_t randNumber = SysTick->VAL+(TIMG12->COUNTERREGS.CTR&0xFFFF);
                randOre = (holdItem)? (randNumber&1)+1: (randNumber%3)+3; //gives random metal or random gem
            }while((lastOuts[0] | lastOuts[1] | lastOuts[2] | randOre) == (randOre & lastOuts[0] & lastOuts[1] & lastOuts[2]));
            lastOuts[0] = lastOuts[1];
            lastOuts[1] = lastOuts[2];
            lastOuts[2] = randOre;
            return randOre;
        }
        return -1;
        case 2: //small refresh delay before rock can be used again
            workTimer--;
            if(workTimer==0){
                state = 0;
            }
            return -1;

    }
    return -1;
}

void Rock::update(uint8_t input) {
    int8_t machineOut = rockFSM(input);
    if(machineOut > -1){
      if(machineOut == 81){
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
}

//progress bar constructor
Cart::Cart(uint8_t TLX, uint8_t TLY, uint8_t BRX, uint8_t BRY, uint8_t PBX, uint8_t PBY)
        : Machine(TLX, TLY, BRX, BRY, PBX, PBY) {}

//state constructor
Cart::Cart(uint8_t TLX, uint8_t TLY, uint8_t BRX, uint8_t BRY, uint8_t XL, uint8_t XR, uint8_t YT, uint8_t YB, uint8_t s)
        : Machine(TLX, TLY, BRX, BRY, XL, XR, YT, YB, s) {
            sprite = 4;
        }

void Cart::print() {
    static int8_t sent = 0;
    static int8_t in = 5;
    if(sprite == 0){
        ST7735_DrawBitmap(top_L_x, bot_R_y, cart, 46, 43);
        if(holdItem){
            ST7735_FillRect(top_L_x+8, bot_R_y-35, 30, 23, 0x630C);
            ST7735_DrawBitmap(top_L_x+23-sprites[holdItem].w/2, bot_R_y-24+sprites[holdItem].h/2, sprites[holdItem].image, sprites[holdItem].w, sprites[holdItem].h);
        }
    }else if(sprite == 1){
        ST7735_DrawBitmap(top_L_x, bot_R_y, cartHighlight, 46, 43);
        if(holdItem){
            ST7735_FillRect(top_L_x+8, bot_R_y-35, 30, 23, 0x630C);
            ST7735_DrawBitmap(top_L_x+23-sprites[holdItem].w/2, bot_R_y-24+sprites[holdItem].h/2, sprites[holdItem].image, sprites[holdItem].w, sprites[holdItem].h);
        }
    }else if(sprite == 2){//cart leaving
        ST7735_DrawBitmap(top_L_x, bot_R_y-2-sent*7, cart+92, 46, 41-sent*7);
        sent++;
        sent%=6;
    }else if(sprite == 3){//cart coming in
        ST7735_DrawBitmap(top_L_x, bot_R_y-2-in*7, cart+92, 46, 41-in*7);
        ST7735_FillRect(top_L_x, bot_R_y-2-in*7-43, 46, 7, 0x630C);
        in--;
        if(in == -1)in = 5;
    }else if(sprite == 4){  //ladder when cart is away
        ST7735_DrawBitmap(top_L_x+13, bot_R_y, ladder, 20, 44);
    }
}

uint8_t Cart::cartSendError(uint8_t val1, uint8_t val2){
    if((val1&material) == (val2&material)){//same item, different parity bits (assume item is right)
        return val1&material;
    }else if((val1&0xE0) == (val2&0xE0)){//different item, same parity bits
        uint8_t parity = val1&0xE0;
        val1&=material; //just grab the item now
        val2&=material;
        if(parity == 0)return EMPTY;
        if(parity == 0xE0)return (val1 == 17)? TURNIP: BOOT;
        if(parity == 0x80){ //item should be a finished product
            if(val1 > EMERALD && val1 < BOOT)return val1;  //return whichever item fits the parity
            if(val2 > EMERALD && val2 < BOOT)return val2;
        }
        if(parity == 0x40){ //item should be a processed material
            if(val1 > EMERALD_ORE && val1 < SWORD)return val1;  //return whichever item fits the parity
            if(val2 > EMERALD_ORE && val2 < SWORD)return val2;
        }
        if(parity == 0x20){ //item should be raw material
            if(val1 > EMPTY && val1 < SILVER)return val1;  //return whichever item fits the parity
            if(val2 > EMPTY && val2 < SILVER)return val2;
        }
    }
    //now none of the parity or material bits match
    return EMPTY;   //give player empty as "shipping error"
}

int8_t Cart::cartFSM(uint8_t input){
    static uint8_t wasWorking = 0;
    static int8_t debounce = 0;
    switch(state){
      case 0: //wait state
        if(debounce > 0)debounce--;
        if((input&Prox) ==0){      //set to default state
            if(sprite !=0){ //don't reprint if already default
                sprite = 0;
                print();
            }
            return -1;
        }else{//print highlighted state
            if(sprite!=1){
                sprite = 1;
                print();
            } //don't reprint if already highlighted
        }
        if((input&RButton) == 0x40){
            state++;
            workTimer = 60;
            sprite = 2;
            ST7735_FillRect(top_L_x+8, bot_R_y-31, 30, 23, 0x0);    //clear item shown in cart
            return -1;
        }
        if(debounce > 0)return -1;
        if((input&LButton) == 0x20 && (input&material) != EMPTY && holdItem == EMPTY){   //player puts item in cart
            debounce = 10;
            holdItem = input&material;      //print item in cart
            ST7735_FillRect(top_L_x+8, bot_R_y-35, 30, 23, 0x630C);
            ST7735_DrawBitmap(top_L_x+23-sprites[holdItem].w/2, bot_R_y-24+sprites[holdItem].h/2, sprites[holdItem].image, sprites[holdItem].w, sprites[holdItem].h);
            return 0;
        }
        if((input&LButton) == 0x20 && (input&material) == EMPTY){   //player takes item from cart
            debounce = 10;
            int8_t temp = holdItem;
            ST7735_FillRect(top_L_x+8, bot_R_y-35, 30, 23, 0x0);    //clear item shown in cart
            holdItem = 0;
            return temp;
        }
        return -1;
      case 1://leaving animation
      if(toDoOpen==1){return -1;}
        if(workTimer%12 == 0){
            print();
        }
        if(workTimer == 0){
            state++;
            sprite = 4;
            workTimer = 15;
            while(UART2_InChar()){};//empties out FIFO
        }
        workTimer--;
        return -1;
      case 2://send cart state
        if(workTimer == 0){ //sent cart message 15 times with no receive acknowledge
            state++;
            workTimer = 15;
            return -1;
        }
        workTimer--;
        if(UART2_InChar()){ //Non 0 return means there's something in FIFO
            uint8_t count = 0;
            char ack = '2';
            while(ack){ //clear out the whole FIFO
                ack = UART2_InChar();
                if(ack == 'c'){
                    count++;  //c is acknowledge that cart was received
                }
            }
            if(count > 1){
                state++;
            }
        }

        char contents;
        contents = holdItem;
        /*message:
            2 start bytes "<"
            2 content bytes:
                bit 7: finished item
                bit 6: processed resource
                bit 5: raw material
                *EMPTY == bits 7-5 all 0
                *TRASH == bits 7-5 all 1
                bits 4-0: value of the item sent
        */
        if(holdItem != EMPTY){
            if(holdItem < 6){
                contents |= 0x20;
            }else if(holdItem < 11){
                contents |= 0x40;
            }else if(holdItem < 16){
                contents |= 0x80;
            }else{//this is trash or turnip
                contents |= 0xE0;//((holdItem == 16)? 0xE0: 0xEE);
            }
        }
        char msg[4];
        msg[0] = '<';
        msg[1] = '<';
        msg[2] = contents;
        msg[3] = contents;
        UART2_Disable();
        for (int i = 0; i < 4; i++) {
            IRxmt_OutChar(msg[i]);
        }
        UART2_Enable();
        sprite = 4; //ladder sprite because cart left
        return -1;
      case 3://wait state until cart is returned
        uint8_t start0;
        start0 = UART2_InChar();
        if(start0 == '<'){
            uint8_t val1, val2;
            uint8_t start1;
            start1 = UART2_InChar();
            if(start1 == '<'){
                val1 = UART2_InChar();
                val2 = UART2_InChar();
            }else{
                val1 = start1;
                val2 = UART2_InChar();
                UART2_InChar(); //pull the extra item out of the fifo
            }
            if(val1 == val2){
                holdItem = val1&material;
            }else{
                holdItem = cartSendError(val1, val2);
            }
            //received an item
            state++;
            workTimer = 15;
        }
        return -1;
      case 4:   //sending acknowledge of receiving
        if(workTimer == 0){//only tries sending first acknowledge 15 times before moving on
            state++;
            workTimer = 60;
            sprite = 3;
            return -1;
        }
        workTimer--;
        char seen[4];   //resend first acknowledge from receiver to transmitter
        seen[0] = 'c';
        seen[1] = 'c';
        seen[2] = 'c';
        seen[3] = 'c';    
        UART2_Disable();
        for (int i = 0; i < 4; i++) {    //sends acknowledge
            IRxmt_OutChar(seen[i]);
        }
        UART2_Enable();
        return -1;
      case 5://return after getting message
      if(toDoOpen==1){return -1;}
        if(workTimer%12 == 0){
            print();
        }
        if(workTimer == 0){
            state = 0;
            sprite = 0;
            print();
            if(holdItem != EMPTY){  //print the item in the cart
                ST7735_FillRect(top_L_x+8, bot_R_y-35, 30, 23, 0x630C);
                ST7735_DrawBitmap(top_L_x+23-sprites[holdItem].w/2, bot_R_y-24+sprites[holdItem].h/2, sprites[holdItem].image, sprites[holdItem].w, sprites[holdItem].h);
            }
            return -1;
        }
        workTimer--;
        return 80;
    }
    return -1;
}

void Cart::update(uint8_t input) {
    int8_t machineOut = cartFSM(input);
    if(machineOut > -1){
      if(machineOut == 80){
        if(p1.getYPos() <= 80 && p1.getXPos() <= 51){
          ST7735_FillRect(p1.getXPos(), p1.getYPos()-p1.getSize(), p1.getSize(), p1.getSize(), 0x630C);
          p1.setXPos(51);
          p1.setYPos(93);
          deadTimer = 90;
        }
      }else{
        p1.setPossession(machineOut);
        p1.printPosession(machineOut);
      }
    }
}

//progress bar constructor
TurnInArea::TurnInArea(uint8_t TLX, uint8_t TLY, uint8_t BRX, uint8_t BRY, uint8_t PBX, uint8_t PBY)
        : Machine(TLX, TLY, BRX, BRY, PBX, PBY){}

void TurnInArea::print(){
    if(sprite==0){//default state of turn in area
        ST7735_DrawBitmap(top_L_x, bot_R_y, portal, bot_R_x-top_L_x, bot_R_y-top_L_y); 
    }else if(sprite==1){ //twirling state of turn in area
        ST7735_DrawBitmap(top_L_x, bot_R_y, portalHighlight, bot_R_x-top_L_x, bot_R_y-top_L_y);
    }else if(sprite==2){//to do menu with numbers
        unsigned short invertedPortal[1200];
        for(int i=0; i<1200; i++){
            if(portal[i]==0x630C){
                invertedPortal[i] = 0x630C;
            }else{
                uint16_t p = portal[i];

                // Extract RGB565 components
                uint8_t r = (p >> 11) & 0x1F;
                uint8_t g = (p >> 5) & 0x3F;
                uint8_t b = p & 0x1F;

                // Invert each channel
                uint8_t ir = 31 - r;
                uint8_t ig = 63 - g;
                uint8_t ib = 31 - b;

                // Recombine into RGB565
                uint16_t inverted = (ir << 11) | (ig << 5) | ib;

                // Swap bytes for display
                invertedPortal[i] = (inverted >> 8) | (inverted << 8);
            }
        }
        ST7735_DrawBitmap(top_L_x, bot_R_y, invertedPortal, bot_R_x-top_L_x, bot_R_y-top_L_y);
    }
}

int8_t TurnInArea::turnInFSM(uint8_t input){
    switch(state){
      case 0:
        if((input&Prox) == 0){
            if(sprite!=0){
                sprite = 0;
                print();
            }
            return -1;
        }else{
            if(sprite!=1){
                sprite = 1;
                print();
            }
            if((LButton&input)==0x20 && ((input&material)>=1 && (input&material)<=TURNIP)){
                holdItem = input&material;
                workTimer = 100; //set work timer
                state++;
                return EMPTY;
            }
        }
        return -1;
      case 1:
        workTimer--;
        if(workTimer == 0){ //resets the score after "turn in processing is complete"
            sprite = 0;
            print(); //set turn in area back to default
            if(holdItem>=SWORD && holdItem<=KEY){ //fix array
                if(ToDoArr[holdItem-11]>0){
                    ToDoArr[holdItem-11]--; //decrement the item in the arr
                    score+=200;
                    //output good ding
                    Sound_Correct();
                }
            }else{
                score -= 100;
                if(score<-999){
                    score = -999;
                }
                //output bad ding
                Sound_Wrong();
            }
            int x_cursor = 120;
            bool isNeg = false;
            int16_t temp = score;
            if(score<0){
                isNeg = true;
                temp*= -1;
            }
            while(temp!=0){
                ST7735_DrawChar(x_cursor, 2, (temp%10)+48, 0xFFFF, 0x630C, 1);
                temp /=10;
                x_cursor-=6;
            }
            if(isNeg){
                ST7735_DrawChar(x_cursor, 2, '-', 0xFFFF, 0x630C, 1);
            }
            state=0;
            return EMPTY;
        }
        if(workTimer%15 == 0){//flashes 
            if(sprite==0){
                sprite = 2;
                print();
            }else{
                sprite = 0;
                print();
            }
        }
        return -1;
    }
    return -1;
}

void TurnInArea::update(uint8_t input) {
    int8_t machineOut = turnInFSM(input);
    if(machineOut > -1){
      p1.setPossession(machineOut);
      p1.printPosession(machineOut);
    }
}

//state constructor
Counter::Counter(uint8_t TLX, uint8_t TLY, uint8_t BRX, uint8_t BRY, uint8_t XL, uint8_t XR, uint8_t YT, uint8_t YB, uint8_t s)
        : Machine(TLX, TLY, BRX, BRY, XL, XR, YT, YB, s) {}

void Counter::print() {
    if(sprite == 0){
        ST7735_DrawFastVLine(bot_R_x, top_L_y, 160, 0x630C); //right line
        ST7735_DrawFastVLine(bot_R_x+1, top_L_y, 160, 0x630C); //right line again    
    }else if(sprite ==1){//highlight to do
        ST7735_DrawFastVLine(bot_R_x, top_L_y, 160, 0x475F); //right line
        ST7735_DrawFastVLine(bot_R_x+1, top_L_y, 160, 0x475F); //right line again

    }else if(sprite ==3){//highlight single counter
        ST7735_DrawFastHLine(top_L_x+1, top_L_y+1, 22, 0x475F);  //top line
        ST7735_DrawFastVLine(top_L_x+1, top_L_y+1, 22, 0x475F); //left line
        ST7735_DrawFastHLine(top_L_x+1, bot_R_y-1, 22, 0x475F);  //bottom
        ST7735_DrawFastVLine(bot_R_x-5, top_L_y+1, 22, 0x475F); //right line
    }else if(sprite ==4){//print object on single counter
        ST7735_DrawBitmap(top_L_x, bot_R_y, counter, 28, 24);
        if(holdItem!=0){
            uint16_t size = sprites[holdItem].w*sprites[holdItem].h;
            unsigned short dark[size];
        for(int i=0; i<size; i++){
            if(sprites[holdItem].image[i]==0x630C){
                dark[i] = 0x3186;
            }else{
                dark[i] = sprites[holdItem].image[i];
            }
        }
            ST7735_DrawBitmap(((bot_R_x-4-top_L_x)/2)-sprites[holdItem].w/2, ((top_L_y)+(bot_R_y-top_L_y)/2)+sprites[holdItem].h/2, dark, sprites[holdItem].w, sprites[holdItem].h);
        }
    }else if(sprite == 5){//draws the to do button with highlight
        ST7735_DrawFastHLine(top_L_x, top_L_y+150, 34, 0x0);  //top line
        ST7735_DrawFastHLine(top_L_x, top_L_y-1+150, 34, 0x475F);  //top line
        ST7735_DrawFastHLine(top_L_x, top_L_y-2+150, 32, 0x475F);  //top line
        ST7735_DrawFastVLine(bot_R_x, top_L_y+150, 10,  0x475F); //right line
        ST7735_DrawFastVLine(bot_R_x+1, top_L_y+150, 10, 0x475F); //right line again
    }else if(sprite == 6){ //covers to do button
        ST7735_DrawFastHLine(top_L_x, top_L_y-1+150, 34, 0x630C);  //top line
        ST7735_DrawFastHLine(top_L_x, top_L_y-2+150, 34, 0x630C);  //top line
        ST7735_DrawFastVLine(bot_R_x, top_L_y+150, 10, 0x630C); //right line
        ST7735_DrawFastVLine(bot_R_x+1, top_L_y+150, 10, 0x630C); //right line again
    }

}

int8_t Counter::counterFSM(uint8_t input){
    static int8_t debounce = 0;
    switch(state){
      case 0: //to do state 
        if((input&Prox) == 0){
            if(sprite!=0){
                ST7735_DrawBitmap(top_L_x, bot_R_y, todo, 32, 160); //draws the to do list
                sprite = 0;
                print();
                uint8_t y_cursor = 14;
                for(int i=0; i<5; i++){
                    ST7735_DrawChar(2, y_cursor, ToDoArr[i]+48, 0x0, 0xAE3B, 1);
                    y_cursor+=30;
                }
            }
            return 50;
        }else{
            if(sprite!=1){ //highlighted counter
                sprite = 1;
                print();
            }
            if(debounce>0){
                debounce--;
                return 50;
            }
            if((input&RButton) == 0x40){//refreshes the counter
                toDoOpen = 0;
                debounce = 20;
                state = 3;
                ST7735_FillRect(0, 0, 34, 159, 0x630C);
                ST7735_FillRect(0, 10, 40, 43, 0x630C);//covers where the cart would be
                ST7735_DrawFastHLine(bot_R_x, top_L_y, 32, 0x0); //right line again
                ST7735_DrawBitmap(top_L_x, bot_R_y, todo+4800, 32, 10); //draws the to do button at the bottom
                return 22;
            }
        }
        return 50; // the to do is open
      case 1: //at an empty counter
        if(debounce>0){
            debounce--;
            return -1;
        }
        if((input&Prox) == 0){ //not highlighted counter
            if(sprite!=4){
                sprite = 4;
                print();
            }
            return -1;
        }else{
            if(sprite!=3){ //highlighted counter
                sprite = 3;
                print();
            }
            if(((input&LButton) == 0x20) && ((input&material) != EMPTY) && (holdItem==0)){ //take item from player
                holdItem = input&material;
                sprite = 4;
                print(); 
                debounce = 20;
                return EMPTY;               //tells the main to empty player's hand
            }
            if((input&LButton) == 0x20 && (input&material) == EMPTY && (holdItem != EMPTY)){ //give item with no work done to player
                int8_t temp = holdItem;
                sprite = 4;
                holdItem = 0;
                print();
                debounce = 20;
                return temp;          //tells main item to return
            }
            return -1;
        }
      case 3://this is the state with the counter where the person can reactivate the counter
        if((input&Prox) == 0){ //
            if(sprite!=6 && sprite!=4){//to do
                sprite = 6;
                print();
            }
            return -1;
        }else{
            if(sprite!=5){
                sprite = 5;
                print();
            }
            if(debounce>0){
                debounce--;
                return -1;
            }
            if((input&RButton) == 0x40){
                toDoOpen = 1;
                state = 0;
                sprite = 0;
                debounce = 20;
                print();
                ST7735_FillRect(0, 0, 34, 156, 0x630C);
                ST7735_DrawBitmap(top_L_x, bot_R_y, todo, 32, 160); //draws the to do list 
                uint8_t y_cursor = 14;
                for(int i=0; i<5; i++){
                    ST7735_DrawChar(2, y_cursor, ToDoArr[i]+48, 0x0, 0xAE3B, 1);
                    y_cursor+=30;
                }
            }
        }
        return -1;
    }
    return -1;
}

void Counter::update(uint8_t input) {
    int8_t machineOut = counterFSM(input);
    if(machineOut==50){//ToDo open
      if(p1.getXPos() < 34){
        p1.setXPos(34);
        ST7735_DrawBitmap(p1.getXPos(), p1.getYPos(), miner, p1.getSize(), p1.getSize());
      }
      return;
    }else if(machineOut==22){
      if(machineArr1[3]->getSprite() != 2 && machineArr1[3]->getSprite() != 3){ //prints cart if not in animation
        machineArr1[3]->print();
      }
      ST7735_DrawBitmap(p1.getXPos(), p1.getYPos(), miner, p1.getSize(), p1.getSize());
      for(int i=0; i<3; i++){   //reprints all counters
        machineArr1[5+i]->setSprite(4);
        machineArr1[5+i]->print();
      }
    }else if(machineOut > -1){    //does counters
      p1.setPossession(machineOut);
      p1.printPosession(machineOut);
    }
}