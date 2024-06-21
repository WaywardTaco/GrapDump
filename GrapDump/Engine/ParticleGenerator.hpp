#pragma once

#include "Physics/LifespanParticle.hpp"
#include "Model.hpp"
#include "Vector3.hpp"

namespace Physics {
	class LifespanParticle;
}

namespace Engine {
using namespace Physics;

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

