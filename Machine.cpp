 #include <iostream>
 #include "Machine.h"
 #include "../inc/ST7735.h"
 #include "images.h"
#include "../inc/Clock.h"

// //inputs (bits 0-4)
 #define material 0x1F
 enum Materials {EMPTY, SILVER_ORE, GOLD_ORE, DIAMOND_ORE, RUBY_ORE, EMERALD_ORE, 
 SILVER, GOLD, DIAMOND, RUBY, EMERALD, SWORD, SHEILD, RING, STAFF, WAND, TRASH};
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
    switch(state){
        case 0: //wait state
        if((input&Prox) ==0){      //ser to default state
            if(sprite ==0){return -1;} //don't reprint if already default
            else{
                sprite = 0;
                printRefiner(sprite);
                return -1;
            }
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
            //print progress bar
            sprite = 2;
            printRefiner(sprite);
            workTimer = 150;    //150 updates at 30Hz = 5 sec work time
            return -1;
         }
         return -1;
         case 1://working state
            if(((input&Prox) == 0) || (input&RButton) == 0){
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
            if((holdItem) > 2 && (holdItem) < 6){   //makes sure gem was input
                uint8_t temp = holdItem;
                holdItem = EMPTY;
                return temp+5;
            }else{
                holdItem = EMPTY;
                return TRASH;   //input was invalid to refining failed
            }
     }
 }

 int8_t Machine::updateRock(uint8_t input){
     switch(state){
         case 0: //wait state
         case 1://mining
     }

 }

 int8_t Machine::updateAnvil(uint8_t input){
     static int8_t AnvilItems[5];
     static int8_t anvilLength;
     switch(state){
         case 0: //wait state
            if((input&Prox) ==0){
                if(sprite ==0){return -1;}
                else{ //print default state
                sprite = 0;
                printAnvil(0);
                return -1;
                } 
            }else{
                if(sprite==1){return -1;}
                else{ //print highlighted state
                sprite = 1;
                printAnvil(1);
                return -1;
                }
            }
             if((input&LButton)==1){
                 //display menu
                 state++;
                 return -1;
             }
             if(((input&RButton)==1) && anvilLength!=0){
                 //change sprite to working sprite
                 state+=2; //going to the working state
                 return -1;
             } 
         case 1://menu
             if((input&LButton)==1){ //if LButton is pressed, eject from the menu screen (decrement state)
                 state--;
                 return -1;
             }
             if(((input&RButton)==1) && (((input&material)>=6) && ((input&material)<=10)) && anvilLength<=5){ //
                 anvilLength++;
                 AnvilItems[anvilLength-1] = (input&material);
                 return -1;
             }

         case 2: //working
         case 3: //done
     }

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
        ST7735_DrawBitmap(67, 35, refiner, 61, 35);
    }else if(sprite==1){ //highlighted refiner
        ST7735_DrawBitmap(67, 35, refinerHighlight, 61, 35);
    }else if(sprite==2){ //working refiner
        ST7735_DrawBitmap(67, 35, refinerWorking, 61, 35);
    }

 }

 void Machine::printAnvil(uint8_t sprite){
    if(sprite==0){ //default
        ST7735_DrawBitmap(20, 160, anvil, 66, 30);
    }else if(sprite==1){ //highlighted anvil
        ST7735_DrawBitmap(20, 160, anvilHighlight, 66, 30);
    }else if(sprite==2){ //working anvil
        ST7735_DrawBitmap(20, 160, anvilWorking, 66, 30);
    }

 }