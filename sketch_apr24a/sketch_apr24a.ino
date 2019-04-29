#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ArduinoHttpClient.h>

const char* ssid     = "your-ssid";
const char* password = "your-password";

// get this from the wia dashboard. it should start with `d_sk`
const char* device_secret_key = "your-device-secret-key";

char server[] = "api.wia.io";
char path[] = "/v1/events";
int port = 80;

WiFiClient client;
int status = WL_IDLE_STATUS;
HttpClient httpClient = HttpClient(client, server, port);

StaticJsonDocument<200> jsonBuffer;
JsonObject root = jsonBuffer.to<JsonObject>();

void setup() {
  // initialize serial communications and wait for port to open:
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  WiFi.begin(ssid, password);
  Serial.print("Attempting to connect to SSID: ");
  Serial.print(ssid);
  // attempt to connect to WiFi network:
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    // Connect to WPA/WPA2 network. Change this line if using open or WEP  network:
    // wait 5 seconds for connection:
    delay(5000);
  }
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("Connected");
}

void loop() {
  root["name"] = "hello-wia";
  postToWia(root);
  delay(10000);
}

// Adds the correct headers for HTTP and sends Data to Wia
void postToWia(JsonObject& data) {
  String dataStr = "";
  serializeJson(data, dataStr);
  httpClient.beginRequest();
  httpClient.post(path);
  httpClient.sendHeader("Content-Type", "application/json");
  httpClient.sendHeader("Content-Length", dataStr.length());
  httpClient.sendHeader("Authorization", "Bearer "  + String(device_secret_key));
  httpClient.beginBody();
  httpClient.print(dataStr);
  httpClient.endRequest();
