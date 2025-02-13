#ifndef AHRAY_CAMERA_H
#define AHRAY_CAMERA_H
#include <ray.h>
#include <vector3.h>
#include <fragment.h>
#include <viewport.h>
#include <vector>
#include <scene.h>

class Camera{
private:
  Vector3 eye,lookAt,up;
  const int depth = 6;
  float focalLength = 1.0f;
  const int numberOfSamples = 10;
  Viewport *viewport;
public:
  explicit Camera(Viewport &viewport){
    this->viewport = &viewport;
  }

  void initialise(Vector3 eye, Vector3 lookAt, Vector3 up){
    this->eye = eye;
    this->lookAt = lookAt;
    this->up = up;
    this->viewport->calculate(eye,lookAt,up);

    focalLength = (eye-lookAt).length();
  }

  Vector3 getColour(Ray& ray, Scene& scene, int currentDepth){
    if(currentDepth <= 0){
      return Vector3(0.0f,0.0f,0.0f);
    }

    //Create a record to track recursive hits
    HitRecord hitRec;
    if (scene.hitSphere(ray, hitRec)) {
        // 1. Calculate a new direction after hit
        Vector3 dir;
        // 2. Define refractive index
        double refIndex = hitRec.front_face ? 1.0 : 0.8;
        // 3. Calculate new direction for ray
        if(hitRec.currentMat->getRoughness() >= 0.0f){
          // 3.1  Reflective
          dir = Vector3::unitVec(Vector3::reflect(ray.direction, hitRec.normal)) + (Vector3::unitVecRand() * hitRec.currentMat->getRoughness());
        }else if(hitRec.currentMat->getRoughness() >= -1.0f){
          // 3.2  Diffuse
          dir = Vector3::newDirectionOnHemisphere(hitRec.normal);
        }else{
          // 3.3  Refractive
          dir = Vector3::refract(ray.direction,hitRec.normal,refIndex);
        }
        // 4. Create new ray with newly calculated direction
        Ray newRay(hitRec.position, dir);
        // 5. Calculate attenuation value to diminish intensity after each hit
        float attenuation = 0.8;
        // 6. Recursively trace the rays
        return getColour(newRay, scene, currentDepth - 1) * hitRec.currentMat->getColour() * attenuation;
    }

    // If the sphere is not hit (background)
    Vector3 unitDir = Vector3::unitVec(ray.direction);
    auto a = 0.5*(unitDir.y + 1.0);
    auto b = Vector3(1.0, 1.0, 1.0)*(1.0-a) + Vector3(0.5, 0.7, 1.0)*a;
    return b; // Returns a gradient of blue based on the y direction of the ray
  }

  void render(std::vector<Fragment> &frags, Scene scene){
    //Location of the first pixel (top-left corner)
    Vector3 startPixel = (eye - (viewport->w * focalLength) - (viewport->u/2) - (viewport->v/2))
                         + ((viewport->pixelDeltaU + viewport->pixelDeltaV)/2);

    for(int h = 0; h < viewport->imageHeight; h ++){
      for(int w = 0; w < viewport->imageWidth; w ++){
        Vector3 pixelCenter = startPixel + (viewport->pixelDeltaU * w) + (viewport->pixelDeltaV*h);
        Fragment frag;
        Vector3 colour(0.0f,0.0f,0.0f);

        //Obtain multiple samples (antialiasing)
        for(int s = 0; s < numberOfSamples; s ++) {
          Vector3 tempOffset = (viewport->pixelDeltaU + viewport->pixelDeltaV) * (float(rand()%100)/100.0f);
          tempOffset = tempOffset - tempOffset/2;
          Ray ray(eye, Vector3::unitVec(pixelCenter-eye + tempOffset));
          colour = colour + getColour(ray, scene, depth)/float(numberOfSamples);
        }

        frag.setFinalColour(colour.x, colour.y, colour.z);
        frags.push_back(frag);
      }
    }
  }

};
#endif
