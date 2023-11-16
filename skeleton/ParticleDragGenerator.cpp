#include "ParticleDragGenerator.h"
#include <iostream>

// Implementation of the updateForce method
void ParticleDragGenerator::updateForce(Particle* particle, double t) {
    // Check that the particle has finite mass to avoid division by zero
    if (fabs(particle->inverse_massS) < 1e-10) return;

    // Compute the drag force
    Vector3 v = particle->getVel(); // Get particle velocity
    float drag_coef = v.normalize(); // Normalize the velocity and get its magnitude
    Vector3 dragF; // Variable to store drag force
    drag_coef = _k1 * drag_coef + _k2 * drag_coef * drag_coef; // Calculate drag coefficient
    dragF = -v * drag_coef; // Calculate drag force

    // Apply the drag force to the particle
    particle->addForce(dragF);
}