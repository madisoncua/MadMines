// Authors: Evan Roberts and Madison Cua
// Last Modified: 4/24/2025
#include <cstdint>
#include <iostream>
#include "Machine.h"
#include "ST7735.h"
#include "IRxmt.h"
#include "Sound.h"
#include "UART2.h"
#include "images2.h"
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

extern uint8_t menuOpen, sound_flag;
extern int16_t score;
extern uint8_t deadTimer;
extern Machine* machineArr2[10];
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
const uint8_t recipes[5][3] = {{SILVER, DIAMOND, RUBY}, {DIAMOND, RUBY, EMERALD}, {GOLD, DIAMOND, EMERALD}, 
                                {GOLD, GOLD, RUBY}, {SILVER, SILVER, EMERALD}};
Player p2; //player 2
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
Smelter::Smelter(uint8_t TLX, uint8_t TLY, uint8_t BRX, uint8_t BRY, uint8_t PBX, uint8_t PBY)
        : Machine(TLX, TLY, BRX, BRY, PBX, PBY){}

void Smelter::print() {
    if(menuOpen==1){
         return;
    }
    if(sprite == 0){//defaul 
        ST7735_DrawBitmap(top_L_x, bot_R_y, smelter, (bot_R_x-top_L_x), (bot_R_y-top_L_y));    
    }else if(sprite == 1){ //highlight
        ST7735_DrawBitmap(top_L_x, bot_R_y, smelterHighlight, (bot_R_x-top_L_x), (bot_R_y-top_L_y));
    }else if(sprite == 2){//working
        ST7735_DrawBitmap(top_L_x, bot_R_y, smelterWorking, (bot_R_x-top_L_x), (bot_R_y-top_L_y));
    }else if(sprite == 3){ //red scaled image
        unsigned short red[2880];
        for(int i=0; i<2880; i++){
            if(smelterWorking[i]==0x630C){
                red[i] = 0x630C;
            }else{
                uint16_t p = smelterWorking[i];
                uint8_t r = (p >> 11) & 0x1F;
                uint8_t g = (p >> 5) & 0x3F;
                uint8_t b = p & 0x1F;

                // Approximate brightness: R*4 + G*2 + B
                uint8_t bright = (r << 2) + (g >> 1) + (b >> 2);

                // Convert brightness to 5-bit red
                uint16_t red565 = (bright >> 3) << 11;

                // Swap bytes to match display format
                red[i] = (red565 >> 8) | (red565 << 8);
            }
        }
        ST7735_DrawBitmap(top_L_x, bot_R_y, red, (bot_R_x-top_L_x), (bot_R_y-top_L_y));
    }else if(sprite==4){//failed
        ST7735_DrawBitmap(top_L_x, bot_R_y, smelterFail, (bot_R_x-top_L_x), (bot_R_y-top_L_y));  
    }else if(sprite==5){//failed highlight
        ST7735_DrawBitmap(top_L_x, bot_R_y, smelterFailHighlight, (bot_R_x-top_L_x), (bot_R_y-top_L_y));
    }
}

