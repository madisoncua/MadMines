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

     public:
     Player();
     Player(uint8_t, uint8_t);
     void moveUp(void);
     void moveDown(void);
     void moveLeft(void);
     void moveRight(void);    
 };
 #endif
