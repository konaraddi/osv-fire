#include <math.h>
#include "enes100.h"
#include <SoftwareSerial.h>
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"

#define CLOCKWISE 0
#define COUNTERCLOCKWISE 1

float permissibleErrorForTheta= 0.075;//Coordinate Transmissions are accurate to +/- 0.050 radians
float permissibleErrorForXY= 0.075; //Coordinate Transmissions are accurate to +/- 0.050 meters

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
    //motor[0] and motor [1] are motors on the left (top view of OSV with front facing North)
    motor[0]=AFMS.getMotor(1);
    motor[1]=AFMS.getMotor(2);
    //motor[2] and motor [3] are motors on the right (top view of OSV with front facing North)
    motor[2]=AFMS.getMotor(3);
    motor[3]=AFMS.getMotor(4);
}

void loop(){
    //TODO Optimize exiting the wall after the basics work (i.e. implement Travel Time algorithm)
    //EXIT THE WALL THROUGH POINT A (for now, will incorporate distance sensor later)
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


void moveTowardsPoint(float desiredX, float desiredY){
    float distanceItShouldTravel;//the distance we expect the OSV to travel
    float distanceTraveled;//the actual distance the OSV travels

    rf.updateLocation();
    float startingX= marker.x;
    float startingY= marker.y;
    distanceItShouldTravel= sqrt( pow( abs(startingX - desiredX), 2) + pow( abs(startingY - desiredY), 2));

    float changeInY= desiredY - marker.y;
    float changeInX= desiredX - marker.x;

    //the direction the OSV needs to face
    //returns a range of -pi to +pi
    float directionToFace= atan2(changeInY, changeInX);

    face(directionToFace);

    /*
    The below is where the OSV will incremently move until it reaches it's destination.

    The "N" is used, in the unlikely scenario, that the OSV is stuck in an infinite loop
    of going back and forth. In such a case, the OSV reduces it's backward motion by a factor
    of "N", where N starts at 1 and is incremented every time the OSV moves backward and
    does not travel within +/ permissibleErrorForXY of distanceItShouldTravel;.
    */
    int N= 1;
    boolean arrivedAtDestination = false;
    while(arrivedAtDestination == false){

        distanceTraveled= sqrt( pow( abs(marker.x - startingX), 2) + pow( abs(marker.y - startingY), 2));

        if(distanceTraveled < distanceItShouldTravel - permissibleErrorForXY){
            //OSV has yet to reach destination
            moveStraight(typicalSpeed, lengthOfEachBurst, FORWARD);
        }
        else if(distanceTraveled > distanceItShouldTravel + permissibleErrorForXY){
            //OSV has gone past it's destination
            moveStraight(typicalSpeed, (int) lengthOfEachBurst / N, BACKWARD);
            N++;
        }else{
            //OSV reached destination
            rf.updateLocation();
            arrivedAtDestination= true;
        }
    }

    rf.updateLocation();
}


void face(float directionToFace){

    rf.updateLocation();

    for(int i= 0; i < 4; i++){
        motor[i]->setSpeed(typicalSpeed);
    }

    int rotate= rotate_CCW_or_CW(directionToFace);

    if(rotate == CLOCKWISE){
        motor[0]->run(FORWARD);
        motor[1]->run(FORWARD);
        motor[2]->run(BACKWARD);
        motor[3]->run(BACKWARD);
    }else if(rotate ==  COUNTERCLOCKWISE){
        motor[0]->run(BACKWARD);
        motor[1]->run(BACKWARD);
        motor[2]->run(FORWARD);
        motor[3]->run(FORWARD);
    }else{
        rf.println("failure @ turnTowards() execution, unable to decided CCW or CW turning");
    }

    delay(lengthOfEachBurst);

    //stop all motors
    for(int i= 0; i < 4; i++){
        motor[i]->run(RELEASE);
    }

    //convert to 0 -> 2pi system for calculations (Coordinate Transmissions uses -pi -> +pi system)
    int positiveDesiredTheta= directionToFace;
    int positiveCurrentTheta= marker.theta;
    if(directionToFace < 0){ positiveDesiredTheta+= 2 * PI; }
    if(marker.theta < 0){ positiveCurrentTheta+= 2 * PI; }

    rf.updateLocation();

    //check if OSV's current theta is within +/- permissibleErrorForTheta of the desired theta
    if(positiveCurrentTheta > positiveDesiredTheta + permissibleErrorForTheta ||
        positiveCurrentTheta < positiveDesiredTheta - permissibleErrorForTheta){
        face(directionToFace);//TODO optimize this w/ 2nd overloaded method
    }

    rf.updateLocation();

}

//Turn COUNTERCLOCKWISE or CLOCKWISE? That's the decision being made below.
int rotate_CCW_or_CW(float directionToFace){

    //convert to 0 -> 2pi system for calculations
    int positiveDesiredTheta= directionToFace;
    int positiveCurrentTheta= marker.theta;
    if(directionToFace < 0){ positiveDesiredTheta+= 2 * PI; }
    if(marker.theta < 0){ positiveCurrentTheta+= 2 * PI; }

    if(marker.theta < positiveDesiredTheta){
        if(abs(marker.theta - directionToFace) < PI){
            rf.println("OSV will turn COUNTERCLOCKWISE");
            return COUNTERCLOCKWISE;
        }
    }

    rf.println("OSV will turn CLOCKWISE");
    return CLOCKWISE;
}


void moveStraight(int speed, int duration, int movement){

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
