 #ifndef _Machine_h
 #define _Machine_h
 #include <cstdint>
#include <iostream>
 using namespace std;

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

    public:
    Machine(uint8_t, uint8_t, uint8_t, uint8_t); //(top_left_x, top_left_y, bot_right_x, bot_right_y)
    int8_t updateSmelter(uint8_t); //update Smelter edit
    int8_t updateRefiner(uint8_t);
    int8_t updateRock(uint8_t);
    int8_t updateAnvil(uint8_t);
    int8_t updateTurnInArea(uint8_t);
    void printSmelter(uint8_t);
    void printRock(uint8_t);
    void printRefiner(uint8_t);
    void printAnvil(uint8_t);
    uint8_t computeRecipe(int8_t*, int8_t);
    void updateAnvilMenu(int8_t*, int8_t);
};

#endif
