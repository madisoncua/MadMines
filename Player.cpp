 #include <cstdint>
#include <iostream>
 #include "Player.h"
 #include "../inc/ST7735.h"
 #include "Machine.h"

 Player::Player(){
    possession = 0;
     this->posX = 42;
     this->posY = 102;
     this->last = 0;
     this->size = 33;
 }

 int16_t Player::getXPos(){return posX;} //return xPos
 int16_t Player::getYPos(){return posY;} //return yPos
 uint8_t Player::getLast(){return last;} //return last
 uint8_t Player::getSize(){return size;}  //return size

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
      //if(last == 0){//up->left
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

 uint8_t Player::takePosession(){ //to put item in the machine
  uint8_t temp = possession;
  possession = 0;    //player hand is empty
  return temp;      //give machine player item
 }

uint8_t Player::checkProximity(Machine m){ //check the proximity to any given machine
 if((posX>=m.top_L_x) && (posX<= m.bot_R_x) && (posY <= (m.bot_R_y+this->getSize()))){ //checks bounding box for the bottom side of a machine
    return 1;
 }
 if((posX>= (m.top_L_x-this->getSize())) && (posY >= m.top_L_y) && (posY <= m.bot_R_y)){ //checks bounding box for left side of machine
    return 1;
 }
 if((posX>=m.top_L_x) && (posX<= m.bot_R_x) && (posY <= (m.bot_R_y-this->getSize()))){ //checks bounding box for top side of machine
    return 1;
 }
 if((posX>= (m.top_L_x+this->getSize())) && (posY >= m.top_L_y) && (posY <= m.bot_R_y)){ //checks bounding box for right side of machine
    return 1;
 }
 return 0;
 }
