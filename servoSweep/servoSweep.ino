#include <Servo.h>

Servo myservo; // create servo object to control a servo

int pos = 0; // variable to store the servo position

void setup() {
  myservo.attach(13); // attaches the servo on pin 13 to the servo object

}

void loop() {
  for (pos = 0; pos <= 180; pos+=1) {
    myservo.write(pos);
    delay(15);
  }

  for (pos = 180; pos >= 0; pos-=1) {
    myservo.write(pos);
    delay(15);
  }

}
