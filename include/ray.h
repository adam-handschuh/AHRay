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

  Vector3 getDirection(){
    return direction;
  }

  Vector3 getColour(){
    //Test
    Vector3 unitDir = Vector3::unitVec(direction);
    auto t = hit_sphere(0.5);     // Value to measure normals

    // If the sphere is hit at all (sphere)
    if (t > 0.0) {
      // Calculates a colour based on the position of the ray at t upon colliding with the sphere - sphere's position
      Vector3 N = Vector3::unitVec(at(t) - Vector3(0,0,-1));
      return Vector3(N.x+1, N.y+1, N.z+1)/2.0f;
    }

    // If the sphere is not hit (background)
    auto a = 0.5*(unitDir.y + 1.0);
    auto b = Vector3(1.0, 1.0, 1.0)*(1.0-a) + Vector3(0.5, 0.7, 1.0)*a;
    return b; // Returns a gradient of blue based on the y direction of the ray
  }

  double hit_sphere(double radius) {
    Vector3 oc = Vector3(0.0,0.0,-1.0) - origin;
    auto a = direction.dot(direction);
    auto b = -2.0 * direction.dot(oc);
    auto c = oc.dot(oc) - radius*radius;
    auto discriminant = b*b - 4*a*c;

    if (discriminant < 0) {
      return -1.0;
    } else {
      return (-b - std::sqrt(discriminant) ) / (2.0*a);
    }
  }


  //Represents function P(t) which gives the point at t time of the ray-cast
  Vector3 at(float t){
    return origin + (direction*t);
  }
};

#endif