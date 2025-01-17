#ifndef AHRAY_VIEWPORT_H
#define AHRAY_VIEWPORT_H

#include <vector3.h>

class Viewport{
public:
  float width, height;
  float imageWidth, imageHeight;
  Vector3 u,v;
  Vector3 pixelDeltaU, pixelDeltaV;


  Viewport(float imageWidth, float imageHeight){
    this->imageWidth = imageWidth;
    this->imageHeight = imageHeight;

    height = 2.0f;
    width = height * (double(imageWidth) / imageHeight);

    this->u = Vector3(width,0,0);
    this->v = Vector3(0, -height, 0);

    this->pixelDeltaU = u/imageWidth;
    this->pixelDeltaV = v/imageHeight;
  }

};

#endif
