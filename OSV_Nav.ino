#include <math.h>
#include "enes100.h"
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"

SoftwareSerial mySerial(2, 3);
Marker marker(10); //look at QR code's back for number
RF_Comm rf(&mySerial, &marker);

Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *motor[4];

#define CLOCKWISE 0
#define COUNTERCLOCKWISE 1

float permissibleErrorForTheta= 0.075;//Coordinate Transmissions are accurate to +/- 0.050 radians
float permissibleErrorForXY= 0.075; //Coordinate Transmissions are accurate to +/- 0.050 meters

//dictates the speed of the OSV's general movement
#define TURBO_BOOST 255 //Just for kicks
#define AVG_SPEED 175 //0-255 PWM
#define DURATION_OF_BURST 200 //in milliseconds

//FOR EXITING THE WALL
//Area A (an area to which the OSV should go to before making an exit)
#define Ax 0.5
#define Ay 0.325
//Area B (an area to which the OSV should go to before making an exit)
#define Bx 0.5
#define By 1.675
//Point of EXIT from area A
#define EXIT_Ax 1.0
#define EXIT_Ay 0.325
//Point of EXIT from area B
#define EXIT_Bx 1.0
#define EXIT_By 1.675

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
    //moveTowardsPoint(Ax, Ay);
    //moveTowardsPoint(EXIT_Ax, EXIT_Ay);
    //rf.println("connection established");
    rf.updateLocation();
    rf.println(marker.theta);

    moveTowardsPoint(1.0, 2.0);
    //moveTowardsPoint(1.0, 2.0);
    //face(PI / 2);
    //face(3.1);
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
    reportLocation();
    float startingX= marker.x;
    float startingY= marker.y;
    distanceItShouldTravel= sqrt( pow( startingX - desiredX, 2) + pow( startingY - desiredY, 2));

    double changeInY= (double) desiredY - marker.y;
    double changeInX= (double) desiredX - marker.x;

    //the direction the OSV needs to face
    //returns a range of -pi to +pi
    //TODO double-check
    float directionToFace= atan2(changeInY, changeInX);
    rf.println("OSV needs to face: ");
    rf.print(directionToFace);
    rf.println("");
    face(directionToFace);

    /*
    The below is where the OSV will incremently move until it reaches it's destination.

    The "N" is used, in the unlikely scenario, that the OSV is stuck in an infinite loop
    of going back and forth. In such a case, the OSV reduces it's backward motion by a factor
    of "N", where N starts at 1 and is incremented every time the OSV moves backward and
    does not travel within +/ permissibleErrorForXY of distanceItShouldTravel;.
    */
    int N= 1;
    bool arrivedAtDestination = false;

    while(fabs(distanceTraveled - distanceItShouldTravel) > 0.5){
        move(AVG_SPEED, FORWARD);
        rf.updateLocation();
        distanceTraveled= sqrt( pow( fabs(marker.x - startingX), 2) + pow( fabs(marker.y - startingY), 2));
    }
    stop();

    while(!arrivedAtDestination){

        distanceTraveled= sqrt( pow( fabs(marker.x - startingX), 2) + pow( fabs(marker.y - startingY), 2));

        if(distanceTraveled < distanceItShouldTravel - permissibleErrorForXY)
        {
            //OSV has yet to reach destination
            move(AVG_SPEED, FORWARD);
            delay(DURATION_OF_BURST);
            stop();
        }
        else if(distanceTraveled > distanceItShouldTravel + permissibleErrorForXY)
        {
            //OSV has gone past it's destination
            move(AVG_SPEED, BACKWARD);
            delay((int) DURATION_OF_BURST / N);
            stop();
            N++;
        }
        else
        {
            //OSV reached destination
            arrivedAtDestination= true;
            rf.updateLocation();
            reportLocation();
        }
    }

}


