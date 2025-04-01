#ifndef AHRAY_SPHERE_H
#define AHRAY_SPHERE_H

#include <vector3.h>
#include <ray.h>
#include <hitrecord.h>
#include <material.h>

class Sphere{
private:

public:
  Vector3 pos;
  float radius;
  Material* material;


  Sphere(Vector3 pos, float radius, Material &material){
    this->pos = pos;
    this->radius = radius;
    this->material = &material;
  }

  bool hit(Ray &ray, double min, double max, HitRecord &hitRec) {
    Vector3 oc = pos - ray.origin;
    //Improvements:
    // 1. length squared is equal to a vector dotted with itself so x.dot(x) is replaced with x.lengthSqrd()
    auto a = ray.direction.lengthSqrd();
    auto b = ray.direction.dot(oc);
    auto c = oc.lengthSqrd() - radius*radius;
    auto discriminant = b*b - a*c;

    if (discriminant < 0) {
      return false;
    }


    auto sqrtd = std::sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range.
    auto root = (b - sqrtd) / a;
    if (root <= min || max <= root) {
      root = (b + sqrtd) / a;
      if (root <= min || max <= root) {
        return false;
      }
    }

    hitRec.t = root;
    hitRec.position = ray.at(hitRec.t);
    Vector3 outward_normal = (hitRec.position - this->pos) / this->radius;
    hitRec.setFaceNormal(ray, outward_normal);
    hitRec.setMaterial(*material);
    return true;
  }

};
#endif
