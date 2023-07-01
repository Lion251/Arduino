/* EasyIR.cpp - an Arduino library for infrared communication intended for educational purposes,
 * especially adapted by Simon Pauw for the Simple Walker II project (simplewalker.com).
 * This library is built on IRRemote v2.2.0 which is released under GNU LESSER GENERAL PUBLIC LICENSE as is this library.
 */
 
#include "EasyIR.h"
#include "EasyIRremote.h"
#include <Arduino.h>

IRrecv *My_Receiver; //(IR_PIN);
decode_results results;

void EasyIR::init(int pin)
{
    My_Receiver = new IRrecv(pin);
    My_Receiver->enableIRIn(); // Start the receiver
}

long EasyIR::read()
{
    long result = 0;
    if (My_Receiver->decode(&results)) {
        //Restart the receiver so it can be capturing another code
        //while we are working on decoding this one.
        My_Receiver->resume();
        result = results.value;
    }
    return result;
}