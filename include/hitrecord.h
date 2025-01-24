#ifndef AHRAY_HITRECORD_H
#define AHRAY_HITRECORD_H

#include <ray.h>
#include <vector3.h>

class HitRecord{
public:
  Vector3 position;
  Vector3 normal;
  float t;

  bool front_face;

  void set_face_normal(Ray& r, Vector3 outward_normal) {
    front_face = r.direction.dot(outward_normal) < 0;
    normal = front_face ? outward_normal : outward_normal*-1;
  }
};

#endif
