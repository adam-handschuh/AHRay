#ifndef AHRAY_VIEWPORT_H
#define AHRAY_VIEWPORT_H

#include <vector3.h>

class Viewport{
public:
  float width, height;
  int imageWidth, imageHeight;
  Vector3 u,v,w;
  Vector3 pixelDeltaU, pixelDeltaV;

  Viewport(int imageWidth, int imageHeight, float fov){
    this->imageWidth = imageWidth;
    this->imageHeight = imageHeight;

    height = 2.0f * tan((fov * 0.5f) * (M_PI / 180.0f));
    width = height * (float(imageWidth)/float(imageHeight));
  }

  void calculate(Vector3 eye, Vector3 lookAt, Vector3 up){
    // Calculate orthonormal basis
    this->w = Vector3::unitVec(eye - lookAt);    // Forward (negative z-axis)
    Vector3 u = Vector3::unitVec(up.cross(this->w)); // Right (x-axis)
    Vector3 v = this->w.cross(u); // True Up (y-axis)

    this->u = u * this->width;
    this->v = (v * -1) * this->height;

    this->pixelDeltaU = this->u/float(imageWidth);
    this->pixelDeltaV = this->v/float(imageHeight);
  }

};

#endif
