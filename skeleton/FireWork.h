#pragma once
#include "RenderUtils.hpp"
#include <iostream>
#include "Particle.h"
using namespace physx;
class ParticleSystem;
class Firework
{
    const float pi = 3.14159265359f;
private:
    ParticleSystem* partS;
    Vector3 vel;
    PxTransform trans;
    RenderItem* rend;
    Vector3 acel;
    Vector3 gS;
    int gen;
    float damping;
    float lifetime = 0.0f;
    bool dest = false;
public:
    Firework(PxTransform pos, Vector3 vel, Vector3 acc, Vector3 grav, float damping, Vector4 c, ParticleSystem* pS, int generation = 0);
    Firework(PxTransform pos, Vector3 dir, int generation, ParticleSystem* pS);
    ~Firework();

    void update(double t);
    void integrate(double t);


    RenderItem* getRend() const { return rend; }
    PxTransform* getPos() { return &trans; }
    bool getDest() { return dest; }
};