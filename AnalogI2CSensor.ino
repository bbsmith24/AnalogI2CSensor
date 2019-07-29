//
//
//
#include <Wire.h>

#define ANALOGPIN A0

int brightness = 0;    // how bright the LED is
float sensorRange[2];
float sensorValue = 0;
float sensorPercent = 0;
String pctStr;
bool strReady = true;

void setup() 
{
  Wire.begin(8);                // join i2c bus with address #8
  Wire.onRequest(requestEvent); // register event
  Serial.begin(9600);  // start serial for output

   sensorRange[0] = 1024;
   sensorRange[1] = -1024;
}

void loop() 
{
  sensorValue = (float)(analogRead(ANALOGPIN));
  sensorRange[0] = sensorRange[0] < sensorValue ? sensorRange[0] : sensorValue;
  sensorRange[1] = sensorRange[1] > sensorValue ? sensorRange[1] : sensorValue;
  sensorPercent = ((sensorValue - sensorRange[0]) / (sensorRange[1]-sensorRange[0]));

  strReady = false;
  pctStr = String((100.0 * sensorPercent),2);
  while(pctStr.length() < 7)
  {
    pctStr = " " + pctStr;
  } 
  pctStr += " ";
  strReady = true;
  Serial.println(pctStr);         // print the string

}

// function that executes whenever data is requested by master
// this function is registered as an event, see setup()
void requestEvent() 
{
  Wire.write(pctStr.c_str());
}
