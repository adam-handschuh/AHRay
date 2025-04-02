//
// Created by Adam on 02/04/2025.
//

#ifndef AHRAY_POINTLIGHT_H
#define AHRAY_POINTLIGHT_H

#include <vector3.h>
class PointLight{
public:
  Vector3 position;
  Vector3 colour;
  Vector3 intensity;

  PointLight(Vector3 position, Vector3 colour, Vector3 intensity){
    this->position = position;
    this->colour = colour;
    this->intensity = intensity;
  }
};
#endif // AHRAY_POINTLIGHT_H
