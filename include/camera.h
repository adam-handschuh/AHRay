#ifndef AHRAY_CAMERA_H
#define AHRAY_CAMERA_H
#include <ray.h>
#include <vector3.h>
#include <fragment.h>
#include <viewport.h>
#include <vector>
#include <scene.h>

class Camera{
public:
  Vector3 eye, lookAt, up;
  float aspectRatio;
  float fov = 50.0f;
  float depth = 50.0f;
  float focalLength = 1.0f;
  Viewport* viewport;


  explicit Camera(Viewport* viewport){
    this->viewport = viewport;
    this->aspectRatio = viewport->width/viewport->height;
  }

  void initialise(Vector3 eye, Vector3 lookAt, Vector3 up){
    this->eye = eye;
    this->lookAt = lookAt;
    this->up = up;
  }

  void render(std::vector<Fragment> &frags, Scene &scene){
    Ray ray(eye);
    //Location of the first pixel (top-left corner)
    Vector3 startPixel = (eye - Vector3(0,0,focalLength) - (viewport->u/2) - (viewport->v/2))
                         + ((viewport->pixelDeltaU + viewport->pixelDeltaV)/2);

    for(int h = 0; h < viewport->imageHeight; h ++){
      for(int w = 0; w < viewport->imageWidth; w ++){
        Vector3 pixelCenter = startPixel + (viewport->pixelDeltaU * w) + (viewport->pixelDeltaV*h);
        ray.setDirection(pixelCenter);
        Fragment frag;

        Vector3 colour = ray.getColour(&scene);
        frag.setFinalColour(colour.x,colour.y,colour.z);

        frags.push_back(frag);
      }
    }
  }

};
#endif
