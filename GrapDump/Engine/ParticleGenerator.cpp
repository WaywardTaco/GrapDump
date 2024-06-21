#include "ParticleGenerator.hpp"

ParticleGenerator::ParticleGenerator() : position(0.f, 0.f, 0.f){}

ParticleGenerator::ParticleGenerator(int limit, Vector3 position) : 
    limit(limit), position(position){
        this->particleTemplate = new Model("3D/sphere.obj", glm::vec3(0.f, 0.f, 0.f));
    }

int ParticleGenerator::toLimit(int num) {
    if (this->limit - num > 0) return this->limit - num;
    return 0;
}

LifespanParticle* ParticleGenerator::GenerateLifespanParticle() {

    float xForce = rand() % 111 + 10 - 65;
    float yForce = rand() % 111 + 10;
    float zForce = rand() % 111 + 10 - 65;
    Vector3 initialForce = Vector3 (xForce * 100, yForce * 100, zForce * 100);

    float r = rand() % 256 / 255.0;
    float g = rand() % 256 / 255.0;
    float b = rand() % 256 / 255.0;
    float scale = rand() % 9 + 2;
    double life = rand() % 10 + 1;

    RenderParticle particle(new Particle(1.0f), this->particleTemplate, Vector3(r, g, b), scale);
    particle.Base()->position = this->position;
    particle.Base()->AddForce(initialForce);
    return new LifespanParticle(life, particle);
}