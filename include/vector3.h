#ifndef AHRAY_VECTOR3_H
#define AHRAY_VECTOR3_H

#include <cmath>
#include <iostream>
#include <random>

class Vector3{
public:
  float x = 0.0f,y = 0.0f,z = 0.0f;

  // Vector Constructor
  Vector3(){
      this->x = 0.0f;
      this->y = 0.0f;
      this->z = 0.0f;
  }
  Vector3(float x, float y, float z){
    this->x = x;
    this->y = y;
    this->z = z;
  }

  // Vector Utility Functions
  float length() const{
    return std::sqrt(x*x + y*y + z*z);
  }
  float lengthSqrd() const{
    return x * x + y * y + z * z;
  }

  inline std::ostream& operator<<(std::ostream& out) const{
    return out << this->x << ' ' << this->y << ' ' << this->z;
  }

  inline Vector3 operator+(const Vector3& v) const{
    return Vector3(this->x + v.x, this->y + v.y, this->z + v.z);
  }

  inline Vector3 operator-(const Vector3& v) const{
    return Vector3(this->x - v.x, this->y - v.y, this->z - v.z);
  }

  inline Vector3 operator*(const Vector3& v) const{
    return Vector3(this->x * v.x, this->y * v.y, this->z * v.z);
  }

  inline Vector3 operator*(float t) const{
    return Vector3(t*this->x, t*this->y, t*this->z);
  }

  inline Vector3 operator/(float t) const{
    return Vector3(this->x,this->y, this->z) * (1/t);
  }

  inline double dot(const Vector3& v) const{
    return this->x * v.x
           + this->y * v.y
           + this->z * v.z;
  }

  inline Vector3 cross(const Vector3& v) {
    return Vector3(this->y * v.z - this->z * v.y,
                   this->z * v.x - this->x * v.z,
                   this->x * v.y - this->y * v.x);
  }

  static inline Vector3 unitVec(const Vector3& v){
    return v/v.length();
  }


  static Vector3 random(float min, float max) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(min, max);
    return Vector3(dis(gen), dis(gen), dis(gen));
  }

  static inline Vector3 unitVecRand() {
    while (true) {
      auto p = Vector3::random(-1.0,1.0);
      if (1e-160 < p.lengthSqrd() && p.lengthSqrd() <= 1)
        return p / std::sqrt(p.lengthSqrd());
    }
  }

  static inline Vector3 newDirectionOnHemisphere(Vector3 normal) {
    Vector3 on_unit_sphere = unitVecRand();
    if (on_unit_sphere.dot(normal) > 0.0) { // In the same hemisphere as the normal
      return on_unit_sphere;
    }else {
      return on_unit_sphere * -1.0f;
    }
  }
};
#endif
