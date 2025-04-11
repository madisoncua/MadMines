 #ifndef _Player_h
 #define _Player_h
 #include <iostream>
 using namespace std;
 class Player{
     friend class machine; //machines can access
    
     private:
     int possession;
     int16_t posX;
     int16_t posY;
     uint8_t last;
     uint8_t size;  //square so h == w

     public:
     Player();
     Player(uint8_t, uint8_t);
     bool moveUp(void);
     bool moveDown(void);
     bool moveLeft(void);
     bool moveRight(void);   
     void resetCoordinates(void);
     int16_t getXPos(void);
     int16_t getYPos(void);
     uint8_t getLast(void);
 };
 #endif
