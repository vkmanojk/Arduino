#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

#define REED_SWITCH A0 //D1
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                   // use 8883 for SSL
#define AIO_USERNAME    "wox_inc"
#define AIO_KEY         "98ac987ce2ed4a2680b29bff675b0933"

WiFiClient client;

Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);
Adafruit_MQTT_Publish photocell = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/sensor");

int status = WL_IDLE_STATUS; //not required.

const char* ssid = "MANOJKUMAR";
const char* password = "1a2b3c4d";
int doorClosed = 1;
void MQTT_connect();


void setup() {
  pinMode(REED_SWITCH, INPUT_PULLUP);

  Serial.begin(115200);

  setupWifi();
   
   //get_http();

}

void setupWifi()
{
     // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
   WiFi.mode(WIFI_STA);
   status = WiFi.begin(ssid, password);    

   Serial.print("Attempting to connect to SSID: ");
   Serial.println(ssid);  

   // Wait for connection
   while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
   }
   Serial.println("Connected to wifi");
}

void loop() {
   if (WiFi.status() != WL_CONNECTED)
   {
     setupWifi();
   }
  // put your main code here, to run repeatedly:
    MQTT_connect();
    int force = analogRead(REED_SWITCH);
    if (! photocell.publish(force)) {
    Serial.println(F("Failed"));
  } else {
    Serial.println(F("OK!"));
  }
    if ((analogRead(REED_SWITCH) > 1000))
    {
      Serial.println("DOOR OPEN!!");
      while (get_http(String("DOOR_OPEN_")) != 0);      
      doorClosed = 0;
    } 
    else 
    {
      Serial.println("DOOR CLOSED!!");
      while (get_http(String("DOOR_CLOSED_")) != 0);      
      doorClosed = 1;
    }
    delay(10);


}

void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds
       retries--;
       if (retries == 0) {
         // basically die and wait for WDT to reset me
         while (1);
       }
  }
  Serial.println("MQTT Connected!");
}

int get_http(String state)
{
   HTTPClient http;
   int ret = 0;
   Serial.print("[HTTP] begin...\n");
   // configure ifttt server and url  should be HTTP only..not https!!!  (http://)   
    http.begin("http://maker.ifttt.com/trigger/Intruder_Alert/with/key/cKu5MuX-qQHFq8SjlUwhSj"); //HTTP
   //If you have enabled value1 from iftt settings then uncomment below line to send value and comment out above line  
   //http.begin("http://maker.ifttt.com/trigger/door/with/key/your_key_from_Iftt/?value1="+state); //HTTP

    Serial.print("[HTTP] GET...\n");
    // start connection and send HTTP header
    int httpCode = http.GET();
    // httpCode will be negative on error
    if(httpCode > 0) {
    // HTTP header has been send and Server response header has been handled
    Serial.printf("[HTTP] GET code: %d\n", httpCode);

      if(httpCode == HTTP_CODE_OK) {
        String payload = http.getString();
        Serial.println(payload);
      }
    } else {
        ret = -1;
        Serial.printf("[HTTP] GET failed, error: %s\n", http.errorToString(httpCode).c_str());
        delay(500); // wait for half sec before retry again
    }

    http.end();
    return ret;
}  
