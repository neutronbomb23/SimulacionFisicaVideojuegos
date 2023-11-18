#include "ParticleSystem.h"

// Destructor: Cleans up memory by deleting particles, fireworks, and the force registry
ParticleSystem::~ParticleSystem() {
    for (auto& particle : particleList) {
        forceRegistry->deleteParticleRegistry(particle);
        delete particle;
    }
    for (auto& firework : fireworkList) {
        delete firework;
    }
    delete forceRegistry;
}

// Update method: Called each frame to update particles and fireworks
void ParticleSystem::performUpdate(float duration) {
    if (areForcesActive) {
        forceRegistry->updateForces(duration);
    }
    auto particleIt = particleList.begin();
    while (particleIt != particleList.end()) {
        auto nextIt = particleIt;
        ++nextIt;
        if ((*particleIt)->getDest()) {
            forceRegistry->deleteParticleRegistry(*particleIt);
            delete* particleIt;
            particleList.erase(particleIt);
        }
        else {
            (*particleIt)->update(duration);
        }
        particleIt = nextIt;
    }

   

    
}

// Add a new particle to the system
void ParticleSystem::addNewParticle(Particle* particle) {
    particleList.push_back(particle);
    // Additional force applications can be added here if needed
}

// Add a new firework to the system
void ParticleSystem::addNewFirework(Firework* firework) {
    fireworkList.push_back(firework);
}

// Toggle the application of forces to particles
void ParticleSystem::toggleParticleForces() {
    areForcesActive = !areForcesActive;
}

// Apply a burst force to all particles
void ParticleSystem::applyBurstToAllParticles() {
    for (auto& particle : particleList) {
        applyBurstForce(particle);
    }
}

// Apply a tornado movement force to all particles
void ParticleSystem::applyTornadoToAllParticles() {
    for (auto& particle : particleList) {
        applyVortexForce(particle);
    }
}

// Apply a vientecillo force to all particles
void ParticleSystem::applyVentiscaToAllParticles() {
    for (auto& particle : particleList) {
        applyWindForce(particle);
    }
}

// Apply gravity force to all particles
void ParticleSystem::applyGravityToAllParticles() {
    for (auto& particle : particleList) {
        applyGravityForce(particle);
    }
}

// Apply gravity force to a single particle
void ParticleSystem::applyGravityForce(Particle* particle) {
    float simulatedVelocity = particle->getVelS(); // Simulated velocity in m/s
    float realVelocity = particle->getVelR(); // Real velocity in m/s
    Vector3 realGravity = Vector3(0, -9.8f, 0); // Real gravity
    Vector3 lunarGravity = Vector3(0, -1.63f, 0); // Gravedad lunar
    Vector3 simulatedGravity = realGravity * powf((simulatedVelocity / realVelocity), 2); // Simulated gravity
    GravityForceGenerator* gravity = new GravityForceGenerator(lunarGravity);
    forceRegistry->addRegistry(gravity, particle);
}

// Apply wind force to a single particle
void ParticleSystem::applyWindForce(Particle* particle) {
    Vector3 windDirection = Vector3(-10, 0, 0); // Wind direction and strength
    float airFrictionCoefficient = 0.25, secondaryCoefficient = 0.1;
    WindForceGenerator* wind = new WindForceGenerator(windDirection, airFrictionCoefficient, secondaryCoefficient);
    forceRegistry->addRegistry(wind, particle);
}

// Apply vortex force to a single particle
void ParticleSystem::applyVortexForce(Particle* particle) {
    Vector3 vortexBaseWind = Vector3(-10, 0, 0); // Base wind direction and strength for the vortex
    float vortexForceConstant = 0.01; // Vortex force constant
    VortexForceGenerator* vortex = new VortexForceGenerator(vortexForceConstant, vortexBaseWind);
    forceRegistry->addRegistry(vortex, particle);
}

void ParticleSystem::clearAllParticles() {
    // Iterate through the particle list and delete each particle
    for (auto& particle : particleList) {
        forceRegistry->deleteParticleRegistry(particle); // Remove the particle from the force registry
        delete particle; // Delete the particle
    }

    // Clear the list after deleting all particles
    particleList.clear();
}


// Apply burst force to a single particle
void ParticleSystem::applyBurstForce(Particle* particle) {
    float explosionForceConstant = 100; // Explosion force constant
    float range = 1500; // Range of the burst effect
    float duration = 2; // Duration of the effect
    BurstForceGenerator* burst = new BurstForceGenerator(explosionForceConstant, range, duration);
    forceRegistry->addRegistry(burst, particle);
}
