#ifndef AHRAY_HITRECORD_H
#define AHRAY_HITRECORD_H

#include <ray.h>
#include <vector3.h>
#include <material.h>

class HitRecord{
public:
  Vector3 position;
  Vector3 normal;
  float t;
  Material* currentMat;


  bool front_face;

  void setFaceNormal(Ray& r, Vector3 outwardsNormal) {
    front_face = r.direction.dot(outwardsNormal) < 0;
    normal = front_face ? outwardsNormal : outwardsNormal*-1;
  }

  void setMaterial(Material &currentMat){
    this->currentMat = &currentMat;
  }
};

#endif
