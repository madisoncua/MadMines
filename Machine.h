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

    public:
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
