 #ifndef _Machine_h
 #define _Machine_h
 #include <iostream>
 using namespace std;

class Machine{
    //global variables needed:
    friend class Player;
    private:
    uint8_t input;
    uint8_t state;
    uint8_t top_L_x;
    uint8_t top_L_y;
    uint8_t bot_R_x;
    uint8_t bot_R_y;

    public:
    Machine(uint8_t, uint8_t, uint8_t, uint8_t);
    void updateSmelter(uint8_t); //update Smelter edit
    void updateRefiner(uint8_t);
    void updateRock(uint8_t);
    void updateAnvil(uint8_t);
    void updateTurnInArea(uint8_t);
    void printSmelter(void);
    void printRock(void);
    void printRefiner(void);
    void printAnvil(void);
};

#endif
