#include "RigidBodySystem.h"

RigidBodySystem::RigidBodySystem(PxScene* Scene, PxPhysics* Physics):scene(Scene), physics(Physics), numRB(0) {

	forceRegistry = new RigidBodyForceRegistry();
}

void RigidBodySystem::update(double t) {
	//Generar
	for (RigidBodyGenerator* g : rbGenerators) { //Generadores de partículas
		addRBS(g->generateBodies());
		g->update(t);
	}
	forceRegistry->updateForces(t); // Actualiza las fuerzas ejercidas sobre los RB
	for (RigidBody* rb : rbs) { // RB
		if (rb->isAlive()) { rb->integrate(t); }
		else { rbsToDelete.push_back(rb); }
	}

	deleteUnusedRB();
}

void RigidBodySystem::deleteUnusedRB() {
	for (auto it = rbsToDelete.begin(); it != rbsToDelete.end();) {
		forceRegistry->deleteRigidBodyRegistry(*it);
		rbs.remove(*it);
		delete(*it);
		it = rbsToDelete.erase(it);
		numRB--;
	}
}

void RigidBodySystem::addRBS(list<RigidBody*> lrb) {
	for (RigidBody* r : lrb) {
		if (rbs.size() < MAXRBS) {
			rbs.push_back(r);
			numRB++;
			for (auto fg : forceGenerators) // Añade las particulas al registro de fuerzas 
				forceRegistry->addRegistry(fg, r);
		}
		else delete r;
	}
}

void RigidBodySystem::createGenerators(GeneratorType T) {
	Camera* cam = GetCamera();
	Vector3 pos = cam->getEye() + cam->getDir() * 50;
	Vector3 perpendicular = Vector3(0, 1, 0).cross(cam->getDir());
	switch (T) {
	case g_sphere: {
		RigidBody* rb_ = new RigidBody(scene, physics, pos, Vector3(-10, 0, 0), Vector3(0, 0, 0), 1, 20, s_sphere, Vector4(1, 0, 0, 1));
		rbGenerators.push_back(new RigidBodyGenerator(scene, physics, rb_, pos, perpendicular * 20, 1, 0.7));
		break;
	}
	case g_cube: {
		RigidBody* rb_ = new RigidBody(scene, physics, pos,  Vector3(-10, 0, 0), Vector3(0, 0, 0), 1, 20, s_cube, Vector4(1, 0, 0, 1));
		rbGenerators.push_back(new RigidBodyGenerator(scene, physics, rb_, pos, Vector3(0, 0, 0), 1, 0.7));
		break;
	}
	case g_capsule: {
		RigidBody* rb_ = new RigidBody(scene, physics, pos, Vector3(-10, 0, 0), Vector3(0, 0, 0), 1, 20, s_capsule, Vector4(1, 0, 0, 1));
		rbGenerators.push_back(new RigidBodyGenerator(scene, physics, rb_, pos, Vector3(0, 0, 0), 1, 0.7));
		break;
	}
	}
}

void RigidBodySystem::shootRB() {
	Camera* cam = GetCamera();
	Vector3 pos = cam->getEye() + cam->getDir();
	Vector3 dir = Vector3(cam->getDir().x*190, cam->getDir().y*340, cam->getDir().z*150);

	RigidBody* rb_ = new RigidBody(scene, physics, pos, dir, Vector3(0, 0, 0), 100, 20, s_sphere, Vector4(1, 0, 0, 1),  4.5);
	rbs.push_back(rb_);
}
