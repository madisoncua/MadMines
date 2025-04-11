 #include <cstdint>
#include <iostream>
 #include "Player.h"
 #include "../inc/ST7735.h"

//  Player::Player(int x, int y){
//      posX = x;
//      posY = y;
//      last = 0;
//  }

 Player::Player(){
     this->posX = 56;
     this->posY = 70;
     this->last = 0;
 }

 int16_t Player::getXPos(){return posX;} //return xPos
 int16_t Player::getYPos(){return posY;} //return yPos
 uint8_t Player::getLast(){return last;} //return last

bool Player::moveUp(){
     if(last == 2 && posX < 116){//left->up
        uint16_t temp = posX;
        posX = posY-44;
        posY = 160-temp;
        ST7735_SetRotation(0);
        posY-=2;
        last = 0;
        if(posY <= 44){posY = 44;}
      }else if(last == 3 && posX > 44){//right->up
        uint16_t temp = posX;
        posX = 128-posY;
        posY = temp+44;
        ST7735_SetRotation(0);
        posY-=2;
        last = 0;
        if(posY <= 44) {posY = 44;}
      }else if(last == 0){
        posY-=2;
        if(posY <= 44) {posY = 44;}
        last = 0;
      }
      return true;
 }

bool Player::moveDown(){
     if(last == 2 && posX > 44){//left->down
        uint16_t temp = posX;
        posX = 128-posY;
        posY = temp+44;
        ST7735_SetRotation(2);
        posY-=2;
        if(posY <= 44) {posY = 44;}
        last = 1;
        return true;
      }else if(last == 3 && posX < 116){//right->down
        uint16_t temp = posX;
        posX = posY-44;
        posY = 160-temp;
        ST7735_SetRotation(2);
        posY-=2;
        if(posY <= 44){posY = 44;}
        last = 1;
        return true;
      }else if(last == 0){//up->down
        posY = 160-posY+44;
        posX = 128-posX-44;
        ST7735_SetRotation(2);
        posY-=2;
        if(posY <= 44){posY = 44;}
        last = 1;
        return true;
      }
      return false;
 }
bool Player::moveLeft(){
      //if(last == 0){//up->left
      uint16_t temp = posY;
      posY = posX+44;
      posX = 160-temp;
      ST7735_SetRotation(3);
      last = 2;
      posY-=2;
      if(posY <= 44){posY = 44;}
      return true;
 }
bool Player::moveRight(){
      uint16_t temp = posX;
      posX = posY-44;
      posY = 128-temp;
      ST7735_SetRotation(1);
      last = 3;
      posY-=2;
      if(posY <= 44){posY = 44;}
      return true;
 }

 void Player::resetCoordinates(){
    if(last == 1){//down->up
      posY = 160-posY+44;
      posX = 128-posX-44;
    }else if(last == 2){//left->up
      uint16_t temp = posX;
      posX = posY-44;
      posY = 160-temp;
    }else if(last == 3){//right->up
      uint16_t temp = posX;
      posX = 128-posY;
      posY = temp+44;
    }
    last=0;
 }
