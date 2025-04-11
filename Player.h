 #ifndef _Player_h
 #define _Player_h
 #include <iostream>
 using namespace std;
 class Player{
     friend class machine; //machines can access
     friend class Lab9HMain;

     private:
     int possession;
     uint8_t xPos;
     uint8_t yPos;
     uint8_t last;

     public:
     Player();
     Player(uint8_t, uint8_t);
     bool moveUp(void);
     bool moveDown(void);
     bool moveLeft(void);
     bool moveRight(void);   
     void resetCoordinates(void);
 };
 #endif
