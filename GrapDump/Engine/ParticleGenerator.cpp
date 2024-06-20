#include "ParticleGenerator.hpp"

ParticleGenerator::ParticleGenerator() {}

ParticleGenerator::ParticleGenerator(int limit) : 
    limit(limit){}

int ParticleGenerator::toLimit(int num) {
    if (this->limit - num > 0) return this->limit - num;
    return 0;
}

LifespanParticle* ParticleGenerator::GenerateLifespanParticle() {
    Model* particleSphere = new Model("3D/sphere.obj", glm::vec3(0.f, 0.f, 0.f));

    float r = rand() % 256 / 255.0;
    float g = rand() % 256 / 255.0;
    float b = rand() % 256 / 255.0;
    float scale = rand() % 9 + 2;
    double life = rand() % 10 + 1;

    RenderParticle particle(new Particle(1.0f), particleSphere, Vector3(r, g, b), scale);
    particle.Base()->position = Vector3(0.0, 1.0 * 50, 0.0);
    return new LifespanParticle(life, particle);
}