/* Copyright © 2017 Elan Hamburger, Karam Singh, Bryan Zeug
 *
 * This software created for the exclusive use of the University of Maryland
 * at College Park. Any other use of this software requires the explicit
 * permission of the creators.
 */

#ifndef RF_COMM_H
#define RF_COMM_H

#include "marker.h"
#include <Arduino.h>
#include <SoftwareSerial.h>

class RF_Comm {
  private:
    SoftwareSerial *sSerial;
    Marker *marker;

  public:
    RF_Comm(SoftwareSerial *s, Marker *m) {
      this->sSerial = s;
      this->sSerial->begin(9600);
      this->sSerial->flush();

      this->marker = m;
    }

    void print(const char *msg) {
      // Flush the buffer
      this->sSerial->flush();
      // Transmit message
      this->sSerial->print(msg);
      // Flush the buffer
      this->sSerial->flush();
    }

    void print(int num) {
      // Flush the buffer
      this->sSerial->flush();
      // Transmit message
      this->sSerial->print(num);
      // Flush the buffer
      this->sSerial->flush();
    }

    void print(double num) {
      // Flush the buffer
      this->sSerial->flush();
      // Transmit message
      this->sSerial->print(num);
      // Flush the buffer
      this->sSerial->flush();
    }

    void println(const char *msg) {
      // Flush the buffer
      this->sSerial->flush();
      // Transmit message
      this->sSerial->print(msg);
      this->sSerial->print('\n');
      // Flush the buffer
      this->sSerial->flush();
    }

    void println(int num) {
      // Flush the buffer
      this->sSerial->flush();
      // Transmit message
      this->sSerial->print(num);
      this->sSerial->print('\n');
      // Flush the buffer
      this->sSerial->flush();
    }

    void println(double num) {
      // Flush the buffer
      this->sSerial->flush();
      // Transmit message
      this->sSerial->print(num);
      this->sSerial->print('\n');
      // Flush the buffer
      this->sSerial->flush();
    }

    void startMission() {
      // Flush the buffer
      this->sSerial->flush();
      // Transmit command
      this->sSerial->print("#startTimer*");
      // Flush the buffer
      this->sSerial->flush();
    }

    void splitMission() {
      // Flush the buffer
      this->sSerial->flush();
      // Transmit command
      this->sSerial->print("#lapTimer*");
      // Flush the buffer
      this->sSerial->flush();
    }

    void stopMission() {
      // Flush the buffer
      this->sSerial->flush();
      // Transmit command
      this->sSerial->print("#stopTimer*");
      // Flush the buffer
      this->sSerial->flush();
    }

    int updateLocation() {
      // Send request to Vision System
      this->sSerial->print('#');
      this->sSerial->print(this->marker->num);
      this->sSerial->print('*');
      // Flush the buffer
      this->sSerial->flush();
      // Begin tracking time
      unsigned long start = millis();
      int state = 0;
      while((millis() - start) < 600) {
        if(this->sSerial->available()) {
          switch(state) {
            // Marker ID
            case 0:
              if(this->sSerial->parseInt() == this->marker->num)
                state++;
              break;
            // X Coordinate
            case 1:
              this->marker->x = this->sSerial->parseFloat();
              state++;
              break;
            // Y Coordinate
            case 2:
              this->marker->y = this->sSerial->parseFloat();
              state++;
              break;
            // Theta
            case 3:
              this->marker->theta = this->sSerial->parseFloat();
              return 1;
              break;
            default:
              return 0;
              break;
          }
        }

      }

      return 0;
    }
};

#endif
