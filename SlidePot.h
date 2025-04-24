// SlidePot.h
// Runs on MSPM0
// Provide functions that initialize ADC1 channel 5, PB18 and use a slide pot to measure distance
// Created: 12/26/2024
// Student names: Evan Roberts and Madsion Cua
// Last modification date: 04/02/2025

#ifndef SLIDEPOT_H
#define SLIDEPOT_H
#include <stdint.h>

class SlidePot{ 
  private:
  uint32_t data;     // raw ADC value
  int32_t flag;      // 0 if data is not valid, 1 if valid
  uint16_t distanceX;
  uint16_t distanceY; // distance in 0.001cm

public:
  SlidePot(void);
  void Init(void);
  uint32_t In(void);                // return last ADC sample value (0 to 4095)
  void Save(uint32_t n);            // save ADC, set semaphore
  void Sync(void);                  // wait for semaphore
  uint16_t DistanceX(void);          // return last distance value (0 to 2000), 0.001cm
  uint16_t DistanceY(void);          // return last distance value (0 to 2000), 0.001cm
};
#endif
