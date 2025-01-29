#ifndef AHRAY_SCENE_H
#define AHRAY_SCENE_H
#include <vector>
#include <vector3.h>
#include <sphere.h>
#include <iostream>

class Scene {
public:
  std::vector<Sphere> spheres;

  void addToScene(std::string objectName, Vector3 position, float scale, Material &material) {
    if (objectName == "sphere") {
      Sphere newSphere(position, scale, material);
      spheres.push_back(newSphere);
    }
  }

  bool hitSphere(Ray& r, HitRecord& rec) const{
    HitRecord temp_rec;
    bool hitDetected = false;
    auto min = 0.001;
    auto max = 1000000000.0;

    for (Sphere sphere : spheres) {
      if (sphere.hit(r, min, max, temp_rec)) {
        hitDetected = true;
        max = temp_rec.t;
        rec = temp_rec;
      }
    }

    return hitDetected;
  }
};
#endif
