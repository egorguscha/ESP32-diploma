#include <Arduino.h>
#include <WiFi.h>
#include <SPIFFS.h>
#include <FS.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <HardwareSerial.h>

//16 17

const int ledPin = 15;
long interval = 1000;
int allowBlink = 0;
int ledState = LOW;

unsigned long previousMillis = 0;
unsigned long currentMillis = 0;

const char *ssid = "ESP32AP";
const char *password = "11111111";

AsyncWebServer server(80);
AsyncWebSocket ws("/");

AsyncWebSocketClient * globalClient = NULL;

void onWsEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len){



  if(type == WS_EVT_CONNECT){
    Serial.println("Websocket client connection received");
    globalClient = client;
  } else if(type == WS_EVT_DISCONNECT){
    Serial.println("Websocket client connection finished");
    globalClient = NULL;
  } else if(type == WS_EVT_DATA) {
    String msg = "";
    for(int i=0; i < len; i++) {
      msg += (char) data[i];
    }

    Serial.println("message:");
    Serial.println(atoi(msg.c_str()));
    Serial.println(msg);

    if(msg == "on"){
      digitalWrite(ledPin, HIGH);
    }else if(msg == "off"){
      digitalWrite(ledPin, LOW);
      allowBlink = 0;
    }else if(atoi(msg.c_str())){
      interval = atoi(msg.c_str());

      Serial.println(interval);
    }else if(msg == "allowblink"){
      allowBlink = 1;
    }




    Serial.println();
  }
}


void setup(){
  Serial.begin(115200,SERIAL_8N1);

  pinMode(ledPin, OUTPUT);

  if(!SPIFFS.begin()){
     Serial.println("An Error has occurred while mounting SPIFFS");
     return;
  }

 WiFi.softAP(ssid, password);

  Serial.println();
  Serial.print("IP address: ");
  Serial.println(WiFi.softAPIP());



  ws.onEvent(onWsEvent);
  server.addHandler(&ws);



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

  server.begin();
}

void loop(){
    if(allowBlink){
   currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;


    if (ledState == LOW) {
      ledState = HIGH;
    } else {
      ledState = LOW;
    }
    digitalWrite(ledPin, ledState);
  }
    }

}