int8_t Smelter::smelterFSM(uint8_t input){ //we will need to make each machine, which will only call its own update function
    switch(state){
      case 0: //wait state
        if((input&Prox)==0){//player is not in proximity
            //highlight sprite here -->update graphic 
           if(sprite!=0){
                sprite=0;
                print();
            }
            return -1;
        }else{
            if(sprite!=1){
                sprite=1;
                print();
            }
        }
        if(workTimer > 0){
            workTimer--;
            return -1;
        }
        if((input&LButton) == 0x20 && ((input&material)==1 || (input&material)==2)){
            holdItem = input&material;
            workTimer = 150; //set work timer
            sprite = 2; //set sprite to working sprite
            print();
            ST7735_DrawFastHLine(progX, progY, progH, 0x0);  //top line
            ST7735_DrawFastVLine(progX, progY, progW, 0x0); //left line
            ST7735_DrawFastHLine(progX, progY+progW-1, progH, 0x0);  //bottom
            ST7735_DrawFastVLine(progX+progH-1, progY, progW, 0x0); //right line
            ST7735_FillRect(progX+1, progY+1, progH-2, progW-2, 0x4208); //fills inside of empty progress bar
            state++;
            //output a sound??
            return 80;               //tells the main to empty player's hand
        }else if((input&LButton) == 0x20){
            Sound_Wrong();
        }
        return -1;
      case 1://working state
        workTimer--;
        if(workTimer==0){
            //output sound???
            ST7735_FillRect(progX, progY, progH, progW, 0x630C); //fills inside of empty progress bar
            state++;
            workTimer = 150;//this is the done timer
            return -1;
        }
        if(workTimer%15==0){
            ST7735_FillRect(progX+progH-2*((150-workTimer)/15)-1, progY+1, (progH-2)/10, progW-2, 0x001F);
        }
        return -1;
      case 2: //done
        workTimer--;
        if(workTimer==0){
            //play fail sound 
            state++;
            return -1;
        }
        if((input&Prox)==0){//player is not in proximity
            if(workTimer%15==0){
                if(sprite == 3){ //sprite ==3 represents the red scaled image
                    sprite = 2;
                    print();
                }else{
                    sprite = 3;
                    print();
                }
            }
        }else{
            if((input&LButton) == 0 || (input&material) != EMPTY){
                if(sprite!=1){
                    sprite = 1;
                    print();
                }
           }else{
                state = 0;
                int temp = holdItem+5;
                holdItem = EMPTY;
                workTimer = 15;
                return temp;
            }
        }
        return -1;
      case 3: //failure
        if((input&Prox)==0){
            if(sprite!=4){
                sprite = 4;
                print();
            }
            return -1;
        }else{
            if(sprite!=5){
                sprite = 5;
                print();
            }
            if((input&LButton) == 0x20 && (input&material) == EMPTY){
                state = 0;
                sprite = 0;
                workTimer = 20;
                print();
                return TURNIP;
            }
        }
        return -1;
    }
    return -1;
}

void Smelter::update(uint8_t input) {
    int8_t machineOut = smelterFSM(input);
    if(machineOut > -1){
      if(machineOut == 80){
        if(p2.getXPos() > 73){
          p2.setXPos(73);
          ST7735_FillRect(72, 43, 55, 25, 0x630C);
          ST7735_DrawBitmap(p2.getXPos(), p2.getYPos(), miner, p2.getSize(), p2.getSize());
        }
        p2.setPossession(0);
        p2.printPosession(0);
      }else{
        p2.setPossession(machineOut);
        p2.printPosession(machineOut);
      }
    }
}

//proximity constructor
Anvil::Anvil(uint8_t TLX, uint8_t TLY, uint8_t BRX, uint8_t BRY, uint8_t PBX, uint8_t PBY, uint8_t XL, uint8_t XR, uint8_t YT, uint8_t YB)
        : Machine(TLX, TLY, BRX, BRY, PBX, PBY, XL, XR, YT, YB) {}

void Anvil::print() {
    if(sprite==0){ //default
        ST7735_DrawBitmap(top_L_x, bot_R_y, anvil, 66, 30);
        if(holdItem!=0){
            ST7735_FillRect(top_L_x+14, top_L_y+5, 35, 24, 0x39E7);
             uint32_t size = sprites[holdItem].h * sprites[holdItem].w;
             unsigned short blendedItem[size];
             for(int i=0; i<size; i++){
                 if(sprites[holdItem].image[i] == 0x630C){
                     blendedItem[i] = 0x39E7;
                 }else{
                    blendedItem[i] = sprites[holdItem].image[i];
                 }
            }
            ST7735_DrawBitmap((top_L_x)+((bot_R_x-top_L_x)/2)-sprites[holdItem].w/2, ((top_L_y)+(bot_R_y+2-top_L_y)/2)+sprites[holdItem].h/2, blendedItem, sprites[holdItem].w, sprites[holdItem].h);
        }
    }else if(sprite==1){ //highlighted anvil
        ST7735_DrawBitmap(top_L_x, bot_R_y, anvilHighlight, 66, 30);
        if(holdItem!=0){
            ST7735_FillRect(top_L_x+14, top_L_y+5, 35, 24, 0x39E7);
             uint32_t size = sprites[holdItem].h * sprites[holdItem].w;
             unsigned short blendedItem[size];
             for(int i=0; i<size; i++){
                 if(sprites[holdItem].image[i] == 0x630C){
                     blendedItem[i] = 0x39E7;
                 }else{
                    blendedItem[i] = sprites[holdItem].image[i];
                 }
            }
            ST7735_DrawBitmap((top_L_x)+((bot_R_x-top_L_x)/2)-sprites[holdItem].w/2, ((top_L_y)+(bot_R_y+2-top_L_y)/2)+sprites[holdItem].h/2, blendedItem, sprites[holdItem].w, sprites[holdItem].h);
        }
    }else if(sprite==2){ //working anvil
        ST7735_DrawBitmap(top_L_x, bot_R_y, anvilWorking, 66, 30);
    }else if(sprite == 3){//print menu
        ST7735_DrawBitmap(25, 112, anvilMenu, 78, 64);
    }
}

