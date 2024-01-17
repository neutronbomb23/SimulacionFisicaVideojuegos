#include <ctype.h>

#include <PxPhysicsAPI.h>

#include <vector>
#include <list>
#include "Particle.h"
#include "ParticleGenerator.h"
#include "RigidBodySystem.h"
#include "Generator.h"
#include "core.hpp"
#include "RenderUtils.hpp"
#include "callbacks.hpp"

#include <iostream>

std::string display_text = "PFINAL";
std::string display_cont = "TIEMPO RESTANTE: ";
std::string display_title = "Que no salten la valla!";



std::string display_niv;

bool displayWinText;
bool displayGameOverText;
bool displayContText = false;
bool displayTitleText = true;


using namespace physx;
using namespace std;

PxDefaultAllocator		gAllocator;
PxDefaultErrorCallback	gErrorCallback;

PxFoundation*			gFoundation = NULL;
PxPhysics*				gPhysics	= NULL;


PxSphereGeometry		sphereGeometry = NULL;
PxMaterial*				gMaterial	= NULL;

PxPvd*                  gPvd        = NULL;
ParticleGenerator* partGen = nullptr;
PxDefaultCpuDispatcher*	gDispatcher = NULL;
PxScene*				gScene      = NULL;
ContactReportCallback gContactReportCallback;

Generator* Gen = nullptr;
PxRigidStatic* suelo;
RenderItem* item;


// Initialize physics engine
void initPhysics(bool interactive)
{
	PX_UNUSED(interactive);

	gFoundation = PxCreateFoundation(PX_FOUNDATION_VERSION, gAllocator, gErrorCallback);

	gPvd = PxCreatePvd(*gFoundation);
	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
	gPvd->connect(*transport, PxPvdInstrumentationFlag::eALL);

	gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale(), true, gPvd);

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

	suelo = gPhysics->createRigidStatic(PxTransform{ 200,0, 100 });
	PxShape* shape = CreateShape(PxBoxGeometry(300, 0.1, 300));
	suelo->attachShape(*shape);
	gScene->addActor(*suelo);

	// Update this line to change the color to dark blue
	item = new RenderItem(shape, suelo, { 0.0, 0.0, 0.1, 0.1 });

	shape = CreateShape(PxBoxGeometry(0.1, 0.1, 0.1));
	item->shape = shape;


	Gen = new Generator(gScene, gPhysics);
	GetCamera()->setGen(Gen);

	 // Start with level 1 directly
	if (Gen->nivel1()) {
		displayWinText = false; displayGameOverText = false; displayContText = true; displayTitleText = false;
		PxShape* shape = CreateShape(PxBoxGeometry(300, 0.1, 300));
		item->shape = shape;
	}
}


// Function to configure what happens in each step of physics
// interactive: true if the game is rendering, false if it offline
// t: time passed since last call in milliseconds
void stepPhysics(bool interactive, double t)
{
	PX_UNUSED(interactive);

	Gen->update(t);
	if (Gen->win) 
	{ 
		displayWinText = true; displayContText = false; 
		PxShape* shape = CreateShape(PxBoxGeometry(0.1, 0.1, 0.1));
		item->shape = shape;
	}
	else if (Gen->lose) {
		displayGameOverText = true; displayContText = false; 
		PxShape* shape = CreateShape(PxBoxGeometry(0.1, 0.1, 0.1));
		item->shape = shape;
	}
	if(!Gen->win && !Gen->lose)display_cont = "TIEMPO RESTANTE: " + to_string(Gen->cont);
	partGen->update(t);
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

	delete Gen;
}


// Function called when a key is pressed
void keyPress(unsigned char key, const PxTransform& camera)
{
	PX_UNUSED(camera);

	switch(toupper(key))
	{
	case ' ':
		Gen->shootRBWithForce();
		break;
	case 'E':
		Gen->addExplosion();
		break;
	case 'B':
		partGen->generate(4);
		break;
	case 'V':
		Gen->addWind();
		break;
	case 'P':
		Gen->shootRBAlternate();
		break;
	case 'T':
		Gen->createFireworks(3);
	default:
		break;
	}
}


void onCollision(physx::PxActor* actor1, physx::PxActor* actor2)
{
	PX_UNUSED(actor1);
	PX_UNUSED(actor2);
	Gen->callHit();
}



int main(int, const char* const*)
{
#ifndef OFFLINE_EXECUTION 
	extern void renderLoop();
	renderLoop();
#else
	static const PxU32 frameCount = 100;
	initPhysics(false);
	for (PxU32 i = 0; i < frameCount; i++)
		stepPhysics(false);
	cleanupPhysics(false);
#endif

	return 0;
}