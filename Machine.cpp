// #include <iostream>
// //inputs (bits 0-4)
// #define material 0x1F
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
// #define LButton (1<<5)
// //Bit 6: RButton
// #define RButton (1<<6)
// //Bit 7:player proximity
// #define Prox (1<<7)

// void Machine::updateSmelter(uint8_t input){ //we will need to make each machine, which will only call its own update function
//     switch(state){
//         case 0: //wait state
//             if((input&Prox)==0){return;} //player is not in proximity
//             //highlight sprite here -->update graphic 

//             if(input&LButton){return;}  //no interaction 

//             if((input&material)==1 || (input&material)==2){
//                 //set work timer
//                 //change sprite to working sprite 
//                 state++;
//                 //output a sound??
//                 return;
//             }
//         case 1://working state
//         case 2: //done
//         case 3: //failure
//     }
// }

// void updateRefiner(uint8_t input){
//     switch(state){
//         case 0: //wait state
//         case 1://working state
//         case 2: //done
//     }

// }

// void updateRock(uint8_t input){
//     switch(state){
//         case 0: //wait state
//         case 1://mining
//     }

// }

// void updateAnvil(uint8_t input){
//     static int8_t AnvilItems[5];
//     static int8_t anvilLength;
//     switch(state){
//         case 0: //wait state
//             if((intput&Prox)==0){return;}
//             //Highlight sprite
//             if((input&LButton)==1){
//                 //display menu
//                 state++;
//                 return;
//             }
//             if(((input&RButton)==1) && anvilLength!=0){
//                 //change sprite to working sprite
//                 state+=2; //going to the working state
//                 return;
//             } 
//         case 1://menu
//             if((input&LButton)==1){ //if LButton is pressed, eject from the menu screen (decrement state)
//                 state--;
//                 return;
//             }
//             if(((input&RButton)==1) && (((input&material)>=6) && ((input&material)<=10)) && anvilLength<=5){ //
//                 anvilLength++;
//                 AnvilItems[anvilLength-1] = (input&material);
//                 return;
//             }

//         case 2: //working
//         case 3: //done
//     }

// }

// void updateTurnInArea(uint8_t input){
//     if((input&Prox) == 0){return;}
//     //Highlight sprite
//     if(((input&LButton)==1) && ((input&material)>=1 && (input&material)<=16)){ //checks for interaction and input
//         //update order graphic
//         //update score
//         //output good ding sound effect 
//     }else{
//         //deduct from score
//         //output stinky ding
//     }
// }