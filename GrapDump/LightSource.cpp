
#include "LightSource.hpp"

LightSource::LightSource() :
	color(1.f), ambientColor(1.f), brightness(150.f),
	ambientStrength(0.1f), specularStrength(0.5f), specularPhong(16.f) {}

void LightSource::apply(Shader* shader) {
    shader->use();
    shader->passVec3("lightColor", this->color);
    shader->passFloat("brightness", this->brightness);
    shader->passFloat("ambientStr", this->ambientStrength);
    shader->passVec3("ambientColor", this->ambientColor);
    shader->passFloat("specStr", this->specularStrength);
    shader->passFloat("specPhong", this->specularPhong);
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

void LightSource::adjustBrightness(float brightness) {
    this->brightness += brightness;
}

void LightSource::setSpecStrength(float strength){
	this->specularStrength = strength;
}

void LightSource::setSpecPhong(float phong){
	this->specularPhong = phong;
}