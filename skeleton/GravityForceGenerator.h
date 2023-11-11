#pragma once
#include "ForceGenerator.h"
#include "core.hpp"
using namespace std;

class GravityForceGenerator :
    public ForceGenerator
{
public:
    GravityForceGenerator(const Vector3& g) :_gravity(g) {}
    virtual void updateForce(Particle* particle, double t);
  
protected:
    Vector3 _gravity;
};

