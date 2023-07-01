/* FILE:    HCPCF8574.h
   DATE:    27/11/17
   VERSION: 0.2
   AUTHOR:  Andrew Davies
   
24/05/17 version 0.1: Original version
27/11/17 version 0.2: Correct mask error in pinMode function

Arduino library for the PCF8574 I2C to 8-bit digital port expander. In particular this  library has been 
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

#ifndef HCPCF8574_h
#define HCPCF8574_h

#include <Wire.h>
#include "Arduino.h"



class HCPCF8574
{
  public:
	HCPCF8574(byte I2C_Add);
	void init(void);
	void pinMode(byte Pin, boolean Mode);
	void portWrite(byte Data);
	void pinWrite(byte Pin, boolean State);
	byte portRead(void);
	boolean pinRead(byte Pin);
	

  private:
	byte _Dir = 0xFF;
	byte _I2C_Add;
}; 


#endif