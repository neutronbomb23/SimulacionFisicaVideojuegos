#pragma once
#include "ForceGenerator.h"
#include "RigidBody.h"
const double e = 2.7182818284;
class BurstForceGenerator :
    public ForceGenerator
{
public:
    BurstForceGenerator(const float k, const float r, const float t) : K(k), R(r), T(t) {}
    virtual void updateForce(Particle* particle, double t);
    void updateForce(RigidBody* rb, double t);
protected:
    float K /*intensidad*/;
    float T /*constante de tiempo*/ ;
    float R /*radio de la explosión*/;
    float calcR(Vector3 c, Vector3 p);
};

