//
// 2 channel A2D I2C sensor
//
#include <Wire.h>

#define STEERINGPIN A0
#define THROTTLEPIN A1
#define BRAKE_PIN  5
// I2C info
#define STDMODE   100000 // 100K
#define FASTMODE  400000 // 400K


union DataPacket
{
  float dataValue[3];
  long  intValue[3];
  char dataCharArray[12];
};

float sensorValue = 0;
DataPacket sensorPacket;

void setup() 
{
  Wire.setClock(FASTMODE);
  // read jumpers for address?
  Wire.begin(8);                // join i2c bus with address #8
  Wire.onRequest(requestEvent); // register event
  Serial.begin(9600);  // start serial for output

  pinMode(BRAKE_PIN, INPUT_PULLUP);
}

void loop() 
{
  long v = analogRead(STEERINGPIN);
  sensorPacket.intValue[0] = v;
  v = analogRead(THROTTLEPIN);
  sensorPacket.intValue[1] = v;
  v = (long)digitalRead(BRAKE_PIN);
  sensorPacket.intValue[2] = v;
  #ifdef DEBUGSTR 
  String outStr = String(sensorPacket.intValue[0]) + " " +
                  String(sensorPacket.intValue[1]) + " " +
                  String(sensorPacket.intValue[2]);
  Serial.println(outStr);
  #endif
}

// function that executes whenever data is requested by master
// this function is registered as an event, see setup()
void requestEvent() 
{
  #ifdef DEBUGSTR 
  Serial.println("sent");
  #endif
  Wire.write(sensorPacket.dataCharArray, 12);
}   
