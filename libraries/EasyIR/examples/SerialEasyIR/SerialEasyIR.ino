/* Serial Easy IR
 * An example for using the EasyIR library. It attempts to read
 * from the IR sensor and write the value to the Serial port.
 *
 * created November 2016
 * by Simon Pauw
 */

#include <EasyIR.h>

void setup() {
    // Initialize IR sensor at pin 2
    EasyIR::init(2);

    // Start Serial at speed 9600
    Serial.begin(9600);
}

void loop() {
    // Read from IR, returns 0 if nothing to be read
    long result = EasyIR::read();

    // If there was data to be read, write the result to serial.
    if(result)
    {
        Serial.println(result);
    }

    // Wait 100 msec
    delay(100);
}