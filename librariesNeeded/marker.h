#ifndef MARKER_H
#define MARKER_H

class Marker{
  public:
    float x;
    float y;
    float theta;
    int num;

  Marker(int number){
    this->num = number;
  }
};

#endif
