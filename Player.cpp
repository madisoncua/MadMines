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
    this->size = 26;
 }

 int16_t Player::getXPos(){return posX;} //return xPos
 int16_t Player::getYPos(){return posY;} //return yPos
 uint8_t Player::getLast(){return last;} //return last
 uint8_t Player::getSize(){return size;}  //return size

 extern itemHeld sprites[17];
 extern int8_t toDoOpen;

bool Player::moveUp(){
      posY-=2;
      if(posY <= size+10) {
        posY = size+10;
        return false;
      }
      return true;
 }

 void Player::setXPos(int16_t x){
  posX = x;
 }

 void Player::setYPos(int16_t y){
  posY = y;
 }

bool Player::moveDown(){
  posY = 160-posY+size;
  posX = 128-posX-size;
  posY-=2;
  if(posY <= size){
    posY = size;
    return false;
  }
  last = 1;
  return true;
}
void Player::translateDown(){
  posY = 160-posY+size;
  posX = 128-posX-size;
  last = 1;
  ST7735_SetRotation(2);
}

bool Player::moveLeft(){
  uint16_t temp = posY;
  posY = posX+size;
  posX = 160-temp;
  posY-=2;
  if(posY <= size){
    posY = size;
    return false;
  }
  last = 2;
  return true;
 }
void Player::translateLeft(){
  uint16_t temp = posY;
  posY = posX+size;
  posX = 160-temp;
  last = 2;
  ST7735_SetRotation(3);
}

bool Player::moveRight(){
  uint16_t temp = posX;
  posX = posY-size;
  posY = 128-temp;
  posY-=2;
  if(posY <= size){
    posY = size;
    return false;
  }
  last = 3;
  return true;
 }
void Player::translateRight(){
  uint16_t temp = posX;
  posX = posY-size;
  posY = 128-temp;
  ST7735_SetRotation(1);
  last = 3;
}

void Player::resetCoordinates(int16_t* X, int16_t* Y){
  if(last == 1){//down->up
    *Y = 160-(*Y)+size;
    *X = 128-(*X)-size;
  }else if(last == 2){//left->up
    uint16_t temp = (*X);
    *X = (*Y)-size;
    *Y = 160-temp;
  }else if(last == 3){//right->up
    uint16_t temp = (*X);
    (*X) = 128-(*Y);
    (*Y) = temp+size;
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

bool Player::inBounds(int16_t x, int16_t y, Machine** m, uint8_t length, uint8_t mainNum){
  int16_t smaller;
  int16_t bigger;
  uint8_t delta;
  int16_t m_rightX;
  uint8_t m_leftX;
  //////////////////////////////for main 2//////////////////////////////////////
  //smelter: if main=2, index is 0, and state = 1

  //////////////////////////////for main 1//////////////////////////////////////
  //refiner: if main=1, index = 0, and was working = 1
  //rock1: if main=1, index=2, and wasWorking=1
  for(int i=0; i<length; i++){
    

    if(toDoOpen==0 && i==4){continue;}
    m_rightX = m[i]->bot_R_x;
    m_leftX = m[i]->top_L_x;
    if(i == 3 && m[i]->sprite == 4){  //cart sprite is ladder (smaller hit box)
      m_rightX-=15;
    }
    //progress bar checks
    if(mainNum==1){
      if((i==0)&& (m[i]->wasWorking==1)){ //refiner
        m_leftX-=20;
      }else if((i==2)&&(m[i]->wasWorking==1)){ //rock1
        if(inBounds(x, y, m+11, 1, 0)==false){
          return false;
        }
      }
    }
    if((mainNum==2)&&(i==0)&&(m[i]->state==1)){//smelter
      if(inBounds(x, y, m+9, 1, 0)==false){
        return false;
      }
    }

    if(x<m_leftX){
      smaller = x;
      bigger = m_leftX;
      delta = size;
    }else{
      smaller = m_leftX;
      bigger = x;
      delta = m_rightX - m_leftX;
    }
    if(bigger > smaller+delta){
      continue;
    }
    if(y<m[i]->bot_R_y){
      smaller = y;
      bigger = m[i]->bot_R_y;
      delta = m[i]->bot_R_y - m[i]->top_L_y;
    }else{
      smaller = m[i]->bot_R_y;
      bigger = y;
      delta = size;
    }
    if(bigger-delta <= smaller){
      return false;
    }
  }
  return true;
}