#pragma once
#include "WindForceGenerator.h"

// VortexForceGenerator class, derived from WindForceGenerator
class VortexForceGenerator :
    public WindForceGenerator
{
public:
    // Constructor
    VortexForceGenerator(const float k, const Vector3& w) : K(k), WindForceGenerator(w) {}

    // Overridden method to update force based on vortex behavior
    virtual void updateForce(Particle* particle, double t);

protected:
    float K; // Vortex strength constant
    Vector3 calc(Vector3 c, Vector3 p); // Method to calculate the vortex force
};