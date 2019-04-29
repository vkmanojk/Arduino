/////////////////////////////////
// Generated with a lot of love//
// with TUNIOT FOR ESP32     //
// Website: Easycoding.tn      //
/////////////////////////////////
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include <WiFi.h>

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883
#define AIO_USERNAME  "wox_inc"
#define AIO_KEY  "98ac987ce2ed4a2680b29bff675b0933"
WiFiClient client;

Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

Adafruit_MQTT_Subscribe manoj_udhay = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/manoj_udhay");
boolean MQTT_connect();

boolean MQTT_connect() {  int8_t ret; if (mqtt.connected()) {    return true; }  uint8_t retries = 3;  while ((ret = mqtt.connect()) != 0) { mqtt.disconnect(); delay(2000);  retries--;if (retries == 0) { return false; }} return true;}

void setup()
{
Serial.begin(9600);


  mqtt.subscribe(&manoj_udhay);
  WiFi.disconnect();
  delay(3000);
  Serial.println("START");
  WiFi.begin("MANOJKUMAR","1a2b3c4d");
  while ((!(WiFi.status() == WL_CONNECTED))){
    delay(300);
    Serial.print("..");

  }
  Serial.println("Connected");
  Serial.println("Your IP is");
  Serial.println((WiFi.localIP()));

}


void loop()
{

    if (MQTT_connect()) {
      Adafruit_MQTT_Subscribe *subscription_name;
      while ((subscription_name = mqtt.readSubscription(5000))) {
        if (subscription_name == &manoj_udhay) {
          Serial.println(((char *)manoj_udhay.lastread));
          Serial.println("manoj_udhay");

        }

      }

    }

}