void face(float directionToFace){

    rf.updateLocation();
    reportLocation();

    for(int i= 0; i < 4; i++){
        motor[i]->setSpeed(AVG_SPEED);
    }

    //convert to 0 -> 2pi system for calculations (Coordinate Transmissions uses -pi -> +pi system)
    //TODO fix math here (positiveDesiredTheta is incorrect)
    float positiveDesiredTheta= directionToFace;
    float positiveCurrentTheta= marker.theta;
    if(directionToFace < 0){ positiveDesiredTheta+= 2 * PI; }
    if(marker.theta < 0){ positiveCurrentTheta+= 2 * PI; }

    //rf.updateLocation();
    reportLocation();

    //check if OSV's current theta is within +/- permissibleErrorForTheta of the desired theta
    while( fabs(positiveCurrentTheta - positiveDesiredTheta) > permissibleErrorForTheta){

        rf.println("OSV isn't close enough to ");
        rf.print(directionToFace);

        if(rotate_CCW_or_CW == CLOCKWISE){
            moveClockwise();
        }else{
            moveCounterClockwise();
        }

        rf.updateLocation();

        positiveCurrentTheta= marker.theta;
        if(marker.theta < 0){
            positiveCurrentTheta+= 2 * PI;
        }

        rf.println("OSV's Theta (in 0->2pi system):  ");
        rf.println(marker.theta);
        rf.println("OSV is ");
        rf.print(fabs(positiveCurrentTheta - positiveDesiredTheta));
        rf.print(" away from directionToFace");
    }

    rf.updateLocation();
    reportLocation();

}

void moveClockwise() {
  motor[0]->run(FORWARD);
  motor[1]->run(FORWARD);
  motor[2]->run(BACKWARD);
  motor[3]->run(BACKWARD);

  delay(DURATION_OF_BURST);

  for(int i=0; i< 4; i++){
      motor[i]->run(RELEASE);
      rf.println("motors stopped");
  }
}

void moveCounterClockwise() {
  motor[0]->run(BACKWARD);
  motor[1]->run(BACKWARD);
  motor[2]->run(FORWARD);
  motor[3]->run(FORWARD);

  delay(DURATION_OF_BURST);

 for(int i=0; i< 4; i++){
     motor[i]->run(RELEASE);
 }
}
//Turn COUNTERCLOCKWISE or CLOCKWISE? That's the decision being made below.
//returns CLOCKWISE or COUNTERCLOCKWISE
int rotate_CCW_or_CW(float directionToFace){

    //convert to 0 -> 2pi system for calculations
    int positiveDesiredTheta= directionToFace;
    int positiveCurrentTheta= marker.theta;
    if(directionToFace < 0){ positiveDesiredTheta+= 2 * PI; }
    if(marker.theta < 0){ positiveCurrentTheta+= 2 * PI; }

    if(marker.theta < positiveDesiredTheta){
        if(fabs(marker.theta - directionToFace) < PI){
            rf.println("OSV will turn COUNTERCLOCKWISE");
            return COUNTERCLOCKWISE;
        }
    }

    rf.println("OSV will turn CLOCKWISE");
    return CLOCKWISE;
}


void move(int speed, int movement){

    for(int i= 0; i < 4; i++){
        motor[i]->setSpeed(speed);
    }

    for(int i= 0; i < 4; i++){
        motor[i]->run(movement);
    }

    rf.println("OSV has started to move @ ");
    rf.print(speed);
    rf.print(" (0-255 PWM)");
}

void stop(){
    for(int i= 0; i < 4; i++){
        motor[i]->run(RELEASE);
    }
    rf.println("OSV has stopped moving.");
}

void reportLocation(){
    /*rf.println("OSV is @ ");
    rf.print("(");
    rf.print(marker.x);
    rf.print(", ");
    rf.print(marker.y);
    rf.print(")");
    rf.println("It's facing ");
    rf.print(marker.theta);
    rf.print(" radians.");
*/}

///TRAVEL TIME ALGORITHM
int expectedArrivalTime(float x, float y){

}
