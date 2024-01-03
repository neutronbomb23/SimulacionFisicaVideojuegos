#pragma once
#include "core.hpp"
#include <list>
#include "RigidBody.h"
#include "RigidBodyForceRegistry.h"
#include "BouyancyForceGenerator.h"
#include "WindForceGenerator.h"
#include "BurstForceGenerator.h"
#include "FireWork.h"
#include "Particle.h"
#include "ParticleSystem.h" 
using namespace std;
class Generator
{
protected:

	int numObstacles;
	bool playing = false;
	bool clic = false;
	int fires = 0;
	int numFires = 0;
	PxScene* scene;
	bool fireworkActive;
	PxPhysics* physics;
	list<RigidBody*> rbs;
	std::vector<AnchoredSpringFG*> anchoredSprings;
	list<RigidBody*> rbsMoving;
	list<RigidBody*> rbsToDelete;
	list<RigidBody*> shoots;
	list<RigidBody*> shootsToDelete;
	RigidBodyForceRegistry* rbRgis;
	BuoyancyForceGenerator* water = nullptr;
	list<Firework*> fireworks;
	list<Particle*> particles;
	void deleteUnusedRB();
public:
	bool win = false;
	bool lose = false;
	int bulletCount; //number of bullets 
	float cont = 0;
	Generator(PxScene* Scene, PxPhysics* Physics) :scene(Scene), physics(Physics), playing(false)
	{
		srand(static_cast<unsigned int>(time(nullptr)));
		fireworkActive = false;
		bulletCount = 0;
		rbRgis = new RigidBodyForceRegistry();
	};
	virtual void update(float t);
	void shootRB();
	bool nivel1();
	void winGame();
	void shootRBAlternate();
	void createFireworks(int n);
	void createAnchoredSprings();
	void shootRBWithForce();
	void applyRandomForce(RigidBody* rb);
	void applyWind(RigidBody* rb);
	void applySpecialForce(RigidBody* rb);
	void borraWin();
	void Fireworks(float t);
	void gameOver();
	void borraFlot();
	bool getWin() {
		return win;
	}
	void Generator::spawnMovingBlocks() {
		createAnchoredSprings();
		// Posición inicial lejos en el eje Z positivo
		Vector3 startPos = Vector3(-60, 10, -150);
		float spacing = 15.0f;

		// Los bloques se moverán hacia la cámara (hacia el eje Z negativo)
		Vector3 moveDirection = Vector3(0, 0, -1);
		float moveSpeed = 10.0f;

		// Generar colores aleatorios más claros
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<> dis(0.5, 1); // Ajusta este rango para colores más claros

		for (int i = 0; i < 10; i++) {
			Vector4 color = Vector4(dis(gen), dis(gen), dis(gen), 1);

			Vector3 pos = startPos + Vector3(spacing * i, 0, 0);
			Vector3 velocity = moveDirection * moveSpeed;

			RigidBody* rb = new RigidBody(scene, physics, pos, velocity, Vector3(0, 0, 0), 1.0, 20, s_cube, color, 3.5);
			rb->setRandomMass(1.0f, 10.0f);
			rb->setRandomZVelocity(1.0f, 20.0f);
			rbsMoving.push_back(rb);
		}
	}


	void applyRandomForce() {
		// Randomly choose between wind and explosion
		int choice = rand() % 2; // 0 or 1

		if (choice == 0) {
			addWind();
		}
		else {
			addExplosion();
		}
	}
	void callHit() {
		
	}

	void addWind() {
		if (playing) {
			WindForceGenerator* wind = new WindForceGenerator(Vector3(-60, 0, 0), 2005, 0);
			for (auto rb : shoots)
				rbRgis->addRegistry(wind, rb);
		}
	}

	void addExplosion() {
		if (playing) {
			BurstForceGenerator* burst = new BurstForceGenerator(1000000, 15000000, 5);
			for (auto rb : shoots)
				rbRgis->addRegistry(burst, rb);
		}
	}

	void addFirework(Firework* f);
	void addParticle(Particle* p);
};

