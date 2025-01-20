#include <config.h>
#include <vector>

int renderWidth, renderHeight;

std::vector<Fragment> frags;

void renderScene(int width, int height){
    //Set the resolution
    renderWidth = width;
    renderHeight = height;

    std::cout << "Initialisation" << std::endl;
    Viewport* viewport = new Viewport(float(renderWidth), float(renderHeight));
    Camera camera(viewport);
    camera.initialise(Vector3(0,0,0), Vector3(0,0,100), Vector3(0,1,0));

    //Set up scene
    Scene scene;
    //spheres
    scene.addToScene("sphere", Vector3(0,0,-1), 0.5f);
    scene.addToScene("sphere", Vector3(0,-100.5,-1), 100.0f);



    std::cout << "Rendering" << std::endl;
    //Render to an array of fragments based on resolution
    camera.render(frags, scene);
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
    renderScene(400, 400);
    saveAsPPM("render.ppm");
    return 0;
}
