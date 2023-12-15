#include "WindForceGenerator.h"
void WindForceGenerator::updateForce(Particle* p, double t) {
	// Check that the particle has Finite Mass
	if (fabs(p->inverse_massS) < 1e-10)return;
	Vector3 v = p->getVel();
	Vector3 windForce = _k1 * (_wind - v) + _k2 * prodVect((abs(_wind - v)), (_wind - v));
	p->addForce(windForce);
}

Vector3 WindForceGenerator::abs(Vector3 a) {
	Vector3 b = Vector3(0, 0, 0);
	b = Vector3(pow(a.x, 2), pow(a.y, 2), pow(a.z, 2));
	b = Vector3(sqrtf(b.x), sqrtf(b.y), sqrtf(b.z));
	return b;
}
Vector3 WindForceGenerator::prodVect(Vector3 a, Vector3 b) {
	return Vector3((a.y * b.z - a.z * b.y), (a.x * b.z - a.z * b.x), (a.x * b.y - a.y * b.x));
}

void WindForceGenerator::updateForce(RigidBody* rb, double t) {
	// Check that the particle has Finite Mass
	if (fabs(rb->getInvMass()) < 1e-10)return;
	Vector3 v = rb->getLinearVelocity();
	Vector3 windForce = _k1 * (_wind - v) + _k2 * prodVect((abs(_wind - v)), (_wind - v));
	rb->addForce(windForce);
}