 #include <cstdint>
#include <iostream>
 #include "Machine.h"
 #include "../inc/ST7735.h"
 #include "images.h"
 #include "../inc/Clock.h"
#include <stdio.h>
#include <stdint.h>
#include <ti/devices/msp/msp.h>
#include "../inc/LaunchPad.h"
#include "../inc/TExaS.h"
#include "../inc/Timer.h"

extern uint8_t menuOpen;
// //inputs (bits 0-4)
 #define material 0x1F
 enum Materials {EMPTY, SILVER_ORE, GOLD_ORE, DIAMOND_ORE, RUBY_ORE, EMERALD_ORE, 
 SILVER, GOLD, DIAMOND, RUBY, EMERALD, SWORD, SHIELD, WATCH, RING, KEY, TRASH};
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
// //16-trash
// //Bit 5: LButtong 
 #define LButton (1<<5)
// //Bit 6: RButton
 #define RButton (1<<6)
// //Bit 7:player proximity
 #define Prox (1<<7)
typedef struct itemHeld{
  const uint16_t* image;
  uint8_t w;
  uint8_t h;
}itemHeld;
extern itemHeld sprites[17];
const uint8_t recipes[5][5] = {{SILVER, DIAMOND, RUBY, 0, 0}, {SILVER, GOLD, DIAMOND, EMERALD, 0}, 
        {GOLD, GOLD, DIAMOND, DIAMOND, 0}, {GOLD, GOLD, DIAMOND, RUBY, EMERALD}, {SILVER, SILVER, EMERALD, 0, 0}};


 Machine::Machine(uint8_t TLX, uint8_t TLY, uint8_t BRX, uint8_t BRY){
    sprite = 100;   //start of game engine prints default
    state = 0;
    top_L_x = TLX;
    top_L_y = TLY;
    bot_R_x = BRX;
    bot_R_y = BRY;
    workTimer = 0;
 }

 int8_t Machine::updateSmelter(uint8_t input){ //we will need to make each machine, which will only call its own update function
    switch(state){
         case 0: //wait state
             if((input&Prox)==0){return -1;} //player is not in proximity
             //highlight sprite here -->update graphic 

             if(input&LButton){return -1;}  //no interaction 

             if((input&material)==1 || (input&material)==2){
                 //set work timer
                 //change sprite to working sprite 
                 state++;
                 //output a sound??
                 return -1;
            }
         case 1://working state
         case 2: //done
         case 3: //failure
    }
}

int8_t Machine::updateRefiner(uint8_t input){
    static uint8_t wasWorking = 0;
    static int8_t debounce = 0;
    switch(state){
      case 0: //wait state
        if(debounce > 0)debounce--;
        if((input&Prox) ==0){      //ser to default state
            if(sprite !=0){ //don't reprint if already default
                sprite = 0;
                printRefiner(sprite);
            }
            return -1;
        }else{//print highlighted state
            if(sprite!=1){
                sprite = 1;
                printRefiner(sprite);
            } //don't reprint if already highlighted
        }
        if((input&RButton) == 0X40 && holdItem !=0){ //start working and item to work on
            state++;
            sprite = 2;
            printRefiner(sprite);
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
            return EMPTY;               //tells the main to empty player's hand
        }
        if((input&LButton) == 0x20 && (input&material) == EMPTY && holdItem != EMPTY){ //give item with no work done to player
            int8_t temp = holdItem;
            holdItem = 0;
            debounce = 10;
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
                printRefiner(sprite);
            }else{
                sprite = 2;
                printRefiner(sprite);
            }
            //update progress bar every 10%
        }
        if(workTimer == 0){
            sprite = 0;
            printRefiner(sprite);
            state = 2;
            return -1;
        }
        return -1;
      case 2: //done state (is this state actually necessary if we auto output to nearby counter?)
        wasWorking = 0;
        if((holdItem) > 2 && (holdItem) < 6){   //makes sure gem was input
            uint8_t temp = holdItem;
            holdItem = EMPTY;
            state = 0;
            return temp+5;
        }else{
            holdItem = EMPTY;
            state = 0;
            return TRASH;   //input was invalid to refining failed
        }
    }
 }

 int8_t Machine::updateRock(uint8_t input){
    static uint8_t wasWorking = 0;
    switch(state){
      case 0: //wait state
        if((input&Prox) ==0){      //ser to default state
            if(sprite !=0){ //don't reprint if already default
                sprite = 0;
                printRock(sprite);
            }
            return -1;
        }else{//print highlighted state
            if(sprite!=1){
                sprite = 1;
                printRock(sprite);
            } //don't reprint if already highlighted
        }
        if((input&RButton) == 0x40){
            state++;
            if(!wasWorking){
                workTimer = 150;    //5 sec of work time
                wasWorking = 1;
            }
        }
        return -1;
      case 1://mining
        if(((input&RButton) == 0 || (input&Prox) == 0) && workTimer > 75){
            state--;
            wasWorking = 1;
            return -1;
        }
        if((input&RButton) == 0)return -1;
        workTimer--;
        if(workTimer%15 == 0){
            //update progress bar
            //maybe short sound effect?
        }
        if(workTimer == 75){    //print cracked halfway through
            sprite = 2;
            printRock(sprite);
        }
        if(workTimer == 0){
            wasWorking = 0;
            sprite = 0;
            state = 0;
            printRock(sprite);
            uint8_t randOre = SysTick->VAL%5+1; //should return random 1-5 (not sure how random though)
            return randOre;
        }
        return -1;
    }
    return -1;
 }

