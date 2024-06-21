#pragma once

#include "Model.hpp"
#include "Physics/LifespanParticle.hpp"
#include "Vector3.hpp"

using namespace Physics;
namespace Engine {

class ParticleGenerator
{
	private:
		int limit = 0;
		Model* particleTemplate;
		Vector3 position;

	public:
		ParticleGenerator();
		ParticleGenerator(int limit, Vector3 position);

	public:
		int toLimit(int num);
		LifespanParticle* GenerateLifespanParticle();

};
}

