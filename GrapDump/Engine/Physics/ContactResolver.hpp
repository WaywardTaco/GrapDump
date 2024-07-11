#pragma once

#include "ParticleContact.hpp"
#include <vector>

namespace Physics {
	class ContactResolver{
		public:
			unsigned max_iterations;
			ContactResolver(unsigned _maxIterations) : max_iterations(_maxIterations){}
			void ResolveContacts(std::vector<ParticleContact*> contacts, double deltaTime);

		protected:
			unsigned current_iterations = 0;

	};
}

