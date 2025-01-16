#ifndef AHRAY_FRAGMENT_H
#define AHRAY_FRAGMENT_H
struct Fragment{
    float r,g,b;

    void setFinalColour(float r, float g, float b){
        this->r = r;
        this->g = g;
        this->b = b;
    }
};
#endif
