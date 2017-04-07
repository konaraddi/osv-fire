# OSV Navigation Code


The `OSV_Nav.ino` contains the code for the OSV. Add the following necessary
libraries before uploading to OSV:
1. enes100 (can be found on the [ELMS](https://myelms.umd.edu/courses/1223708/files/folder/Arduino%20Files))
2. Adafruit_MotorShield (check Sketch>Include Library>Recommended Libraries w/in
    the Arduino IDE)
3. Wire (check Sketch>Include Library>Recommended Libraries w/in
    the Arduino IDE)

## Bulletin Board
###### *Last Updated: Fri 04/07/2017*

#### What we have so far
* OSV can move and turn
* OSV can communicate with command
* The OSV can turn towards any specified direction, regardless of its initial direction [Fri 04/07/2017]

#### What's happening now
* Clean up code
* Have the OSV move from any point to any point
    * code for this needs to be fixed and tested

#### Upcoming
1. Learn to use distance sensor & implement it
2. Write Travel Time algorithm
    * Will require testing to figure out how long it takes the OSV to move and turn
