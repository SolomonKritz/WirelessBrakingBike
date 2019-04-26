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
#define BUZZER       9

// Singleton instance of the radio driver
RH_RF69 rf69(RFM69_CS, RFM69_INT);

// Class to manage message delivery and receipt, using the driver declared above
RHReliableDatagram rf69_manager(rf69, MY_ADDRESS);


int16_t packetnum = 0;  // packet counter, we increment per xmission

void setup() 
{
  Serial.begin(115200);
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

int vread = 0;
int voltage = 0;
int oldVoltage = -10;
void loop() {
  Serial.println("ON");
  delay(250);  // Wait 1 second between transmits, could also 'sleep' here!
 voltage = analogRead(IN_1);
 // vread = analogRead(IN_1);
 // voltage = (-(1023/548) * vread) + 1247.0146; 
  // Should see how much measureout varies by before transmitting
  if (voltage - oldVoltage < -3 || voltage - oldVoltage > 3) {
    char radiopacket[5];
    itoa(voltage, radiopacket, 10);
    // Send a message to the DESTINATION!
    if (!rf69_manager.sendtoWait((uint8_t *)radiopacket, strlen(radiopacket), DEST_ADDRESS)) {
      digitalWrite(LED, HIGH);
      tone(BUZZER, 1000, 500);
    }
    else {
      oldVoltage = voltage;
      digitalWrite(LED, LOW);
    }
    rf69.sleep();
  }
}
