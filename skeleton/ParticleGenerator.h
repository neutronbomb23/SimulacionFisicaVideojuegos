#pragma once
#include "particle.h"
#include "ParticleSystem.h"
class ParticleGenerator
{
private:
	ParticleSystem* partSyst;
	float timeSinceLastGeneration;
	float generationInterval;
public:
	ParticleGenerator();
	~ParticleGenerator();
	Vector3 UniformDistribution(int size);
	Vector3 NormalDistribution(float m, float d);
	float RandomNormal(float m, float d);
	Vector3 UniformDirection();
	void update(float t);
	void generate();
	void generatePart(PxTransform pos);
	void generateFire(PxTransform pos, Vector3 vel, Vector3 acc, Vector3 grav, float weight, float damping, Vector4 c, float radius, int gen);

	Vector4 RandomColor();
};

