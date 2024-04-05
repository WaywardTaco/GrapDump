
#pragma once

#include "Model.hpp"
#include "SpotLight.hpp"
#include "PerspectiveCamera.hpp"

class Player {
private:
	Model* ship;
	SpotLight* spotlight;
	PerspectiveCamera 
		*firstPOV,
		*thirdPOV,
		*currentCam;
	bool usingFirstPOV;

public:
	Player(Model* ship, SpotLight* spotlight, PerspectiveCamera* firstPOV, PerspectiveCamera* thirdPOV);
	void apply(Shader* model_shader, Shader* sky_shader);
	void render(Shader* model_shader);
	glm::vec3 getPosition();
	void move(glm::vec3 movement);
	void turn(float degrees, glm::vec3 axis);
	void panCamera(float degrees, glm::vec3 axis);
	void toggleCamera();
	glm::mat4 getRotationMat();
	bool isUsingFirstPOV();
};