#include "BouyancyForceGenerator.h"

BuoyancyForceGenerator::BuoyancyForceGenerator(float Height, float Volume, float LiquidDensity) {
	height = Height;
	volume = Volume;
	liquidDensity = LiquidDensity;
	Vector4 color = Vector4(0.1, 0.5, 0.6, 0.7);
	PxTransform tr = PxTransform(0, 0, 0);
	liquidParticle = new Particle(CreateShape(PxBoxGeometry(110, 110, 110)), tr, Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(0, 0, 0), 0.85, color);
	RegisterRenderItem(liquidParticle->getRenderItem());
}

BuoyancyForceGenerator::~BuoyancyForceGenerator() {
	delete(liquidParticle);
}

void BuoyancyForceGenerator::updateForce(Particle* particle, double t) {
	if (fabs(particle->getInvMass()) < 1e-10) return;
	else {
		const float h = particle->getTransform()->p.y;
		const float h0 = liquidParticle->getTransform()->p.y;

		Vector3 BuoyancyForce(0, 0, 0);
		float inmersed = 0.0f;
		if (h - h0 < height * 0.5f) {
			inmersed = 0.0f;
		}
		else if (h0 - h > height * 0.5f) {
			inmersed = 1.0f;
		}
		else {
			inmersed = (h0 - h) / height + 0.5f;
		}

		BuoyancyForce.y = liquidDensity * volume * inmersed * gravity;
		//if (particle->getTransform()->p.y >= height)BuoyancyForce.y = -BuoyancyForce.y;

		particle->addForce(BuoyancyForce);
	}
}

void BuoyancyForceGenerator::updateForce(RigidBody* rb, double t) {
	if (fabs(rb->getInvMass()) < 1e-10) return;
	else {
		const float h = rb->getPosition().y;
		const float h0 = liquidParticle->getTransform()->p.y;

		Vector3 BuoyancyForce(0, 0, 0);
		float inmersed = 0.0f;
		if (h - h0 > height * 0.5f) {
			inmersed = 0.0f;
		}
		else if (h0 - h > height * 0.5f) {
			inmersed = 1.0f;
		}
		else {
			inmersed = (h0 - h) / height + 0.5f;
		}

		BuoyancyForce.y = liquidDensity * volume * inmersed * gravity;
		//if (particle->getTransform()->p.y >= height)BuoyancyForce.y = -BuoyancyForce.y;

		rb->addForce(BuoyancyForce);
	}
}