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
        frags[i].setFinalColour(0.0f,0.0f,0.0f);
    }
}
void renderFrags(){
  float currentG = 0;
  for(int i = 1; i < renderWidth*renderHeight; i ++){
    frags[i].r = frags[i-1].r + 1.0f/renderWidth;
    frags[i].g = currentG;
    if(i%renderWidth == 0){
      if(i+1 <renderWidth*renderHeight) {
        i++;
      }
      std::cout << "new row!\n";
      currentG += 1.0f/renderHeight;
    }
  }
}
void renderToPPM(const std::string& filename){
    std::ofstream file(filename);
    if(!file.is_open()){
        std::cerr << "Unable to open file";
    }

    //PPM file header
    file << "P3\n" << renderWidth << " " << renderHeight << "\n255\n";

    for(int i = 0; i < renderWidth*renderHeight; i ++){
        float red = (std::clamp(frags[i].r, 0.0f, 1.0f) * 255.0f);
        float green = (std::clamp(frags[i].g, 0.0f, 1.0f) * 255.0f);
        float blue = (std::clamp(frags[i].b, 0.0f, 1.0f) * 255.0f);

        file << red << " " << green << " " << blue << std::endl;
    }
    file.close();
}

int main() {
    renderInit(800, 600);
    renderFrags();
    renderToPPM("render.ppm");
    return 0;
}
