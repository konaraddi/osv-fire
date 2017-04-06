/*
The purpose of this is so we can know how long it takes the OSV rotate once at various speeds
and how long it takes the OSV to move a certain distance at various speeds. This enables us
to implement the Travel Time Algorithm.
*/

#include <math.h>
#include "enes100.h"
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"

SoftwareSerial mySerial(8, 9);//the ports to which the virtual RX and TX go in (the TX requires PWM)
Marker marker(108); //look at QR code's back for number
RF_Comm rf(&mySerial, &marker);

Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *motor[4];

#define SPEED_1 50
#define SPEED_2 100
#define SPEED_3 150
#define SPEED_4 200
#define SPEED_5 250
#define MAX_SPEED 255

#define DURATION_OF_TURN 1000;
#define DURATION_OF_MOVEMENT 2000;

void setup(){
    Serial.begin(9600);
    rf.transmitData(START_MISSION, NO_DATA);
    rf.transmitData(NAV, FIRE);
    rf.updateLocation();

    AFMS.begin();
    //motor[0] and motor [1] are motors on the left (top view of OSV with front facing North)
    motor[0]=AFMS.getMotor(1);
    motor[1]=AFMS.getMotor(2);
    //motor[2] and motor [3] are motors on the right (top view of OSV with front facing North)
    motor[2]=AFMS.getMotor(3);
    motor[3]=AFMS.getMotor(4);
}

void loop(){

    while(true){}
}

runRotationTestAt(int speed){
    motor[0]->run(FORWARD);
    motor[1]->run(FORWARD);
    motor[2]->run(BACKWARD);
    motor[3]->run(BACKWARD);
    

}
