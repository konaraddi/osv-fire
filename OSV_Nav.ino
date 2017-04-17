//Copyright 2017 ENES100 (0201) Fire Team Members

#include <math.h>
#include "enes100.h"
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"
#include <NewPing.h>//for the Ultrasonic distance sensor

SoftwareSerial mySerial(2, 3);
Marker marker(11); //look at QR code's back for number
RF_Comm rf(&mySerial, &marker);

Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *motor[4];

#define CLOCKWISE 0
#define COUNTERCLOCKWISE 1

float permissibleErrorForTheta= 0.075;//Coordinate Transmissions are accurate to +/- 0.050 radians
float permissibleErrorForXY= 0.075; //Coordinate Transmissions are accurate to +/- 0.050 meters

//dictates the speed of the OSV's general movement
#define MAX_SPEED 255 //Just for kicks
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

//DISTANCE SENSOR SETUP BELOW
    #define TRIG_PIN 11 //wherever the trig pin is put in
    #define ECHO_PIN 2 //wherever the echo pin is put in
    #define MAX_DISTANCE 100 //in centimeters
    NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE);

//FIRE SENSORS (FROM HIGHEST TO LOWEST)
    const int FIRE_SENSOR_1= A0;
    const int FIRE_SENSOR_2= A1;
    const int FIRE_SENSOR_3= A2;
    const int FIRE_SENSOR_4= A3;

/*

Fire Base's center @ (2.9, 1.3)
4 corners of the Fire Base:
    Bottom Left @ (2.75, 1.15)
    Top Left @ (2.75, 1.45)
    Top Right @ (3.05, 1.45)
    Bottom Right @ (3.05, 1.15)
The above coordinates aren't what we actually use since we need to account for the dimensions of the OSV
while going around the Fire Base.

*/

void setup(){
    rf.transmitData(START_MISSION, NO_DATA);
    rf.transmitData(NAV, FIRE);
    rf.updateLocation();
    rf.println("CONNECTION ESTALISHED");
    reportLocation();

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
    /*
    moveTowardsPoint(Ax, Ay);
    face(0);
    delay(1000);
    int distanceAhead= (int) sonar.ping_cm();//casting unsigned long to an int
    if(sonar.ping_cm() > 5 && sonar.ping_cm() < 60){
        moveTowardsPoint(Bx, By);
        moveTowardsPoint(EXIT_Bx, EXIT_By);
    }else{
        moveTowardsPoint(EXIT_Ax, EXIT_Ay);
    }
    */

    //TRAVEL TOWARDS FIRE SITE
    /*
    move(AVG_SPEED, FORWARD);
    delay(1000);
    stop();
    moveTowardsPoint(2.0, 1.0);
    face(0);//OSV should be in center of arena, facing East
    */
    //FIRE SITE ROUND 1

    //FIRE SITE ROUND 2

    /*
    The statement below will run forever so this current loop() will never
    finish running. The purpose of this is for the loop() to never repeat.
    */
}


//Use this method for the OSV to move from its current point to any other point
void moveTowardsPoint(float desiredX, float desiredY){
    rf.println("reached move towards point");
    double distanceItShouldTravel;//the distance we expect the OSV to travel
    double distanceTraveled;//the actual distance the OSV travels

    rf.updateLocation();
    reportLocation();

    float initialX= marker.x;
    float initialY= marker.y;

    //distance between the initial coords and the desired coords
    distanceItShouldTravel= sqrt( pow( initialX - desiredX, 2) + pow( initialY - desiredY, 2));

    double changeInY= desiredY - marker.y;
    double changeInX= desiredX - marker.x;

    //the direction the OSV needs to face
    //atan2(~) returns a range of -pi to +pi
    //since atan2 returns a double, the double is casted to a float for later use
    //by casting a double as a float, there may be some loss in precision
    float directionToFace= (float) atan2(changeInY, changeInX);

    rf.println("OSV needs to face: ");
    rf.print(directionToFace);
    rf.println("");
    face(directionToFace);

    /*
    The below is where the OSV will incremently move until it reaches it's destination.

    The "N" is used, in the unlikely scenario, that the OSV is stuck in an infinite loop
    of going back and forth. In such a case, the OSV reduces it's backward motion by a factor
    of "N", where N starts at 1 and is incremented every time the OSV moves backward and
    does not travel within +/- permissibleErrorForXY of distanceItShouldTravel;.
    */
    int N= 1;
    bool arrivedAtDestination = false;

    //while the OSV hasn't arrived at its destination
    while(!arrivedAtDestination){
        rf.updateLocation();

        distanceTraveled= sqrt( pow( marker.x - initialX, 2) + pow( marker.y - initialY, 2));

        if(distanceTraveled < distanceItShouldTravel - permissibleErrorForXY)
        {
            //OSV has yet to reach destination so OSV should move forward a little bit
            move(AVG_SPEED, FORWARD);
            delay(DURATION_OF_BURST * 2);//x2 to make it go faster
            stop();
        }
        else if(distanceTraveled > distanceItShouldTravel + permissibleErrorForXY)
        {
            //OSV has gone past it's destination so OSV should move a backward a little bit
            move(AVG_SPEED, BACKWARD);
            delay((int) DURATION_OF_BURST / N);
            stop();
            N++;//this makes sure the OSV doesn't end up in an infinite loop of going backward and forward
        }
        else
        {
            //OSV reached destination
            arrivedAtDestination= true;
            rf.updateLocation();
            reportLocation();
            rf.println("OSV reached destination <-------------------");
        }

        reportLocation();
    }

}

