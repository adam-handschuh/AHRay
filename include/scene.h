#ifndef AHRAY_SCENE_H
#define AHRAY_SCENE_H
class Vector3{
public:
    float x,y,z;
};

class Ray{
public:
    Vector3 origin, direction;
};

class Sphere{
public:
    float center, radius, roughness;
};

class Light{
public:
    float position, lookat;
};

struct Fragment{
    float r,g,b;

    void setFinalColour(float r, float g, float b){
        this->r = r;
        this->g = g;
        this->b = b;
    }
};

struct Camera{
public:
  Vector3 position, lookAt;
};

#endif//AHRAY_SCENE_H
