#include "VortexForceGenerator.h"

void VortexForceGenerator::updateForce(Particle* p, double t) {
	// Check that the particle has Finite Mass
	if (fabs(p->inverse_massS) < 1e-10)return;

	Camera* camera = GetCamera();

	Vector3 c = Vector3(camera->getTransform().p.x + 150, camera->getTransform().p.y - 350, camera->getTransform().p.z - 150);
	//Vector3 c = Vector3(0, 0, 0);
	Vector3 pos = p->getTransform()->p;
	Vector3 vortexForce = K * calc(c, pos);
	p->addForce(vortexForce);
}

Vector3 VortexForceGenerator::calc(Vector3 c, Vector3 p) {
	//return Vector3(-(p.x - c.x), (p.y - c.y) - 50, (p.z - c.z));
	//return Vector3((p.x - c.x), 50 - (p.y - c.y), -(p.z - c.z));
	return Vector3(-(p.z - c.z), 50 - (p.y - c.y), (p.x - c.x));
}

void VortexForceGenerator::updateForce(RigidBody* rb, double t) {
	// Check that the particle has Finite Mass
	if (fabs(rb->getInvMass()) < 1e-10)return;

	Camera* camera = GetCamera();

	//Vector3 c = Vector3(camera->getTransform().p.x + 150, camera->getTransform().p.y - 350, camera->getTransform().p.z - 150);
	Vector3 c = Vector3(0, 0, 0);
	Vector3 pos = rb->getPosition();
	Vector3 vortexForce = K * calc(c, pos);
	rb->addForce(vortexForce);
}
