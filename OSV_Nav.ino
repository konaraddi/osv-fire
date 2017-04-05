#include <math.h>
#include "enes100.h"
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

//dictates the speed of the OSV's general movement
const int typicalSpeed= 50; //0-255 (PWM)
const int lengthOfEachBurst= 100; //in milliseconds

//FOR EXITING THE WALL
//Area A (an area to which the OSV should go to before making an exit)
const float Ax= 0.5;
const float Ay= 0.325;
//Area B (an area to which the OSV should go to before making an exit)
const float Bx= 0.5;
const float By= 1.675;
//Point of EXIT from area A
const float EXIT_Ax= 0.1;
const float EXIT_Ay= 0.325;
//Point of EXIT from area B
const float EXIT_Bx= 0.1;
const float EXIT_By= 1.675;


void setup(){
    Serial.begin(9600);
    rf.transmitData(START_MISSION, NO_DATA);
    rf.transmitData(NAV, FIRE);
    rf.updateLocation();

    AFMS.begin();
    motor[0]=AFMS.getMotor(1);
    motor[1]=AFMS.getMotor(2);
    motor[2]=AFMS.getMotor(3);
    motor[3]=AFMS.getMotor(4);
    //motor[0] and motor [1] are motors on the left (top view of OSV with front facing North)
    //motor[2] and motor [3] are motors on the right (top view of OSV with front facing North)

}

void loop(){
    //TODO Optimize exiting the wall after the basics work (i.e. implement Travel Time algorithm)
    //EXIT THE WALL THROUGH POINT A (FOR NOW, WILL OPTIMIZE LATER)
    moveTowardsPoint(Ax, Ay);
    moveTowardsPoint(EXIT_Ax, EXIT_Ay);

    //TRAVEL TOWARDS FIRE SITE

    //FIRE SITE ROUND 1

    //FIRE SITE ROUND 2

    /*
    The statement below will run forever so this current loop() will never
    finish running. The purpose of this is for the loop() to never repeat.
    */
    while(1);
}

/*
The functions below use speed (PWM: 0-255) and duration.
The direction should be FORWARD or BACKWARD.

TESTED thru Serial print
*/
void moveStraight(int speed, int duration, char movement){

    for(int i= 0; i < 4; i++){
        motor[i]->setSpeed(speed);
    }

    for(int i= 0; i < 4; i++){
        motor[i]->run(movement);
    }

    delay(duration);

    for(int i= 0; i < 4; i++){
        motor[i]->run(RELEASE);
    }

    //the below is to check if it's working
    rf.println("moveStraight(~) executed for");
    rf.print(duration);

}

/*
NEEDS TO BE TESTED THOROUGHLY.
*/
void moveTowardsPoint(float desiredX, float desiredY){

    rf.updateLocation();
    float startingX= marker.x;
    float startingY= marker.y;
    float distanceItShouldTravel= sqrt( pow( abs(startingX - desiredX), 2) + pow( abs(startingY - desiredY), 2));

    float Ymovement= desiredY - marker.y;
    float Xmovement= desiredX - marker.x;

    //the direction the OSV needs to face
    //returns a range of -pi to +pi
    float theta= atan2(Ymovement, Xmovement);

    if(theta < 0){
        //OSV will turn clockwise
        turnClockWise(theta);
    }else{
        //OSV will turn counterclockwise
        turnCounterClockWise(theta);
    }

    float permissibleErrorForX= 0.1;//the OSV's X coordinate must be within +/- this of the desired X
    float permissibleErrorForY= 0.1;//the OSV's Y coordinate must be within +/- this of the desired Y

    boolean arrivedAtDestination;
    while(arrivedAtDestination == false){

        moveStraight(typicalSpeed, lengthOfEachBurst, 'FORWARD');
        rf.updateLocation();
        float distanceTraveled= sqrt( pow( abs(marker.x - startingX), 2) + pow( abs(marker.y - startingY), 2));

        if(marker.x < desiredX + permissibleErrorForX || marker.x > desiredX - permissibleErrorForX ||
            marker.y < desiredY + permissibleErrorForY || marker.y > desiredY - permissibleErrorForY){
            arrivedAtDestination= true;
        }

        if(distanceTraveled < distanceItShouldTravel){

        }

        if(distanceTraveled > distanceItShouldTravel){

        }

    }
    rf.updateLocation();
}

void turnClockWise(float radiansToTurn){
    turn(typicalSpeed, radiansToTurn, 'clockwise', lengthOfEachBurst);
}
void turnCounterClockWise(float radiansToTurn){
    turn(typicalSpeed, radiansToTurn, 'counterclockwise', lengthOfEachBurst);
}

//if direction is C, the Clockwise turning.
//if direction is not C, then counterclockWise turning.
void turn(int speed, float radiansToTurn, char direction, int duration){
    //it knows it faced the right direction if the initial direction + radiansToTurn = desired direction.
    //need to conduct test to figure out how long it takes for the OSV to make a 360 pivot
    for(int i= 0; i < 4; i++){
        motor[i]->setSpeed(speed);
    }
    //intentionally kept ouside of previous loop so the motors are set before they run
    if(direction == 'clockwise'){
        motor[0]->run(FORWARD);
        motor[1]->run(FORWARD);
        motor[2]->run(BACKWARD);
        motor[3]->run(BACKWARD);
    }else if(direction == 'counterclockwise'){
        motor[0]->run(BACKWARD);
        motor[1]->run(BACKWARD);
        motor[2]->run(FORWARD);
        motor[3]->run(FORWARD);
    }else{
        rf.println("used turn() incorrectly, check spelling of clockwise or counterclockwise");
    }

    //TODO write code to check if it actually faced that direction and recursively call something to fix it
    delay(duration);
}
