#pragma once
#include "ForceGenerator.h"
using namespace std;

// ParticleDragGenerator class, derived from ForceGenerator
class ParticleDragGenerator :
    public ForceGenerator
{
public:
    // Default constructor
    ParticleDragGenerator() {};

    // Constructor with drag coefficients
    ParticleDragGenerator(const float k1, const float k2) : _k1(k1), _k2(k2) {}

    // Method to update force based on drag, to be implemented in derived classes
    virtual void updateForce(Particle* particle, double t);

    // Setters for drag coefficients
    inline void setDrag(float k1, float k2) { _k1 = k1; _k2 = k2; }
    inline float getK1() { return (_k1); }
    inline float getK2() { return (_k2); }

protected:
    float _k1; // Coefficient for velocity-based drag
    float _k2; // Coefficient for square velocity-based drag
};