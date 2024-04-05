
#include "Player.hpp"

Player::Player(Model* ship, SpotLight* spotlight, PerspectiveCamera* firstPOV, PerspectiveCamera* thirdPOV) :
	ship(ship), spotlight(spotlight), firstPOV(firstPOV), thirdPOV(thirdPOV), currentCam(thirdPOV) 
{
	/* Player Setup */
	glm::vec3 shipCenter = { 0.f, 0.f, 0.f };
	glm::vec3 shipNose = { 0.f, 0.f, 0.3f };
	glm::vec3 thirdPOVOffset = { -0.5f, 0.5f, -1.f };

	this->ship->setPosition(shipCenter);
	this->ship->setRotation(-90.f, {1.f, 0.f, 0.f});
	this->ship->rotate(90.f, {0.f, 1.f, 0.f});

	this->spotlight->setPosition(shipNose);
	this->spotlight->setBrightness(0.3);
	this->spotlight->setCutoffs(10.f, 20.f);
	
	this->firstPOV->setPosition(shipNose);
	this->firstPOV->setCenter(shipNose + glm::vec3(0.f, 0.f, 1.f));
	this->firstPOV->setZfar(1.f);

	this->thirdPOV->setPosition(thirdPOVOffset);
	this->thirdPOV->setCenter(shipCenter);
	this->thirdPOV->setZfar(100.f);
}

void Player::apply(Shader* model_shader, Shader* sky_shader) {
	this->currentCam->apply(model_shader, sky_shader);
	this->spotlight->apply(model_shader);
}

void Player::render(Shader* model_shader) {
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
	this->firstPOV->rotateAround(this->ship->getPosition(), degrees, axis);
	this->thirdPOV->rotateAround(this->ship->getPosition(), degrees, axis);
	this->spotlight->rotateAround(this->ship->getPosition(), degrees, axis);
	this->spotlight->setDirection(this->spotlight->getPosition() - this->ship->getPosition());
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