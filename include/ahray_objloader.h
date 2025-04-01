//
// Created by Adam on 01/04/2025.
//

#ifndef AHRAY_AHRAY_OBJLOADER_H
#define AHRAY_AHRAY_OBJLOADER_H
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <vector3.h>

// A face represented by three vertex indices.
class OBJMesh {
public:
  struct Face {
    int v[3];  // OBJ files index vertices starting at 1, so subtract 1 for 0-based indexing.
  };
  std::vector<Vector3> vertices;
  std::vector<Face> faces;

  // Load and parse an OBJ file.
  bool load(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
      std::cerr << "Error: Cannot open file " << filename << std::endl;
      return false;
    }

    std::string line;
    while (std::getline(file, line)) {
      if (line.empty()) continue;

      std::istringstream iss(line);
      std::string prefix;
      iss >> prefix;

      // Parse vertex positions.
      if (prefix == "v") {
        float x, y, z;
        iss >> x >> y >> z;
        vertices.push_back(Vector3(x, y, z));
      }
      // Parse face definitions (assuming triangles).
      else if (prefix == "f") {
        Face face;
        // Here, we extract only the vertex index.
        for (int i = 0; i < 3; i++) {
          std::string token;
          iss >> token;
          std::istringstream tokenStream(token);
          std::string indexStr;
          std::getline(tokenStream, indexStr, '/');
          face.v[i] = std::atoi(indexStr.c_str()) - 1; // Convert to 0-based index.
        }
        faces.push_back(face);
      }
      // Extend here for parsing normals ("vn") or texture coordinates ("vt") if needed.
    }
    return true;
  }
};
#endif // AHRAY_AHRAY_OBJLOADER_H
