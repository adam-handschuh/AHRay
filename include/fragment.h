#ifndef AHRAY_FRAGMENT_H
#define AHRAY_FRAGMENT_H
#include <vector3.h>

struct Fragment{
public:
    float r,g,b;

    void setFinalColour(float r, float g, float b){
        this->r = r;
        this->g = g;
        this->b = b;
    }
};
#endif
