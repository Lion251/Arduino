/* FILE:    HCPCF8574.cpp
   DATE:    27/11/17
   VERSION: 0.2
   AUTHOR:  Andrew Davies
   
24/05/17 version 0.1: Original version
27/11/17 version 0.2: Correct mask error in pinMode function

Arduino library for the PCF8574 8-Bit I/O Expander. In particular this  library has been 
written for the following products:

Hobby Components PCF8574 I2C to 8-bit digital port expander (HCMODU0120). 

More information about this library can be found in the software section of our support 
forum here:

http://forum.hobbycomponents.com/software

You may copy, alter and reuse this code in any way you like, but please leave
reference to HobbyComponents.com in your comments if you redistribute this code.
This software may not be used directly for the purpose of selling products that
directly compete with Hobby Components Ltd's own range of products.
THIS SOFTWARE IS PROVIDED "AS IS". HOBBY COMPONENTS MAKES NO WARRANTIES, WHETHER
EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE, ACCURACY OR LACK OF NEGLIGENCE.
HOBBY COMPONENTS SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR ANY DAMAGES,
INCLUDING, BUT NOT LIMITED TO, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY
REASON WHATSOEVER.
*/


#include "HCPCF8574.h"


/* I2C Constructor to initialise the library where:
   
   I2C_Add is the I2C address of the PCF8574.
		Valid values for I2C_Add are 0x38 to 0x3F 
*/
HCPCF8574::HCPCF8574(byte I2C_Add)
{
	//Save the I2C address
	_I2C_Add = I2C_Add;
}



/* Initialises the HCPCF8574 and wire library. All pins are set high */
void HCPCF8574::init(void)
{
	//Initiliase the I2C interface
	Wire.begin(); 
	
	//Make sure all pins are set to default HIGH state
	portWrite(0xFF);
}



/* Configures a pin to be either and input or an output where:
   
   Pin is the pin number to configure. Valid values for Pin are 0 to 7
		
   Mode is the required pin direction. Valid values for Mode are 
		OUTPUT (pin is configured to be an output)
		INPUT (pin is configured to be an input)
		
   Note when configured to an input the pin will be driven high and 
   masked so that is cannot be written to by the pinWrite function.
*/
void HCPCF8574::pinMode(byte Pin, boolean Mode)
{
  _Dir = (_Dir & ~(1 << Pin)) | (!Mode << Pin);
  portWrite(_Dir);
}



/* Writes to all 8 pins in one operation where:
	
   Data is an 8 bit binary value representing the state of all 8 pins.
*/
void HCPCF8574::portWrite(byte Data)
{
  Wire.beginTransmission(_I2C_Add); 
  Wire.write(Data); 
  Wire.endTransmission(true); 
}



/* Sets the output state of one of the 8 pins where:

   Pin is the number of the pin to alter. Valid values for Pin are 0 to 7
   
   State is the state to set the pin to. Valid values for State are 
		HIGH (pin is pulled high)
		LOW (pin is pulled low)
		
   Note to read the state of a pin it must be configured as an output by pinMode();
*/
void HCPCF8574::pinWrite(byte Pin, boolean State)
{
  byte Data = portRead();

  Data = ((Data & ~(1 << Pin)) | (State << Pin)) | _Dir;

  portWrite(Data);
}



/* Reads the state of all 8 pins in one operation.

   Returns and 8 bit value representing the state of a 8 pins.
*/
byte HCPCF8574::portRead(void)
{
  byte Data;
  
  Wire.requestFrom(_I2C_Add, 1);
  Data = Wire.read(); 
  Wire.endTransmission(); 
  
  return Data; 
}



/* Reads the state on one of the 8 pins where:

   Pin is the number of the pin to read. Valid values for Pin are 0 to 7
   
   Returns a boolean value representing the state of the pin. 
   
   Note to read the state of a pin it must be configured as an input by pinMode();
*/
boolean HCPCF8574::pinRead(byte Pin)
{
  byte Data = portRead();
  
  return (Data >> Pin) & 1;
}