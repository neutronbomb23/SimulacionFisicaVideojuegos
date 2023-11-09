#pragma once
#include "core.hpp"
#include "RenderUtils.hpp"

class Particle
{
public:
    // Constructor: Initializes a new Particle with a position and velocity.
    Particle(Vector3 Pos, Vector3 Vel);

    // Destructor: Cleans up resources used by the Particle.
    ~Particle();

    // integrate: Updates the particle's position based on its velocity and the time step.
    void integrate(double t);

protected:

private:
    Vector3 vel; // The velocity of the particle.
    physx::PxTransform position; // The position of the particle in the physics world.
    RenderItem* renderItem; // The visual representation of the particle for rendering.
};