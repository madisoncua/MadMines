 #ifndef _Machine_h
 #define _Machine_h
 #include <cstdint>
#include <iostream>
 using namespace std;

typedef struct itemHeld{
  const uint16_t* image;
  uint8_t w;
  uint8_t h;
}itemHeld;

class Machine{
    //global variables needed:
    friend class Player;
    private:
    uint8_t state;
    uint8_t holdItem;
    uint8_t workTimer;
    uint8_t sprite; //keeps track of what sprite the machine is displaying
    uint8_t top_L_x;
    uint8_t top_L_y;
    uint8_t bot_R_x;
    uint8_t bot_R_y;

    uint8_t proXL; //left proximity
    uint8_t proXR; //right proximity
    uint8_t proYT; //top proximity
    uint8_t proYB; //bottom proximity 

    uint8_t progX;
    uint8_t progY;

    public:
    //(top_left_x, top_left_y, bot_right_x, bot_right_y, progX, progY)
    Machine(uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t); 
    //(top_left_x, top_left_y, bot_right_x, bot_right_y, progX, progY, proXL, proXR, proYT, proYB)
    Machine(uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t); 
    //(top_left_x, top_left_y, bot_right_x, bot_right_y, proXL, proXR, proYT, proYB, state)
    Machine(uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t); 
    int8_t updateSmelter(uint8_t); //update Smelter edit
    int8_t updateRefiner(uint8_t);
    int8_t updateRock(uint8_t);
    int8_t updateAnvil(uint8_t);
    int8_t updateTurnInArea(uint8_t);
    int8_t updateCart(uint8_t);
    int8_t updateCounters(uint8_t, Machine*);

    void printSmelter();
    uint8_t cartSendError(uint8_t, uint8_t);
    void printRock();
    void printRefiner();
    void printAnvil();
    void printCart();
    void printToDo();
    void printTurnInArea();
    void printCounters(Machine*);
    void setSprite(uint8_t);

    uint8_t computeRecipe(int8_t*, int8_t);
    void updateAnvilMenu(int8_t*, int8_t);
};

#endif
