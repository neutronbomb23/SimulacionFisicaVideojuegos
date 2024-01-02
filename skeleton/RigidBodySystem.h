#pragma once
#include "core.hpp"
#include <vector>
#include <list>
#include "RigidBody.h"
#include "RigidBodyGenerator.h"
#include "RigidBodyForceRegistry.h"
#include "GravityForceGenerator.h"
#include "WindForceGenerator.h"
#include "VortexForceGenerator.h"
#include "BurstForceGenerator.h"

using namespace std;

const int MAXRBS = 100;

enum GeneratorType { g_sphere, g_capsule, g_cube };

class RigidBodySystem {
protected:
	PxScene* scene;
	PxPhysics* physics;

	int numRB;
	list<RigidBody*> rbs;
	list<RigidBody*> rbsToDelete;
	vector<RigidBodyGenerator*> rbGenerators;

	RigidBodyForceRegistry* forceRegistry;
	list<ForceGenerator*> forceGenerators;

public:
	RigidBodySystem(PxScene* Scene, PxPhysics* Physics);
	void update(double t);
	void deleteUnusedRB();
	void addRBS(list<RigidBody*> lrb);

	void createGenerators(GeneratorType T);
	void shootRB();

	void addGravity() {
		GravityForceGenerator* g = new GravityForceGenerator(Vector3(0, 20, 0));
		for (auto rb : rbs)
			forceRegistry->addRegistry(g, rb);
	}
	void addWind() {
		WindForceGenerator* wind = new WindForceGenerator(Vector3(-60, 0, 0), 0.25, 0.1);
		for (auto rb : rbs)
			forceRegistry->addRegistry(wind, rb);
	}
	void addVortex() {
		VortexForceGenerator* t = new VortexForceGenerator(2, Vector3(-60, 0, 0));
		for (auto rb : rbs)
			forceRegistry->addRegistry(t, rb);
	}
	void addExplosion() {
		BurstForceGenerator* megumin = new BurstForceGenerator(1000, 1500, 2);
		for (auto rb : rbs)
			forceRegistry->addRegistry(megumin, rb);
	}
};


