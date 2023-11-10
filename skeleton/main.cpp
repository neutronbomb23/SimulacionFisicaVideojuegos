#include <ctype.h>
#include <PxPhysicsAPI.h>
#include <cstdlib> // For rand() and srand()
#include <ctime>   // For time()
#include <vector>
#include <list>
#include "Particle.h"
#include "ParticleGenerator.h"
#include "core.hpp"
#include "RenderUtils.hpp"
#include "callbacks.hpp"

#include <iostream>

std::string display_text = "This is a test";


using namespace physx;
using namespace std;

PxDefaultAllocator		gAllocator;
PxDefaultErrorCallback	gErrorCallback;

PxFoundation*			gFoundation = NULL;
PxPhysics*				gPhysics	= NULL;


PxSphereGeometry		sphereGeometry = NULL;
PxMaterial*				gMaterial	= NULL;

PxPvd*                  gPvd        = NULL;

PxDefaultCpuDispatcher*	gDispatcher = NULL;
PxScene*				gScene      = NULL;
ContactReportCallback gContactReportCallback;
Particle* part = nullptr;
ParticleGenerator* partGen = nullptr;
list<Particle*> shots;

// Generates a random float between -1.0 and 1.0
float randomFloat() {
	return static_cast<float>(rand()) / static_cast<float>(RAND_MAX / 2.0f) - 1.0f;
}

// Generates a random direction
Vector3 randomDirection() {
	return Vector3(randomFloat(), randomFloat(), randomFloat()).getNormalized();
}

// Initialize physics engine
void initPhysics(bool interactive)
{
	PX_UNUSED(interactive);

	gFoundation = PxCreateFoundation(PX_FOUNDATION_VERSION, gAllocator, gErrorCallback);

	gPvd = PxCreatePvd(*gFoundation);
	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
	gPvd->connect(*transport,PxPvdInstrumentationFlag::eALL);

	gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale(),true,gPvd);

	gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.6f);

	partGen = new ParticleGenerator();

	// For Solid Rigids +++++++++++++++++++++++++++++++++++++
	PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.8f, 0.0f);
	gDispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = gDispatcher;
	sceneDesc.filterShader = contactReportFilterShader;
	sceneDesc.simulationEventCallback = &gContactReportCallback;
	gScene = gPhysics->createScene(sceneDesc);
	}


// Function to configure what happens in each step of physics
// interactive: true if the game is rendering, false if it offline
// t: time passed since last call in milliseconds
void stepPhysics(bool interactive, double t)
{
	PX_UNUSED(interactive);

	//part->update();
	auto it = shots.begin();
	while (it != shots.end())
	{
		auto aux = it;
		++aux;
		PxTransform* trans = (*it)->getTransform();
		if ((*it)->getDest())
		{
			delete *it;
			shots.erase(it);
		}
		else (*it)->update(t);
		it = aux;
	}

	partGen->updateEveryFrame(t);
	gScene->simulate(t);
	gScene->fetchResults(true);
}

// Function to clean data
// Add custom code to the begining of the function
void cleanupPhysics(bool interactive)
{
	PX_UNUSED(interactive);

	// Rigid Body ++++++++++++++++++++++++++++++++++++++++++
	gScene->release();
	gDispatcher->release();
	// -----------------------------------------------------
	gPhysics->release();	
	PxPvdTransport* transport = gPvd->getTransport();
	gPvd->release();
	transport->release();
	gFoundation->release();

	//delete part;
	for (auto& shot : shots)delete shot;
	shots.clear();

	delete partGen;

}

/// Function called when a key is pressed
void keyPress(unsigned char key, const PxTransform& camera)
{
	PX_UNUSED(camera);

	switch (toupper(key))
	{
#pragma region Toggle Particle Generation
	case 'X':
	case 'C':
		partGen->toggleGeneration();
		break;
#pragma endregion

#pragma region Create a Shooting Star
	case '1':
	{
		Camera* cam = GetCamera();

		PxShape* s = CreateShape(PxSphereGeometry(2));
		PxTransform tr = cam->getTransform();
		Vector3 vel = randomDirection() * 200; // Random direction with a fixed speed
		Vector3 acc = Vector3(0, 0, 0);
		Vector3 gS = Vector3(0, -0.02245925758, 0);
		float damp = 0.95;
		Vector4& color = Vector4(1, 0.9, 0.1, 1);

		Particle* shootingStar = new Particle(s, tr, vel, acc, gS, damp, color);
		shootingStar->getRenderItem()->transform = shootingStar->getTransform();
		RegisterRenderItem(shootingStar->getRenderItem());
		shots.push_back(shootingStar);
		break;
	}
#pragma endregion

#pragma region Create a Bubble
	case '2':
	{
		Camera* cam = GetCamera();

		PxShape* s = CreateShape(PxSphereGeometry(3)); // Larger and more visible
		PxTransform tr = cam->getTransform();
		Vector3 vel = Vector3(0, 25, 0); // Slow upward movement
		Vector3 acc = Vector3(0, 5, 0); // Slight upward acceleration to simulate buoyancy
		Vector3 gS = Vector3(0, -0.02245925758, 0); // Simulated gravity
		float damp = 0.99; // Low damping to allow floating effect
		Vector4& color = Vector4(0.7, 0.8, 1, 0.5); // Light blue, semi-transparent

		Particle* bubble = new Particle(s, tr, vel, acc, gS, damp, color);
		bubble->getRenderItem()->transform = bubble->getTransform();
		RegisterRenderItem(bubble->getRenderItem());
		shots.push_back(bubble);
		break;
	}
#pragma endregion

#pragma region Toggle Particle Forces
	case 'L':
		partGen->getSystem()->toggleParticleForces();
		break;
#pragma endregion

#pragma region Apply Burst to All Particles
	case 'B':
		partGen->getSystem()->applyVentiscaToAllParticles();
		break;
#pragma endregion

#pragma region Apply Gravity to All Particles
	case 'N':
		partGen->getSystem()->applyGravityToAllParticles();
		break;
#pragma endregion

#pragma region delete all particles
	case 'U':
		partGen->getSystem()->clearAllParticles();
		break;
#pragma endregion
		
	default:
		break;
	}
}


void onCollision(physx::PxActor* actor1, physx::PxActor* actor2)
{
	PX_UNUSED(actor1);
	PX_UNUSED(actor2);
}


int main(int, const char*const*)
{
#ifndef OFFLINE_EXECUTION 
	extern void renderLoop();
	renderLoop();
#else
	static const PxU32 frameCount = 100;
	initPhysics(false);
	for(PxU32 i=0; i<frameCount; i++)
		stepPhysics(false);
	cleanupPhysics(false);
#endif

	return 0;
}