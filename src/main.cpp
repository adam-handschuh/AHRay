#include <config.h>

int renderWidth;
int renderHeight;

Fragment* frags;

void renderInit(int width, int height){
    //Set the resolution
    renderWidth = width;
    renderHeight = height;

    //Create an array of fragments based on resolution
    frags = new Fragment[renderWidth*renderHeight];

    for(int i = 0; i < renderWidth*renderHeight; i++){
        frags[i].setFinalColour(1.0f,0.0f,0.0f);
    }
}

void renderToPPM(const std::string& filename){
    std::ofstream file(filename,std::ios::binary);
    if(!file.is_open()){
        std::cerr << "Unable to open file";
    }

    //PPM file header
    file << "P6\n" << renderWidth << " " << renderHeight << "\n255\n";

    for(int i = 0; i < renderWidth*renderHeight; i ++){
        unsigned char red = static_cast<unsigned char>(std::clamp(frags[i].r, 0.0f, 1.0f) * 255.0f);
        unsigned char green = static_cast<unsigned char>(std::clamp(frags[i].g, 0.0f, 1.0f) * 255.0f);
        unsigned char blue = static_cast<unsigned char>(std::clamp(frags[i].b, 0.0f, 1.0f) * 255.0f);

        file.put(red);
        file.put(green);
        file.put(blue);
    }
    file.close();
}

int main() {
    renderInit(800, 600);

    renderToPPM("render.ppm");
    return 0;
}
