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
// //Bit 5: LButton pick up/put down
 #define LButton (1<<5)
// //Bit 6: RButton interaction
 #define RButton (1<<6)
// //Bit 7:player proximity
 #define Prox (1<<7)

 Machine::Machine(uint8_t TLX, uint8_t TLY, uint8_t BRX, uint8_t BRY){
    sprite = 0;
    top_L_x = TLX;
    top_L_y = TLY;
    bot_R_x = BRX;
    bot_R_y = BRY;
 }

 void Machine::updateSmelter(uint8_t input){ //we will need to make each machine, which will only call its own update function
     switch(state){
         case 0: //wait state
             if((input&Prox)==0){

                return;
            } //player is not in proximity
             //highlight sprite here -->update graphic 

             if(input&LButton){return;}  //no interaction 

             if((input&material)==1 || (input&material)==2){
                 //set work timer
                 //change sprite to working sprite 
                 state++;
                 //output a sound??
                 return;
            }
         case 1://working state
         case 2: //done
         case 3: //failure
     }
 }

 void Machine::updateRefiner(uint8_t input){
     switch(state){
         case 0: //wait state
         if((input&Prox) ==0){      //ser to default state
            if(sprite ==0){return;} //don't reprint if already default
            else{
                sprite = 0;
                printRefiner(0);
                return;
            } 
         }else{
            if(sprite==2){return;}
            else{ //print highlighted state
                sprite = 2;
                printRefiner(2);
                return;
            }
         }
         case 1://working state
         case 2: //done
     }
 }

 void Machine::updateRock(uint8_t input){
     switch(state){
         case 0: //wait state
         case 1://mining
     }

 }

 void Machine::updateAnvil(uint8_t input){
     static int8_t AnvilItems[5];
     static int8_t anvilLength;
     switch(state){
         case 0: //wait state
            if((input&Prox) ==0){
                if(sprite ==1){return;}
                else{ //print default state
                sprite = 1;
                printAnvil(1);
                return;
                } 
            }else{
                if(sprite==2){return;}
                else{ //print highlighted state
                sprite = 2;
                printAnvil(2);
                return;
                }
            }
             if((input&LButton)==1){
                 //display menu
                 state++;
                 return;
             }
             if(((input&RButton)==1) && anvilLength!=0){
                 //change sprite to working sprite
                 state+=2; //going to the working state
                 return;
             } 
         case 1://menu
             if((input&LButton)==1){ //if LButton is pressed, eject from the menu screen (decrement state)
                 state--;
                 return;
             }
             if(((input&RButton)==1) && (((input&material)>=6) && ((input&material)<=10)) && anvilLength<=5){ //
                 anvilLength++;
                 AnvilItems[anvilLength-1] = (input&material);
                 return;
             }

         case 2: //working
         case 3: //done
     }

 }

 void Machine::updateTurnInArea(uint8_t input){
     if((input&Prox) == 0){return;}
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
    if(sprite==0){ //default wait
        ST7735_DrawBitmap(67, 35, refiner, 61, 35);
    }else if(sprite==1){ //highlighted refiner
        ST7735_DrawBitmap(67, 35, refinerHighlight, 61, 35);
    }else if(sprite==2){ //working refiner
        ST7735_DrawBitmap(67, 35, refinerWorking, 61, 35);
    }

 }

 void Machine::printAnvil(uint8_t sprite){
    if(sprite==0){ //default wait
        ST7735_DrawBitmap(40, 160, anvil, 66, 30);
    }else if(sprite==1){ //highlighted anvil
        ST7735_DrawBitmap(40, 160, anvilHighlight, 66, 30);
    }else if(sprite==2){ //working anvil
        ST7735_DrawBitmap(40, 160, anvilWorking, 66, 30);
    }

 }