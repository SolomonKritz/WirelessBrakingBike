// -*- mode: C++ -*-
// Addressed Receiver Demo

// https://learn.adafruit.com/adafruit-feather-32u4-radio-with-rfm69hcw-module/using-the-rfm69-radio

#include <SPI.h>
#include <RH_RF69.h>
#include <RHReliableDatagram.h>
#include <Servo.h>

/************ Radio Setup ***************/

// Change to 434.0 or other frequency, must match RX's freq!
#define RF69_FREQ 915.0

// who am i? (server address)
#define MY_ADDRESS     3

//// Feather M0 w/Radio
//#define RFM69_CS      8
//#define RFM69_INT     3
//#define RFM69_RST     4

// Feather 32u4 w/Radio
#define RFM69_CS      8
#define RFM69_INT     7
#define RFM69_RST     4

//#define OUT_1         A0
#define SERVO_PIN     11

// Singleton instance of the radio driver
RH_RF69 rf69(RFM69_CS, RFM69_INT);

// Class to manage message delivery and receipt, using the driver declared above
RHReliableDatagram rf69_manager(rf69, MY_ADDRESS);

// Servo Setup
Servo myservo;

void setup() 
{
  //Serial.begin(115200);
  
  pinMode(RFM69_RST, OUTPUT);
  digitalWrite(RFM69_RST, LOW);

  // manual reset
  digitalWrite(RFM69_RST, HIGH);
  delay(10);
  digitalWrite(RFM69_RST, LOW);
  delay(10);
  
  if (!rf69_manager.init()) {
    // RFM69 radio init failed
    while (1);
  }
  // RFM69 radio init OK!
  
  // Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM (for low power module)
  if (!rf69.setFrequency(RF69_FREQ)) {
    // setFrequency failed
  }

  // If you are using a high power RF69 eg RFM69HW, you *must* set a Tx power with the
  // ishighpowermodule flag set like this:
  rf69.setTxPower(20, true);  // range from 14-20 for power, 2nd arg must be true for 69HCW

  // The encryption key has to be the same as the one in the Transmitter
  uint8_t key[] = { 0x40, 0x4E, 0x63, 0x52, 0x66, 0x55, 0x6A, 0x58,
                    0x6E, 0x32, 0x72, 0x35, 0x75, 0x38, 0x78, 0x21};
  rf69.setEncryptionKey(key);
}


// Dont put this on the stack:
uint8_t buf[RH_RF69_MAX_MESSAGE_LEN];
int val = 0;

void loop() {
  // Is more efficient battery wise to do available or recvfromAck?
  // Can sleep be incorporated on the receiver
  //if (rf69_manager.available()) {
    // Wait for a message addressed to us from the client
    uint8_t len = sizeof(buf);
    uint8_t from;
    if (rf69_manager.recvfromAck(buf, &len, &from)) {
      buf[len] = 0; // zero out remaining string
      val = atoi((char*)buf);
      myservo.attach(SERVO_PIN);
      myservo.write(val);  // sets the servo position according to the scaled value
      //Serial.println(val);
      delay(50);  
      myservo.detach();
    }
  //}
}
