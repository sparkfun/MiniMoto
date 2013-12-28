#include <Wire.h>
#include "minimoto.h"

// The address of the part is set by a jumper on the board. 
//  See datasheet or schematic for details; default is 0xD0.
MiniMoto::MiniMoto(byte addr)
{
	_addr = addr >> 1;
	//The user passes in the I2C address of the DRV8830 but the Arduino Wire 
	//library requires the I2C address to be 7-bits (exclude the R/W bit).
  
	Wire.begin(); //Starts I2C communication at 100kHz
}

// Return the fault status of the DRV8830 chip. Also clears any existing faults.
byte MiniMoto::getFault()
{
	//Request the fault status
	Wire.beginTransmission(_addr);
	Wire.write(REG_FAULT); //Ask for the fault register
	Wire.endTransmission();

	Wire.requestFrom(_addr, 1); //Ask for one byte
	byte buffer = Wire.read(); //Get the one byte

	//Clear any faults
	Wire.beginTransmission(_addr);
	Wire.write(REG_FAULT); //Write to register 0x01
	Wire.write(1<<CLEAR); //Clear the fault status
	Wire.endTransmission();

	return buffer;
}

// Send the drive command over I2C to the DRV8830 chip. Bits 7:2 are the speed
//  setting; range is 0-63. Bits 1:0 are the mode setting:
//  - 00 = HI-Z
//  - 01 = Reverse
//  - 10 = Forward
//  - 11 = H-H (brake)
void MiniMoto::drive(int speed)
{
	// Before we do anything, we'll want to
	//  clear the fault status. To do that
	//  write 0x80 to register 0x01 on the
	//  DRV8830.
	Wire.beginTransmission(_addr); //Talk to the driver specified by the user
	Wire.write(REG_FAULT); //Write to register 0x01
	Wire.write(1<<CLEAR); //Clear the fault status
	Wire.endTransmission();

	//Clean up the incoming speed value
	int regValue = (byte)abs(speed);      // Find the abs value of the input

	if (regValue > 63) regValue = 63; // Cap the value at 63.

	regValue <<= 2;           // Left shift to make room for bits 1:0

	if (speed < 0) regValue |= 0x01;  // Set bits 1:0 based on sign of input.
	else           regValue |= 0x02;

	Wire.beginTransmission(_addr); //Talk to the driver specified by the user
	Wire.write(REG_CONTROL); //Write to register 0x00
	Wire.write(regValue); //Write the speed value
	Wire.endTransmission();
}

// Coast to a stop by hi-z'ing the drivers.
void MiniMoto::stop()
{
	Wire.beginTransmission(_addr); //Talk to the driver specified by the user
	Wire.send(REG_CONTROL); //Write to register 0x00
	Wire.send(0x00); //Stop the motors. See above for bit 1:0 explanation.
	Wire.endTransmission();
}

// Stop the motor by providing a heavy load on it.
void MiniMoto::brake()
{
	Wire.beginTransmission(_addr); //Talk to the driver specified by the user
	Wire.write(REG_CONTROL); //Write to register 0x00
	Wire.write(0x03); //Brake the motors. See above for bit 1:0 explanation.
	Wire.endTransmission();
}
