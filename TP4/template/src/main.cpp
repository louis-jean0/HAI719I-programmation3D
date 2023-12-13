// OPENGL includes
#include <GL/glew.h>
#include <GL/glut.h>
// STD includes
#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>
#include <algorithm>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <float.h>
#include <queue>
// GLM includes
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
// Local includes
#include "../include/Camera.h"
#include "../include/Shader.h"
#include "../include/Material.h"
#include "../include/Context.h"
#include "../include/Mouse.h"
#include "../include/Keyboard.h"
#include "../include/SceneLoader.h"
#include "../include/Texture.h"

// -------------------------------------------
// OpenGL/GLUT application code.
// -------------------------------------------

static GLint window;

void initSkybox() {
	Context::skyboxProgram = load_shaders("shaders/skybox/skybox_vertex.glsl","shaders/skybox/skybox_fragment.glsl");
	std::vector<std::string> skyboxImages;
	skyboxImages.push_back("data/skybox/right.jpg");
	skyboxImages.push_back("data/skybox/left.jpg");
	skyboxImages.push_back("data/skybox/top.jpg");
	skyboxImages.push_back("data/skybox/bottom.jpg");
	skyboxImages.push_back("data/skybox/front.jpg");
	skyboxImages.push_back("data/skybox/back.jpg");
	Context::skyboxTexture = loadSkybox(skyboxImages);

	float skybox[] = {
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f
	};
	glGenVertexArrays(1, &Context::skyboxVAO);
	glGenBuffers(1, &Context::skyboxVBO);
	glBindVertexArray(Context::skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, Context::skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skybox), &skybox, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glBindVertexArray(0);
}

void init() {
	// Context::camera.initPos();
	Context::camera.resize(SCREENWIDTH, SCREENHEIGHT);
	glCullFace (GL_BACK);
	glEnable (GL_CULL_FACE);
	glDepthFunc (GL_LESS);
	glEnable (GL_DEPTH_TEST);
	glClearColor (0.2f, 0.2f, 0.3f, 1.0f);
	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return;
	}
	initSkybox();
}

void beforeLoop() {
	// Take first instance center as target
	if (!Context::instances.empty()) {
		glm::vec3 centerOfInstance0 = Context::instances.at(0).mesh->center;
		std::cerr << glm::to_string(centerOfInstance0) << std::endl;

		glm::vec3 target(Context::instances.at(0).matrix * glm::vec4(centerOfInstance0, 1.0));
		Context::camera.position = glm::vec3(0.5,0.5,0.5);
		Context::camera.target = target;//lookat
		Context::camera.forward = glm::normalize(target - Context::camera.position);
		Context::camera.view = Context::camera.getViewMatrix();
	}

	// std::cerr << "projection:" << glm::to_string(Context::camera::projection) << std::endl;
	// std::cerr << "view:" << glm::to_string(Context::camera::view) << std::endl;

}

void draw() {
	if (Context::refreshMatrices) {
		// Context::camera.refreshMatrices();
		Context::camera.view = Context::camera.getViewMatrix();
		Context::camera.projection = Context::camera.getProjectionMatrix();
		Context::refreshMatrices = false;
	}
	// Clear the screen
	glClear(GL_COLOR_BUFFER_BIT);

	glDepthMask(GL_FALSE);
	glUseProgram(Context::skyboxProgram);
	glUniformMatrix4fv(glGetUniformLocation(Context::skyboxProgram, "projectionMatrix"), 1, false, glm::value_ptr(Context::camera.getProjectionMatrix()));
	glUniformMatrix4fv(glGetUniformLocation(Context::skyboxProgram, "viewMatrix"), 1, false, glm::value_ptr(glm::mat4(glm::mat3(Context::camera.getViewMatrix()))));
	glBindVertexArray(Context::skyboxVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, Context::skyboxTexture);
	glDrawArrays(GL_TRIANGLES, 0, 6*6);
	glBindVertexArray(0);
	glDepthMask(GL_TRUE);

	for (int i = 0; i < Context::instances.size(); ++i) {
		Instance& inst = Context::instances[i];
		Material* material = inst.material;
		Mesh* mesh = inst.mesh;
		material->bind();
		material->setMatrices(Context::camera.projection, Context::camera.view, inst.matrix);
		mesh->draw();
	}
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	draw();
	glFlush();
	glutSwapBuffers();
}



int main (int argc, char ** argv) {
	if (argc != 5) {
		std::cout<<argc<<std::endl;
		std::cerr << "Missing parameter: <path-to-model> <path-to-texture> <path-to-normal-map-texture> <path-to-pbr-packed-texture>" << std::endl;
		exit (EXIT_FAILURE);
	}
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(SCREENWIDTH, SCREENHEIGHT);
	window = glutCreateWindow("TP");

	init();
	glutIdleFunc(idle);
	glutDisplayFunc(display);
	glutKeyboardFunc(key);
	glutReshapeFunc(reshape);
	glutMotionFunc(motion);
	glutMouseFunc(mouse);
	key('?', 0, 0);
	Context::rendering_type = PBR;
	std::string model_path(argv[1]);
	loadDataWithAssimp(model_path);
	beforeLoop();
	std::string texture_path(argv[2]);
	std::string normal_map_texture_path(argv[3]);
	std::string pbr_packed_texture_path(argv[4]);
	Context::materials[0]->m_texture = loadTexture2DFromFilePath(texture_path);
	Context::materials[0]->m_normal_map_texture = loadTexture2DFromFilePath(normal_map_texture_path);
	Context::materials[0]->m_pbr_packed_texture = loadTexture2DFromFilePath(pbr_packed_texture_path);
	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
	glutMainLoop();
	clearContext();
	return EXIT_SUCCESS;
}