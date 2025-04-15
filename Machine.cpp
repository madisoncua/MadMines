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
             if((input&Prox)==0){//player is not in proximity
             //highlight sprite here -->update graphic 
                if(sprite!=0){
                    sprite=0;
                    printSmelter(sprite);
                }
                return -1;
             }else{
                if(sprite!=1){
                    sprite=1;
                    printSmelter(sprite);
                }
             }
             if((input&LButton) == 0x20 && (input&material)==1 || (input&material)==2){
                holdItem = input&material;
                workTimer = 150; //set work timer
                sprite = 2; //set sprite to working sprite
                printSmelter(sprite);
                state++;
                 //output a sound??
                return EMPTY;               //tells the main to empty player's hand
            }
            return -1;
         case 1://working state
            workTimer--;
            if(workTimer==0){
                //output sound???
                state++;
                workTimer = 150;//this is the done timer
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
                        printSmelter(sprite);
                    }else{
                        sprite = 3;
                        printSmelter(sprite);
                     }
                }
             }else{
                if((input&LButton) == 0){
                    if(sprite!=1){
                    sprite = 1;
                    printSmelter(sprite);
                    }
                }else{
                    state = 0;
                    int temp = holdItem+5;
                    holdItem = EMPTY;
                    return temp;
                }
             }
             return -1;
         case 3: //failure
            if((input&Prox)==0){
                if(sprite!=4){
                    sprite = 4;
                    printSmelter(sprite);
                }
                return -1;
            }else{
                if(sprite!=5){
                    sprite = 5;
                    printSmelter(sprite);
                }
                if((input&LButton) == 0x20 && (input&material) != EMPTY){
                    state = 0;
                    sprite = 0;
                    printSmelter(sprite);
                    return TRASH;
                }
            }
            return -1;
    }
}

