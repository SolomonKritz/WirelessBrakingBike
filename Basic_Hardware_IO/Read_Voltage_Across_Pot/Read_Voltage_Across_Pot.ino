
#include <SPI.h>

#define IN_1 A0

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
  int measure_in = analogRead(IN_1);
  Serial.print("OutVoltage: " ); 
  Serial.println(measure_in);
}
