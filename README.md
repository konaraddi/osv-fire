# OSV Navigation Code


The `OSV_Nav.ino` contains the code for the OSV. Add the following necessary
libraries before uploading to OSV:
1. enes100 (can be found on the [ELMS](https://myelms.umd.edu/courses/1223708/files/folder/Arduino%20Files))
2. Adafruit_MotorShield (check Sketch>Include Library>Recommended Libraries w/in
    the Arduino IDE)
3. Wire (check Sketch>Include Library>Recommended Libraries w/in
    the Arduino IDE)

## Bulletin Board
###### *Last Updated: 04/04/2017*

#### What we have so far
* We know how to use RF communication
* We know how to use the Adafruit_MotorShield library
    * i.e. We can control the motors
* Wrote code for updating OSV's current location and communication
    * Tested with tanks
* Wrote code for various calculations (e.g. should the OSV turn CW or CCW?)
    * More to come

#### Currently Doing....
* Sean will write code for M5 by Thursday 04/06/2017
* Finish implementing short bursts for movement and turing by Thursday 04/06/2017
* Finish writing code for exiting the wall (w/out distance sensor for now) by Thursday 04/06/2017

#### Upcoming
1. Optimize movement and turning.
    * OSV will move continuously for 75% of the distance it expects to travel before moving in short bursts and checking coordinates intermittently. This is in contrast to always moving and turning in short bursts (the current approach).
2. Write Travel Time algorithm
    * Will require testing to figure out how long it takes the OSV to move and turn

## FAQs

#### 1. Why hasn't Omkar made any recent commits?
I may be learning how to implement some functionality (e.g. learning C/C++).
My practice files are not pushed to this repository. <b>This repository is
solely for code that should run on the OSV.</b>

#### 2. Why is there so little code? Is this all Omkar has done?
Please see the answer to [Question #1](https://github.com/konaraddio/OSV_Nav#1-why-hasnt-omkar-made-any-recent-commits).
And most of coding is thinking.

#### 3. How do I run this code?
Download this using the green "Clone or download" button. Add the necessary
libraries to your /Arduino/library/ folder. Upload to OSV.

#### 4. Hmmm, something's not working. What should I do?
Message me (Omkar).
