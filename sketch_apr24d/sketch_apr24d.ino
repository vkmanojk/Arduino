#include <WiFi.h>
#include <HTTPClient.h>

WiFiServer server(80); 

 const char* ssid = "thingQbator";
const char* password =  "Qbator369";
const char* web = WiFi.localIP();
 
void setup() {
 Serial.begin(115200);
 
    // Let's connect to wifi network 
    Serial.print("Connecting to ");
    Serial.print(ssid);
    WiFi.begin(ssid, password);       //Connecting to wifi network

    while (WiFi.status() != WL_CONNECTED) { //Waiting for the responce of wifi network
        delay(500);
    Serial.print("Connecting ");
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("Connection Successful");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());         //Getting the IP address at which our webserver will be created
    Serial.println("Put the above IP address into a browser search bar");
    server.begin();  
    
 
}
 
void loop() {
 
 if(WiFi.status()== WL_CONNECTED){   //Check WiFi connection status
 
   HTTPClient http;   
 
   http.begin(web);  //Specify destination for HTTP request
   http.addHeader("Content-Type", "text/html");             //Specify content-type header
 
   int httpResponseCode = http.POST("POSTING from ESP32");   //Send the actual POST request
 
   if(httpResponseCode>0){
 
    String response = http.getString();                       //Get the response to the request
 
    Serial.println(httpResponseCode);   //Print return code
    Serial.println(response);           //Print request answer
 
   }else{
 
    Serial.print("Error on sending POST: ");
    Serial.println(httpResponseCode);
 
   }
 
   http.end();  //Free resources
 
 }else{
 
    Serial.println("Error in WiFi connection");   
 
 }
 
  delay(10000);  //Send a request every 10 seconds
 
}
