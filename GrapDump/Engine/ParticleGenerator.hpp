#pragma once

#include "Model.hpp"
#include "Physics/LifespanParticle.hpp"

using namespace Physics;
class ParticleGenerator
{
	private:
		int limit = 0;

	public:
		ParticleGenerator();
		ParticleGenerator(int limit);

	public:
		int toLimit(int num);
		LifespanParticle* GenerateLifespanParticle();

};

