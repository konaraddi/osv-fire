#include <enes100.h>
#include <marker.h>
#include <rf_comm.h>

#include <SoftwareSerial.h>
#include <Arduino.h>

//did i put the Adafruit library in the right place? Should the .h file be taken out of the Adafruit folder
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"


//The below is the MAIN thread
void setup(){
    //TO GET THE ARDUINO TO MOVE
    Adafruit_MotorShield AFMS = Adafruit_MotorShield();
    /*
    Check ports and change if necessary before running!
    These ports are the M ports
    */

    //TODO
    //make the motors an array and provide methods for the real deal
    Adafruit_DCMotor *motor1 = AFMS.getMotor(1);
    //motor1->setSpeed(50); //0-255
    Adafruit_DCMotor *motor2 = AFMS.getMotor(2);
    //motor2->setSpeed(50);
    Adafruit_DCMotor *motor3 = AFMS.getMotor(3);
    //motor3->setSpeed(50);
    Adafruit_DCMotor *motor4 = AFMS.getMotor(4);
    //motor4->setSpeed(50);

    /*makes the OSV revolve on a point, unless the include tags failed (or library incorrectly imported)
    OR, if the OSV moved forward, the motors' wires need to be swapped in a place or more
    */
    /*motor1->run(FORWARD);
    motor2->run(FORWARD);
    motor3->run(BACKWARD);
    motor4->run(BACKWARD);*/
}

void loop(){
  
}

