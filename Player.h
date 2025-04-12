 #ifndef _Player_h
 #define _Player_h
 #include <iostream>
 #include "Machine.h"
 using namespace std;
 
 class Player{
     friend class Machine; //machines can access
    
     private:
     uint8_t possession;
     int16_t posX;
     int16_t posY;
     uint8_t last;
     uint8_t size;  //square so h == w

     public:
     Player();
     Player(uint8_t, uint8_t);
     bool moveUp(void); //movement methods
     bool moveDown(void);
     bool moveLeft(void);
     bool moveRight(void);   
     void resetCoordinates(void);
     int16_t getXPos(void); //gets for variables
     int16_t getYPos(void);
     uint8_t getLast(void);
     uint8_t getSize(void);
     uint8_t getPosession(void);

     uint8_t checkProximity(Machine); //checks the proximity to any machine
     uint8_t getMachineInput(Machine);
     
 };
 #endif
