#include <NewPing.h>

#define TRIG_PIN 11
#define ECHO_PIN 2
#define MAX_DISTANCE 100 //in centimeters

NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE);

void setup(){
    Serial.begin(9600);
}

void loop(){
    delay(1000);
    Serial.println("");
    Serial.print("Ping: ");
    Serial.print(sonar.ping_cm());
    Serial.println("cm");
}
