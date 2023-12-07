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
	switch(Context::rendering_type) {

		case Unlit:
			m_program = load_shaders("./shaders/unlit/unlit_vertex.glsl", "./shaders/unlit/unlit_fragment.glsl");
		break;

		case Phong:
			m_program = load_shaders("./shaders/phong/phong_vertex.glsl", "./shaders/phong/phong_fragment.glsl");
		break;

		case Reflective:
			m_program = load_shaders("./shaders/reflective/reflective_vertex.glsl", "./shaders/reflective/reflective_fragment.glsl");
		break;

		case PBR:
		break;
	
	}
	check();
	// TODO : set initial parameters
	m_color = {1.0, 1.0, 1.0, 1.0};
	m_texture = -1;
	m_normal_map_texture = -1;
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
	if(m_texture != - 1) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_texture);
		glUniform1i(getUniform("colorTexture"), 0);
	}
	if(m_normal_map_texture != -1) {
		glActiveTexture(GL_TEXTURE0 + 1);
		glBindTexture(GL_TEXTURE_2D, m_normal_map_texture);
		glUniform1i(getUniform("normalMapTexture"),1);
	}
	switch(Context::rendering_type) {
		case Unlit:
		break;

		case Phong: {
			// Gestion de la caméra
			glUniform3fv(getUniform("camPos"),1,glm::value_ptr(Context::camera.position));
			// Gestion de la lumière
			glm::vec3 light_position = glm::vec3(1.0f, 1.0f, 0.0f);
			glm::vec3 light_color = glm::vec3(1.0f,1.0f,1.0f);
			GLint lightPosLoc = getUniform("lightPosition");
			GLint lightColorLoc = getUniform("lightColor");
			glUniform3fv(lightPosLoc,1,glm::value_ptr(light_position));
			glUniform3fv(lightColorLoc,1,glm::value_ptr(light_color));
			// Les coefficients ambients, diffus et spéculaires sont à modifier directement dans le shader
			// Idéalement, il faudrait les récupérer depuis les modèles ou bien pouvoir les modifier directement dans le programme C++
		}
		break;

		case Reflective: {
			glUniform3fv(getUniform("camPos"),1,glm::value_ptr(Context::camera.position));
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_CUBE_MAP, Context::skyboxTexture);
		}

		case PBR:
		break;

		default:
		break;

	}
	// TODO : Add your custom parameters here
	
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
