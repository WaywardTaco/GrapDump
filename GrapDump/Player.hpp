
#pragma once

#include "Model.hpp"
#include "PointLight.hpp"
#include "PerspectiveCamera.hpp"

class Player {
private:
	Model* ship;
	PointLight* spotlight;
	PerspectiveCamera 
		*firstPOV,
		*thirdPOV,
		*currentCam;
	bool usingFirstPOV;

public:
	Player(Model* ship, PointLight* spotlight, PerspectiveCamera* firstPOV, PerspectiveCamera* thirdPOV);
	void apply(Shader* normal_shader, Shader* monochrome_shader, Shader* skybox_shader, Shader* mono_sky_shader);
	void render(Shader* normal_shader, Shader* monochrome_shader);
	glm::vec3 getPosition();
	void move(glm::vec3 movement);
	void turn(float degrees, glm::vec3 axis);
	void panCamera(float degrees, glm::vec3 axis);
	void toggleCamera();
	glm::mat4 getRotationMat();
	bool isUsingFirstPOV();
};