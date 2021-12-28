//
// ProMini combo sensor
//   4 A2D channels
//   8 Digital channels
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
//#define DEBUGPRINT
// I2C info
#define STDMODE   100000 // 100K
#define FASTMODE  400000 // 400K
// data packet to send to master - 3 floats/longs (can mix), 12 characters
#define DATA_SIZE 9
struct SensorPacket
{
  byte digVal;
  int a2dVal[4];
};
union DataPacket
{
  SensorPacket sensors;
  char dataVal[DATA_SIZE];
} dataPacket;

byte digVal = 0;
int a2dVal[4];
char buffer[125];
bool scanPorts = true;
//
// initialize - set pin modes, read address jumper pins, initialize I2C
//              set up I2C data request callback
//
void setup() 
{
  #ifdef DEBUGPRINT
  Serial.begin(115200);
  Serial.println("YamuraLog v4 (ProMini digital/A2D combo sensor)");
  #endif
  // digital pin setup
  // 8 digital channels
  pinMode( 5, INPUT_PULLUP);
  pinMode( 6, INPUT_PULLUP);
  pinMode( 7, INPUT_PULLUP);
  pinMode( 8, INPUT_PULLUP);
  pinMode( 9, INPUT_PULLUP);
  pinMode(10, INPUT_PULLUP);
  pinMode(11, INPUT_PULLUP);
  pinMode(12, INPUT_PULLUP);
  // set I2C clock
  Wire.setClock(FASTMODE);
  // read jumpers for address
  int wireAddr = 0x08;
  Wire.begin(wireAddr);         // join i2c bus with address #8
  Wire.onRequest(requestEvent); // register event
  Wire.onReceive(receiveEvent); // register event
}
//
// read sensor values to sensorPacket
//
void loop() 
{
  // read to temp values
  digVal = 0;
  if(scanPorts) digVal += digitalRead( 5);
  if(scanPorts) digVal += (digitalRead( 6) << 1);
  if(scanPorts) digVal += (digitalRead( 7) << 2);
  if(scanPorts) digVal += (digitalRead( 8) << 3);
  if(scanPorts) digVal += (digitalRead( 9) << 4);
  if(scanPorts) digVal += (digitalRead(10) << 5);
  if(scanPorts) digVal += (digitalRead(11) << 6);
  if(scanPorts) digVal += (digitalRead(12) << 7);
  if(scanPorts) a2dVal[0] = analogRead(0);
  if(scanPorts) a2dVal[1] = analogRead(1);
  if(scanPorts) a2dVal[2] = analogRead(2);
  if(scanPorts) a2dVal[3] = analogRead(3);
  // transfer temp values to structure after all reads are complete
  // reduce risk of I2C request getting data partway through a read (not unlikely for digital reads)
  // need to implement a 'data ready' request to get last complete read
  if(scanPorts) 
  {
    dataPacket.sensors.digVal = digVal;
    for(int idx = 0; idx < 4; idx++)
    {
      dataPacket.sensors.a2dVal[idx] = a2dVal[idx];
    }
  }
}
//
// function that executes whenever data is requested by master
// this function is registered as an event, see setup()
//
void requestEvent() 
{
  Wire.write(dataPacket.dataVal, DATA_SIZE);
  #ifdef DEBUGPRINT
  Serial.print("Sent ");
  Serial.println(DATA_SIZE);
  Serial.println(" bytes, scanPorts set to true");
  #endif
  scanPorts = true;
}
void receiveEvent()
{
  // at this point consider any send to this device a request to prepare to send data
  #ifdef DEBUGPRINT
  Serial.print("Received ");
  #endif
  while(Wire.available() > 1) 
  {
    char c = Wire.read();        // receive byte as a character
    #ifdef DEBUGPRINT
    Serial.print(c);
    #endif
  }
  scanPorts = false;
  #ifdef DEBUGPRINT
  Serial.println(" scanPorts set to false");
  #endif
}
