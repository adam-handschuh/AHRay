#ifndef AHRAY_MATERIAL_H
#define AHRAY_MATERIAL_H

#include <ray.h>
#include <vector3.h>

class Material{
private:
  Vector3 diffuseColour;
  float roughness = 0;
public:

  Material(Vector3 colour, float roughness){
    this->diffuseColour = colour;
    this->roughness = roughness;
  }

  Vector3 getColour(){
    return diffuseColour;
  }

  float getRoughness(){
    return roughness;
  }
};

#endif