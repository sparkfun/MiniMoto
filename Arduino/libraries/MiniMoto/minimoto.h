#ifndef minimoto_h
#define minimoto_h

#include <Arduino.h>

#define I2C_READ    0x01 // I2C read bit set
// Some values we'll load into TWCR a lot
#define START_COND  0xA4 // (1<<TWINT) | (1<<TWSTA) | (1<<TWEN)
#define STOP_COND   0x94 // (1<<TWINT) | (1<<TWSTO) | (1<<TWEN)
#define CLEAR_TWINT 0x84 // (1<<TWINT) | (1<<TWEN)
#define NEXT_BYTE   0xC4 // (1<<TWINT) | (1<<TWEA) | (1<<TWEN)

class MiniMoto
{
  public:
    MiniMoto(byte addr);
    void drive(int speed);
    void stop();
    void brake();
  private:
    void xlWriteBytes(byte addr, byte *buffer, byte len);
    void xlReadBytes(byte addr, byte *buffer, byte len);
    byte _addr;
};

#endif