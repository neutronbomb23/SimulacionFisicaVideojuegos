#pragma once
#include "core.hpp"
#include <vector>
#include <list>
#include <random>
#include <cstdlib> // para std::rand y std::srand
#include <ctime>   // para std::time
#include "RigidBody.h"
#include "RigidBodyGenerator.h"
#include "RigidBodyForceRegistry.h"
#include "GravityForceGenerator.h"
#include "WindForceGenerator.h"
#include "VortexForceGenerator.h"
#include "BurstForceGenerator.h"

const int MAXRBS = 20;
enum BowlingPinPattern { TRIANGULAR, DIAMOND, IN_LINE };
enum GeneratorType { g_sphere, g_capsule, g_cube, g_cylinder, g_cone, g_pyramid, g_torus};

class RigidBodySystem {
protected:
	list<RigidBody*> bolosToDelete;
	PxScene* scene;
	PxPhysics* physics;
	bool hit;
	float hitTimer;
	int numRB;
	list<RigidBody*> rbs;
	list<RigidBody*> rbsToDelete;
	vector<RigidBodyGenerator*> rbGenerators;

	RigidBodyForceRegistry* forceRegistry;
	list<ForceGenerator*> forceGenerators;

public:
	RigidBody* controlParticle;
	// Método adicional para manejar colisiones
	void checkCollisions();
	void createGenerators(GeneratorType T);
	void shootRB();
	void createRoadParticles();
	void addExplosion() {
		BurstForceGenerator* megumin = new BurstForceGenerator(4000, 2000, 5); // radio y tiempo
		for (auto rb : rbs)
			forceRegistry->addRegistry(megumin, rb);
	}

	void RigidBodySystem::createControlParticle() {
		Vector3 startPosition = Vector3(0, 5, 0); // Posición inicial en la carretera
		Vector4 color = Vector4(1, 0, 0, 1); // Color rojo
		float size = 2.0f; // Tamaño de la partícula

		// Crea la partícula
		controlParticle = new RigidBody(scene, physics, startPosition, Vector3(0, 0, 0), Vector3(0, 0, 0), 1, size, s_sphere, color);
		controlParticle->setMass(1.0f); // Establece una masa
		rbs.push_back(controlParticle);
	}

	void RigidBodySystem::createCurvedRoad() {
		int numParticles = 200; // Número de partículas para simular la carretera
		Vector4 roadColor = Vector4(1, 0, 0, 1); // Color rojo para la carretera

		// Parámetros de la elipse
		float a = 100.0f; // Semieje mayor
		float b = 50.0f; // Semieje menor
		Vector3 center = Vector3(-100, 0, 0); // Centro de la elipse
		float laneOffset = 5.0f; // Desplazamiento entre los carriles

		for (int i = 0; i < numParticles; i++) {
			float angle = static_cast<float>(i) / numParticles * 2.0f * 3.14; // Ángulo en radianes

			// Posición para el carril izquierdo
			float xLeft = center.x + (a + laneOffset) * cos(angle);
			float zLeft = center.z + (b + laneOffset) * sin(angle);
			Vector3 positionLeft = Vector3(xLeft, center.y, zLeft);

			// Posición para el carril derecho
			float xRight = center.x + (a - laneOffset) * cos(angle);
			float zRight = center.z + (b - laneOffset) * sin(angle);
			Vector3 positionRight = Vector3(xRight, center.y, zRight);

			// Crear partícula para el carril izquierdo
			RigidBody* roadParticleLeft = new RigidBody(scene, physics, positionLeft, Vector3(0, 0, 0), Vector3(0, 0, 0), 1, 20, s_cube, roadColor);
			roadParticleLeft->setMass(0.0f);
			roadParticleLeft->setLifeTime(100000000000);
			rbs.push_back(roadParticleLeft);

			// Crear partícula para el carril derecho
			RigidBody* roadParticleRight = new RigidBody(scene, physics, positionRight, Vector3(0, 0, 0), Vector3(0, 0, 0), 1, 20, s_cube, roadColor);
			roadParticleRight->setMass(0.0f);
			roadParticleRight->setLifeTime(100000000000);
			rbs.push_back(roadParticleRight);
		}
	}

	void generateBowlingPins() {
		// Seleccionar un patrón aleatoriamente
		BowlingPinPattern pattern = static_cast<BowlingPinPattern>(std::rand() % 3);

		switch (pattern) {
		case TRIANGULAR:
			generateTriangularPattern();
			break;
		case DIAMOND:
			generateDiamondPattern();
			break;
		case IN_LINE:
			generateInLinePattern();
			break;
		}
	}
	void RigidBodySystem::generateInLinePattern() {
		Vector3 startPosition = Vector3(0, 10, 0); // Posición central elevada
		float spacing = 20.0f; // Espaciado entre los bolos

		for (int i = 0; i < 9; i++) {
			Vector3 position = startPosition + Vector3(i * spacing, 0, 0);
			createBolo(position);
		}
	}
	void RigidBodySystem::createBolo(const Vector3& position) {
		Vector4 redColor = Vector4(1, 0, 0, 1); // Color rojo
		RigidBody* bolo = new RigidBody(scene, physics, position, Vector3(0, 0, 0), Vector3(0, 0, 0), 1, 20, s_cube, redColor);
		bolo->setMass(0.0f); // Sin gravedad
		bolo->setLifeTime(1000000); // Vida útil larga
		rbs.push_back(bolo);
	}

	void RigidBodySystem::generateDiamondPattern() {
		Vector3 startPosition = Vector3(0, 50, 0); // Posición central elevada
		float spacing = 20.0f; // Espaciado entre los bolos

		// Parte superior del diamante
		for (int row = 0; row < 2; row++) {
			for (int col = 0; col <= row; col++) {
				Vector3 position = startPosition + Vector3(row * spacing, 0, col * spacing - row * spacing / 2.0f);
				createBolo(position);
			}
		}

		// Parte inferior del diamante
		for (int row = 1; row >= 0; row--) {
			for (int col = 0; col <= row; col++) {
				Vector3 position = startPosition + Vector3(3 * spacing - row * spacing, 0, col * spacing - row * spacing / 2.0f);
				createBolo(position);
			}
		}
	}

	void RigidBodySystem::generateTriangularPattern() {
		Vector3 startPosition = Vector3(0, 50, 0); // Posición central elevada
		float spacing = 20.0f; // Espaciado entre los bolos

		for (int row = 0; row < 3; row++) {
			for (int col = 0; col <= row; col++) {
				Vector3 position = startPosition + Vector3(row * spacing, 0, col * spacing - row * spacing / 2.0f);
				createBolo(position);
			}
		}
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

