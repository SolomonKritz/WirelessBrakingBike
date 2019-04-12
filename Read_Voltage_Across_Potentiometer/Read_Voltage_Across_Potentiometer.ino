
#include <SPI.h>

#define OUT_1 A0

void setup() 
{
  Serial.begin(115200);
  while (!Serial) {
    delay(1);
  }
}

float resistance;

void loop()
{
  delay(1000); // Wait 1 second between transmits, could also 'sleep' here!
  float measureout = analogRead(OUT_1);
  measureout *= 3.3;  // Multiply by 3.3V, our reference voltage
  measureout /= 1024; // convert to voltage
  Serial.print("OutVoltage: " ); 
  Serial.println(measureout);
}
