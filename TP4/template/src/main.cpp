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

void initLight () {
    GLfloat light_position1[4] = {22.0f, 16.0f, 50.0f, 0.0f};
    GLfloat direction1[3] = {-52.0f,-16.0f,-50.0f};
    GLfloat color1[4] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat ambient[4] = {0.3f, 0.3f, 0.3f, 0.5f};

    glLightfv (GL_LIGHT1, GL_POSITION, light_position1);
    glLightfv (GL_LIGHT1, GL_SPOT_DIRECTION, direction1);
    glLightfv (GL_LIGHT1, GL_DIFFUSE, color1);
    glLightfv (GL_LIGHT1, GL_SPECULAR, color1);
    glLightModelfv (GL_LIGHT_MODEL_AMBIENT, ambient);
    glEnable (GL_LIGHT1);
    glEnable (GL_LIGHTING);
}

void init() {
	// Context::camera.initPos();
	Context::camera.resize(SCREENWIDTH, SCREENHEIGHT);
	initLight();
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
	if (argc < 2) {
		std::cerr << "Missing parameter: <path-to-model> <path-to-texture>" << std::endl;
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

	std::string path(argv[1]);
	loadDataWithAssimp(path);
	beforeLoop();
	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
	glutMainLoop();
	clearContext();
	return EXIT_SUCCESS;
}