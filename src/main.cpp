#include <config.h>

int renderWidth, renderHeight;
std::vector<Fragment> frags;
std::vector<float> dFrags;
std::vector<Vector3> nFrags;
std::vector<Vector3> aFrags;

Scene scene;

//Materials
Material base(Vector3(1,1,1), -1.0);
Material metal(Vector3(0.9,0.6,0.9), 0.25);
Material diffuse(Vector3(1,1,1), 0.1);
Material mirror(Vector3(0.9,0.9,0.9), 0.0);
Material glass(Vector3(0.8,0.8,0.9), -1.2);
Material refr(Vector3(0.9,0.7,0.8), -1.5);

void createScene(){
  //Big Sphere (Base)
  scene.addSphereToScene(Vector3(0,-100.5,-1), 100.0f, base);
  scene.addModelToScene("C:\\Users\\Adam\\Desktop\\College\\Trinity\\Year_4\\Capstone\\AHRay\\cube.obj", Vector3(-0.2, -0.245, -2), 0.25f, base);
  scene.addSphereToScene(Vector3(0.3,0,-3.5),0.5f,base);
  scene.addLightToScene(Vector3(0.4,2.0,-4), Vector3(1.0f,1.0f,1.0f), Vector3(1.0f,1.0f,1.0f));
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

  std::cout << "Successfully created PNG file: " << pngFilename << std::endl;
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
    int d = static_cast<int>(255 - std::clamp(dFrags.at(i),0.0f,5.0f) * 51);

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

int main() {
  std::cout << "Creating scene" << std::endl;
  createScene();
  std::cout << "Rendering scene" << std::endl;
  renderScene(700, 700, Vector3(0,0.1,0));
  std::cout << "Exporting main render" << std::endl;
  saveAsPNG("Render");
  std::cout << "Exporting depth map" << std::endl;
  saveDepthMap("DepthMap");
  std::cout << "Exporting normal map" << std::endl;
  saveNormalMap("NormalMap");
  std::cout << "Exporting albedo map" << std::endl;
  saveAlbedoMap("AlbedoMap");
  std::cout << std::endl << "Process finished!";
  return 0;
}