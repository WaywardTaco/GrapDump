
#include "Player.hpp"

Player::Player(Model* ship, PointLight* spotlight, PerspectiveCamera* firstPOV, PerspectiveCamera* thirdPOV) :
	ship(ship), spotlight(spotlight), firstPOV(firstPOV), thirdPOV(thirdPOV), currentCam(thirdPOV) 
{
	glm::vec3 shipCenter = { 0.f, 0.f, 0.f };
	glm::vec3 shipNose = { 0.f, 0.f, 1.f };
	glm::vec3 thirdPOVOffset = { -0.5f, 0.5f, -1.f };

	this->ship->setPosition(shipCenter);
	this->ship->setRotation(0.f, {0.f, 0.f, 1.f});

	this->spotlight->setPosition(shipNose);
	
	this->firstPOV->setPosition(shipNose);
	this->firstPOV->setCenter(shipNose + glm::vec3(0.f, 0.f, 1.f));

	this->thirdPOV->setPosition(thirdPOVOffset);
	this->thirdPOV->setCenter(shipCenter);
}

void Player::renderApply(Shader* normal_shader, Shader* monochrome_shader, Shader* skybox_shader) {
	Shader* model_shader = normal_shader;

	if (this->currentCam == this->firstPOV)
		model_shader = monochrome_shader;
	
	this->currentCam->apply(model_shader, skybox_shader);
	this->spotlight->apply(model_shader);
	this->ship->render(model_shader);
}

glm::vec3 Player::getPosition() {
	return this->ship->getPosition();
}

void Player::move(glm::vec3 movement) {
	this->ship->move(movement);
	this->spotlight->move(movement);
	this->firstPOV->moveBy(movement);
	this->thirdPOV->moveBy(movement);
}

void Player::turn(float degrees, glm::vec3 axis) {
	this->ship->rotate(degrees, axis);
	this->spotlight->rotateAround(this->ship->getPosition(), degrees, -axis);
	this->firstPOV->turn(degrees, axis);
	this->thirdPOV->rotateAround(degrees, axis);
}

void Player::panCamera(float degrees, glm::vec3 axis) {
	if (this->currentCam == this->thirdPOV)
		this->thirdPOV->rotateAround(degrees, axis);
}

void Player::toggleCamera() {
	if (this->currentCam == this->firstPOV)
		this->currentCam = this->thirdPOV;
	else if (this->currentCam == this->thirdPOV)
		this->currentCam = this->firstPOV;
}

