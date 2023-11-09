#include "Particle.h"

// Constructor implementation.
Particle::Particle(Vector3 Pos, Vector3 Vel) : vel(Vel)
{
    position = physx::PxTransform(Pos.x, Pos.y, Pos.z); // Set the initial position of the particle.
    // Create a render item with a sphere geometry, link it to the position, and give it a color.
    renderItem = new RenderItem(CreateShape(physx::PxSphereGeometry(0.5)), &position, { 0.0, 8.0, 5.0, 1 });
}

// Destructor implementation.
Particle::~Particle()
{
    // Remove the render item from the scene to prevent it from being rendered after destruction.
    DeregisterRenderItem(renderItem);
}

// Integrate implementation: updates the particle's position based on its velocity and time step.
void Particle::integrate(double t)
{
    // Calculate new position using the velocity and time step, then update the transform.
    position = physx::PxTransform(position.p.x + vel.x * t, position.p.y + vel.y * t, position.p.z + vel.z * t);
}