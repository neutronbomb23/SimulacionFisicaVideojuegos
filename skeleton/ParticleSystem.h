#pragma once
#include <list>
#include "Particle.h"
#include "Firework.h"
#include "ParticleForceRegistry.h"
#include "GravityForceGenerator.h"
#include "WindForceGenerator.h"
#include "VortexForceGenerator.h"
#include "BurstForceGenerator.h"
#include "SpringForceGenerator.h"
#include "AnchoredSpringFG.h"
#include "BouyancyForceGenerator.h"
using namespace std;

class ParticleSystem
{
public:enum SpringType{SPRING, ANCHORED, FLOTABILITY, BUOYANCY, GOMAELASTICA, NONE};
private:
	ParticleForceRegistry* partRgis;
	list<Particle*> particles;
	list<Firework*> fireworks;
	SpringForceGenerator* f1 = nullptr;
	SpringForceGenerator* f2 = nullptr;
	AnchoredSpringFG* f3 = nullptr;
	int current = 0;
	WindForceGenerator* w = nullptr;
	BurstForceGenerator* b = nullptr;
	//list<SpringForceGenerator*> springs;

public:
	ParticleSystem() { partRgis = new ParticleForceRegistry();  }
	~ParticleSystem();
	void update(float t);
	void addParticle(Particle* p);
	void addFirework(Firework* f);
	void addExplosion();
	void quitaBurst();
	void generateSpringDemo(SpringType type);
	void generateBuoyancy();
	void addK(int k);
	ParticleForceRegistry* getPartRegis() { return partRgis; }
protected:
	void addGravity(Particle* p);
	void addWind(Particle* p);
	void addVortex(Particle* p);
	void addBurst(Particle* p);
	void activateBuoyancy() {
		BuoyancyForceGenerator* water = new BuoyancyForceGenerator(6, 0.5, 1000);
		Vector4 color = Vector4(1, 0, 0, 1);
		PxTransform tr = PxTransform(0, 20, 0);
		Particle* p = new Particle(CreateShape(PxBoxGeometry(1, 1, 1)), tr, Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(0, 0, 0), 1, color, this);

		tr = PxTransform(10, 20, 0);
		color = Vector4(1, 1, 0, 1);
		Particle* p1 = new Particle(CreateShape(PxBoxGeometry(1, 1, 1)), tr, Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(0, 0, 0), 1, color, this);
		p1->setMass(3000);

		tr = PxTransform(20, 20, 0);
		color = Vector4(0, 1, 0, 1);
		Particle* p2 = new Particle(CreateShape(PxBoxGeometry(1, 1, 1)), tr, Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(0, 0, 0), 1, color, this);
		p2->setMass(20);

		tr = PxTransform(30, 10, 0);
		color = Vector4(0, 1, 1, 1);
		Particle* p3 = new Particle(CreateShape(PxBoxGeometry(4, 4, 4)), tr, Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(0, 0, 0), 1, color, this);
		p3->setMass(120);

		particles.push_back(p);
		particles.push_back(p1);
		particles.push_back(p2);
		particles.push_back(p3);
		partRgis->addRegistry(water, p);
		partRgis->addRegistry(water, p1);
		partRgis->addRegistry(water, p2);
		partRgis->addRegistry(water, p3);

		buoyancyTest = p2;
	}

private:
	Particle* buoyancyTest = nullptr;

public:
	void addTestMass(int M) {
		if (buoyancyTest != nullptr) {
			if (buoyancyTest->massS + M > 0) {
				buoyancyTest->setMass(buoyancyTest->massS + M);
				cout << buoyancyTest->massS << endl;
			}
		}
	}
	
};