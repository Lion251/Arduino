/* FILE:    HCPCF8574_Multiple_Device_Blink_Example.cpp
   DATE:    24/05/17
   VERSION: 0.1
   AUTHOR:  Andrew Davies
   
24/05/17 version 0.1: Original version

This example sketch demonstrates how to use multiple instances of the HCPCF8574 library to control
several PCF8574 devices with each device set to a different I2C address. 

The sketch uses the HCPCF8574 library to control two devices (set to 0x38 & 0x39) to toggle a 
digital pin on each PCF8574 ans so emulating the standard Arduino 'blink' sketch. 

The sketch has been written specifically for the Hobby Components PCF8574 I2C to 8-bit digital 
port expander (HCMODU0120). To use the sketch change the I2C_ADD1 and I2C_ADD2 to match the address
of your devices (default is 0x38 for HCMODU0120) and connect an LED (via a current limiting resistor) 
to the PCF8574's digital pin 0 (marked D0 on HCMODU0120) on the first device and pin digital 7 
(marked D7 on HCMODU0120). 

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


#include "HCPCF8574.h"      //Include the HCPCF8574 library

#define I2C_ADD1 0x38       //I2C address of the first PCF8574 device
#define I2C_ADD2 0x39       //I2C address of the second PCF8574 device

HCPCF8574 Port1(I2C_ADD1);  //Create an instance of the library to control the first device
HCPCF8574 Port2(I2C_ADD2);  //Create an instance of the library to control the second device


void setup() 
{
  Port1.init();             //Initiliase the first PCF8574 (all pins are set to high)
  Port2.init();             //Initiliase the second PCF8574 (all pins are set to high)

  Port1.pinMode(0, OUTPUT); //Set digital pin 0 on the first device to an output
  Port2.pinMode(7, OUTPUT); //Set digital pin 7 on the second device to an output
}


void loop() 
{
  Port1.pinWrite(0, HIGH);  //Set digital pin 0 on the first device high
  Port2.pinWrite(7, HIGH);  //Set digital pin 0 on the second device high
  delay(1000);              //Wait 1 second
  Port1.pinWrite(0, LOW);   //Set digital pin 0 on the first device low
  Port2.pinWrite(7, LOW);   //Set digital pin 0 on the second device low
  delay(1000);              //Wait another second
}