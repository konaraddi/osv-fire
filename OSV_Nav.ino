#include <enes100.h>
#include <marker.h>
#include <rf_comm.h>

#include <SoftwareSerial.h>
#include <Arduino.h>

#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"

float currentX;
float currentY;
float currentTheta;

SoftwareSerial mySerial(8, 9);//the ports to which the virtual RX and TX go in
Marker marker(108); //look at QR code's back for number
RF_Comm rf(&mySerial, &marker);

void setup(){

}

void loop(){



    /*
    The statement below will run forever so this current loop() will never
    finish running. The purpose of this is for the loop() to never repeat.
    */
    while(1);
}

void updateCurrentLocation(){
    if(rf.updateLocation()){
        currentX= marker.x;
        currentY= marker.y;
        currentTheta= marker.theta;
    }else{
        rf.println("oh lawrd");
        rf.println("updateCurrentLocation broke");
    }
}

/*
The functions below use speed (PWM: 0-255) and duration. For now, the duration
is time-based. It will be changed to rotations in the future. The direction
indicates FORWARD or BACKWARD.
*/
void moveStraight(int speed, int duration, char direction){
    Adafruit_MotorShield AFMS = Adafruit_MotorShield();
    AFMS.begin();
    Adafruit_DCMotor *motor1 = AFMS.getMotor(1);
    Adafruit_DCMotor *motor2 = AFMS.getMotor(2);
    Adafruit_DCMotor *motor3 = AFMS.getMotor(3);
    Adafruit_DCMotor *motor4 = AFMS.getMotor(4);

    motor1->setSpeed(speed);
    motor2->setSpeed(speed);
    motor3->setSpeed(speed);
    motor4->setSpeed(speed);

    if(direction == 'FORWARD'){
        motor1->run(FORWARD);
        motor2->run(FORWARD);
        motor3->run(FORWARD);
        motor4->run(FORWARD);
    }else if(direction == 'BACKWARD'){
        motor1->run(BACKWARD);
        motor2->run(BACKWARD);
        motor3->run(BACKWARD);
        motor4->run(BACKWARD);
    }

    delay(duration);

    motor1->run(RELEASE);
    motor2->run(RELEASE);
    motor3->run(RELEASE);
    motor4->run(RELEASE);
}