int8_t Machine::updateRefiner(uint8_t input){
    static uint8_t wasWorking = 0;
    switch(state){
      case 0: //wait state
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
        if((input&LButton) == 0x20 && (input&material) != EMPTY){ //take item in for playing to work on
            holdItem = input&material;
            return EMPTY;               //tells the main to empty player's hand
        }
        if((input&LButton) == 0x20 && (input&material) == EMPTY && holdItem != EMPTY){ //give item with no work done to player
            int8_t temp = holdItem;
            holdItem = 0;
            return temp;          //tells main item to return
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
        if((input&RButton) == 0 || (input&Prox) == 0 && workTimer > 75){
            state--;
            return -1;
        }
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

 int8_t Machine::updateAnvil(uint8_t input){
    static int8_t AnvilItems[5];
    static int8_t anvilLength = 0;
    static int8_t menuDebounce = 0;
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
        if((input&LButton)==0x20){ 
            if(menuDebounce > 0){   //debounce time after removing menu
                menuDebounce--;
                return -1;
            }
            //print menu sprite and disable movement
            menuOpen = 1;
            sprite = 3;
            printAnvil(sprite);
            menuDebounce = 3;

            //specific item printing here

            state++; //going to the menu state
            return -1;
        }
        return -1;
    case 1://menu
        if((input&LButton)!=0){ //if LButton is pressed, eject from the menu screen (decrement state)
            if(menuDebounce > 0){   //debounce time after printing menu
                menuDebounce--;
                return -1;
            }
            sprite = 0;
            ST7735_FillRect(29, 52, 69, 57, 0x630C);//cover menu
            menuOpen = 0;
            state--;
            menuDebounce = 3;
            return 1;
        }
        if(((input&RButton)!=0) && anvilLength<5 && (input&material) != EMPTY){ //add player item
            AnvilItems[anvilLength++] = (input&material);
            return 0;
        }
        if(((input&RButton)!=0) && anvilLength>0 && (input&material) == EMPTY){ //give player item
            return AnvilItems[--anvilLength];
        }
        return -1;
        case 2: //working
        case 3: //done
     }

 }

 int8_t Machine::updateTurnInArea(uint8_t input){
    static int itemsArr[5]; 
    static score = 0;
    switch(state){
        case 0:
        if((input&Prox) == 0){
            return -1;
        }else{
            if(sprite!=1){
            sprite = 1;
            //print out the highlighted sprite
            }
            if((LButton&input)==0x20 && ((input&material)>=1 && (input&material)<=16)){
                holdItem = item&material;
                workTimer = 100; //set work timer
                state++;
                if(holdItem>=SWORD && holdItem<=KEY){ //fix array
                    if(itemArr[holdItem-11]>0){
                        itemArr[holdItem-11]--; //decrement the item in the arr
                        score+=200;
                        //output good ding
                    }
                }else{
                    score -= 100;
                    //output bad ding
                }
            }
            if((RButton&input)==1){
                sprite = 2;
                printTurnInArea(sprite);
                state = 2;
            }
        }
        return -1;
        case 1:
            workTimer--
            if(workTimer == 0){ //resets the score after "turn in processing is complete"
                sprite = 0;
                printTurnInArea(sprite); //set turn in area back to default
                int temp = score;
                int x_cursor = 110;
                while(temp!=0){
                    ST7735_SetCursor(x_cursor, 134);
                    ST7735_OutChar(score%10);
                    score /=10;
                    x_cursor -=2;
                }
                state=0;
            }
            if(workTime%15 == 0){//flashes 
                if(sprite==0){
                    sprite = 1;
                    printTurnInArea(sprite);
                }else{
                    sprite = 0;
                    printTurnInArea(sprite);
                }
            }
        return -1;
        case 2:
            if((RButton&input)==1){
                sprite = 0;
                printCounter();
                state = 0;
                return 100; //tell the cart to print 
            }
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
        ST7735_DrawBitmap(20, 160, anvil, 66, 30);
    }else if(sprite==1){ //highlighted anvil
        ST7735_DrawBitmap(20, 160, anvilHighlight, 66, 30);
    }else if(sprite==2){ //working anvil
        ST7735_DrawBitmap(20, 160, anvilWorking, 66, 30);
    }else if(sprite == 3){//print menu
        ST7735_DrawBitmap(29, 108, anvilMenu, 69, 57);
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

 void Machine::printSmelter(uint8_t sprite){
    if(sprite == 0){//defaul 
        ST7735_DrawBitmap(34, 159, smelter, 60, 48);    
    }else if(sprite == 1){ //highlight
        ST7735_DrawBitmap(34, 159, smelterHighlight, 60, 48);
    }else if(sprite == 2){//working
        ST7735_DrawBitmap(34, 159, smelterWorking, 60, 48);
    }else if(sprite == 3){ //red scaled image
        unsigned short red[2880];
        for(int i=0; i<2880; i++){
            if(smelterWorking[i]==0x630C){
                red[i] = 0x630C;
            }else{
                uint16_t p = smelterWorking[i];
                uint8_t r = (p >> 11) & 0x1F, g = (p >> 5) & 0x3F, b = p & 0x1F;

                // Approximate brightness: weights ≈ R*4 + G*2 + B
                uint8_t bright = (r << 2) + (g >> 1) + (b >> 2);

                // Keep red dominant, scale back to 5-bit
                red[i] = (bright & 0xF8) << 8;
            }
        }
        ST7735_DrawBitmap(34, 159, red, 60, 48);
    }else if(sprite==4){//failed
        ST7735_DrawBitmap(34, 159, smelterFail, 60, 48);  
    }else if(sprite==5){//failed highlight
        ST7735_DrawBitmap(34, 159, smelterFailHighlight, 60, 48);
    }
 }

 void Machine::printTurnInArea(uint8_t sprite, int* arr){
    if(sprite==0){//default state of turn in area
    }else if(sprite==1){ //twirling state of turn in area

    }else if(sprite==2){//to do menu with numbers
        ST7735_DrawBitmap(0, 159, todo, 32, 160); //draws the to do list
        int y_cursor = 40;
        for(int i=0; i<5; i++){
            ST7735_SetCursor(5, y_cursor);
            ST7735_OutChar(arr[i]+48);
            y_cursor+=40;
        }
    }
 }

 void Machine::printCounter(uint8_t sprite){

 }