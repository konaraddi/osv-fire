#include <math.h>

#include <enes100.h> //I doubt we need this, but it's here just to be safe. I'll test it if we need it.
#include <marker.h>
#include <rf_comm.h>

#include <SoftwareSerial.h>
#include <Arduino.h>

#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"

Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *motor[4];

SoftwareSerial mySerial(8, 9);//the ports to which the virtual RX and TX go in
Marker marker(108); //look at QR code's back for number
RF_Comm rf(&mySerial, &marker);

//OSV's CURRENT COORDINATES
float currentX;
float currentY;
float currentTheta;

float previousX;
float previousY;
float previousTheta;

unsigned long time;

void setup(){
    Serial.begin(9600);
    AFMS.begin();
    rf.startMission();
    updateCurrentLocation();
}

void loop(){

    //find average time to pivot at certain PWMs (speeds) and print to RF lab computer
    gatherFromLinearMovement(50, 2000);
    gatherFromLinearMovement(100, 2000);
    gatherFromLinearMovement(150, 2000);
    gatherFromLinearMovement(200, 2000);
    gatherFromLinearMovement(250, 2000);
    //find average time to move at certain PWMs(speeds) and print to RF Lab computer
    //gatherFromTurning();
    /*
    The statement below will run forever so this current loop() will never
    finish running. The purpose of this is for the loop() to never repeat.
    */
    while(1);
}

void gatherFromLinearMovement(int speed, int duration){
    updateCurrentLocation();
    rf.print("0 --->");

    for(int i= 0; i < 4; i++){
        motor[i]= AFMS.getMotor(i + 1);
        motor[i]->setSpeed(speed);
    }
    for(int i= 0; i < 4; i++){
        motor[i]->run(FORWARD);
    }

    delay(duration);

    for(int i= 0; i < 4; i++){
        motor[i]->run(RELEASE);
    }

    rf.print(duration);
    rf.print("seconds of forward movement:");
    updateCurrentLocation();
    reportChangeInLocation();
}

void gatherFromTurning(){

}

void timeStamp(char arrowOrNah){
    rf.println("");
    time= millis();
    rf.print(arrowOrNah);
    rf.print(time);
}
/*
The functions below use speed (PWM: 0-255) and duration. For now, the duration
is time-based (milliseconds). It will be changed to rotations in the future.
The direction should be FORWARD or BACKWARD.

TESTED thru Serial print
*/
void moveStraight(int speed, int duration, char direction){

    //motors on the left are motor[0] and motor [1] (top view of OSV with front facing North)
    //motors on the right are motor[2] and motor [3] (top view of OSV with front facing North)
    for(int i= 0; i < 4; i++){
        motor[i]= AFMS.getMotor(i + 1);
        motor[i]->setSpeed(speed);
    }

    /*
    The if-statement has been kept outside of the loops intentionally. While this
    does take up more space, this is far more efficient than keeping the if-statements
    inside one loop.

    The following statements set the motors to the specified direction (FORWARD or BACKWARD).
    */
    if(direction == 'F'){
        for(int i= 0; i < 4; i++){
            //intentionally kept out of previous loop so motors are set up before moving
            motor[i]->run(FORWARD);
        }
    }else if(direction == 'B'){
        for(int i= 0; i < 4; i++){
            //intentionally kept out of previous loop so motors are set up before moving
            motor[i]->run(FORWARD);
        }
    }else{
        Serial.println("You need to use F or B, with quotes, for direction in moveStraight(~)");
    }

    delay(duration);

    //RELEASE is for turning off the motors
    for(int i= 0; i < 4; i++){
        //intentionally kept out of previous loop so motors are set up before moving
        motor[i]->run(RELEASE);
    }

    //the below is to check if it's working
    Serial.println("moveStraight(~) executed");
    Serial.println(speed);
    Serial.println(duration);
    Serial.println(direction);
    Serial.print("ms");

}


void updateCurrentLocation(){

    if(rf.updateLocation()){
        previousX= currentX;
        previousY= currentY;
        previousTheta= currentTheta;

        currentX= marker.x;
        currentY= marker.y;
        currentTheta= marker.theta;
        rf.println("location updated");
    }else{
        //this shouldn't be happening....
        rf.println("oh lawrd");
        rf.println("updateCurrentTheta broke");
    }

}

void reportChangeInLocation(){
    rf.println("change in X:");
    rf.print(abs(previousX - currentX));
    rf.println("change in Y:");
    rf.print(abs(previousY - currentY));
    rf.println("change in theta: ");
    rf.print(previousTheta);
    rf.print("--->");
    rf.print(currentTheta);
}
