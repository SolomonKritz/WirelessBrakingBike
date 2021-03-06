// -*- mode: C++ -*-

#include <SPI.h>
#include <RH_RF69.h>
#include <RHReliableDatagram.h>
/************ Radio Setup ***************/

// Change to 434.0 or other frequency, must match RX's freq!
#define RF69_FREQ 915.0

// Where to send packets to!
#define DEST_ADDRESS   3
// change addresses for each client board, any number :)
#define MY_ADDRESS     6

// Feather M0 w/Radio
//#define RFM69_CS      8
//#define RFM69_INT     3
//#define RFM69_RST     4

// Feather 32u4 w/Radio
#define RFM69_CS      8
#define RFM69_INT     7
#define RFM69_RST     4

#define IN_1         A0
#define LED          13
#define BUZZER       6

#define MAX_SERVO 180
#define MIN_SERVO 10
#define MAX_POT 670
#define MIN_POT 210

#define THRESHOLD 8 //5

// Singleton instance of the radio driver
RH_RF69 rf69(RFM69_CS, RFM69_INT);

// Class to manage message delivery and receipt, using the driver declared above
RHReliableDatagram rf69_manager(rf69, MY_ADDRESS);


int16_t packetnum = 0;  // packet counter, we increment per xmission

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

  // The encryption key has to be the same as the one in the Reciever
  uint8_t key[] = { 0x40, 0x4E, 0x63, 0x52, 0x66, 0x55, 0x6A, 0x58,
                    0x6E, 0x32, 0x72, 0x35, 0x75, 0x38, 0x78, 0x21};
  rf69.setEncryptionKey(key);
}

int V_Read = 0;
int V_Scaled = 0;
int old_V_Read = 500;
void loop() {
  
  delay(250);  // Wait .25 second between transmits.
  V_Read = analogRead(IN_1);
  //Serial.println(V_Read);

  //if (V_Read - old_V_Read < -THRESHOLD || V_Read - old_V_Read > THRESHOLD) {
  if (V_Read < MAX_POT - THRESHOLD || old_V_Read < MAX_POT - THRESHOLD) {
    // Varies between 670 and like 210 (can get to 140)
    // Need to go between 10 and 180? Need to configure more once justin gets the 
    // motor mounted. 

    V_Scaled = (V_Read - MIN_POT)*long(MAX_SERVO-MIN_SERVO)/(MAX_POT-MIN_POT)+MIN_SERVO;
    if (V_Scaled > MAX_SERVO) V_Scaled = MAX_SERVO;
    else if (V_Scaled < MIN_SERVO) V_Scaled = MIN_SERVO;
//    V_Scaled = V_Scaled * -1;
//    V_Scaled = V_Scaled + 190;
    char radioPacket[5];
    itoa(V_Scaled, radioPacket, 10);
       
    // Send a message to the DESTINATION!
    if (!rf69_manager.sendtoWait((uint8_t *)radioPacket, strlen(radioPacket), DEST_ADDRESS)) {
      digitalWrite(LED, HIGH);
      tone(BUZZER, 1000, 500);
    }
    else {
      digitalWrite(LED, LOW);
    }
    old_V_Read = V_Read;
    rf69.sleep();
  }
}
