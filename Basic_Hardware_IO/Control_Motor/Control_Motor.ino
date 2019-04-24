/*
 Controlling a servo position using a potentiometer (variable resistor)
 by Michal Rinott <http://people.interaction-ivrea.it/m.rinott>

 modified on 8 Nov 2013
 by Scott Fitzgerald
 http://www.arduino.cc/en/Tutorial/Knob
*/

#include <Servo.h>

Servo myservo;  // create servo object to control a servo

int potpin = 0;  // analog pin used to connect the potentiometer
int val=0;    // variable to read the value from the analog pin
int prev_val = 0;
void setup() {
  
}

void loop() {
 
  val = analogRead(potpin);            // reads the value of the potentiometer (value between 0 and 1023)

  if ( abs(prev_val-val)>3){
  
  val = map(val, 0, 1023, 70, 120);     // scale it to use it with the servo (value between 0 and 180)

  myservo.attach(9);
  
  myservo.write(val);                  // sets the servo position according to the scaled value
  
  
  delay(50);  
  myservo.detach();
 
  }
  
    prev_val = val;

 
  delay(15);
 // myservo.detach(9);
  // waits for the servo to get there
}
