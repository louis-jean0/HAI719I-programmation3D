#include "../include/Light.h"
#include "../include/Shader.h"

Light::~Light() {
	if (l_program != 0) {
		glDeleteProgram(l_program);
	}
}

void Light::init() {
    l_program = load_shaders("shaders/unlit/vertex.glsl", "shaders/unlit/fragment.glsl");
    check();
    light_position = glm::vec4(22.0f, 16.0f, 50.0f, 0.0f);
    direction = glm::vec3(-52.0f,-16.0f,-50.0f);
    color = glm::vec4(1,1,1,1);
}

void Light::bind() {
    glUseProgram(l_program);
    check();
    setup_light();
}

void Light::setup_light() {
    GLint lightPosLoc = glGetUniformLocation(l_program, "lightPosition");
    GLint lightDirLoc = glGetUniformLocation(l_program, "lightDirection");
    GLint lightColorLoc = glGetUniformLocation(l_program, "lightColor");

    if (lightPosLoc == -1 || lightDirLoc == -1 || lightColorLoc == -1) {
        throw std::runtime_error("Invalid uniform location in shader");
    }

    glUniform4fv(lightPosLoc, 1, glm::value_ptr(light_position));
    glUniform3fv(lightDirLoc, 1, glm::value_ptr(direction));
    glUniform4fv(lightColorLoc, 1, glm::value_ptr(color));
}
