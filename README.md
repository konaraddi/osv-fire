# OSV Navigation Code
&copy; 2017 ENES100 (0201) Fire Team Members

The `OSV_Nav.ino` contains the code for the OSV. Add the following necessary libraries before uploading to OSV:
1. enes100 (can be found on the [ELMS](https://myelms.umd.edu/courses/1223708/files/folder/Arduino%20Files))
2. Adafruit_MotorShield (check Sketch>Include Library>Recommended Libraries w/in
    the Arduino IDE)
3. Wire (check Sketch>Include Library>Recommended Libraries w/in
    the Arduino IDE)
4. [NewPing](https://bitbucket.org/teckel12/arduino-new-ping/downloads/)
    * For the ultrasonic range sensor

## Bulletin Board
###### *Last Updated: Thurs 04/20/2017*

#### How's the OSV coming along?
* OSV can communicate with "command"
* OSV can orient itself in any direction
* OSV can move straight to any specified point, regardless of its initial orientation or position
* We know how to use the fire & ultrasonic sensors
* OSV can exit the wall (if we're not using K-turns)
    * however, it digs in the sand when trying to turn

#### What's next?
* Switch OSV's turning mechanism to K-turns so it does not dig in the sand
    * K-turns need to be made shorter and faster

#### Upcoming
1. Implement fire detection and extinguishment in code
    * Test that OSV can complete it's objective
2. Write Travel Time algorithm
3. Rewrite code for turning and moving so it's smooth
    * Right now we use short bursts in linear movement and turning. In between these bursts,
    we check the OSV's coordinates so the OSV can decide whether or not to keep moving forward.
    However, this is slow and is for testing purposes. So later I'll be writing code such that
    the OSV only uses bursts when it's coming close to desired position/orientation.
