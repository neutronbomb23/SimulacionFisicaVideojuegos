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
	void spawnMovingBlocks() {
		createAnchoredSprings();
		// Posici�n inicial lejos en el eje Z positivo
		Vector3 startPos = Vector3(-60, 10, -150); // Ajusta seg�n la ubicaci�n de tu c�mara
		float spacing = 15.0f; // Espaciado reducido entre los bloques para acomodar 15

		// Los bloques se mover�n hacia la c�mara (hacia el eje Z negativo)
		Vector3 moveDirection = Vector3(0, 0, -1); // Direcci�n de movimiento hacia la c�mara
		float moveSpeed = 10.0f; // Velocidad de movimiento

		// Generar colores aleatorios excepto rojo
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<> dis(0, 1);

		for (int i = 0; i < 10; i++) {
			// Evitar el color rojo (1, 0, 0)
			Vector4 color;
			do {
				color = Vector4(dis(gen), dis(gen), dis(gen), 1);
			} while (color.x > 0.8 && color.y < 0.2 && color.z < 0.2); // Rechazar tonos rojos

			Vector3 pos = startPos + Vector3(spacing * i, 0, 0); // Ajustar la posici�n en el eje X para cada bloque
			Vector3 velocity = moveDirection * moveSpeed; // Velocidad constante hacia la c�mara

			RigidBody* rb = new RigidBody(scene, physics, pos, velocity, Vector3(0, 0, 0), 1.0, 20, s_cube, color, 3.5);
			rb->setRandomMass(1.0f, 10.0f); // Masa aleatoria entre 1 y 10
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

