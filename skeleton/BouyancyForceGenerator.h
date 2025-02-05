#pragma once
#include "ForceGenerator.h"
#include "core.hpp"
#include "RigidBody.h"

class BuoyancyForceGenerator : public ForceGenerator {
protected:
	float height, volume, liquidDensity, gravity = 9.8;
	Particle* liquidParticle;

public:
	BuoyancyForceGenerator(float Height, float Volume, float LiquidDensity);
	~BuoyancyForceGenerator();
	Particle* getPart() { return liquidParticle; }
	virtual void updateForce(Particle* particle, double t);
	virtual void updateForce(RigidBody* rb, double t);

};

