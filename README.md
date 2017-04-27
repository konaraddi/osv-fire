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
###### *Last Updated: Thurs 04/27/2017*
Message Omkar if something is unclear.

#### How's the OSV coming along?
* OSV can do everything up to the basic mission objectives
    * Unable to consistently backup beside the fire site. It goes off course
    when backing up.

#### What's next?
* Get the OSV to consistently get to the fire site and accurately detect fires
* Write code for the OSV to extinguish fires (bonus objective)

#### Upcoming
* Write code for the OSV to extinguish fires (bonus objective)
