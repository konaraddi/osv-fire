# OSV_Fire

This repository contains the code used by the 2017 Section 0201 Fire Group.
The Fire Group's Over Sand Vehicle (OSV) was capable of navigating a course
with obstacles, detecting fires, detecting the location of fires, extinguishing
fires, and communicating with a computer using RF.

## Requirements
The `OSV_Nav.ino` contains the code for the OSV. Add the following necessary libraries before uploading to OSV:
1. [umdenes100](https://github.com/umdenes100/arduinolibrary)
    * Rename to `enes100`
2. [Adafruit_MotorShield](https://github.com/adafruit/Adafruit_Motor_Shield_V2_Library)
    * If you're using Linux, you may need to comment out the `yield()`
    * Rename to `Adafruit_MotorShield`
3. Wire (check Sketch>Include Library>Recommended Libraries w/in
    the Arduino IDE)
4. [NewPing](https://bitbucket.org/teckel12/arduino-new-ping/downloads/)
    * For the ultrasonic range sensor
