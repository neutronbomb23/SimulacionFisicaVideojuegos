#pragma once
#include <list>
#include "Particle.h"
#include "Firework.h"
#include "ParticleForceRegistry.h"
#include "GravityForceGenerator.h"
#include "WindForceGenerator.h"
#include "VortexForceGenerator.h"
#include "BurstForceGenerator.h"
using namespace std;

class ParticleSystem {
private:
    ParticleForceRegistry* forceRegistry; // Registry for managing particle forces
    list<Particle*> particleList; // List of particles in the system
    list<Firework*> fireworkList; // List of fireworks in the system
    bool areForcesActive; // Flag to toggle forces on and off

public:
    // Constructor: Initializes the particle force registry
    ParticleSystem() { forceRegistry = new ParticleForceRegistry(); areForcesActive = true; }

    // Destructor: Cleans up memory by deleting particles, fireworks, and the force registry
    ~ParticleSystem();

    // Update method: Called each frame to update particles and fireworks
    void performUpdate(float duration);

    // Particle management
    void addNewParticle(Particle* particle);
    void addNewFirework(Firework* firework);

    // Force management
    void toggleParticleForces();
    void applyBurstToAllParticles();
    void applyTornadoToAllParticles();
    void applyVentiscaToAllParticles();
    void applyGravityToAllParticles();

    // Force application methods
    void applyGravityForce(Particle* particle);
    void applyWindForce(Particle* particle);
    void applyVortexForce(Particle* particle);
    void clearAllParticles();
    void applyBurstForce(Particle* particle);
};