// Local includes
#include "../include/Material.h"
#include "../include/Shader.h"
#include "../include/Texture.h"
#include "../include/Context.h"
// GLM includes
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
// OPENGL includes
#include <GL/glew.h>
#include <GL/glut.h>
#include <iostream>

Material::~Material() {
	if (m_program != 0) {
		glDeleteProgram(m_program);
	}
}

void Material::init() {
	// TODO : Change shader by your
	switch (Context::rendering_type) {

		case Unlit:
			m_program = load_shaders("shaders/unlit/unlit_vertex.glsl", "shaders/unlit/unlit_fragment.glsl");
		break;

		case Phong:
			m_program = load_shaders("shaders/phong/phong_vertex.glsl", "shaders/phong/phong_fragment.glsl");
		break;

		case PBR:
		break;
	
	}
	m_program = load_shaders("shaders/unlit/vertex.glsl", "shaders/unlit/fragment.glsl");
	check();
	// TODO : set initial parameters
	m_color = {1.0, 1.0, 1.0, 1.0};
	m_texture = -1;//loadTexture2DFromFilePath("./data/BarramundiFish_baseColor.png");
	m_normal_map_texture = -1; //loadTexture2DFromFilePath("./data/BarramundiFish_normal.png");
	setDefaultTexture2DParameters(m_texture);
	setDefaultTexture2DParameters(m_normal_map_texture);
}

void Material::clear() {
	// nothing to clear
	// TODO: Add the texture or buffer you want to destroy for your material
}

void Material::bind() {
	check();
	glUseProgram(m_program);
	internalBind();
}

void Material::internalBind() {
	// bind parameters
	GLint color = getUniform("color");
	glUniform4fv(color, 1, glm::value_ptr(m_color));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glUniform1i(getUniform("colorTexture"), 0);
	glActiveTexture(GL_TEXTURE0 + 1);
	glBindTexture(GL_TEXTURE_2D, m_normal_map_texture);
	glUniform1i(getUniform("normalMapTexture"),1);
	// TODO : Add your custom parameters here
	// Gestion de la caméra
	glm::vec3 camPos = glm::vec3(1,0,0); 
	GLint camPosLoc = getUniform("camPos");
	glUniform3fv(camPosLoc,1,glm::value_ptr(camPos));
	// Gestion de la lumière
	glm::vec3 light_position = glm::vec3(1.0f, 1.0f, 0.0f);
	glm::vec3 light_color = glm::vec3(1.0f,1.0f,1.0f);
	GLint lightPosLoc = getUniform("lightPosition");
	GLint lightColorLoc = getUniform("lightColor");
	glUniform3fv(lightPosLoc,1,glm::value_ptr(light_position));
	glUniform3fv(lightColorLoc,1,glm::value_ptr(light_color));
}
	

void Material::setMatrices(glm::mat4& projectionMatrix, glm::mat4& viewMatrix, glm::mat4& modelMatrix)
{
	check();
	glUniformMatrix4fv(getUniform("projection"), 1, false, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(getUniform("view"), 1, false, glm::value_ptr(viewMatrix));
	glUniformMatrix4fv(getUniform("model"), 1, false, glm::value_ptr(modelMatrix));
}

GLint Material::getAttribute(const std::string& in_attributeName) {
	check();
	return glGetAttribLocation(m_program, in_attributeName.c_str());
}

GLint Material::getUniform(const std::string& in_uniformName) {
	check();
	return glGetUniformLocation(m_program, in_uniformName.c_str());
}
