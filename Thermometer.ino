#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include "DHTesp.h"

#ifndef STASSID
#define STASSID "NetComm 4649"
#define STAPSK  "XXXXXXXXX"
#endif

const char* ssid = STASSID;
const char* password = STAPSK;

ESP8266WebServer server(80);

DHTesp dht;

void handleRoot() {
  String html = "<html><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"><link rel='stylesheet' type='text/css' href='https://hardcore-roentgen-978b07.netlify.com/thermometer.css'></head><body><div id='wrapper'><div id='termometer'><div id='temperature' style='height:0' data-value='1&deg;C'></div><div id='graduations'></div></div><div id='playground' style='display: none'> <div id='range'> <input id='minTemp' type='text' value='-20'> <input type='range' min='-20' max='50' value='42'> <input id='maxTemp' type='text' value='50'> </div><p id='unit'>Celcius C&deg;</p></div><div id='humidity'>Humidity:&nbsp;<span id='humidityValue'>99</span>%</div></div><script src='https://ajax.googleapis.com/ajax/libs/jquery/3.4.1/jquery.min.js'></script><script type='text/javascript' src='https://hardcore-roentgen-978b07.netlify.com/thermometer.js' type='text/javascript'></script></body></html>" ;
  server.send(200, "text/html", html);
}

void handleTemperature(){
  float humidity = dht.getHumidity();
  float temperature = dht.getTemperature();
  String json = "{\"temp\": ";
  json += temperature;
  json +=", \"humidity\":";
  json += humidity;
  json += "}";
  server.send(200, "application/json", json);
 }

void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void setup(void) {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  //if (MDNS.begin("esp8266")) {
  // Serial.println("MDNS responder started");
  // }

  server.on("/", handleRoot);
  server.on("/temperature", handleTemperature);

  server.onNotFound(handleNotFound);

  server.begin();
  dht.setup(16, DHTesp::DHT11); // Connect DHT sensor to GPIO 16
}

void loop(void) {
  server.handleClient();
  MDNS.update();
}
