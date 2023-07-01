/* EasyIR.cpp - an Arduino library for infrared communication intended for educational purposes,
 * especially adapted by Simon Pauw for the Simple Walker II project (simplewalker.com).
 * This library is built on IRRemote V2.2.0 which is released under GNU LESSER GENERAL PUBLIC LICENSE as is this library.
 */

#ifndef EasyIR_h
#define EasyIR_h
#include <Arduino.h>

class EasyIR {
public:
     static void init(int pin);
     static long read();
};


#endif EasyIR_h
