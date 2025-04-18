#include <config.h>

int renderWidth, renderHeight;

//Time Setup
time_t now;

//Main Render
std::vector<Fragment> frags;
//Standard Maps
std::vector<float> dFrags;
std::vector<Vector3> nFrags;
//Experimental Maps
std::vector<Vector3> aFrags;
std::vector<float> sFrags;
//Light Maps
std::vector<Vector3> rfrFrags;
std::vector<Vector3> rflFrags;

Scene scene;

//Materials
Material base(Vector3(1,1,1), -1.0);
Material diffuse1(Vector3(0,0.9,0), -0.1);
Material diffuse2(Vector3(0.4,0.4,0), -0.1);
Material diffuse3(Vector3(0,0,1), -0.1);
Material diffuse4(Vector3(0.85,0.85,0), -0.1);

Material mirror(Vector3(0.9,0.9,0.9), 0.0);
Material glass(Vector3(0.95,1.0,0.95), -1.5);

void createScene(){
//  scene.addSphereToScene(Vector3(0,-100.5,-1), 100.0f, base);

  // Diffuse
//  scene.addSphereToScene(Vector3(0,-0.15,-2.0f), 0.35f, diffuse);

  // Refraction
//  scene.addModelToScene(R"(C:\Users\Adam\Desktop\College\Trinity\Year_4\Capstone\AHRay\cup.obj)", Vector3(0.0, -0.25, -2), 0.25f, glass);
//  scene.addModelToScene(R"(C:\Users\Adam\Desktop\College\Trinity\Year_4\Capstone\AHRay\straw.obj)", Vector3(0.0, -0.25, -2), 0.25f, diffuse);

  // Reflection
//  scene.addModelToScene(R"(C:\Users\Adam\Desktop\College\Trinity\Year_4\Capstone\AHRay\mirror_base.obj)", Vector3(-0.53,0, -2.5), 0.25f, diffuse2);
//  scene.addModelToScene(R"(C:\Users\Adam\Desktop\College\Trinity\Year_4\Capstone\AHRay\mirror_surface.obj)", Vector3(-0.53,0, -2.5), 0.25f, mirror);

  // Complex
  Vector3 scenePosition(-0.95,-0.82,0.96);
  scene.addModelToScene(R"(C:\Users\Adam\Desktop\College\Trinity\Year_4\Capstone\AHRay\complex_scene\floor.obj)", scenePosition, 0.25f, diffuse1);
  scene.addModelToScene(R"(C:\Users\Adam\Desktop\College\Trinity\Year_4\Capstone\AHRay\complex_scene\cup.obj)", scenePosition, 0.25f,   glass);
  scene.addModelToScene(R"(C:\Users\Adam\Desktop\College\Trinity\Year_4\Capstone\AHRay\complex_scene\chair.obj)", scenePosition+Vector3(0.3,0,-0.3), 0.25f, diffuse2);
  scene.addModelToScene(R"(C:\Users\Adam\Desktop\College\Trinity\Year_4\Capstone\AHRay\complex_scene\table.obj)", scenePosition, 0.25f, diffuse3);
  scene.addModelToScene(R"(C:\Users\Adam\Desktop\College\Trinity\Year_4\Capstone\AHRay\complex_scene\mirror_frame.obj)", scenePosition, 0.25f, diffuse4);
  scene.addModelToScene(R"(C:\Users\Adam\Desktop\College\Trinity\Year_4\Capstone\AHRay\complex_scene\mirror_plane.obj)", scenePosition, 0.25f, mirror);


  //scene.addSphereToScene(Vector3(0.3,0,-3.5),0.5f,mirror);
  scene.addLightToScene(Vector3(-1,3.0,0), Vector3(1.0f,1.0f,1.0f), Vector3(0.25,0.25,0.25));
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
                      Vector3(-1.5,-0.2,1.5),
                      Vector3(0,1,0));

    //Render the scene using the camera
    camera.render(frags, scene);     //Render to an array of fragments based on resolution

    dFrags = camera.getDepthMap();
    nFrags = camera.getNormalMap();
    aFrags = camera.getAlbedoMap();
    sFrags = camera.getShadowMap();
    rfrFrags = camera.getRefractMap();
    rflFrags = camera.getReflectMap();
}

void convertPPMToPNG(const std::string& ppmFilename, const std::string& pngFilename) {
  std::ifstream infile(ppmFilename);
  if (!infile) {
    std::cerr << "Error opening PPM file: " << ppmFilename << std::endl;
    return;
  }

  std::string line;

  // Read the magic number (should be "P3")
  std::getline(infile, line);
  if (line != "P3") {
    std::cerr << "Invalid PPM format. Expected 'P3' but got '" << line << "'" << std::endl;
    return;
  }

  // Skip any comment lines and read width and height
  int width = 0, height = 0;
  while (std::getline(infile, line)) {
    if (line.empty() || line[0] == '#') continue;
    std::istringstream dims(line);
    if (dims >> width >> height)
      break;
  }
  if (width <= 0 || height <= 0) {
    std::cerr << "Invalid image dimensions." << std::endl;
    return;
  }

  // Skip comments and read the max color value
  int maxVal = 0;
  while (std::getline(infile, line)) {
    if (line.empty() || line[0] == '#') continue;
    std::istringstream maxLine(line);
    if (maxLine >> maxVal)
      break;
  }
  if (maxVal <= 0) {
    std::cerr << "Invalid maximum color value." << std::endl;
    return;
  }

  // Prepare a container for pixel data in RGBA format.
  // lodepng expects a vector of unsigned char with each pixel as R, G, B, A.
  std::vector<unsigned char> image;
  image.reserve(width * height * 4);

  // Read pixel data (r, g, b values) from the file
  int r, g, b;
  int pixelCount = 0;
  while (infile >> r >> g >> b) {
    // Normalize each color channel to the 0-255 range.
    unsigned char red   = static_cast<unsigned char>(r * 255 / maxVal);
    unsigned char green = static_cast<unsigned char>(g * 255 / maxVal);
    unsigned char blue  = static_cast<unsigned char>(b * 255 / maxVal);

    image.push_back(red);
    image.push_back(green);
    image.push_back(blue);
    image.push_back(255);  // Fully opaque alpha channel

    ++pixelCount;
  }

  if (pixelCount != width * height) {
    std::cerr << "Warning: Pixel count (" << pixelCount
              << ") does not match image dimensions (" << width * height << ")." << std::endl;
    // Depending on your application you might want to exit or handle the discrepancy.
  }

  // Encode the image into a PNG file using lodepng.
  lodepng::encode("output/" + pngFilename, image, width, height);

  time(&now);
  std::string dt = ctime(&now);

  std::cout << "Successfully created PNG file: " << pngFilename << " ---------> [" << dt.substr(11,8) << "]" << std::endl;
}
void saveAsPNG(const std::string& filename){
    std::ofstream file(filename + ".ppm");
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

    // Convert PPM to PNG
    convertPPMToPNG(filename+".ppm", filename+".png");
}

