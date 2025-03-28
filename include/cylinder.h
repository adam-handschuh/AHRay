#ifndef AHRAY_CYLINDER_H
#define AHRAY_CYLINDER_H

#include <vector3.h>
#include <ray.h>
#include <hitrecord.h>
#include <material.h>

class Cylinder {
private:
  Vector3 baseCenter;
  double radius,height;
  Material* material;

public:
  Cylinder(Vector3 baseCenter, double radius, double height, Material &material){
    this->baseCenter = baseCenter;
    this->radius = radius;
    this->height = height;
    this->material = &material;
  }

  bool hit(Ray &ray, double min, double max, HitRecord &hitRec){
    bool hitOuter = hitCylinder(ray, min, max, hitRec, radius, true); // Outer wall
    bool hitInner = hitCylinder(ray, min, max, hitRec, radius*0.9, false); // Inner wall
    bool hitBottom = hitDisk(ray, min, max, hitRec, baseCenter.y, radius); // Cup bottom

    return hitOuter || hitInner || hitBottom;
  }

private:
  // Cylinder intersection (for outer & inner walls)
  bool hitCylinder(Ray &ray, double min, double max, HitRecord &hitRec, double radius, bool isOuter) const {
    Vector3 oc = ray.origin - baseCenter;
    double a = ray.direction.x * ray.direction.x + ray.direction.z * ray.direction.z;
    double b = 2.0 * (oc.x * ray.direction.x + oc.z * ray.direction.z);
    double c = oc.x * oc.x + oc.z * oc.z - radius * radius;
    double discriminant = b * b - 4 * a * c;

    if (discriminant < 0) return false; // No hit

    double sqrtD = sqrt(discriminant);
    double root = (-b - sqrtD) / (2.0 * a);
    if (root < min || root > max) {
      root = (-b + sqrtD) / (2.0 * a);
      if (root < min || root > max) return false;
    }

    double yHit = ray.origin.y + root * ray.direction.y;
    if (yHit < baseCenter.y || yHit > baseCenter.y + height) return false; // Check within height

    // Set hit record
    hitRec.t = root;
    hitRec.position = ray.at(hitRec.t);
    hitRec.normal = Vector3::unitVec(Vector3(hitRec.position.x - baseCenter.x, 0, hitRec.position.z - baseCenter.z));

    hitRec.setFaceNormal(ray, isOuter ? hitRec.normal : hitRec.normal*-1);
    hitRec.setMaterial(*material);
    return true;
  }

  // Bottom disk intersection
  bool hitDisk(Ray &ray, double min, double max, HitRecord &hitRec, double y, double radius) const {
    if (abs(ray.direction.y) < 1e-6) return false; // Avoid division by zero

    double t = (y - ray.origin.y) / ray.direction.y;
    if (t < min || t > max) return false;

    Vector3 p = ray.at(t);
    if ((p - baseCenter).lengthSqrd() > radius * radius) return false; // Check within disk

    // Set hit record
    hitRec.t = t;
    hitRec.position = p;
    hitRec.normal = Vector3(0, -1, 0);
    hitRec.setFaceNormal(ray, hitRec.normal);
    hitRec.setMaterial(*material);
    return true;
  }
};
#endif