//The method below allows the OSV to face any direction, regardless of its current orientation
void face(float directionToFace){

    rf.updateLocation();
    reportLocation();

    for(int i= 0; i < 4; i++){
        motor[i]->setSpeed(AVG_SPEED);
    }

    float positive2PI_DesiredTheta= directionToFace;
    float positive2PI_CurrentTheta= marker.theta;

    if(directionToFace == 0){

        if(marker.theta < 0){
            positive2PI_DesiredTheta= 2 * PI;
            positive2PI_CurrentTheta= 2 * PI;
        }

        if(marker.theta >= 0){
            positive2PI_DesiredTheta= 0;
        }

    }else{

        if(directionToFace < 0){
            positive2PI_DesiredTheta+= 2 * PI;
        }

        if(marker.theta < 0){
            positive2PI_CurrentTheta+= 2 * PI;
        }

    }


    //check if OSV's current theta is within +/- permissibleErrorForTheta of the desired theta
    while( fabs(positive2PI_CurrentTheta - positive2PI_DesiredTheta) > permissibleErrorForTheta){

        rf.print("OSV isn't close enough to ");
        rf.println(directionToFace);

        if(rotate_CCW_or_CW(directionToFace) == CLOCKWISE){
            moveClockwise();
        }else{
            moveCounterClockwise();
        }

        rf.updateLocation();

        positive2PI_CurrentTheta= marker.theta;
        if(marker.theta < 0){
            positive2PI_CurrentTheta+= 2 * PI;
        }

        rf.print("OSV's Theta (in 0->2pi system):  ");
        rf.println(marker.theta);
        rf.println("OSV is ");
        rf.print(fabs(positive2PI_CurrentTheta - positive2PI_DesiredTheta));
        rf.print(" away from directionToFace");
    }

    rf.updateLocation();
    reportLocation();

}

void moveClockwise() {

    for(int i= 0; i < 4; i++){
    //    motor[i]->setSpeed(MAX_SPEED);
    }

    motor[0]->run(FORWARD);
    motor[1]->run(FORWARD);
    motor[2]->run(BACKWARD);
    motor[3]->run(BACKWARD);

    delay(DURATION_OF_BURST);

    stop();

    for(int i= 0; i < 4; i++){
    //    motor[i]->setSpeed(AVG_SPEED);
    }
}

void moveCounterClockwise() {

    for(int i= 0; i < 4; i++){
    //    motor[i]->setSpeed(MAX_SPEED);
    }

    motor[0]->run(BACKWARD);
    motor[1]->run(BACKWARD);
    motor[2]->run(FORWARD);
    motor[3]->run(FORWARD);

    delay(DURATION_OF_BURST);

    stop();

    for(int i= 0; i < 4; i++){
    //    motor[i]->setSpeed(AVG_SPEED);
    }
}

//Turn COUNTERCLOCKWISE or CLOCKWISE? That's the decision being made below.
//returns CLOCKWISE or COUNTERCLOCKWISE
int rotate_CCW_or_CW(float directionToFace){

    //convert to 0 -> 2pi system for calculations
    int positive2PI_DesiredTheta= directionToFace;
    int positive2PI_CurrentTheta= marker.theta;
    if(directionToFace < 0){ positive2PI_DesiredTheta+= 2 * PI; }
    if(marker.theta < 0){ positive2PI_CurrentTheta+= 2 * PI; }

    if(positive2PI_CurrentTheta < positive2PI_DesiredTheta){
        if(fabs(marker.theta - directionToFace) < PI){
            rf.println("OSV will turn COUNTERCLOCKWISE");
            return COUNTERCLOCKWISE;
        }
    }

    rf.println("OSV will turn CLOCKWISE");
    return CLOCKWISE;
}

//OSV will move at a specified speed and direction (FORWARD or BACKWARD)
void move(int speed, int movement){

    for(int i= 0; i < 4; i++){
        motor[i]->setSpeed(speed);
    }

    for(int i= 0; i < 4; i++){
        motor[i]->run(movement);
    }
}


//Stop all wheels
void stop(){
    for(int i= 0; i < 4; i++){
        motor[i]->run(RELEASE);
    }
}

//Report the OSV's location
void reportLocation(){
    rf.println("");
    rf.println("OSV is @ ");
    rf.print("(");
    rf.print(marker.x);
    rf.print(", ");
    rf.print(marker.y);
    rf.print(")");
    rf.println("OSV is facing ");
    rf.print(marker.theta);
    rf.print(" radians.");
    rf.println("");
}

///TRAVEL TIME ALGORITHM
int expectedArrivalTime(float x, float y){

}
