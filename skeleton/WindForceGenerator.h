#pragma once
#include "ParticleDragGenerator.h"
class WindForceGenerator :
    public ParticleDragGenerator
{
public:
    WindForceGenerator(const Vector3& w, const float k1 = 0.25, const float k2 = 0) : ParticleDragGenerator(k1, k2), _wind(w) {}
    virtual void updateForce(Particle* particle, double t);
    virtual void updateForce(RigidBody* rb, double t);
    inline void setWind(Vector3 w) { _wind = w; }
protected:
    Vector3 _wind;
    float d = 0.00119/*kg / L*/;//densidad del aire a 25º (1.19 g/L)
    float vol = 1000 /*L*/;// volumen sobre el que se va a ejercer el viento
    float mass = d * vol /*kg*/;

    Vector3 abs(Vector3 a);
    Vector3 prodVect (Vector3 a, Vector3 b);
};

