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
###### *Last Updated: Friday 04/21/2017*
Message Omkar if something is unclear.

#### How's the OSV coming along?
* OSV can communicate with "command"
* OSV can orient itself in any direction
* OSV can move straight to any specified point, regardless of its initial orientation or position
* We know how to use the fire & ultrasonic sensors
* OSV can exit the wall <b>using K-turns</b> sometimes
    * "sometimes" b/c programmatically offsetting for the QR code's position works sometimes
    * we technically can offset the QR code in real time (as opposed to having an explicit offset), but it might be easier to physically put
    the marker in the center

#### What's next?
* Offset the coordinates the OSV should go to in real time programmatically
    * or move the marker physically to the center (e.g. using a popsicle stick)

#### Upcoming
1. Implement fire detection and extinguishment in code
    * Test that OSV can complete it's objective
2. Write Travel Time algorithm
    * so the OSV can exit the wall as fast as possible
3. Make linear movements smooth (i.e. no bursts)
