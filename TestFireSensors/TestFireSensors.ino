#include <math.h>
#include "enes100.h"
#include <Wire.h>
SoftwareSerial mySerial(2, 3);
Marker marker(11); //look at QR code's back for number
RF_Comm rf(&mySerial, &marker);

const int fs1 = A0;// FS 1 - Quadrant A Pin A0
const int fs2 = A1;// FS 2 - Quadrant B Pin A1
const int fs3 = A2;// FS 3 - Quadrant C Pin A2
const int fs4 = A3;// FS 4 - Quadrant D Pin A3

bool quadrantA= false;
bool quadrantB= false;
bool quadrantC= false;
bool quadrantD= false;

int trials= 0;

void setup()
{
    rf.transmitData(START_MISSION, NO_DATA);
}

void loop()
{
    trials++;
    rf.println("---------------------------");
    rf.print("TRIAL: ");
    rf.println(trials);
    rf.println("---------------------------");
    int afs1 = analogRead(fs1); //read the value quadrant A
    int afs2 = analogRead(fs2); //read the value quadrant B
    int afs3 = analogRead(fs3); //read the value quadrant C
    int afs4 = analogRead(fs4); //read the value quadrant D
    //Set quadrant state
    if(afs1 <= 975) {
        quadrantA=true;
    }
    if(afs2 <= 975) {
        quadrantB=true;
    }
    if(afs3 <= 975) {
        quadrantC=true;
    }
    if(afs4 <= 975) {
        quadrantD=true;
    }

    //Transmit State to base
    if(quadrantA == true){
        rf.println("QUADRANT A IS ON FIRE");
        quadrantA= false;
    }
    if(quadrantB == true){
        rf.println("QUADRANT B IS ON FIRE");
        quadrantB= false;
    }
    if(quadrantC == true){
        rf.println("QUADRANT C IS ON FIRE");
        quadrantC= false;
    }
    if(quadrantD == true){
        rf.println("QUADRANT D IS ON FIRE");
        quadrantD= false;
    }

    delay(500); // used to slow down readings while calibrating
}
