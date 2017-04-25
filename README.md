# OSV Navigation Code
&copy; 2017 ENES100 (0201) Fire Team Members

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

## Bulletin Board
###### *Last Updated: Friday 04/21/2017*
Message Omkar if something is unclear.

#### How's the OSV coming along?
* OSV can communicate with "command"
* OSV can orient itself in any direction
* OSV can move straight to any specified point, regardless of its initial orientation or position
* We know how to use the fire & ultrasonic sensors
* OSV can consistently exit the wall, regardless of wall's orientation
* OSV can drive next to the fire site and detect fires accurately

#### What's next?
* Write code for the OSV to move to the fire site
    * i.e. have the OSV do everything in one run
* Write code for the OSV to extinguish fires (bonus objective)

#### Upcoming
1. Write code for the OSV to extinguish fires (bonus objective)
2. Write Travel Time algorithm
    * so the OSV can exit the wall as fast as possible
