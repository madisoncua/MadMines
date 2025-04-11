 #include <cstdint>
#include <iostream>
 #include "Player.h"
 #include "../inc/ST7735.h"

 Player::Player(){
     this->posX = 42;
     this->posY = 102;
     this->last = 0;
     this->size = 31;
 }

 int16_t Player::getXPos(){return posX;} //return xPos
 int16_t Player::getYPos(){return posY;} //return yPos
 uint8_t Player::getLast(){return last;} //return last
 uint8_t Player:getSize(){return size;}  //return size

bool Player::moveUp(){
     if(last == 2 && posX < 116){//left->up
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
      }else if(last == 3 && posX < 116){//right->down
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
