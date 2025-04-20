 #ifndef _Player_h
 #define _Player_h
 #include <iostream>
 #include "Machine.h"
 using namespace std;
 
 class Player{
     friend class Machine; //machines can access
    
     private:
     uint8_t possession;    //player's item held
     int16_t posX;  //lower left coordinate
     int16_t posY;
     uint8_t last;  //last orientation
     uint8_t size;  //square so h == w

     public:
     Player();
     Player(uint8_t, uint8_t);
     bool moveUp(void); //movement methods
     bool moveDown(void);
     void translateDown(void);
     bool moveLeft(void);
     void translateLeft(void);
     bool moveRight(void);
     void translateRight(void);

     void resetCoordinates(int16_t*, int16_t*);
     int16_t getXPos(void); //gets for variables
     int16_t getYPos(void);
     void setXPos(int16_t);
     void setYPos(int16_t);
     uint8_t getLast(void);
     uint8_t getSize(void);
     uint8_t getPosession(void);
     void setPossession(uint8_t);
     void printPosession(uint8_t);

     uint8_t checkProximity(Machine); //checks the proximity to any machine
     uint8_t getMachineInput(Machine);
     bool inBounds(int16_t, int16_t, Machine**, uint8_t); //determines if the person is hitting anything important returns true if in bounds and false if out of bounds
     
 };
 #endif
