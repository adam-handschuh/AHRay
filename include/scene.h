#ifndef AHRAY_SCENE_H
#define AHRAY_SCENE_H
#include <vector>
#include <vector3.h>
#include <sphere.h>
#include <iostream>
#include <cylinder.h>
#include <ahray_objloader.h>
#include <triangle.h>

class Scene {
public:
  std::vector<Sphere> spheres;
  std::vector<Cylinder> cylinders;
  std::vector<Triangle> triangles;





  void addToScene(std::string objectName, Vector3 position, float scale, Material &material) {
    if (objectName == "sphere") {
      Sphere newSphere(position, scale, material);
      spheres.push_back(newSphere);
    }
    if (objectName == "cylinder"){
      Cylinder newCylinder(position, scale, scale*3, material);
      cylinders.push_back(newCylinder);
    }
  }

  void addModelToScene(const std::string &modelPath, Vector3 position, float scale, Material &material){
    OBJMesh mesh;
    if (!mesh.load(modelPath)) {
      std::cerr << "Failed to load OBJ file." << std::endl;
    }

    // For each face, create a Triangle primitive and add it to your world.
    for (const auto& face : mesh.faces) {
      // Create a triangle from the mesh vertices.
      Triangle tri(
          (mesh.vertices[face.v[0]]*scale)+position,
          (mesh.vertices[face.v[1]]*scale)+position,
          (mesh.vertices[face.v[2]]*scale)+position,
          material
      );
      triangles.push_back(tri);  // Add the triangle to your scene.
    }
  }

  bool hitSomething(Ray& r, HitRecord& rec){
    HitRecord temp_rec;
    bool hitDetected = false;
    auto min = 0.001;
    auto max = 1000000000.0;

    for (Sphere sphere : spheres) {
      //Check for hits
      if (sphere.hit(r, min, max, temp_rec)) {
        hitDetected = true;
        max = temp_rec.t;
        rec = temp_rec;
      }
    }

    for(Cylinder cylinder:cylinders){
      if(cylinder.hit(r,min,max,temp_rec)){
        hitDetected = true;
        max = temp_rec.t;
        rec = temp_rec;
      }
    }

    for(Triangle triangle:triangles){
      if(triangle.hit(r,min,max,temp_rec)){
        hitDetected = true;
        max = temp_rec.t;
        rec = temp_rec;
      }
    }

    return hitDetected;
  }
};
#endif
