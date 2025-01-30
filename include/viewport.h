#ifndef AHRAY_VIEWPORT_H
#define AHRAY_VIEWPORT_H

#include <vector3.h>

class Viewport{
public:
  float width, height;
  int imageWidth, imageHeight;
  Vector3 u,v;
  Vector3 pixelDeltaU, pixelDeltaV;

  Viewport(int imageWidth, int imageHeight, float fov){
    this->imageWidth = imageWidth;
    this->imageHeight = imageHeight;

    height = 2.0f * tan((fov * 0.5f) * (M_PI / 180.0f));
    width = height * (float(imageWidth)/float(imageHeight));

    this->u = Vector3(width,0,0);
    this->v = Vector3(0, -height, 0);

    this->pixelDeltaU = u/float(imageWidth);
    this->pixelDeltaV = v/float(imageHeight);
  }

};

#endif
