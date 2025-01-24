#include <config.h>
#include <vector>

int renderWidth, renderHeight;
std::vector<Fragment> frags;
Scene scene;

void createScene(){
  scene.addToScene("sphere", Vector3(0,-100.5,-1), 100.0f);
  scene.addToScene("sphere", Vector3(0,0,-1), 0.5f);
}

void renderScene(int width, int height){
    //Set the resolution
    renderWidth = width;
    renderHeight = height;

    //Initialise the viewport and camera
    std::cout << "Initialisation" << std::endl;
    Viewport* viewport = new Viewport(float(renderWidth), float(renderHeight));
    Camera camera(viewport);
    camera.initialise(Vector3(0,0,0), Vector3(0,0,100), Vector3(0,1,0));

    //Render the scene using the camera
    std::cout << "Rendering" << std::endl;
    camera.render(frags, scene);     //Render to an array of fragments based on resolution
}
void saveAsPPM(const std::string& filename){
    std::cout << "\nExporting file as render.ppm...";
    std::ofstream file(filename);
    if(!file.is_open()){
        std::cerr << "Unable to open file";
    }

    //PPM file header
    file << "P3\n" << renderWidth << " " << renderHeight << "\n255\n";

    for(int i = 0; i < renderWidth*renderHeight; i ++){
        float red = (std::clamp(frags.at(i).r, 0.0f, 1.0f) * 255.0f);
        float green = (std::clamp(frags.at(i).g, 0.0f, 1.0f) * 255.0f);
        float blue = (std::clamp(frags.at(i).b, 0.0f, 1.0f) * 255.0f);

        file << red << " " << green << " " << blue << std::endl;
    }
    file.close();

    std::cout << "\nImage Exported!\n\n";
}

int main() {
    createScene();
    renderScene(300, 300);
    saveAsPPM("render.ppm");
    return 0;
}
