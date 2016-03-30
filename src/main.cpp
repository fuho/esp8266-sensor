#include "Arduino.h"
//ESP8266 Core WiFi Library (you most likely already have this in your sketch)
#include <ESP8266WiFi.h>
//Local DNS Server used for redirecting all requests to the configuration portal
#include <DNSServer.h>
//Local WebServer used to serve the configuration portal
#include <ESP8266WebServer.h>
//https://github.com/tzapu/WiFiManager WiFi Configuration Magic
#include <WiFiManager.h>

void setup()
{
  WiFiManager wifiManager;
  wifiManager.autoConnect("ESP8266 Sensor");
  // initialize LED digital pin as an output.
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{
  // turn the LED on (HIGH is the voltage level)
  digitalWrite(LED_BUILTIN, HIGH);
  // wait for a second
  delay(100);
  // turn the LED off by making the voltage LOW
  digitalWrite(LED_BUILTIN, LOW);
   // wait for a second
  delay(100);
}
