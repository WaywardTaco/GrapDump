
#include "LightSource.hpp"

LightSource::LightSource() :
	color(1.f), ambientColor(1.f), brightness(150.f),
	ambientStrength(0.1f), specularStrength(0.5f), specularPhong(16.f) {}

void LightSource::apply(GLuint shaderProgram) {
    glUseProgram(shaderProgram);

    GLuint colorAdrs = glGetUniformLocation(shaderProgram, "lightColor");
    glUniform3fv(colorAdrs, 1, glm::value_ptr(this->color));

    GLuint brightAdrs = glGetUniformLocation(shaderProgram, "brightness");
    glUniform1f(brightAdrs, this->brightness);

    GLuint ambStrAdrs = glGetUniformLocation(shaderProgram, "ambientStr");
    glUniform1f(ambStrAdrs, this->ambientStrength);

    GLuint ambColorAdrs = glGetUniformLocation(shaderProgram, "ambientColor");
    glUniform3fv(ambColorAdrs, 1, glm::value_ptr(this->ambientColor));

    GLuint specStrAdrs = glGetUniformLocation(shaderProgram, "specStr");
    glUniform1f(specStrAdrs, this->specularStrength);

    GLuint specPhongAdrs = glGetUniformLocation(shaderProgram, "specPhong");
    glUniform1f(specPhongAdrs, this->specularPhong);
}

void LightSource::setColor(glm::vec3 color) {
	this->color = color;
	this->ambientColor = color;
}

void LightSource::setAmbientColor(glm::vec3 color){
	this->ambientColor = color;
}

void LightSource::setBrightness(float brightness){
	this->brightness = brightness;
}

void LightSource::setSpecStrength(float strength){
	this->specularStrength = strength;
}

void LightSource::setSpecPhong(float phong){
	this->specularPhong = phong;
}