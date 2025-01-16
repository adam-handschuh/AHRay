#ifndef AHRAY_VECTOR3_H
#define AHRAY_VECTOR3_H

#include <cmath>
#include <iostream>

class Vector3{
public:
  float x,y,z = 0.0f;

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
  float length(){
    return std::sqrt(x*x + y*y + z*z);
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

  inline Vector3 cross(const Vector3& u, const Vector3& v) {
    return Vector3(this->y * v.z - this->z * v.y,
                   this->z * v.x - this->x * v.z,
                   this->x * v.y - this->y * v.x);
  }
};
#endif
