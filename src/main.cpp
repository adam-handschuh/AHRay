#include <config.h>

int renderWidth, renderHeight;
std::vector<Fragment> frags;
std::vector<float> dFrags;
std::vector<Vector3> nFrags;
std::vector<Vector3> aFrags;
std::vector<Vector3> cFrags;

Scene scene;
float x = 0;

//Materials
Material base(Vector3(1,1,1), -1.0);
Material metal(Vector3(0.9,0.6,0.9), 0.25);
Material diffuse(Vector3(0,0.9,0), 0.0);
Material mirror(Vector3(0.9,0.9,0.9), 0.0);
Material glass(Vector3(0.8,0.8,0.9), -1.2);
Material refr(Vector3(0.9,0.7,0.8), -1.5);

void createScene(){
  //Big Sphere (Base)
  scene.addToScene("sphere", Vector3(0,-100.5,-1), 100.0f, base);
  //Medium Sphere (Mirror [Reflection])
  scene.addToScene("cylinder", Vector3(-0.2,-0.495,-2.0), 0.3f, glass);
  //Small Sphere (Glass [Refraction])
  scene.addToScene("sphere", Vector3(0.1f,-0.297,-1.3f), 0.2f, refr);
  //Small Sphere (Metal)
//    scene.addToScene("sphere", Vector3(0.65f,-0.25,-1.55f), 0.25f, metal);
  //Smaller Sphere (Diffuse)
//    scene.addToScene("sphere", Vector3(-0.4f,-0.35,-1.0f), 0.15f, diffuse);
}
void renderScene(int width, int height, Vector3 camPos){
    //Store resolution
    renderWidth = width;
    renderHeight = height;

    //Initialise the viewport and camera
    Viewport viewport(width,
                      height,
                      80.0f);

    Camera camera(viewport);
    camera.initialise(camPos,
                      Vector3(0,0,-1.5),
                      Vector3(0,1,0));

    //Render the scene using the camera
    camera.render(frags, scene);     //Render to an array of fragments based on resolution

    dFrags = camera.getDepthMap();
    nFrags = camera.getNormalMap();
    aFrags = camera.getAlbedoMap();
}
void saveAsPPM(const std::string& filename){
    std::ofstream file(filename);
    if(!file.is_open()){
        std::cerr << "Unable to open file";
    }

    //PPM file header
    file << "P3\n" << renderWidth << " " << renderHeight << "\n255\n";

    for(int i = 0; i < renderWidth*renderHeight; i ++){
        int red = static_cast<int>(std::clamp(frags.at(i).r, 0.0f, 1.0f) * 255.0f);
        int green = static_cast<int>(std::clamp(frags.at(i).g, 0.0f, 1.0f) * 255.0f);
        int blue = static_cast<int>(std::clamp(frags.at(i).b, 0.0f, 1.0f) * 255.0f);

        file << red << " " << green << " " << blue << std::endl;
    }
    file.close();
}

void saveDepthMap(const std::string& filename){
  std::ofstream file(filename);
  if(!file.is_open()){
    std::cerr << "Unable to open file";
  }

  //PPM file header
  file << "P3\n" << renderWidth << " " << renderHeight << "\n255\n";

  for(int i = 0; i < renderWidth*renderHeight; i ++){
    int d = static_cast<int>(255 - std::clamp(dFrags.at(i),0.0f,5.0f) * 51);

    file << d << " " << d << " " << d << std::endl;
  }
  file.close();
}
void saveNormalMap(const std::string& filename){
  std::ofstream file(filename);
  if(!file.is_open()){
    std::cerr << "Unable to open file";
  }

  //PPM file header
  file << "P3\n" << renderWidth << " " << renderHeight << "\n255\n";

  for(int i = 0; i < renderWidth*renderHeight; i ++){
    int r = static_cast<int>(((nFrags.at(i).x/2) + 0.5) * 255);
    int g = static_cast<int>(((nFrags.at(i).y/2) + 0.5) * 255);
    int b = static_cast<int>(((nFrags.at(i).z/2) + 0.5) * 255);


    file << r << " " << g << " " << b << std::endl;
  }
  file.close();
}
void saveAlbedoMap(const std::string& filename){
  std::ofstream file(filename);
  if(!file.is_open()){
    std::cerr << "Unable to open file";
  }

  //PPM file header
  file << "P3\n" << renderWidth << " " << renderHeight << "\n255\n";

  for(int i = 0; i < renderWidth*renderHeight; i ++){
    int r = static_cast<int>(aFrags.at(i).x*255);
    int g = static_cast<int>(aFrags.at(i).y*255);
    int b = static_cast<int>(aFrags.at(i).z*255);

    file << r << " " << g << " " << b << std::endl;
  }
  file.close();
}


int main() {
  std::cout << "Creating scene" << std::endl;
  createScene();
  std::cout << "Rendering scene" << std::endl;
  renderScene(1000, 1000, Vector3(0,0.1,0));
  std::cout << "Exporting main render" << std::endl;
  saveAsPPM("Render.ppm");
  std::cout << "Exporting depth map" << std::endl;
  saveDepthMap("DepthMap.ppm");
  std::cout << "Exporting normal map" << std::endl;
  saveNormalMap("NormalMap.ppm");
  std::cout << "Exporting albedo map" << std::endl;
  saveAlbedoMap("AlbedoMap.ppm");
  std::cout << std::endl << "Process finished!";
  return 0;
}
