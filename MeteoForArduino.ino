#include <Wire.h>
#include <SPI.h>
#include <Adafruit_BMP280.h>

#include <DFRobot_QMC5883.h>

#include "DHT.h"

#include <SoftwareSerial.h>

#define DHTPIN 4
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);
TwoWire wires =  TwoWire();
void Enc();
DFRobot_QMC5883 compass(&wires, 0x0D);

Adafruit_BMP280 bmp = Adafruit_BMP280(&Wire);

#define RXPIN 5
#define TXPIN 3
#define ENCPIN 2
long enc=0;
int oldenc=0;
SoftwareSerial MyESP(RXPIN, TXPIN);

void setup() {
delay(1000);
  Serial.begin(9600);
  MyESP.begin(9600);
  pinMode(ENCPIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(ENCPIN), Enc, CHANGE);

  while (!compass.begin())
  {
    Serial.println("Could not find a valid 5883 sensor, check wiring!");
    delay(500);
  }

  while ( !Serial )
    delay(100);
  /*while(!MyESP.available()){
    Serial.println("WHERE IS MyESP?");
    delay(200);
    }*/
  unsigned status;

  status = bmp.begin(0x76);

  if (!status) {
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring or "
                     "try a different address!"));
    Serial.print("SensorID was: 0x"); Serial.println(bmp.sensorID(), 16);
    Serial.print("        ID of 0xFF probably means a bad address, a BMP 180 or BMP 085\n");
    Serial.print("   ID of 0x56-0x58 represents a BMP 280,\n");
    Serial.print("        ID of 0x60 represents a BME 280.\n");
    Serial.print("        ID of 0x61 represents a BME 680.\n");
    while (1) delay(10);
  }
  dht.begin();

  compass.isQMC();
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,
                  Adafruit_BMP280::SAMPLING_X2,
                  Adafruit_BMP280::SAMPLING_X16,
                  Adafruit_BMP280::FILTER_X16,
                  Adafruit_BMP280::STANDBY_MS_500);
}
void Enc() {
  if (digitalRead(ENCPIN)) {
    enc++;
  }
}
void loop() {

  float declinationAngle = (4.0 + (26.0 / 60.0)) / (180 / PI);
  compass.setDeclinationAngle(declinationAngle);
  sVector_t mag = compass.readRaw();
  compass.getHeadingDegrees();

  Serial.print(F("Temperature = "));
  Serial.print((bmp.readTemperature() + dht.readTemperature()) / 2);
  Serial.println(" *C");

  Serial.print(F("Pressure = "));
  Serial.print(bmp.readPressure() / 100);
  Serial.println(" hPa");

  Serial.print(F("Humidity = "));
  Serial.print(dht.readHumidity());
  Serial.println(" %");

  Serial.print("Heading = ");
  Serial.println(mag.HeadingDegress);

  Serial.println();

  String forESP = "";
  forESP += String((enc-oldenc)/6) + '%';
  forESP += String((bmp.readTemperature() + dht.readTemperature()) / 2, 2) + '!';
  forESP += String(bmp.readPressure() / 100, 2) + '@';
  forESP += String(dht.readHumidity(), 2) + '#';
  forESP += String(mag.HeadingDegress, 0) + '$';
  Serial.println(forESP);
  MyESP.println(forESP);
  //oldenc=enc;
  delay(10000);
}
