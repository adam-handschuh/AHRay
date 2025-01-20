#ifndef AHRAY_SCENE_H
#define AHRAY_SCENE_H
#include <vector>
#include <vector3.h>
#include <sphere.h>
#include <iostream>

class Scene {
public:
  std::vector<Sphere> spheres;

  void addToScene(std::string objectName, Vector3 position, Vector3 scale) {
    if (objectName == "sphere") {
      Sphere newSphere(position, scale.x);
      spheres.push_back(newSphere);
    }
  }

  void addToScene(std::string objectName, Vector3 position, float scale) {
    if (objectName == "sphere") {
      Sphere newSphere(position, scale);
      spheres.push_back(newSphere);
    }
  }
};
#endif
