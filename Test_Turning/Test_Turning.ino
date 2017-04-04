#include "enes100.h"
#include <Adafruit_MotorShield.h>
#include <Wire.h>

Adafruit_MotorShield AFMS= Adafruit_MotorShield();

Adafruit_DCMotor *motor1 = AFMS.getMotor(1),
                 *motor2 = AFMS.getMotor(2),
                 *motor3 = AFMS.getMotor(3),
                 *motor4 = AFMS.getMotor(4);
void setup(){
    AFMS.begin();
}


void loop() {
  motor1->setSpeed(127);
  motor2->setSpeed(127);
  motor3->setSpeed(127);
  motor4->setSpeed(127);

  motor1->run(FORWARD);
  motor2->run(FORWARD);
  motor3->run(BACKWARD);
  motor4->run(BACKWARD);

  delay(5000);

  motor1->run(BACKWARD);
  motor2->run(BACKWARD);
  motor3->run(FORWARD);
  motor4->run(FORWARD);

  delay(5000);
}





