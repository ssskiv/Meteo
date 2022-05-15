#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

#ifndef APSSID
#define APSSID "MeteoConnect"
#define APPSK  "okokokok"
#endif

const char *ssid = APSSID;
const char *password = APPSK;



String hum, temp, pres, head,rot;

ESP8266WebServer server(80);




// http://192.168.4.1

void handleRoot() {
  String str = Serial.readStringUntil('$');
  rot=str.substring(0,str.indexOf('%'));
  hum = str.substring(str.indexOf('@') + 1, str.indexOf('#'));
  temp = str.substring(str.indexOf('%')+1, str.indexOf('!'));
  pres = str.substring(str.indexOf('!') + 1, str.indexOf('@'));
  head = str.substring(str.indexOf('#') + 1);
  String HTML = "<!DOCTYPE html> <html>\n";
  HTML += "<p>Влажность: " + hum + " %</p>\n";
  HTML += "<p>Температура: " + temp + " °C</p>\n";
  HTML += "<p>Атмосферное давление: " + pres + " hPa</p>\n";
  HTML += "<p>Направление ветра: " + head + " °</p>\n";
  HTML += "<p>Количество оборотов в секунду: " + rot + "</p>\n";
  server.send(200, "text/html", HTML);
}

void setup() {
  delay(1000);

  Serial.begin(115200);
  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();
  server.on("/", handleRoot);
  server.begin();
}

void loop() {
  server.handleClient();
}
