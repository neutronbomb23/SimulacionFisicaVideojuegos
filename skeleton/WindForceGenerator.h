#pragma once
#include "ParticleDragGenerator.h"

class WindForceGenerator :
    public ParticleDragGenerator
{
public:
    WindForceGenerator(const Vector3& w, const float k1 = 0.25, const float k2 = 0);
    virtual void updateForce(Particle* particle, double t);
    void normalize(Vector3& vec);
    void setWindDirectionRandom(); // Sets a random wind direction
    inline void setWind(Vector3 w) { _wind = w; }  // Sets the wind vector

protected:
    Vector3 _wind;  // Wind vector
    Vector3 abs(Vector3 a);  // Function to get absolute value of a vector
    Vector3 prodVect(Vector3 a, Vector3 b);  // Function to get cross product of two vectors
    float d = 0.00119;  // Air density at 25 degrees Celsius
    float vol = 1000;  // Volume affected by wind
    float mass = d * vol;  // Mass calculation using volume and density
};