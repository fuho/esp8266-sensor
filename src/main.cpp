#include <Arduino.h>
//ESP8266 Core WiFi Library (you most likely already have this in your sketch)
#include <ESP8266WiFi.h>
//Local DNS Server used for redirecting all requests to the configuration portal
#include <DNSServer.h>
//Local WebServer used to serve the configuration portal
#include <ESP8266WebServer.h>
// Arduino over the air update
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
//https://github.com/tzapu/WiFiManager WiFi Configuration Magic
#include <WiFiManager.h>
//HTU21D Humidity Sensor
#include <Htu21d.h>


//Initialization
// Sensor
Htu21d htu = Htu21d();

void setup()
{
  // Init serial
  Serial.begin(230400);
  Serial.println("Booting ESP8266 Htu21d wireless sensor");
  // Init wifi manager
  WiFiManager wifiManager;
  wifiManager.autoConnect("ESP8266 Sensor");
  // Init ArduinoOta
  // Port defaults to 8266
  // ArduinoOTA.setPort(8266);
  // Hostname defaults to esp8266-[ChipID]
  ArduinoOTA.setHostname("ESP8266FU");
  // No authentication by default
  // ArduinoOTA.setPassword((const char *)"123");
  ArduinoOTA.onStart([]() {
    Serial.println("*OTA: Start");
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\n*OTA: End");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("*OTA: Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("*OTA: Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("*OTA: Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("*OTA: Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("*OTA: Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("*OTA: Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("*OTA: End Failed");
  });
  ArduinoOTA.begin();
  Serial.println("*OTA: Ready");
  Serial.print("*OTA: IP address: ");Serial.println(WiFi.localIP());
  // Init the humidity sensor
  ArduinoOTA.begin();
  while (!htu.begin()) {
    Serial.println("Couldn't find sensor!");
    delay(5);
  }
}

void loop()
{
  // turn the LED on (HIGH is the voltage level)
  digitalWrite(LED_BUILTIN, HIGH);
  // wait for a second
  delay(1000);
  // turn the LED off by making the voltage LOW
  digitalWrite(LED_BUILTIN, LOW);
   // wait for a second
  delay(1000);
  Serial.print("Temp: "); Serial.println(htu.readTemperature());
  //Serial.print("\t\tHum: "); Serial.println(htu.readHumidity());
}
