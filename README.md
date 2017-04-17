# OSV Navigation Code


The `OSV_Nav.ino` contains the code for the OSV. Add the following necessary libraries before uploading to OSV:
1. enes100 (can be found on the [ELMS](https://myelms.umd.edu/courses/1223708/files/folder/Arduino%20Files))
2. Adafruit_MotorShield (check Sketch>Include Library>Recommended Libraries w/in
    the Arduino IDE)
3. Wire (check Sketch>Include Library>Recommended Libraries w/in
    the Arduino IDE)
4. [NewPing](https://bitbucket.org/teckel12/arduino-new-ping/downloads/)
    * For the ultrasonic range sensor

## Bulletin Board
###### *Last Updated:  Mon 04/17/2017*

#### What we have so far
* OSV can move and turn
* OSV can communicate with command
* The OSV can turn towards any specified direction, regardless of its initial direction
* Code for OSV move from any to any point
    * It moves toward the point but it doesn't stop at the point
* We know how to use the ultrasonic range sensor
    * Implemented into code, requires testing
* OSV can detect fires and extinguish fires
    * needs to be implemented into the main code

#### What's happening now
* Debug/fix code for OSV moving from a given point X to a given point Y
    * OSV doesn't stop when it reaches point Y but heads in the right direction
* ~~Implement distance sensor into navigation (so the OSV can see if it's in front of the wall)~~ [COMPLETED]
    * Requires testing

#### Upcoming
1. Implement fire detection and extinguishment in code
2. Write Travel Time algorithm
    * Will require testing to figure out how long it takes for the OSV to move and turn
3. Rewrite code for turning and moving so it's smooth
    * Right now we use short bursts in linear movement and turning. In between these bursts,
    we check the OSV's coordinates so the OSV can decide whether or not to keep moving forward.
    However, this is slow and is for testing purposes. So later I'll be writing code such that
    the OSV only uses bursts when it's coming close to desired position/orientation.
