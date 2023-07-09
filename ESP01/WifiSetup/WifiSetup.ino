/*
    WifiSettings basic example

    Source and further documentation available at
    https://github.com/Juerd/ESP-WiFiSettings

    Note: this example is written for ESP32.
    For ESP8266, use LittleFS.begin() instead of SPIFFS.begin(true).
*/

#include <ArduinoJson.h>
#include <ArduinoMqttClient.h>
#include <ESP8266WiFi.h>
#include <LittleFS.h>
#include <Streaming.h>
#include <WiFiSettings.h>

#define LED_ON  LOW
#define LED_OFF HIGH
#define LED_PIN LED_BUILTIN
#define PWM_PIN LED_BUILTIN

WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);

StaticJsonDocument<256> mqttFilter;


struct MqttReader {
  int read();                                     // Reads one byte, or returns -1
  size_t readBytes(char* buffer, size_t length);  // Reads several bytes, returns the number of bytes read.
} mqttReader;


int MqttReader::read(void) {
  if (!mqttClient.available()) return -1;
  else return (unsigned char)mqttClient.read();
}

size_t MqttReader::readBytes(char *buffer, size_t length) {
  int N=0;

  while (length-- && mqttClient.available()) {
    *buffer++ = (char)mqttClient.read();
    N++;
  }
  return N;
}

void mqttHandler(int MessageLength) {
  StaticJsonDocument<256> Message;

  deserializeJson(Message, mqttReader, DeserializationOption::Filter(mqttFilter));

  if (Message["name"] == WiFiSettings.hostname) { // is the message for us?
    analogWrite(PWM_PIN, 1023 - ((int)Message["nvalue"] ? (int)Message["svalue1"] : 0)*10);
  }
//  Serial.print((const char *)Message["name"]);
//  serializeJson(Message, Serial); Serial.print("\n");
}

void PortalWaitLoop(void) {
  if (mqttClient.connected()) {
    mqttClient.poll();
  } else {
    Serial << F("Connecting to MQTT-server ") << WiFiSettings.mqtt_server << F(" port ") << WiFiSettings.mqtt_port << endl;
    if (mqttClient.connect(WiFiSettings.mqtt_server.c_str(), WiFiSettings.mqtt_port)) {
      mqttClient.subscribe("devices/"+WiFiSettings.hostname);
      mqttClient.onMessage(mqttHandler);
      Serial << F("Connected! Waiting for commands") << endl;
    } else {
      Serial<<F("Bummer! Didn't work!\n");
      digitalWrite(LED_PIN, LED_ON);
      delay(200);
      digitalWrite(LED_PIN, LED_OFF);
      delay(400);
    }
  }
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LED_OFF);

  // These are the only three items we're interested in
  mqttFilter["name"]    = true; // Name of the device
  mqttFilter["nvalue"]  = true; // 0 or 1 for on or off
  mqttFilter["svalue1"] = true; // 0-100 dimmer value
  
  Serial.begin(115200);
  LittleFS.begin();  // Will format on the first run after failing to mount

  // Use stored credentials to connect to your WiFi access point.
  // If no credentials are stored or if the access point is out of reach,
  // an access point will be started with a captive portal to configure WiFi.
  WiFiSettings.connect();

  Serial << F("My ip: ") << WiFi.localIP().toString() << endl;

  WiFiSettings.onPortalWaitLoop = PortalWaitLoop;
  WiFiSettings.portal(false); // don't start access point, stay connected, but keep portal open for configuration changes.
}

void loop() {
  // We never come here. The mqttClient.poll() is handled from the waitloop of the portal;
//  mqttClient.poll();
  Serial << 'M';
  delay(100);
}
