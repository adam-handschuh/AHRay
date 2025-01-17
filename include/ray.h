#ifndef AHRAY_RAY_H
#define AHRAY_RAY_H
#include <vector3.h>

class Ray{
private:
  Vector3 origin, direction;
public:
  //Constructor which must initialise origin point and direction vector
  Ray(Vector3 origin){
      this->origin = this->origin;
  }

  void setDirection(Vector3 pixelCenter){
    direction = pixelCenter - origin;
  }

  void getColour(){

  }

  //Represents function P(t) which gives the point at t time of the ray-cast
  Vector3 at(float t){
    return origin + (direction*t);
  }
};

#endif