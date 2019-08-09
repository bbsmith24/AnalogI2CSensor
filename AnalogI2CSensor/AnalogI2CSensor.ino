//
//
//
#include <Wire.h>

#define ANALOGPIN A0
#define LEDPIN 5

int brightness = 0;    // how bright the LED is
float sensorRange[2];
float sensorValue = 0;
//float sensorPercent = 0;
String pctStr;
bool strReady = true;

union DataPacket
{
  float dataValue;
  char dataCharArray[4];
};

DataPacket sensorPercent;

void setup() 
{
  Wire.setClock(400000);
  // read jumpers for address?
  Wire.begin(8);                // join i2c bus with address #8
  Wire.onRequest(requestEvent); // register event
  Serial.begin(9600);  // start serial for output
  pinMode(LEDPIN, OUTPUT);
  sensorRange[0] = 1024;
  sensorRange[1] = -1024;
}

void loop() 
{
  sensorValue = (float)(analogRead(ANALOGPIN));
  sensorRange[0] = sensorRange[0] < sensorValue ? sensorRange[0] : sensorValue;
  sensorRange[1] = sensorRange[1] > sensorValue ? sensorRange[1] : sensorValue;
  sensorPercent.dataValue = ((sensorValue - sensorRange[0]) / (sensorRange[1]-sensorRange[0])) * 100.0;

  strReady = false;
  pctStr = String(sensorPercent.dataValue,2);

  strReady = true;
  Serial.println(pctStr);         // print the string
  brightness = (int)(2.55 * sensorPercent.dataValue);
  analogWrite(LEDPIN, brightness);
}

// function that executes whenever data is requested by master
// this function is registered as an event, see setup()
void requestEvent() 
{
  // one option
  //Wire.beginTransmission();
  //Wire.write(pctStr.c_str());  
  //Wire.endTransmission();
  // another option...
  //Wire.write(pctStr.c_str());
  Wire.write(sensorPercent.dataCharArray, 4);
}   
