//
// 2 channel A2D I2C sensor
//
#include <Wire.h>

#define STEERINGPIN A0
#define THROTTLEPIN A1
#define STEERING_LEDPIN 5
#define THROTTLE_LEDPIN 6

int steeringBrightness = 0;    // how bright the LED is
int throttleBrightness = 0;    // how bright the LED is
float steeringRange[2];
float throttleRange[2];
float sensorValue = 0;

bool strReady = true;

union DataPacket
{
  float dataValue[2];
  char dataCharArray[8];
};

DataPacket sensorPacket;

void setup() 
{
  Wire.setClock(400000);
  // read jumpers for address?
  Wire.begin(8);                // join i2c bus with address #8
  Wire.onRequest(requestEvent); // register event
  Serial.begin(9600);  // start serial for output

  pinMode(STEERING_LEDPIN, OUTPUT);
  steeringRange[0] = 1024;
  steeringRange[1] = -1024;
  
  pinMode(THROTTLE_LEDPIN, OUTPUT);
  throttleRange[0] = 1024;
  throttleRange[1] = -1024;
}

void loop() 
{
  sensorValue = (float)(analogRead(STEERINGPIN));
  steeringRange[0] = steeringRange[0] < sensorValue ? steeringRange[0] : sensorValue;
  steeringRange[1] = steeringRange[1] > sensorValue ? steeringRange[1] : sensorValue;
  sensorPacket.dataValue[0] = ((sensorValue - steeringRange[0]) / (steeringRange[1]-steeringRange[0])) * 100.0;
  
  sensorValue = (float)(analogRead(THROTTLEPIN));
  throttleRange[0] = throttleRange[0] < sensorValue ? throttleRange[0] : sensorValue;
  throttleRange[1] = throttleRange[1] > sensorValue ? throttleRange[1] : sensorValue;
  sensorPacket.dataValue[0] = ((sensorValue - throttleRange[0]) / (throttleRange[1]-throttleRange[0])) * 100.0;

  steeringBrightness = (int)(2.55 * sensorPacket.dataValue[0]);
  analogWrite(STEERING_LEDPIN, steeringBrightness);

  throttleBrightness = (int)(2.55 * sensorPacket.dataValue[1]);
  analogWrite(THROTTLE_LEDPIN, steeringBrightness);
}

// function that executes whenever data is requested by master
// this function is registered as an event, see setup()
void requestEvent() 
{
  Wire.write(sensorPacket.dataCharArray, 8);
}   
