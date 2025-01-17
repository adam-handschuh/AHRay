#include <config.h>

float renderWidth, renderHeight;

Fragment* frags;

void renderInit(int width, int height){
    //Set the resolution
    renderWidth = width;
    renderHeight = height;

    Viewport viewport(renderWidth, renderHeight);
    Camera camera(&viewport);
    camera.initialise(Vector3(0,0,0), Vector3(0,0,100), Vector3(0,1,0));

    //Create an array of fragments based on resolution

}
void renderBackground(){
  std::cout << "Step 1. Rendering Background" << std::endl;
  float currentB = 0.0f;
  for(int i = 1; i < renderWidth*renderHeight; i ++){
    frags[i].b = 1.0f;
    frags[i].r = currentB;
    frags[i].g = currentB;
    if(i%renderWidth == 0){
      if(i+1 <renderWidth*renderHeight) {
        i++;
      }
      currentB += 1.0f/renderHeight;
    }
  }
}
void renderFrags(){
  renderBackground();
  std::cout << "Step 2. Rendering Sphere" << std::endl;
  //Render
}
void renderToPPM(const std::string& filename){
    std::cout << "\nExporting file as render.ppm...";
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

    std::cout << "\nImage Exported!\n\n";
}

int main() {
    renderInit(500, 500);
    renderFrags();
    renderToPPM("render.ppm");

    return 0;
}
