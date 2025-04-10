#include <iostream>

class machine{
    //global variables needed:
    friend class Lab9HMain
    private:
    uint8_t input;
    uint8_t state;


    public:
    updateSmelter(uint8_t); //update Smelter edit
    updateRefiner(uint8_t);
    updateRock(uint8_t);
    updateAnvil(uint8_t);
    updateTurnInArea(uint8_t);
}