uint8_t Anvil::computeRecipe(int8_t* list, int8_t len){
    if(len <3)return TURNIP;
    uint8_t used[len];
    for (int k = 0; k < 5; k++) {   //test each item
        if(len != 3)continue;
        for (int i = 0; i<len; i++) {
            used[i] = 0;
        }
        uint8_t isItem = 1;
        for (int i = 0; i < 3; i++){
            uint8_t found = 0;
            for (int j = 0; j<len; j++) {
                if(used[j])continue;
                if(recipes[k][i] == list[j]){
                    found = 1;
                    used[j] = 1;
                    break;
                }
            }
            if(!found){
                isItem = 0;
                break;
            }
        }
        if(isItem)return SWORD+k;
    }
    return TURNIP;
}

void Anvil::updateAnvilMenu(int8_t* AnvilItems, int8_t anvilLength){
    print();
    if(anvilLength > 0){
        ST7735_FillRect(28, 63, 22, 22, 0x630C);
        ST7735_DrawBitmap(39-sprites[AnvilItems[0]].w/2, 74+sprites[AnvilItems[0]].h/2, sprites[AnvilItems[0]].image, sprites[AnvilItems[0]].w, sprites[AnvilItems[0]].h);
    }
    if(anvilLength > 1){
        ST7735_FillRect(53, 63, 22, 22, 0x630C);
        ST7735_DrawBitmap(64-sprites[AnvilItems[1]].w/2, 74+sprites[AnvilItems[1]].h/2, sprites[AnvilItems[1]].image, sprites[AnvilItems[1]].w, sprites[AnvilItems[1]].h);
    }
    if(anvilLength > 2){
        ST7735_FillRect(78, 63, 22, 22, 0x630C);
        ST7735_DrawBitmap(89-sprites[AnvilItems[2]].w/2, 74+sprites[AnvilItems[2]].h/2, sprites[AnvilItems[2]].image, sprites[AnvilItems[2]].w, sprites[AnvilItems[2]].h);
    }
    if(anvilLength > 3){
        ST7735_FillRect(41, 88, 22, 22, 0x630C);
        ST7735_DrawBitmap(52-sprites[AnvilItems[3]].w/2, 99+sprites[AnvilItems[3]].h/2, sprites[AnvilItems[3]].image, sprites[AnvilItems[3]].w, sprites[AnvilItems[3]].h);
    }
    if(anvilLength > 4){
        ST7735_FillRect(66, 88, 22, 22, 0x630C);
        ST7735_DrawBitmap(77-sprites[AnvilItems[4]].w/2, 99+sprites[AnvilItems[4]].h/2, sprites[AnvilItems[4]].image, sprites[AnvilItems[4]].w, sprites[AnvilItems[4]].h);
    }
}

