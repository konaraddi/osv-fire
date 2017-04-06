# OSV Navigation Code


The `OSV_Nav.ino` contains the code for the OSV. Add the following necessary
libraries before uploading to OSV:
1. enes100 (can be found on the [ELMS](https://myelms.umd.edu/courses/1223708/files/folder/Arduino%20Files))
2. Adafruit_MotorShield (check Sketch>Include Library>Recommended Libraries w/in
    the Arduino IDE)
3. Wire (check Sketch>Include Library>Recommended Libraries w/in
    the Arduino IDE)

## Bulletin Board
###### *Last Updated: 04/06/2017*

#### What we have so far
* We know how to use RF communication (the updated library is nbd)
* We know how to use the Adafruit_MotorShield library
    * i.e. We can control the motors
* Wrote code for updating OSV's current location and communication
    * Tested with tanks
* Wrote code for various calculations (e.g. should the OSV turn CW or CCW?)
    * More to come
* Wrote code for OSV moving from any point to any point
    * To be tested on Fri 04/07
#### What's happening now
* Test code for having OSV move from any point to any point on Fri 04/07

#### Upcoming
1. Learn to use distance sensor & implement it
2. Write Travel Time algorithm
    * Will require testing to figure out how long it takes the OSV to move and turn
