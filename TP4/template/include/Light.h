#ifndef LIGHT_H_DEFINED
#define LIGHT_H_DEFINED
#include <GL/glew.h>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <stdexcept>
#include <vector>

struct Light {

    GLuint l_program;
    glm::vec4 light_position;
    glm::vec3 direction;
    glm::vec4 color;

    inline void check() {
		if (l_program == 0) {
			throw std::runtime_error("Shader program not initialized");
		}
	}

    Light(): l_program(0) {

    }

    virtual ~Light();

    void init();

    void bind();

    void setup_light();

};

#endif