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
    uint8_t sprite; //keeps track of what sprite the machine is displaying
    uint8_t top_L_x;
    uint8_t top_L_y;
    uint8_t bot_R_x;
    uint8_t bot_R_y;

    public:
    Machine(uint8_t, uint8_t, uint8_t, uint8_t); //(top_left_x, top_left_y, top_right_x, top_right_y)
    void updateSmelter(uint8_t); //update Smelter edit
    void updateRefiner(uint8_t);
    void updateRock(uint8_t);
    void updateAnvil(uint8_t);
    void updateTurnInArea(uint8_t);
    void printSmelter(uint8_t);
    void printRock(uint8_t);
    void printRefiner(uint8_t);
    void printAnvil(uint8_t);
};

#endif
