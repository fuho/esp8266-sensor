#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <DNSServer.h>
#include <WiFiManager.h>
#include <PubSubClient.h>

#include <Htu21d.h>

//const char* mqtt_server = "iot.eclipse.org";
const char* mqtt_server = "dotko.co";

//Initialization
// PubSub
WiFiClient espClient;
PubSubClient pubSubClient(espClient);
long lastMsg = 0;
long lastReconnectAttempt = 0;
char msg[50];
int value = 0;
// Sensor
Htu21d htu = Htu21d();

boolean reconnect() {
  Serial.print("*PubSub: Attempting MQTT connection...");
  // Attempt to connect
  if (pubSubClient.connect("ESP8266Client")) {
    Serial.println("connected");
    // Once connected, publish an announcement...
    pubSubClient.publish("outTopic", "hello world");
    // ... and resubscribe
    pubSubClient.subscribe("inTopic");
  } else {
    Serial.print("failed, rc=");
    Serial.println(pubSubClient.state());
  }
  return pubSubClient.connected();
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("#MQTT: Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

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
  // Init the PubSub Client
  pubSubClient.setServer(mqtt_server, 1883);
  pubSubClient.setCallback(callback);
  lastReconnectAttempt = 0;
  // Init the humidity sensor
  while (!htu.begin()) {
    Serial.println("Couldn't find sensor!");
    delay(100);
  }
}

void loop()
{
  ArduinoOTA.handle();
  if (!pubSubClient.connected()) {
      if (lastReconnectAttempt + 10000 < millis()) {
        lastReconnectAttempt = millis();
        // Attempt to reconnect
        if (reconnect()) {
          lastReconnectAttempt = 0;
        }
      }
  } else {
    pubSubClient.loop();
  }
  float temperature = htu.readTemperature();
  float humidity = htu.readHumidity();
  Serial.print("Temp: "); Serial.print(temperature);
  Serial.print("\tHum: "); Serial.println(humidity);
  long now = millis();
  if (now > lastMsg + 2000) {
    lastMsg = now;
    ++value;
    snprintf (
      msg, 75, "{\"temperature\":\"%ld\",\"humidity\":\"%ld\"}",
      (int) (temperature * 100),
      (int) (humidity * 100)
    );
    Serial.print("Publish message: ");Serial.println(msg);
    pubSubClient.publish("outTopic", msg);
  }
  delay(350);
}
