	#include <ctype.h>

	#include <PxPhysicsAPI.h>

	#include <vector>
	#include <list>
	#include "Particle.h"
	#include "ParticleGenerator.h"
	#include "RigidBodySystem.h"
	#include "core.hpp"
	#include "RenderUtils.hpp"
	#include "callbacks.hpp"

	#include <iostream>

	std::string display_text = "";


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
	RigidBodySystem* RBSys = nullptr;
	RigidBody* controlparticle;
	list<Particle*> shots;

	void setCameraAtRoadStart(PxTransform& camera) {
		// Suponiendo que el inicio de la carretera está en (x, y, z)
		PxVec3 roadStartPosition = PxVec3(-100, 5, 0); // Ajusta estas coordenadas según sea necesario
		PxVec3 roadDirection = PxVec3(1, 0, 0); // Dirección hacia la que mira la carretera

		// Ajustar la posición de la cámara
		camera.p = roadStartPosition;

		// Ajustar la orientación de la cámara para que mire hacia la dirección de la carretera
		// Calcula la rotación necesaria para que la cámara mire en la dirección de la carretera
		PxVec3 up = PxVec3(0, 1, 0); // Vector 'up' estándar
		camera.q = PxQuat(PxHalfPi, up.cross(roadDirection).getNormalized()); // Ajustar según la orientación específica de tu sistema
	}


	// Initialize physics engine
	void initPhysics(bool interactive)
	{
		PX_UNUSED(interactive);
		PxTransform cameraTransform;
		setCameraAtRoadStart(cameraTransform);
		gFoundation = PxCreateFoundation(PX_FOUNDATION_VERSION, gAllocator, gErrorCallback);

		gPvd = PxCreatePvd(*gFoundation);
		PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
		gPvd->connect(*transport, PxPvdInstrumentationFlag::eALL);

		gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale(), true, gPvd);

		gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.6f);

		// For Solid Rigids +++++++++++++++++++++++++++++++++++++
		PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
		sceneDesc.gravity = PxVec3(0.0f, -9.8f, 0.0f);
		gDispatcher = PxDefaultCpuDispatcherCreate(2);
		sceneDesc.cpuDispatcher = gDispatcher;
		sceneDesc.filterShader = contactReportFilterShader;
		sceneDesc.simulationEventCallback = &gContactReportCallback;
		gScene = gPhysics->createScene(sceneDesc);

		//platform
		PxRigidStatic* platform = gPhysics->createRigidStatic(PxTransform{ 0,0,0 });
		PxShape* shape = CreateShape(PxBoxGeometry(100, 0.1, 100));
		platform->attachShape(*shape);
		gScene->addActor(*platform);
		RenderItem* item = new RenderItem(shape, platform, { 0.0,0.3,0.0,1 });

		partGen = new ParticleGenerator();
		RBSys = new RigidBodySystem(gScene, gPhysics);
		// Instanciar la controlParticle
		RBSys->createControlParticle();
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

		partGen->update(t);
		RBSys->update(t);


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

	// Function called when a key is pressed
	void keyPress(unsigned char key, const PxTransform& camera)
	{
		PxTransform cameraCopy = camera; // Hacer una copia de la cámara
		PX_UNUSED(camera);

		switch(toupper(key))
		{
		case 'B':
			setCameraAtRoadStart(cameraCopy);
			// Aquí debes aplicar los cambios de cameraCopy a tu cámara real
			break;
		case '6':
			partGen->getSys()->generateRedRectangles();
			break;
		case 'K':
			partGen->getSys()->addK(10);
			break;	
		case '7':
			partGen->getSys()->generateSpringDemo(partGen->getSys()->SPRING);
			break;
		case '8':
			partGen->getSys()->generateSpringDemo(partGen->getSys()->FLOTABILITY);
			break;
		case '9':
			partGen->getSys()->generateBuoyancy();
			break;
		case 'E':
			RBSys->addExplosion();
			break;

		case 'N':
			RBSys->createCurvedRoad();
			partGen->getSys()->generateRoadParticles();
			break;
		// P5 de los rigidos solidos papu
		case 'X':
			RBSys->createGenerators(g_sphere);
			break;
		case 'C':
			RBSys->createGenerators(g_cube);
			break;
		case 'V':
			RBSys->shootRB();
			break;
		//case 'I':
		//	RBSys->controlParticle->setVelocity(currentVelocity + Vector3(0, 0, -10)); // Mover hacia adelante
		//	break;
		//case 'K':
		//	RBSys->controlParticle->setVelocity(currentVelocity + Vector3(0, 0, 10)); // Mover hacia atrás
		//	break;
		//case 'J':
		//	RBSys->controlParticle->setVelocity(currentVelocity + Vector3(-10, 0, 0)); // Mover a la izquierda
		//	break;
		//case 'L':
		//	RBSys->controlParticle->setVelocity(currentVelocity + Vector3(10, 0, 0)); // Mover a la derecha
		//	break;
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