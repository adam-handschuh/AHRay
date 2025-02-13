#include <config.h>

int renderWidth, renderHeight;
std::vector<Fragment> frags;
Scene scene;

const int frames = 0;

//Materials
Material base(Vector3(1,1,1), -1.0);
Material metal(Vector3(0.9,0.6,0.9), 0.25);
Material diffuse(Vector3(0,0.9,0), 0.0);
Material mirror(Vector3(0.9,0.9,0.9), 0.0);
Material glass(Vector3(0.7,0.7,0.9), -1.2);


void createScene(){
  //Big Sphere (Base)
  scene.addToScene("sphere", Vector3(0,-100.5,-1), 100.0f, base);
  //Medium Sphere (Mirror [Reflection])
  scene.addToScene("sphere", Vector3(-0.2,0.2,-2.0), 0.7f, mirror);
  //Small Sphere (Glass [Refraction])
  scene.addToScene("sphere", Vector3(0.1f,-0.29,-1.0f), 0.2f, glass);
  //Small Sphere (Metal)
  scene.addToScene("sphere", Vector3(0.65f,-0.25,-1.55f), 0.25f, metal);
  //Smaller Sphere (Diffuse)
  scene.addToScene("sphere", Vector3(-0.4f,-0.35,-1.0f), 0.15f, diffuse);
}
void renderScene(int width, int height){
    //Store resolution
    renderWidth = width;
    renderHeight = height;

    //Initialise the viewport and camera
    std::cout << "1. Initialising...";
    Viewport viewport(width,
                      height,
                      80.0f);

    Camera camera(viewport);
    camera.initialise(Vector3(-1,2,0),
                      Vector3(0,0,-1),
                      Vector3(0,1,0));

    //Render the scene using the camera
    std::cout << "Complete\n2. Rendering...";
    camera.render(frags, scene);     //Render to an array of fragments based on resolution
}
void saveAsPPM(const std::string& filename){
    std::cout << "Complete\n3. Exporting file as render.ppm...";
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

    std::cout << "Complete\n\nImage Exported!\n\n";
}

int main() {
  createScene();
  renderScene(200, 200);
  saveAsPPM("render.ppm");
  return 0;
}
