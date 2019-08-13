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
  steeringRange[0] = 1024;
  steeringRange[1] = -1024;
  
  pinMode(THROTTLE_LEDPIN, OUTPUT);
  throttleRange[0] = 1024;
  throttleRange[1] = -1024;
}

void loop() 
{
  sensorPacket.intValue[0] = (long)(analogRead(STEERINGPIN));
  sensorPacket.intValue[1] = (long)(analogRead(THROTTLEPIN));
  sensorPacket.intValue[2] = (long)digitalRead(BRAKE_PIN);
 
  //steeringBrightness = (int)(2.55 * sensorPacket.dataValue[0]);
  //analogWrite(STEERING_LEDPIN, steeringBrightness);

  //throttleBrightness = (int)(2.55 * sensorPacket.dataValue[1]);
  //analogWrite(THROTTLE_LEDPIN, steeringBrightness);
  /*
  String outStr = String(sensorPacket.dataValue[0], 3) + " " +
                  String(sensorPacket.dataValue[1], 3) + " " +
                  String(sensorPacket.intValue[2]);
  Serial.println(outStr);
  for(int byteIdx = 0; byteIdx < 12; byteIdx++)
  {
    Serial.print(sensorPacket.dataCharArray[byteIdx]);
  }
  Serial.println();
  */
}

// function that executes whenever data is requested by master
// this function is registered as an event, see setup()
void requestEvent() 
{
  Serial.println("sent");
  Wire.write(sensorPacket.dataCharArray, 12);
}   
