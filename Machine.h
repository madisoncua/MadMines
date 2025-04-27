// Authors: Evan Roberts and Madison Cua
// Last Modified: 4/24/2025
#ifndef _Machine_h
#define _Machine_h
#include <cstdint>
//#include <iostream>
//using namespace std;

typedef struct itemHeld{
  const uint16_t* image;
  uint8_t w;
  uint8_t h;
}itemHeld;

//abstract class for machines to inherit for polymorphism
class Machine{
  //global variables needed:
  friend class Player;
  protected:
  uint8_t state;
  uint8_t holdItem;
  uint8_t workTimer;
  uint8_t wasWorking;
  
  uint8_t sprite; //keeps track of what sprite the machine is displaying
  uint8_t top_L_x;
  uint8_t top_L_y;
  uint8_t bot_R_x;
  uint8_t bot_R_y;

  uint8_t proXL; //left proximity
  uint8_t proXR; //right proximity
  uint8_t proYT; //top proximity
  uint8_t proYB; //bottom proximity 

  uint8_t progX;  //progress bar location
  uint8_t progY;

  public:
  //(top_left_x, top_left_y, bot_right_x, bot_right_y, progX, progY)
  Machine(uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t); 
  //(top_left_x, top_left_y, bot_right_x, bot_right_y, progX, progY, proXL, proXR, proYT, proYB)
  Machine(uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t); 
  //(top_left_x, top_left_y, bot_right_x, bot_right_y, proXL, proXR, proYT, proYB, state)
  Machine(uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t); 
  
  virtual void update(uint8_t);
  
  virtual void print(void);
  uint8_t getSprite();
  void setSprite(uint8_t);
};

class Refiner : public Machine{
  private:
    int8_t refinerFSM(uint8_t);
  public:
    void update(uint8_t) override;
    void print() override;
    //progress bar constructor
    Refiner(uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t);
};

class Smelter : public Machine{
  private:
    int8_t smelterFSM(uint8_t);
  public:
    void update(uint8_t) override;
    void print() override;
    //progress bar constructor
    Smelter(uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t);
};

class Anvil : public Machine{
  private:
    int8_t anvilFSM(uint8_t);
    void updateAnvilMenu(int8_t*, int8_t);
    uint8_t computeRecipe(int8_t*, int8_t);
  public:
    void update(uint8_t) override;
    void print() override;
    //proximity constructor
    Anvil(uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t); 
};

class Rock : public Machine{
  private:
    int8_t rockFSM(uint8_t);
  public:
    void update(uint8_t) override;
    void print() override;
    //proximity constructor (also sets rock type to metal)
    Rock(uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t);
    //progress bar constructor (also sets the rock type to gems)
    Rock(uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t);
};

class Cart : public Machine{
  private:
    int8_t cartFSM(uint8_t);
    uint8_t cartSendError(uint8_t, uint8_t);
  public:
    void update(uint8_t) override;
    void print() override;
    //progress bar constructor 
    Cart(uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t);
    //state constructor
    Cart(uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t);
};

class TurnInArea : public Machine{
  private:
    int8_t turnInFSM(uint8_t);
  public:
    void update(uint8_t) override;
    void print() override;
    //progress bar constructor
    TurnInArea(uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t);
};

class Counter : public Machine{
  private:
    int8_t counterFSM(uint8_t);
  public:
    void update(uint8_t) override;
    void print() override;
    //state constructor
    Counter(uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t);
};

#endif
