 #include <cstdint>
#include <iostream>
 #include "Player.h"
 #include "../inc/ST7735.h"
 #include "Machine.h"

 Player::Player(){
    this->possession = 0; 
    this->posX = 42;    
    this->posY = 102;
    this->last = 0; 
    this->size = 24;
 }

 int16_t Player::getXPos(){return posX;} //return xPos
 int16_t Player::getYPos(){return posY;} //return yPos
 uint8_t Player::getLast(){return last;} //return last
 uint8_t Player::getSize(){return size;}  //return size

 extern itemHeld sprites[17];

bool Player::moveUp(){
     if(last == 2 && posX < (160-size)){//left->up
        uint16_t temp = posX;
        posX = posY-size;
        posY = 160-temp;
        ST7735_SetRotation(0);
        posY-=2;
        last = 0;
        if(posY <= size){
          posY = size;
          return false;
        }
      }else if(last == 3 && posX > size){//right->up
        uint16_t temp = posX;
        posX = 128-posY;
        posY = temp+size;
        ST7735_SetRotation(0);
        posY-=2;
        last = 0;
        if(posY <= size) {posY = size;}
      }else if(last == 0){
        posY-=2;
        if(posY <= size) {posY = size;}
        last = 0;
      }
      return true;
 }

bool Player::moveDown(){
     if(last == 2 && posX > size){//left->down
        uint16_t temp = posX;
        posX = 128-posY;
        posY = temp+size;
        ST7735_SetRotation(2);
        posY-=2;
        if(posY <= size) {posY = size;}
        last = 1;
        return true;
      }else if(last == 3 && posX < (160-size)){//right->down
        uint16_t temp = posX;
        posX = posY-size;
        posY = 160-temp;
        ST7735_SetRotation(2);
        posY-=2;
        if(posY <= size){posY = size;}
        last = 1;
        return true;
      }else if(last == 0){//up->down
        posY = 160-posY+size;
        posX = 128-posX-size;
        ST7735_SetRotation(2);
        posY-=2;
        if(posY <= size){posY = size;}
        last = 1;
        return true;
      }
      return false;
 }
bool Player::moveLeft(){
      uint16_t temp = posY;
      posY = posX+size;
      posX = 160-temp;
      ST7735_SetRotation(3);
      last = 2;
      posY-=2;
      if(posY <= size){posY = size;}
      return true;
 }
bool Player::moveRight(){
      uint16_t temp = posX;
      posX = posY-size;
      posY = 128-temp;
      ST7735_SetRotation(1);
      last = 3;
      posY-=2;
      if(posY <= size){posY = size;}
      return true;
 }

 void Player::resetCoordinates(){
    if(last == 1){//down->up
      posY = 160-posY+size;
      posX = 128-posX-size;
    }else if(last == 2){//left->up
      uint16_t temp = posX;
      posX = posY-size;
      posY = 160-temp;
    }else if(last == 3){//right->up
      uint16_t temp = posX;
      posX = 128-posY;
      posY = temp+size;
    }
    last=0;
 }

 uint8_t Player::getMachineInput(Machine m){ //configures the input for a given machine
    uint8_t input = 0;
    input |= possession; //get the player's posession in bits 0-4
    input |= (checkProximity(m)<<7);
    return input;
 }

 void Player::setPossession(uint8_t newItem){ //to put item in the machine
  possession = newItem;
 }

uint8_t Player::checkProximity(Machine m){ //check the proximity to any given machine
//note: there is no size account for the top and right side of the machine because bottom left corner is "close" to those sides for any given time
uint8_t buffer = 2;
//left --> right --> bottom --> top
 if((posX >= (m.proXL-size-buffer)) && (posX<= (m.proXR+buffer)) && (posY <= (m.proYB+size+buffer)) && (posY >= (m.proYT-buffer))){ //checks bounding box 
    return 1;
 } 
 return 0;
 }

 void Player::printPosession(uint8_t machineOut){
  ST7735_FillRect(107, 139, 20, 21, 0x630C);
  if(machineOut != 0){
    ST7735_DrawBitmap(117-sprites[machineOut].w/2, 149+sprites[machineOut].h/2, sprites[machineOut].image, sprites[machineOut].w, sprites[machineOut].h);
  }
 }
