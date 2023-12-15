#include "GravityForceGenerator.h"


void GravityForceGenerator::updateForce(Particle* p, double t) {
	// Check that the particle has Finite Mass
	if (fabs(p->inverse_massS) < 1e-10)return;

	// Apply the mass scaled gravity
	p->addForce(_gravity * p->massS);
}

void GravityForceGenerator::updateForce(RigidBody* rb, double t) {
	// Check that the particle has Finite Mass
	if (fabs(rb->getInvMass()) < 1e-10)return;

	// Apply the mass scaled gravity
	rb->addForce(_gravity * rb->getMass());
}