/* FILE:    HCPCF8574_Blink_Example.cpp
   DATE:    24/05/17
   VERSION: 0.1
   AUTHOR:  Andrew Davies
   
24/05/17 version 0.1: Original version

This example sketch uses the HCPCF8574 library to toggle one of the PCF8574's digital pins
emulating the standard Arduino 'blink' sketch. This sketch has been written specifically for
the Hobby Components PCF8574 I2C to 8-bit digital port expander (HCMODU0120). To use the 
sketch change the I2C_ADD to match the address of your device (default is 0x38 for HCMODU0120)
and connect and LED (via a current limiting resistor) to the PCF8574's digital pin 0 (marked
D0 on HCMODU0120). You can connect your Arduino to the module as follows:

UNO/NANO........HCMODU0120
VCC.............5V
GND.............GND
A4..............SDA
A5..............SCL

More information about the library can be found in the software section of our support 
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


#include "HCPCF8574.h"    //Include the HCPCF8574 library

#define I2C_ADD 0x38      //I2C address of the PCF8574

HCPCF8574 Port(I2C_ADD);  //Create an instance of the library


void setup() 
{
  Port.init();            //Initiliase the PCF8574 (all pins are set to high)

  Port.pinMode(0, OUTPUT); //Set digital pin 0 to an ouput
}


void loop() 
{
  Port.pinWrite(0, HIGH); //Set digital pin 0 high
  delay(1000);            //Wait 1 second
  Port.pinWrite(0, LOW);  //Set digital pin 0 low
  delay(1000);            //Wait another second
}
