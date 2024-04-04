
#include "Player.hpp"

Player::Player(Model* ship, PointLight* spotlight, PerspectiveCamera* firstPOV, PerspectiveCamera* thirdPOV) :
	ship(ship), spotlight(spotlight), firstPOV(firstPOV), thirdPOV(thirdPOV), currentCam(thirdPOV) 
{
	/* Player Setup */
	glm::vec3 shipCenter = { 0.f, 0.f, 0.f };
	glm::vec3 shipNose = { 0.f, 0.f, 1.f };
	glm::vec3 thirdPOVOffset = { -0.5f, 0.5f, -1.f };

	this->ship->setPosition(shipCenter);
	this->ship->setRotation(0.f, {0.f, 0.f, 1.f});

	this->spotlight->setPosition(shipNose);
	this->spotlight->setBrightness(0.3);
	
	this->firstPOV->setPosition(shipNose);
	this->firstPOV->setCenter(shipNose + glm::vec3(0.f, 0.f, 1.f));
	this->firstPOV->setZfar(1.f);

	this->thirdPOV->setPosition(thirdPOVOffset);
	this->thirdPOV->setCenter(shipCenter);
	this->thirdPOV->setZfar(100.f);
}

void Player::apply(Shader* normal_shader, Shader* monochrome_shader, Shader* skybox_shader, Shader* mono_sky_shader) {
	Shader* model_shader = normal_shader;
	Shader* sky_shader = skybox_shader;

	if (this->currentCam == this->firstPOV) {
		model_shader = monochrome_shader;
		sky_shader = mono_sky_shader;
	}
	
	this->currentCam->apply(model_shader, sky_shader);
	this->spotlight->apply(model_shader);
}

void Player::render(Shader* normal_shader, Shader* monochrome_shader) {
	Shader* model_shader = normal_shader;

	if (this->currentCam == this->firstPOV)
		model_shader = monochrome_shader;

	this->ship->render(model_shader);
}

glm::vec3 Player::getPosition() {
	return this->ship->getPosition();
}

void Player::move(glm::vec3 movement) {
	this->ship->move(glm::vec3(this->ship->getRotation() * glm::vec4(movement, 1.0)));
	this->spotlight->move(glm::vec3(this->ship->getRotation() * glm::vec4(movement, 1.0)));
	this->firstPOV->moveBy(glm::vec3(this->ship->getRotation() * glm::vec4(movement, 1.0)));
	this->thirdPOV->moveBy(glm::vec3(this->ship->getRotation() * glm::vec4(movement, 1.0)));
}

void Player::turn(float degrees, glm::vec3 axis) {
	this->ship->rotate(degrees, axis);
	this->spotlight->rotateAround(this->ship->getPosition(), degrees, axis);
	this->firstPOV->rotateAround(this->ship->getPosition(), degrees, axis);
	this->thirdPOV->rotateAround(this->ship->getPosition(), degrees, axis);
}

void Player::panCamera(float degrees, glm::vec3 axis) {
	if (this->currentCam == this->thirdPOV)
		this->thirdPOV->rotateAround(this->ship->getPosition(), degrees, axis);
}

void Player::toggleCamera() {
	if (this->currentCam == this->firstPOV)
		this->currentCam = this->thirdPOV;
	else if (this->currentCam == this->thirdPOV)
		this->currentCam = this->firstPOV;
}

glm::mat4 Player::getRotationMat() {
	return this->ship->getRotation();
}

bool Player::isUsingFirstPOV() {
	return this->currentCam == this->firstPOV;
}