int8_t Anvil::anvilFSM(uint8_t input){
    static int8_t AnvilItems[5];
    static int8_t anvilLength = 0;
    static int8_t menuDebounce = 0;
    static uint8_t wasWorking = 0;
    switch(state){
        case 0: //wait state
        if((input&Prox) ==0){
            if(sprite ==0){
                return -1;
            }else{ //print default state
                sprite = 0;
                print();
                return -1;
            } 
        }else{
            if(sprite!=1){//print highlighted state
                sprite = 1;
                print();
            }
        }
        if(menuDebounce > 0){   //debounce time after button press in menu
            menuDebounce--;
            return -1;
        }
        if((input&RButton) == 0x40 && anvilLength > 0){
            //NOT testing if this is a valid combination
            state+=2;
            sprite = 2;
            print();
            if(!wasWorking){
                workTimer = 150;
                wasWorking = 1;
                //progress bar
                ST7735_DrawFastHLine(progX, progY, progW, 0x0);  //top line
                ST7735_DrawFastVLine(progX, progY, progH, 0x0); //left line
                ST7735_DrawFastHLine(progX, progY+progH-1, progW, 0x0);  //bottom
                ST7735_DrawFastVLine(progX+progW-1, progY, progH, 0x0); //right line
                ST7735_FillRect(progX+1, progY+1, progW-2, progH-2, 0x4208); //fills inside of empty progress bar
            }
            return -1;
        }
        if((input&LButton)==0x20){ //print menu sprite and disable movement
            menuOpen = 1;
            sprite = 3;
            menuDebounce = 10;
            updateAnvilMenu(AnvilItems, anvilLength);
            state++; //going to the menu state
            return -1;
        }
        return -1;
    case 1://menu
        if(menuDebounce > 0){   //debounce time after printing menu
            menuDebounce--;
            return -1;
        }
        if((input&LButton)!=0){ //if LButton is pressed, eject from the menu screen (decrement state)
            sprite = 0;
            ST7735_FillRect(25, 48, 78, 65, 0x630C);//cover menu
            menuOpen = 0;
            state--;
            menuDebounce = 10;
            return 20;  //number that isn't an item to indicate reprint player
        }
        if(wasWorking)return -1; //don't let player add or remove items when in the middle of working
        if(((input&RButton) == 0x40) && anvilLength<5 && (((input&material) > EMPTY && (input&material) < SWORD) || (input&material) == TURNIP || (input&material) == BOOT)){ //add player's item
            AnvilItems[anvilLength++] = (input&material);
            menuDebounce = 10;
            updateAnvilMenu(AnvilItems, anvilLength);
            return 0;
        }
        if(((input&RButton)!=0) && anvilLength>0 && (input&material) == EMPTY){ //give player item
            menuDebounce = 10;
            uint8_t temp = AnvilItems[--anvilLength];
            updateAnvilMenu(AnvilItems, anvilLength);
            return temp;
        }
        return -1;
        case 2: //working
            if((input&Prox) == 0 || (input&RButton) == 0){
                state-=2;
                return -1;
            }
            workTimer--;
            if(workTimer%15 == 0){
                Sound_Clang();
                if(sprite == 0){
                    sprite = 2;
                }else{
                    sprite = 0;
                }
                ST7735_FillRect(progX+1, progY+progH-2*((150-workTimer)/15)-1, progW-2, (progH-2)/10, 0x001F);
                print();
            }
            if(workTimer == 0){
                wasWorking = 0;
                holdItem = computeRecipe(AnvilItems, anvilLength);
                anvilLength = 0;
                ST7735_FillRect(progX, progY, progW, progH, 0x630C); //fills inside of empty progress bar
                sprite = 4;
                print();
                state++;
                return -1;
            }
            return -1;
        case 3://done
            if((input&Prox)==0){
                if(sprite!=0){
                    sprite = 0;
                    print();
                }
            }else{
                if(sprite!=1){
                    sprite = 1;
                    print();
                }
                if(menuDebounce > 0){   //debounce time after printing menu
                    menuDebounce--;
                    return -1;
                }
                if((input&LButton)==0x20 && ((input&material)==EMPTY)){ //if LButton is pressed, eject from the menu screen (decrement state)
                    int temp = holdItem;
                    sprite = 0;
                    holdItem = 0;
                    print();
                    state = 0;
                    menuDebounce = 10;
                    return temp;  //number that isn't an item to indicate reprint player
                }
            }
            return -1;  
            
     }
    return -1;
}

void Anvil::update(uint8_t input) {
    int8_t machineOut = anvilFSM(input);
    if(machineOut > -1){
      if(machineOut == 20){
        machineArr2[3]->print();
        ST7735_DrawBitmap(p2.getXPos(), p2.getYPos(), miner, p2.getSize(), p2.getSize());
        machineArr2[0]->print();
      }else{
        p2.setPossession(machineOut);
        p2.printPosession(machineOut);
      }
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
                return -1; //no bounce because progress bar is in unreachable place
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
        p2.setPossession(machineOut);
        p2.printPosession(machineOut);
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
        if(p2.getYPos() <= 80 && p2.getXPos() <= 51){
          ST7735_FillRect(p2.getXPos(), p2.getYPos()-p2.getSize(), p2.getSize(), p2.getSize(), 0x630C);
          p2.setXPos(51);
          p2.setYPos(93);
          deadTimer = 90;
        }
      }else{
        p2.setPossession(machineOut);
        p2.printPosession(machineOut);
      }
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
    }
    return -1;
}

void Counter::update(uint8_t input) {
    int8_t machineOut = counterFSM(input);
    if(machineOut > -1){
        p2.setPossession(machineOut);
        p2.printPosession(machineOut);
    }
}