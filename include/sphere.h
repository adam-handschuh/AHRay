
#ifndef AHRAY_SPHERE_H
#define AHRAY_SPHERE_H

#include <vector3.h>
#include <ray.h>

class Sphere{
public:
  Vector3 pos;
  float radius;

  Sphere(Vector3 pos, float radius){
    this->pos = pos;
    this->radius = radius;
  }

};
#endif
