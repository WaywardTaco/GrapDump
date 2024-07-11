#include "ContactResolver.hpp"

using namespace Physics;

void ContactResolver::ResolveContacts(std::vector<ParticleContact*> contacts, double deltaTime) {
	current_iterations = 0;
	
	while(current_iterations < max_iterations){
		unsigned int current_index = 0;
		float curr_min = contacts[0]->GetSeparatingSpeed();
		float max_depth = contacts[0]->depth;

		for (unsigned i = 0; i < contacts.size(); i++) {
			float ss = contacts[i]->GetSeparatingSpeed();

			if (ss < curr_min &&
				(ss < 0 || 0 < contacts[i]->depth)) {
				current_index = i;
				curr_min = ss;

				if (max_depth < contacts[i]->depth) {
					max_depth = contacts[i]->depth;
				}
			}
		}

		if (curr_min >= 0 && max_depth <= 0)
			return;

		contacts[current_index]->Resolve(deltaTime);
		current_iterations++;
	}
}