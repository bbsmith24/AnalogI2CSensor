//
// YamuraLog cockpit sensor 
//   2 A2D channels (steering and throttle)
//   1 Digital channel (brake on/off)
//   I2C address configurable 0-31
//  
// for Arduino ProMini 5V
// could also run on other Arduino boards
//
// Brian B. Smith
// brianbsmith.com
// August 2019
// 
#include <Wire.h>
// uncomment for debug string output to serial
//#define DEBUGSTR
// sensor pins
#define STEERINGPIN A0
#define THROTTLEPIN A1
#define BRAKE_PIN  10
// I2C address jumpers
#define ADDR_0 5
#define ADDR_1 6
#define ADDR_2 7
#define ADDR_3 8
#define ADDR_4 9
// I2C info
#define STDMODE   100000 // 100K
#define FASTMODE  400000 // 400K
// data packet to send to master - 3 floats/longs (can mix), 12 characters
union DataPacket
{
  float dataValue[3];
  long  intValue[3];
  char dataCharArray[12];
};
// current A2D value
float sensorValue = 0;
// data
DataPacket sensorPacket;
//
// initialize - set pin modes, read address jumper pins, initialize I2C
//              set up I2C data request callback
//
void setup() 
{
  // digital pin setup
  pinMode(BRAKE_PIN, INPUT_PULLUP);
  pinMode(ADDR_0, INPUT_PULLUP);
  pinMode(ADDR_1, INPUT_PULLUP);
  pinMode(ADDR_2, INPUT_PULLUP);
  pinMode(ADDR_3, INPUT_PULLUP);
  pinMode(ADDR_4, INPUT_PULLUP);
  // set I2C clock
  Wire.setClock(FASTMODE);
  // read jumpers for address
  int wireAddr = 0;
  if(digitalRead(ADDR_0) == LOW)
  { wireAddr +=  1; }
  if(digitalRead(ADDR_1) == LOW)
  { wireAddr +=  2; }
  if(digitalRead(ADDR_2) == LOW)
  { wireAddr +=  4; }
  if(digitalRead(ADDR_3) == LOW)
  { wireAddr +=  8; }
  if(digitalRead(ADDR_4) == LOW)
  { wireAddr += 16; }
  Wire.begin(8);                // join i2c bus with address #8
  Wire.onRequest(requestEvent); // register event

  #ifdef DEBUGSTR 
  Serial.begin(9600);
  while (!Serial) { ; }
  Serial.println("Cockpit throttle/steering/brake sensor");
  String outStr = "I2C address " + String(wireAddr);
  Serial.println(outStr);
  #endif
}
//
// read sensor values to sensorPacket
//
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
//
// function that executes whenever data is requested by master
// this function is registered as an event, see setup()
//
void requestEvent() 
{
  #ifdef DEBUGSTR 
  Serial.println("sent");
  #endif
  Wire.write(sensorPacket.dataCharArray, 12);
}   
