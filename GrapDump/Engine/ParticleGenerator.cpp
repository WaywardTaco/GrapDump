#include "ParticleGenerator.hpp"

using namespace Engine;

ParticleGenerator::ParticleGenerator() : position(0.f, 0.f, 0.f){}

ParticleGenerator::ParticleGenerator(int limit, Vector3 position) : 
    limit(limit), position(position){
        /* Creates a single model which all particles share as a base */
        this->particleTemplate = new Model("3D/sphere.obj", glm::vec3(0.f, 0.f, 0.f));
    }

int ParticleGenerator::toLimit(int num) {
    /* Returns the remaining particles that can still be generated */
    if (this->limit - num > 0) return this->limit - num;
    return 0;
}

LifespanParticle* ParticleGenerator::GenerateLifespanParticle() {
    /* Creates a particle with a lifespan */
    
    /* Generates a random force to apply to a particle initially */
    float xForce = rand() % 111 + 10 - 65;
    float yForce = rand() % 111 + 10;
    float zForce = rand() % 111 + 10 - 65;
    Vector3 initialForce = Vector3 (xForce * 100, yForce * 100, zForce * 100);

    /* Randomizes the color, size, and lifespan of a particle */
    float r = rand() % 256 / 255.0;
    float g = rand() % 256 / 255.0;
    float b = rand() % 256 / 255.0;
    float scale = rand() % 9 + 2;
    double life = rand() % 10 + 1;

    /* Creates a particle and applies the appropriate values */
    RenderParticle particle(new Particle(1.0f), this->particleTemplate, Vector3(r, g, b), scale);
    particle.Base()->position = this->position;
    particle.Base()->AddForce(initialForce);
    return new LifespanParticle(life, particle);
}