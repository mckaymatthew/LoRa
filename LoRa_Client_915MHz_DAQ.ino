// rf95_client.pde
// -*- mode: C++ -*-
// Example sketch showing how to create a simple messageing client
// with the RH_RF95 class. RH_RF95 class does not provide for addressing or
// reliability, so you should only use RH_RF95 if you do not need the higher
// level messaging abilities.
// It is designed to work with the other example rf95_server
// Tested with Anarduino MiniWirelessLoRa, Rocket Scream Mini Ultra Pro with
// the RFM95W, Adafruit Feather M0 with RFM95

#include <SPI.h>
#include <RH_RF95.h>

// Singleton instance of the radio driver
RH_RF95 rf95;
//RH_RF95 rf95(5, 2); // Rocket Scream Mini Ultra Pro with the RFM95W
//RH_RF95 rf95(8, 3); // Adafruit Feather M0 with RFM95 

// Need this on Arduino Zero with SerialUSB port (eg RocketScream Mini Ultra Pro)
//#define Serial SerialUSB
const int voltage_pin =A1;
#define NUMSAMPLES 200
//ADC ~10KHz, 10000 samples = 1 second of data

void setup() 
{
  // Rocket Scream Mini Ultra Pro with the RFM95W only:
  // Ensure serial flash is not interfering with radio communication on SPI bus
//  pinMode(4, OUTPUT);
//  digitalWrite(4, HIGH);

pinMode(voltage_pin, INPUT);

  Serial.begin(250000);
  while (!Serial) ; // Wait for serial port to be available
  if (!rf95.init())
    Serial.println("init failed");
  // Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on
  // Set default frequency to 915.0MHz APL 9/11/2018
  // The default transmitter power is 13dBm, using PA_BOOST.
  // If you are using RFM95/96/97/98 modules which uses the PA_BOOST transmitter pin, then 
  // you can set transmitter powers from 5 to 23 dBm:
//  driver.setTxPower(23, false);
  // If you are using Modtronix inAir4 or inAir9,or any other module which uses the
  // transmitter RFO pins and not the PA_BOOST pins
  // then you can configure the power transmitter power for -1 to 14 dBm and with useRFO true. 
  // Failure to do that will result in extremely low transmit powers.
//  driver.setTxPower(14, true);
}


//APL 12/9/18
int output=0;
int voltage;
int volt_str;
String VS;
uint8_t VS_len;
unsigned int len;
int samples[NUMSAMPLES];
uint8_t i;
//uint8_t dataoutgoing[10];

//String fix_voltage(int voltage) {
//
//  return String(voltage);
//}

void loop()
{
  //output = analogRead(voltage_pin); //

  //Test start
 for (i=0; i<NUMSAMPLES; i++){
  samples[i] = analogRead(voltage_pin);
  voltage = samples[i] * (5000.0 / 1024.0); //converts to mv
   Serial.println(voltage);
  break;
  //delay(10);
 }

  //test end
  //voltage = output * (5000.0 / 1024.0); //converts to mv
  float V1 = voltage;
  VS = String(V1);
  char result[8];
  dtostrf(voltage, 6, 2, result);
//  VS_len = VS.length();
//  uint8_t VS_len;
 // volt_str = voltage;
  Serial.println("Sending to rf95_server");
  // Send a message to rf95_server
  //uint8_t data(voltage[i]);
// Serial.println(voltage);
// uint8_t data[] = result;
 //VS.StringToCharArray(voltage,10)
 //uint8_t VS.toCharArray(VS,10);
 //Serial.println(data);
 VS_len = sizeof((uint8_t*) result);
  rf95.send(result, sizeof(result));


  
  rf95.waitPacketSent();
  // Now wait for a reply
  uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
  uint8_t len = sizeof(buf);

  if (rf95.waitAvailableTimeout(1))
  { 
    // Should be a reply message for us now   
    if (rf95.recv(buf, &len))
   {
      Serial.print("got reply: ");
      Serial.println((char*)buf);
//      Serial.print("RSSI: ");
//      Serial.println(rf95.lastRssi(), DEC);    
    }
    else
    {
      Serial.println("recv failed");
    }
  }
 // else
//  {
 //   Serial.println("No reply, is rf95_server running?");
 // }
  delay(1);
}
