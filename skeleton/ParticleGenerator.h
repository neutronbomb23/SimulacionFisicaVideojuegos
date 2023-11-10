#pragma once
#include "Particle.h"
#include "ParticleSystem.h"

// The ParticleGenerator class is responsible for generating particles at regular intervals.
class ParticleGenerator {
private:
    ParticleSystem* particleSystem; // The particle system managed by this generator
    float generationTimer; // Timer to control the rate of particle generation
    bool isGenerating; // Flag to determine if particles should be generated

public:
    ParticleGenerator();
    ~ParticleGenerator();

    // Update the generator each frame
    void updateEveryFrame(float timeElapsed);

    // Generate a random 3D vector within a specified range
    Vector3 randomVectorInRange(int minSize, int maxSize);

    // Generate a random color
    Vector4 generateRandomColor();

    // Check if particle generation is enabled
    bool isGeneratingParticles();

    // Generate and add a new particle to the system
    void createParticle();

    // Toggle particle generation
    void toggleGeneration();

    // Choose a random geometry for particles
    PxGeometry* chooseRandomGeometry();

    // Generate a random mass within a range
    float randomMass(float minMass, float maxMass);

    // Get the particle system
    ParticleSystem* getSystem() { return particleSystem; }
};
