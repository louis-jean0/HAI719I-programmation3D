#ifndef CONTEXT_H_DEFINED
#define CONTEXT_H_DEFINED

#include "Camera.h"
#include "Mesh.h"
#include "Material.h"
// #include "Texture.h"

#include <glm/glm.hpp>

#include <vector>

static unsigned int SCREENWIDTH = 1600;
static unsigned int SCREENHEIGHT = 900;

enum RenderingType {
  Unlit,
  Phong,
  Reflective,
  PBR
};

struct Instance {
  glm::mat4 matrix;
  Mesh* mesh;
  Material* material;
};

struct Context {
  static Camera camera;
  static bool fullScreen;
  static bool refreshMatrices;

  static std::vector<Instance> instances;
  static std::vector<Mesh*> meshes;
  static std::vector<Material*> materials;
  static RenderingType rendering_type;

  static GLuint skyboxProgram;
  static GLuint skyboxTexture;
  static GLuint skyboxVAO;
  static GLuint skyboxVBO;
  static bool displaySkybox;

  // std::vector<Texture*> textures;
};

void idle();
void reshape(int w, int h);
void clearContext();


#endif
