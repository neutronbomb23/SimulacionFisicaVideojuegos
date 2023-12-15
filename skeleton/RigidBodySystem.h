#pragma once
#include "core.hpp"
#include <vector>
#include <list>
#include <random>

#include "RigidBody.h"
#include "RigidBodyGenerator.h"
#include "RigidBodyForceRegistry.h"
#include "GravityForceGenerator.h"
#include "WindForceGenerator.h"
#include "VortexForceGenerator.h"
#include "BurstForceGenerator.h"

const int MAXRBS = 20;

enum GeneratorType { g_sphere, g_capsule, g_cube, g_cylinder, g_cone, g_pyramid, g_torus};

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

	void createGenerators(GeneratorType T);
	void shootRB();
	void addExplosion() {
		BurstForceGenerator* megumin = new BurstForceGenerator(4000, 2000, 5); // radio y tiempo
		for (auto rb : rbs)
			forceRegistry->addRegistry(megumin, rb);
	}

	Vector4 RigidBodySystem::generateRandomColor() {
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<> dis(0, 1);

		Vector4 color(dis(gen), dis(gen), dis(gen), 1.0); // RGBA format
		return color;
	}

	RigidBodySystem(PxScene* Scene, PxPhysics* Physics);
	void update(double t);
	void deleteUnusedRB(); // ELIMINAR LOS QUE YA LLEGEN AL LIMITE DE TIEMPO O 
	void addRBS(list<RigidBody*> lrb);

};

