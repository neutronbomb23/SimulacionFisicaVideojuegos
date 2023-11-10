#pragma once
#include "WindForceGenerator.h"
class VortexForceGenerator :
    public WindForceGenerator
{
public:
    VortexForceGenerator(const float k, const Vector3& w) : K(k), WindForceGenerator(w) {}
    virtual void updateForce(Particle* particle, double t);
protected:
    float K;
    Vector3 calc(Vector3 c, Vector3 p);
};

