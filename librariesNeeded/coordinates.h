/*
If this doesn't work, go back to the Arduino tutorial on libraries and create a header file to see if that brings any changes.
Also, learn why the header file is used sometimes and sometimes not.
*/

#ifndef COORDINATES_H
#define COORDINATES_H

#include "marker.h"
#include "rf_comm.h"
#include <Arduino.h>
#include <SoftwareSerial.h>

class Coordinate{
public:
    float x;
    float y;
    float theta;

    Coordinate(float x, float y, float theta){
        this->x = x;
        this->y = y;
        this->theta = theta;
    }

    void updateCurrentCoordinate(){
        if(rf.updateLocation()){
            this->x = marker.x;
            this->y = marker.y;
            this->theta = marker.theta;
        }
    }

    float distanceBetweenCurrentAnd(Coordinate otherCoordinate){

    }

private:
    SoftwareSerial mySerial(8, 9);//these numbers need to be changed. but to what?
    Marker marker(3);//this number will need to be changed
    RF_Comm rf(&mySerial, &marker);

};

#endif
