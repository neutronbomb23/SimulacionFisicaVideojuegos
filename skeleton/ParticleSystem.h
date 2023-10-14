#pragma once
#include <list>
#include "particle.h"
#include "Firework.h"
using namespace std;

class ParticleSystem
{
private:
	list<particle*> particles;
	list<Firework*> fireworks;

public:
	ParticleSystem();
	~ParticleSystem();
	void update(float t);
	void FireUpdate(float t);
	void PartUpdate(float t);
	void addParticle(particle* p);
	void addFirework(Firework* f);
};