void saveDepthMap(const std::string& filename){
  std::ofstream file(filename + ".ppm");
  if(!file.is_open()){
    std::cerr << "Unable to open file";
  }

  //PPM file header
  file << "P3\n" << renderWidth << " " << renderHeight << "\n255\n";

  for(int i = 0; i < renderWidth*renderHeight; i ++){
    int d = static_cast<int>(255 - std::clamp(dFrags.at(i),0.0f,12.75f) * 20);

    file << d << " " << d << " " << d << std::endl;
  }
  file.close();
  // Convert PPM to PNG
  convertPPMToPNG(filename+".ppm", filename+".png");
}
void saveNormalMap(const std::string& filename){
  std::ofstream file(filename + ".ppm");
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
  // Convert PPM to PNG
  convertPPMToPNG(filename+".ppm", filename+".png");
}
void saveAlbedoMap(const std::string& filename){
  std::ofstream file(filename + ".ppm");
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
  // Convert PPM to PNG
  convertPPMToPNG(filename+".ppm", filename+".png");
}

void saveShadowMap(const std::string& filename){
  std::ofstream file(filename + ".ppm");
  if(!file.is_open()){
    std::cerr << "Unable to open file";
  }

  //PPM file header
  file << "P3\n" << renderWidth << " " << renderHeight << "\n255\n";

  for(int i = 0; i < renderWidth*renderHeight; i ++){
    int s = static_cast<int>(sFrags.at(i));

    file << s << " " << s << " " << s << std::endl;
  }
  file.close();
  // Convert PPM to PNG
  convertPPMToPNG(filename+".ppm", filename+".png");
}
void saveReflectionMap(const std::string& filename){
  std::ofstream file(filename + ".ppm");
  if(!file.is_open()){
    std::cerr << "Unable to open file";
  }

  //PPM file header
  file << "P3\n" << renderWidth << " " << renderHeight << "\n255\n";

  for(int i = 0; i < renderWidth*renderHeight; i ++){
    int r = static_cast<int>(rflFrags.at(i).x*255);
    int g = static_cast<int>(rflFrags.at(i).y*255);
    int b = static_cast<int>(rflFrags.at(i).z*255);

    if(!(r == g && g == b)) {
      if (std::max(r, std::max(g, b)) == r) {
        g /= 2;
        b /= 2;
      } else if (std::max(r, std::max(g, b)) == g) {
        r /= 2;
        b /= 2;
      } else {
        r /= 2;
        g /= 2;
      }
    }

    file << r << " " << g << " " << b << std::endl;
  }
  file.close();
  // Convert PPM to PNG
  convertPPMToPNG(filename+".ppm", filename+".png");
}
void saveRefractionMap(const std::string& filename){
  std::ofstream file(filename + ".ppm");
  if(!file.is_open()){
    std::cerr << "Unable to open file";
  }

  //PPM file header
  file << "P3\n" << renderWidth << " " << renderHeight << "\n255\n";

  for(int i = 0; i < renderWidth*renderHeight; i ++){
    int r = static_cast<int>(rfrFrags.at(i).x*255);
    int g = static_cast<int>(rfrFrags.at(i).y*255);
    int b = static_cast<int>(rfrFrags.at(i).z*255);

    if(!(r == g && g == b)) {
      if (std::max(r, std::max(g, b)) == r) {
        g /= 2;
        b /= 2;
      } else if (std::max(r, std::max(g, b)) == g) {
        r /= 2;
        b /= 2;
      } else {
        r /= 2;
        g /= 2;
      }
    }


    file << r << " " << g << " " << b << std::endl;
  }
  file.close();
  // Convert PPM to PNG
  convertPPMToPNG(filename+".ppm", filename+".png");
}

int main() {
  createScene();
  int resX, resY;
  std::cout << "Enter width:";
  std::cin >> resX;
  std::cout << "Enter height:";
  std::cin >> resY;
  time(&now);
  std::string dt = ctime(&now);

  std::cout << std::endl << "Processing..." << " ----------> [" << dt.substr(11,8) << "]" << std::endl;
  renderScene(resX, resY, Vector3(0,0,0));
  saveAsPNG("Render");
  saveDepthMap("DepthMap");
  saveNormalMap("NormalMap");
  //saveAlbedoMap("AlbedoMap");
  saveShadowMap("ShadowMap");
  saveRefractionMap("RefractionMap");
  saveReflectionMap("ReflectionMap");
  std::cout << std::endl << "Process finished!";
  return 0;
}