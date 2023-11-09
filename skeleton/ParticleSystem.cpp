#include "ParticleSystem.h"

ParticleSystem::ParticleSystem() {

}

ParticleSystem::~ParticleSystem() {
    for (auto& i : particles) delete i;
}

void ParticleSystem::update(float t) {
    PartUpdate(t);
    FireUpdate(t);
}

void ParticleSystem::PartUpdate(float t) {
    auto it = particles.begin();
    while (it != particles.end()) {
        auto aux = it;
        ++aux;
        if ((*it)->getDestroyed()) {
            delete* it;
            particles.erase(it);
        }
        else (*it)->update(t);
        it = aux;
    }
}

void ParticleSystem::FireUpdate(float t) {
    auto it = fireworks.begin();
    while (it != fireworks.end()) {
        auto aux = it;
        ++aux;
        if ((*it)->getDestroyed()) {
            delete* it;
            fireworks.erase(it);
        }
        else (*it)->update(t);
        it = aux;
    }
}

void ParticleSystem::addParticle(particle* p) {
    particles.push_back(p);
}

void ParticleSystem::addFirework(Firework* f) {
    fireworks.push_back(f);
}
