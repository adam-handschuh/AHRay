#ifndef AHRAY_CAMERA_H
#define AHRAY_CAMERA_H
#include <ray.h>
#include <vector3.h>
#include <fragment.h>
#include <viewport.h>
#include <vector>
#include <scene.h>
#include <pointlight.h>

class Camera{
private:
  Vector3 eye;
  const int depth = 10;
  float focalLength = 1.0f;
  const int numberOfSamples = 20;

  std::vector<float> tempDepthCols;
  std::vector<Vector3> tempNormCols;
  std::vector<Vector3> tempAlbedoCols;

  std::vector<float> depthCols;
  std::vector<Vector3> normCols;
  std::vector<Vector3> albedoCols;

  Viewport *viewport;
public:
  explicit Camera(Viewport &viewport){
    this->viewport = &viewport;
  }

  void initialise(Vector3 eye, Vector3 lookAt, Vector3 up){
    this->eye = eye;
    this->viewport->calculate(eye,lookAt,up);

    focalLength = (eye-lookAt).length();
  }

  Vector3 getColour(Ray& ray, Scene& scene, int currentDepth){
    if(currentDepth <= 0){
      return Vector3(0.0f,0.0f,0.0f);
    }

    //Create a record to track recursive hits
    HitRecord hitRec;
    if (scene.hitSomething(ray, hitRec)) {
        // 0. Set depth colour for depth map
        if(currentDepth == depth) {
          tempDepthCols.push_back(hitRec.t);
          tempNormCols.push_back(hitRec.normal);
          tempAlbedoCols.push_back(hitRec.currentMat->getColour());
        }
        // 0.5 Compute direct illumination from all lights
        Vector3 directLight(0.0f, 0.0f, 0.0f);
        for (const PointLight &light : scene.lights) {
          // Vector from hit point to light position
          Vector3 toLight = light.position - hitRec.position;
          float distanceToLight = toLight.length();
          Vector3 lightDir = Vector3::unitVec(toLight);

          // Create a shadow ray from the hit point, offset slightly along the normal to avoid self-intersection
          Ray shadowRay(hitRec.position + hitRec.normal * 0.001, lightDir);

          if (!scene.hitSomething(shadowRay, hitRec)) {
            // Lambertian diffuse shading: dot product of the normal and light direction
            float NdotL = std::max(float(hitRec.normal.dot(lightDir)), 0.0f);
            directLight = directLight + (light.intensity * light.colour * NdotL);
          }
        }

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
        // 6. Recursively trace the rays (to obtain indirectColour)
        Vector3 indirectColour = getColour(newRay, scene, currentDepth - 1) * hitRec.currentMat->getColour() * attenuation;

        // 7. Return the final colour of for the pixel (combo of the other stuff)
        Vector3 finalColour = (directLight * hitRec.currentMat->getColour()) + indirectColour;
        return finalColour;
    }else{
      if(currentDepth == depth){
        tempDepthCols.push_back(10);
        tempNormCols.push_back(Vector3(0,0,255));
        tempAlbedoCols.push_back(Vector3(0,0,0));
      }
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

  std::vector<float> getDepthMap(){
    int counter = 0;
    float dSum = 0;
    for (float tempDepthCol : tempDepthCols) {
      counter ++;
      dSum += tempDepthCol;
      if(counter % numberOfSamples == 0){
        dSum /= float(numberOfSamples);
        depthCols.push_back(dSum);
        dSum = 0;
      }
    }
    return depthCols;
  }

  std::vector<Vector3> getNormalMap(){
    int counter = 0;
    Vector3 nSum(0,0,0);
    for (Vector3 tempNormCol : tempNormCols) {
      counter ++;
      nSum = nSum + tempNormCol;
      if(counter % numberOfSamples == 0){
        nSum = nSum / float(numberOfSamples);
        normCols.push_back(nSum);
        nSum = Vector3(0,0,0);
      }
    }
    return normCols;
  }

  std::vector<Vector3> getAlbedoMap(){
    int counter = 0;
    Vector3 aSum(0,0,0);
    for (Vector3 tempAlbCol : tempAlbedoCols) {
      counter ++;
      aSum = aSum + tempAlbCol;
      if(counter % numberOfSamples == 0){
        aSum = aSum / float(numberOfSamples);
        albedoCols.push_back(aSum);
        aSum = Vector3(0,0,0);
      }
    }
    return albedoCols;
  }

};
#endif