uint8_t Machine::computeRecipe(int8_t* list, int8_t len){
    if(len <3)return TRASH;
    uint8_t used[len];
    for (int k = 0; k < 5; k++) {
        for (int i = 0; i<len; i++) {
            used[i] = 0;
        }
        uint8_t isItem = 1;
        for (int i = 0; i < 3; i++) { //testing the sword
            uint8_t found = 0;
            for (int j = 0; list[j]>0 || j < 5; j++) {
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
    return TRASH;
}

void Machine::updateAnvilMenu(int8_t* AnvilItems, int8_t anvilLength){
    printAnvil(sprite);
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

 int8_t Machine::updateAnvil(uint8_t input){
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
                printAnvil(0);
                return -1;
            } 
        }else{
            if(sprite!=1){//print highlighted state
                sprite = 1;
                printAnvil(1);
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
            printAnvil(sprite);
            if(!wasWorking){
                workTimer = 150;
                wasWorking = 1;
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
        if(((input&RButton) == 0x40) && anvilLength<5 && (((input&material) > EMPTY && (input&material) < SWORD) || (input&material) == TRASH)){ //add player's item
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
                if(sprite == 0){
                    sprite = 2;
                }else{
                    sprite = 0;
                }
                printAnvil(sprite);
            }
            if(workTimer == 0){
                wasWorking = 0;
                uint8_t creation = computeRecipe(AnvilItems, anvilLength);
                anvilLength = 0;
                sprite = 0;
                state = 0;
                return creation;
            }
            return -1;
        case 3: //done
     }

 }

 int8_t Machine::updateCart(uint8_t input){
    static uint8_t wasWorking = 0;
    switch(state){
      case 0: //wait state
        if((input&Prox) ==0){      //ser to default state
            if(sprite !=0){ //don't reprint if already default
                sprite = 0;
                printRock(sprite);
            }
            return -1;
        }else{//print highlighted state
            if(sprite!=1){
                sprite = 1;
                printRock(sprite);
            } //don't reprint if already highlighted
        }
        if((input&RButton) == 0x40){
            state++;
            if(!wasWorking){
                workTimer = 150;    //5 sec of work time
                wasWorking = 1;
            }
        }
        return -1;
      case 1://mining
        if(((input&RButton) == 0 || (input&Prox) == 0) && workTimer > 75){
            state--;
            wasWorking = 1;
            return -1;
        }
        if((input&RButton) == 0)return -1;
        workTimer--;
        if(workTimer%15 == 0){
            //update progress bar
            //maybe short sound effect?
        }
        if(workTimer == 75){    //print cracked halfway through
            sprite = 2;
            printRock(sprite);
        }
        if(workTimer == 0){
            wasWorking = 0;
            sprite = 0;
            state = 0;
            printRock(sprite);
            uint8_t randOre = SysTick->VAL%5+1; //should return random 1-5 (not sure how random though)
            return randOre;
        }
        return -1;
    }
    return -1;
 }

 int8_t Machine::updateTurnInArea(uint8_t input){
     if((input&Prox) == 0){return -1;}
     //Highlight sprite
     if(((input&LButton)==1) && ((input&material)>=1 && (input&material)<=16)){ //checks for interaction and input
         //update order graphic
         //update score
         //output good ding sound effect 
     }else{
         //deduct from score
         //output stinky ding
     }
 }

 
 void Machine::printRefiner(uint8_t sprite){
    if(sprite==0){ //default
        ST7735_DrawBitmap(67, 34, refiner, 61, 35);
    }else if(sprite==1){ //highlighted refiner
        ST7735_DrawBitmap(67, 34, refinerHighlight, 61, 35);
    }else if(sprite==2){ //working refiner
        ST7735_DrawBitmap(67, 34, refinerWorking, 61, 35);
    }

 }

 void Machine::printAnvil(uint8_t sprite){
    if(sprite==0){ //default
        ST7735_DrawBitmap(20, 159, anvil, 66, 30);
    }else if(sprite==1){ //highlighted anvil
        ST7735_DrawBitmap(20, 159, anvilHighlight, 66, 30);
    }else if(sprite==2){ //working anvil
        ST7735_DrawBitmap(20, 159, anvilWorking, 66, 30);
    }else if(sprite == 3){//print menu
        ST7735_DrawBitmap(25, 112, anvilMenu, 78, 64);
    }

 }

 void Machine::printCart(uint8_t sprite){
    static int i = 0;
    if(sprite == 0){
        ST7735_DrawBitmap(20, 54, cart, 46, 43);
    }else if(sprite == 1){
        ST7735_DrawBitmap(20, 54, cartHighlight, 46, 43);
    }else if(sprite == 3){//cart leaving
        ST7735_DrawBitmap(20, 52-i*7, cart, 46, 41);
        i++;
        i%=6;
    }
 }

 void Machine::printRock(uint8_t sprite){
    if(sprite == 0){
        ST7735_DrawBitmap(67, 42, rock, 44, 34);    //default rock
    }else if(sprite == 1){
        ST7735_DrawBitmap(67, 42, highlightRock, 44, 34);
    }else if(sprite == 2){
        ST7735_DrawBitmap(67, 42, workingRock, 44, 34);
    }
 }