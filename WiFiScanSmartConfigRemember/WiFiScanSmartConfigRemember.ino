/*
 *  This sketch demonstrates how to scan WiFi networks.
 *  The API is almost the same as with the WiFi Shield library,
 *  the most obvious difference being the different file you need to include:
 */
#include "WiFi.h"
#include "SPIFFS.h"
#include "Streaming.h"

#define FORMAT_SPIFFS_IF_FAILED true

void listDir(fs::FS &fs, const char *dirname) {
    Serial << "Listing directory: " << dirname << "\r\n";

    File root = fs.open(dirname);
    if(!root){
        Serial << "- failed to open directory" << endl;
        return;
    }
    if(!root.isDirectory()){
        Serial << " - not a directory" << endl;
        return;
    }

    File file = root.openNextFile();
    while (file) {
        Serial << "  FILE: " << file.name() << "\t" << file.readString() << endl;
        file = root.openNextFile();
    }
}

void writePass(fs::FS &fs, const char *Name, const char *Value) {
  File  f = fs.open(Name, FILE_WRITE);
  if (f) { 
    f.print(Value);
    f.close();
  }
}

void setup() {
    Serial.begin(115200);

    // Set WiFi to station mode and disconnect from an AP if it was previously connected
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);

    if(!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED)){
        Serial.println("SPIFFS Mount Failed");
        return;
    }

    listDir(SPIFFS, "/");
    //writePass(SPIFFS, "/Henknet4",         "maakhiermaareenssoepvan" );
    writePass(SPIFFS, "/Punkienet-4 Guests", "Kathmandu#42Juphal21" );
    
    Serial.println("Setup done");
}

void loop() {
    Serial.println("scan start");

    // WiFi.scanNetworks will return the number of networks found
    int n = WiFi.scanNetworks();
    Serial.println("scan done");
    if (n == 0) {
        Serial.println("no networks found");
    } else {
        Serial.print(n);
        Serial.println(" networks found");
        for (int i = 0; i < n; ++i) {
            // Print SSID and RSSI for each network found
            Serial.print(i + 1);
            Serial.print(": ");
            Serial.print(WiFi.SSID(i));
            Serial.print(" (");
            Serial.print(WiFi.RSSI(i));
            Serial.print(")");
            Serial.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN)?" ":"*");
            delay(10);
        }
    }
    Serial.println("");

    // Wait a bit before scanning again
    delay(5000);
}
