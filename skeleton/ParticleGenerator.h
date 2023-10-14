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
	Vector4 RandomColor();
};

