#pragma once
#include "Particle.h"
#include "ParticleSystem.h"
class ParticleGenerator
{
private:
	ParticleSystem* partSyst;
	float timer = 1.0f;
public:
	ParticleGenerator();
	~ParticleGenerator();
	Vector3 UniformDistribution(int size);
	Vector3 NormalDistribution(float m, float d);
	float RandomNormal(float m, float d);
	void update(float t);
	void generate(int gen = 0);
	ParticleSystem* getSys() { return partSyst; }

};

