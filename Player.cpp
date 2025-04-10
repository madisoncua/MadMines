#include <iostream>
#include "Player.h"

Player::Player(int x, int y){
    xPos = x;
    yPos = y;
}

Player::Player(){
    xPos = 56;
    yPos = 70;
}

void Player::moveUp(){
    switch
    yPos+=2;
}
void Player::moveDown(){
    yPos-=2;
}
void Player::moveLeft(){
    xPos-=2;
}
void Player::moveRight(){
    xPos+=2;
}
