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

float permissibleErrorForTheta= 0.1;//Coordinate Transmissions are accurate to +/- 0.050 radians
float permissibleErrorForXY= 0.075; //Coordinate Transmissions are accurate to +/- 0.050 meters

//dictates the speed of the OSV's general movement
#define MAX_SPEED 255 //Just for kicks
#define AVG_SPEED 175 //0-255 PWM
#define LOW_SPEED 170 //0-255 PWM
#define DURATION_OF_BURST 500 //in milliseconds

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
    #define TRIG_PIN 5 //wherever the trig pin is put in
    #define ECHO_PIN 4 //wherever the echo pin is put in
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

    pinMode(8, OUTPUT);
    digitalWrite(8, HIGH);
}

void loop(){

    //EXIT THE WALL
    if(itIsFasterToExitFromA()){//OSV will attempt to exit from A

        rf.println("OSV will move to area A (south area)");

        moveTowardsPoint(Ax - 0.1, Ay - 0.08);
        face(0);
        delay(1000);

        int distanceAhead= (int) sonar.ping_cm();//casting unsigned long to an int
        if(sonar.ping_cm() > 5 && sonar.ping_cm() < 60){
            rf.println("There's a wall, OSV will try to exit from area B now ");
            moveTowardsPoint(Bx - 0.1, By + 0.2);
            moveTowardsPoint(EXIT_Bx, EXIT_By + 0.2);
        }else{
            rf.println("There's no wall here, so OSV will exit");
            moveTowardsPoint(EXIT_Ax, EXIT_Ay - 0.1);
        }

    }else{

        rf.println("OSV will move to area B (north area)");

        moveTowardsPoint(Bx - 0.1, By + 0.2);
        face(0);
        delay(1000);
        int distanceAhead= (int) sonar.ping_cm();//casting unsigned long to an int
        if(sonar.ping_cm() > 5 && sonar.ping_cm() < 60){
            rf.println("There's a wall, OSV will try to exit from area A now");
            moveTowardsPoint(Ax - 0.1, Ay - 0.08);
            moveTowardsPoint(EXIT_Ax, EXIT_Ay - 0.1);
        }else{
            rf.println("There's no wall here, so OSV will exit");
            moveTowardsPoint(EXIT_Bx, EXIT_By + 0.2);
        }

    }


    //TRAVEL TOWARDS FIRE SITE
    move(AVG_SPEED, FORWARD);
    delay(1000);
    stop();

    moveTowardsPoint(2.0, 0.9);
    face(0);//OSV should be in center of arena, facing East

    moveTowardsPoint(3.34, 0.9);//OSV moves towards far corner of the fire site
    face(- PI / 2);//face north

    //FIRE DETECTION
    int delayTime= 150;

    while(marker.y - 1.12 < 0){
        move(AVG_SPEED, BACKWARD);
        delay(delayTime);
        stop();
        rf.updateLocation();
        delay(delayTime);
    }

    delay(500);
    if(fireDetectedBy(FIRE_SENSOR_1) || fireDetectedBy(FIRE_SENSOR_2)){
        rf.transmitData(BASE, FIRE_SITE_A);
    }

    delay(500);
    if(fireDetectedBy(FIRE_SENSOR_3) || fireDetectedBy(FIRE_SENSOR_4)){
        rf.transmitData(BASE, FIRE_SITE_D);
    }

    delay(5000);

    while(marker.y - 1.29 < 0){
        rf.updateLocation();

        //if it's going off course (not moving straight back)
        //TODO
        //May need to decrease the window of error
        if(fabs(marker.theta + PI / 2) < permissibleErrorForTheta){
            face(- PI / 2);
        }

        move(AVG_SPEED, BACKWARD);
        delay(delayTime);
        stop();

    }

    delay(500);
    if(fireDetectedBy(FIRE_SENSOR_1) || fireDetectedBy(FIRE_SENSOR_2)){
        rf.transmitData(BASE, FIRE_SITE_B);
    }

    delay(500);
    if(fireDetectedBy(FIRE_SENSOR_3) || fireDetectedBy(FIRE_SENSOR_4)){
        rf.transmitData(BASE, FIRE_SITE_C);
    }

    /*
    The statement below will run forever so this current loop() will never
    finish running. The purpose of this is for the loop() to never repeat.
    */

    while(1);
}


