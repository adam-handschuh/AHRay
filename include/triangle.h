//
// Created by Adam on 01/04/2025.
//

#ifndef AHRAY_TRIANGLE_H
#define AHRAY_TRIANGLE_H
#include <vector3.h>
#include <ray.h>
#include <hitrecord.h>
#include <material.h>

class Triangle {
public:
  Vector3 v0, v1, v2;
  Material* material;


  Triangle(const Vector3& a, const Vector3& b, const Vector3& c, Material &material){
    v0 = a;
    v1 = b;
    v2 = c;
    this->material = &material;
  }

  virtual bool hit(Ray& r, float t_min, float t_max, HitRecord& rec){
    const float EPSILON = 1e-8;
    Vector3 edge1 = v1 - v0;
    Vector3 edge2 = v2 - v0;

    // Begin calculating determinant - also used to calculate u parameter.
    Vector3 h = r.direction.cross(edge2);
    float a = edge1.dot(h);

    // If the determinant is near zero, ray lies in the plane of the triangle.
    if (a > -EPSILON && a < EPSILON)
      return false;  // Parallel to triangle

    float f = 1.0f / a;
    Vector3 s = r.origin - v0;
    float u = f * s.dot(h);
    if (u < 0.0f || u > 1.0f)
      return false;

    Vector3 q = s.cross(edge1);
    float v = f * r.direction.dot(q);
    if (v < 0.0f || u + v > 1.0f)
      return false;

    // At this stage, we can compute t to find out where the intersection point is on the line.
    float t = f * edge2.dot(q);
    if (t < t_min || t > t_max)
      return false;

    // Record the intersection details.
    rec.t = t;
    rec.position = r.at(t);
    // Compute the face normal (ensuring it is normalised)
    Vector3 normal = Vector3::unitVec(edge1.cross(edge2));
    if(normal.dot(r.direction) > 0)
      normal = normal*-1;
    rec.setFaceNormal(r, normal);
    rec.setMaterial(*material);
    return true;
  }
};

#endif // AHRAY_TRIANGLE_H
