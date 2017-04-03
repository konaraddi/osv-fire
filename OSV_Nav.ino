/*TODO
Handle constants appropriately.
*/

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

//TODO replace the stuff below with results from tests
//assume OSV takes 1 second to rotate its wheels once
const float timeToRotateOnce= 1000;
const float timeToPivot360= 1000;

//circumference of our OSV's wheels (in meters)
float circumferenceOfOSVWheel= 2;

//OSV's CURRENT COORDINATES
float currentX;
float currentY;
float currentTheta;

const int typicalSpeed= 50; //0-255 (PWM)

//FOR EXIT THE WALL
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
    rf.startMission();
    AFMS.begin();

    //obtain OSV's landing coordinates
    updateCurrentLocation();
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

void updateCurrentLocation(){

    if(rf.updateLocation()){
        currentX= marker.x;
        currentY= marker.y;
        currentTheta= marker.theta;
        rf.println("location updated");
    }else{
        //this shouldn't be happening....
        rf.println("oh lawrd");
        rf.println("updateCurrentLocation broke");
    }

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

/*
NEEDS TO BE TESTED THOROUGHLY.
*/
void moveTowardsPoint(float desiredX, float desiredY){

    updateCurrentLocation();

    float Ymovement= desiredY - currentY;
    float Xmovement= desiredX - currentX;

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

    float distanceToMove= sqrt(pow(Ymovement,2) + pow(Xmovement, 2));
    moveStraight(typicalSpeed, (distanceToMove / circumferenceOfOSVWheel) * timeToRotateOnce, 'F');

    updateCurrentLocation();

    float permissibleErrorForX= 0.1;//the OSV's X coordinate must be within +/- this of the desired X
    float permissibleErrorForY= 0.1;//the OSV's Y coordinate must be within +/- this of the desired Y
    float permissibleErrorForTheta= 0.1;//the OSV's Theta coordinate must be within +/- this of the desired theta

    if(currentX > desiredX + permissibleErrorForX || currentX < desiredX - permissibleErrorForX ||
        currentY > desiredY + permissibleErrorForY || currentY < desiredY - permissibleErrorForY ||
        currentTheta < theta - permissibleErrorForTheta || currentTheta > theta + permissibleErrorForTheta){
        //recursively called until OSV's orientation an position are correct
        moveTowardsPoint(desiredX, desiredY);
        //TODO
        //Write code that prevents the OSV from being stuck in a repetitive motion where the OSV goes too over or too under
        //This should be prevented in turnClockWise and turnCounterClockWise but have a safetynet here too.
    }

}

void turnClockWise(float radiansToTurn){
    turn(radiansToTurn, 'C');
}
void turnCounterClockWise(float radiansToTurn){
    turn(radiansToTurn, 'W');
}

//if direction is C, the Clockwise turning.
//if direction is not C, then counterclockWise turning.
void turn(float radiansToTurn, char direction){
    //need to conduct test to figure out how long it takes for the OSV to make a 360 pivot
    for(int i= 0; i < 4; i++){
        motor[i]= AFMS.getMotor(i + 1);
        motor[i]->setSpeed(typicalSpeed);
    }
    //intentionally kept ouside of previous loop so the motors are set before they run
    for(int i= 0; i < 4; i++){
        if(i < 2){
            if(direction == 'C'){
                motor[i]->run(FORWARD);
            }else{
                motor[i]->run(BACKWARD);
            }
        }else{
            if(direction == 'C'){
                motor[i]->run(BACKWARD);
            }else{
                motor[i]->run(FORWARD);
            }
        }
    }
    //the predicted time it would take to face
    //TODO write code to check if it actually faced that direction and recursively call something to fix it
    delay((radiansToTurn / PI ) * timeToPivot360);
}