//Use this method for the OSV to move from its current point to any other point
void moveTowardsPoint(float desiredX, float desiredY){
    double distanceItShouldTravel;//the distance we expect the OSV to travel
    double distanceTraveled;//the actual distance the OSV travels

    rf.updateLocation();

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

        distanceTraveled= sqrt( pow( marker.x - initialX, 2) + pow( marker.y - initialY, 2));

        if(distanceTraveled < distanceItShouldTravel - permissibleErrorForXY)
        {
            rf.println("OSV has YET to reach its destination");
            //OSV has yet to reach destination so OSV should move forward a little bit
            move(AVG_SPEED, FORWARD);
            delay(DURATION_OF_BURST);
            stop();

        }
        else if(distanceTraveled > distanceItShouldTravel + permissibleErrorForXY)
        {
            rf.println("OSV has gone PAST its destination");
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
            rf.println("OSV reached destination <-------------------");
        }

        rf.updateLocation();
        reportLocation();

    }

}

//The method below allows the OSV to face any direction, regardless of its current orientation
void face(float directionToFace){

    float positive2PI_DesiredTheta= directionToFace;
    float positive2PI_CurrentTheta= marker.theta;

    if(directionToFace == 0){

        if(marker.theta < 0){
            positive2PI_DesiredTheta+= 2 * PI;
            positive2PI_CurrentTheta+= 2 * PI;
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

        if(rotate_CCW_or_CW(directionToFace) == CLOCKWISE){
            turnCW();
        }else{
            turnCounterCW();
        }

        rf.updateLocation();

        positive2PI_CurrentTheta= marker.theta;
        if(marker.theta < 0){
            positive2PI_CurrentTheta+= 2 * PI;
        }

    }
}

//Turn CLOCKWISE
void turnCW(){
    rf.println("K- turning CW");
    motor[0]->setSpeed(LOW_SPEED);
    motor[1]->setSpeed(LOW_SPEED);
    motor[2]->setSpeed(MAX_SPEED);
    motor[3]->setSpeed(MAX_SPEED);

    for(int i= 0; i < 4; i++){
        motor[i]->run(BACKWARD);
    }

    delay(DURATION_OF_BURST);
    stop();

    delay(200);
    for(int i= 0; i < 4; i++){
        motor[i]->setSpeed(LOW_SPEED);
    }
    motor[0]->setSpeed(MAX_SPEED);
    motor[1]->setSpeed(MAX_SPEED);

    for(int i=0 ; i < 4; i++){
        motor[i]->run(FORWARD);
    }

    delay(DURATION_OF_BURST);
    stop();

    for(int i= 0; i < 4 ; i ++){
        motor[i]->setSpeed(AVG_SPEED);
    }
}

//turn COUNTERCLOCKWISE
void turnCounterCW(){
    rf.println("K- turning CCW");
    motor[0]->setSpeed(MAX_SPEED);
    motor[1]->setSpeed(MAX_SPEED);
    motor[2]->setSpeed(LOW_SPEED);
    motor[3]->setSpeed(LOW_SPEED);

    for(int i= 0 ; i < 4; i++){
        motor[i]->run(BACKWARD);
    }

    delay(DURATION_OF_BURST);
    stop();

    delay(200);
    for(int i= 0; i < 4; i++){
        motor[i]->setSpeed(LOW_SPEED);
    }
    motor[2]->setSpeed(MAX_SPEED);
    motor[3]->setSpeed(MAX_SPEED);

    for(int i=0 ; i < 4; i++){
        motor[i]->run(FORWARD);
    }

    delay(DURATION_OF_BURST);
    stop();

    for(int i= 0; i < 4 ; i ++){
        motor[i]->setSpeed(AVG_SPEED);
    }
}

//Turn COUNTERCLOCKWISE or CLOCKWISE? That's the decision being made below.
//returns CLOCKWISE or COUNTERCLOCKWISE
int rotate_CCW_or_CW(float directionToFace){
    float tmpDirectionToFace= directionToFace + PI;
    if(tmpDirectionToFace > PI){
        tmpDirectionToFace-= 2 * PI;
    }

    if(tmpDirectionToFace < 0){
        if(tmpDirectionToFace < marker.theta && marker.theta < directionToFace){
            return COUNTERCLOCKWISE;
        }
        return CLOCKWISE;
    }else{
        if(tmpDirectionToFace > marker.theta && marker.theta > directionToFace){
            return CLOCKWISE;
        }

        return COUNTERCLOCKWISE;
    }

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

//fire detection
bool fireDetectedBy(int whichSensor){
    return (analogRead(whichSensor) <= 975);
}

//figures if the OSV should try to exit from A or B first
bool itIsFasterToExitFromA(){
    rf.updateLocation();

    if(marker.theta < 0 && marker.theta > -PI){
        return true;
    }

    return false;
}
