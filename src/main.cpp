#include <Arduino.h>
#include <WiFi.h>
#include <SPIFFS.h>
#include <FS.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>


const char *ssid = "ESP32AP";
const char *password = "11111111";

const int ledPin = 26;
String ledState;


AsyncWebServer server(80);
String processor(const String& var){
  if(var == "STATE"){
    if(digitalRead(ledPin)){
      ledState = "active";

    }
    else{
      ledState = "";

    }
    // Serial.print(ledState);

    return ledState;
  }
  return String();
}

void diod_blink(int count){

    int i = 0;

    if(1){
      digitalWrite(ledPin, LOW);
    }else{
      while(i < count){
        digitalWrite(ledPin, HIGH);
        delay(20);
        digitalWrite(ledPin, LOW);
        delay(20);
        i++;
      }
    }


}


void setup(){
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);

  if(!SPIFFS.begin()){
     Serial.println("An Error has occurred while mounting SPIFFS");
     return;
  }

  WiFi.softAP(ssid, password);

  Serial.println();
  Serial.print("IP address: ");
  Serial.println(WiFi.softAPIP());

  server.on("/normalize.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/css/normalize.css", "text/css");
  });
  server.on("/main.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/css/main.css", "text/css");
  });
  server.on("/index.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/js/index.js", "text/js");
  });
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", "text/html", false);
  });
  server.on("/config.html", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/config.html", "text/html", false  );

  });
  server.on("/on", HTTP_GET, [](AsyncWebServerRequest *request){
    digitalWrite(ledPin, HIGH);
    request->send(SPIFFS, "/index.html", String(),false, processor);
  });
  server.on("/off", HTTP_GET, [](AsyncWebServerRequest *request){
    digitalWrite(ledPin, LOW);
    request->send(SPIFFS, "/index.html",  String(),false, processor);
  });
  server.on("/blink", HTTP_GET, [](AsyncWebServerRequest *request){
    diod_blink(400);
    request->send(SPIFFS, "/index.html", String(),false);
  });
  server.on("/blinkoff", HTTP_GET, [](AsyncWebServerRequest *request){
    diod_blink(800);
    request->send(SPIFFS, "/index.html", String(),false);
  });




  server.begin();
}

void loop(){

}


