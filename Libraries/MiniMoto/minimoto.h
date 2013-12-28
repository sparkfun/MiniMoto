#ifndef minimoto_h
#define minimoto_h

#if defined(ARDUINO) && ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#include <Wire.h>

//Define registers
#define REG_CONTROL 0x00
#define REG_FAULT 0x01

//Fault bit constants
#define CLEAR 7
#define ILIMIT 4
#define OTS 3
#define UVLO 2
#define OCP 1
#define FAULT 0

class MiniMoto
{
  public:
    MiniMoto(byte addr);
    void drive(int speed);
    void stop();
    void brake();
    byte getFault();
  private:
    int _addr;
};

#endif