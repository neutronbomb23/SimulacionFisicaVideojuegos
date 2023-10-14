#pragma once
#include "RenderUtils.hpp"
#include <iostream>
using namespace physx;
// La clase particle representa una partícula en la simulación.
class particle
{
private:
    Vector3 v;        
    Vector4 color;        
    PxTransform p; 
    RenderItem* rend; 
    Vector3 acceleration, gravity;    
    float radius;
    float damp;     
    float mass;
    float lifetime = 0.0f;
    bool dest = false; 

public:
    particle(PxTransform pos, Vector3 vel, Vector3 acc, Vector3 grav, float weight, float damping, Vector4 c, float radius);
    particle(PxTransform pos);
    ~particle();

    void update(double t);
    void setColor(const Vector4& newColor);
    void integrate(double t);
    Vector3 RandVect(int size);
    void setupFireball();
    void setupCanonball();
    void setupLaser();
    RenderItem* getRend() const { return rend; }
    PxTransform* getPos() { return &p; }
    bool getDestroyed() { return dest; }
};
