//
// 2 channel A2D I2C sensor
//
#include <Wire.h>

#define STEERINGPIN A0
#define THROTTLEPIN A1
#define BRAKE_PIN  5
#define STEERING_LEDPIN 6
#define THROTTLE_LEDPIN 7

int steeringBrightness = 0;    // how bright the LED is
int throttleBrightness = 0;    // how bright the LED is
float steeringRange[2];
float throttleRange[2];
float sensorValue = 0;

bool strReady = true;

union DataPacket
{
  float dataValue[3];
  long  intValue[3];
  char dataCharArray[12];
};

DataPacket sensorPacket;

void setup() 
{
  Wire.setClock(100000);
  // read jumpers for address?
  Wire.begin(8);                // join i2c bus with address #8
  Wire.onRequest(requestEvent); // register event
  Serial.begin(9600);  // start serial for output

  pinMode(BRAKE_PIN, INPUT_PULLUP);
  pinMode(STEERING_LEDPIN, OUTPUT);
  
  pinMode(THROTTLE_LEDPIN, OUTPUT);
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
