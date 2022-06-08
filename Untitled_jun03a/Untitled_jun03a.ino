#include "arduino_secrets.h"
/*
  #include <ESP8266WiFi.h>
  #include <WiFiClient.h>
  #include <ESP8266WebServer.h>
*/

#include "thingProperties.h"
float humidityO = 0;
float pressureO = 0;
float temperatureO = 0;
float windSpeedO = 0;
int headingO = 0;
String hum, temp, pres, head, rot;
void setup() {
  // Initialize serial and wait for port to open:
  Serial.begin(9600);
  // This delay gives the chance to wait for a Serial Monitor without blocking if none is found
  delay(1500);

  // Defined in thingProperties.h
  initProperties();

  // Connect to Arduino IoT Cloud
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);

  /*
     The following function allows you to obtain more information
     related to the state of network and IoT Cloud connection and errors
     the higher number the more granular information you’ll get.
     The default is 0 (only errors).
     Maximum is 4
  */
  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();
}

void loop() {
  ArduinoCloud.update();
  String str = Serial.readStringUntil('$');
  rot = str.substring(0, str.indexOf('%'));
  hum = str.substring(str.indexOf('@') + 1, str.indexOf('#'));
  temp = str.substring(str.indexOf('%') + 1, str.indexOf('!'));
  pres = str.substring(str.indexOf('!') + 1, str.indexOf('@'));
  head = str.substring(str.indexOf('#') + 1);
  if (hum == 0 || pres == 0|| temp==NULL||rot==NULL|| str==NULL) {
    return;
  } else{
      humidity = hum.toFloat();
      pressure = pres.toFloat();
      temperature = temp.toFloat();
      windSpeed = rot.toFloat();
      heading = head.toInt();

     /* humidityO = humidity;
      temperatureO = temperature;
      windSpeedO = windSpeed;
      headingO = heading;
      pressureO=pressure;*/
  }
}
