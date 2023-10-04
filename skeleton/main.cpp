#include <ctype.h>
#include <PxPhysicsAPI.h>
#include <vector>
#include <iostream>
#include <list>
#include "core.hpp"
#include "RenderUtils.hpp"
#include "callbacks.hpp"
#include "Particle.h"

std::string display_text = "This is a test";

using namespace physx;

// Constantes para el juego
const double DAMPING = 0.98;

// Instancias globales de PhysX
PxDefaultAllocator		gAllocator;
PxDefaultErrorCallback	gErrorCallback;
PxFoundation* gFoundation = NULL;
PxPhysics* gPhysics = NULL;
PxMaterial* gMaterial = NULL;
PxPvd* gPvd = NULL;

// List of Particle pointers
std::list<Particle*> particles;
PxDefaultCpuDispatcher* gDispatcher = NULL;
PxScene* gScene = NULL;
ContactReportCallback gContactReportCallback;

// Inicializar el motor de física
void initPhysics(bool interactive)
{
    PX_UNUSED(interactive);

    gFoundation = PxCreateFoundation(PX_FOUNDATION_VERSION, gAllocator, gErrorCallback);

    gPvd = PxCreatePvd(*gFoundation);
    PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
    gPvd->connect(*transport, PxPvdInstrumentationFlag::eALL);

    gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale(), true, gPvd);

    gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.6f);

    PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
    sceneDesc.gravity = PxVec3(0.0f, -9.8f, 0.0f);
    gDispatcher = PxDefaultCpuDispatcherCreate(2);
    sceneDesc.cpuDispatcher = gDispatcher;
    sceneDesc.filterShader = contactReportFilterShader;
    sceneDesc.simulationEventCallback = &gContactReportCallback;
    gScene = gPhysics->createScene(sceneDesc);
}

// Función para configurar lo que sucede en cada paso de la física
void stepPhysics(bool interactive, double t)
{
    PX_UNUSED(interactive);

    auto it = particles.begin();
    while (it != particles.end())
    {
        auto aux = it;
        ++aux;
        physx::PxTransform* trans = (*it)->getPosition();
        if (trans->p.y < 20 ||trans->p.y < -20)
        {
            delete* it;
            particles.erase(it);
            printf("sadfw");
        }
        else
        {
            (*it)->updateMovement(t);
        }
        it = aux;
    }
    gScene->simulate(t);
    gScene->fetchResults(true);
}

// Función para limpiar datos
void cleanupPhysics(bool interactive)
{
    PX_UNUSED(interactive);

    gScene->release();
    gDispatcher->release();
    gPhysics->release();
    PxPvdTransport* transport = gPvd->getTransport();
    gPvd->release();
    transport->release();
    for (auto& particle : particles)
    {
        delete particle;
    }
    particles.clear();
    gFoundation->release();
}

// Función llamada cuando se presiona una tecla
void keyPress(unsigned char key, const PxTransform& camera)
{
    PX_UNUSED(camera);

    switch (toupper(key))
    {
    case 'R': // Bola Cañón
    {
        Camera* cam = GetCamera();
        Particle* particle = new Particle(cam->getTransform(), cam->getDir() * 45, Vector3(0, -9.8, 0), 200.0f, DAMPING);
        particle->getRenderItem()->color = Vector4(1, 0.5, 0, 1);
        particle->getRenderItem()->shape = CreateShape(physx::PxSphereGeometry(1.3));
        particle->getRenderItem()->transform = particle->getPosition();
        RegisterRenderItem(particle->getRenderItem());
        particles.push_back(particle);
        break;
    }

    case 'T': // Laser
    {
        Camera* cam = GetCamera();
        Particle* particle = new Particle(cam->getTransform(), cam->getDir() * 200, Vector3(0, 0, 10.0), 0.05f, DAMPING);
        particle->getRenderItem()->color = Vector4(0, 0.5, 0, 1);
        particle->getRenderItem()->shape = CreateShape(physx::PxSphereGeometry(0.3));
        particle->getRenderItem()->transform = particle->getPosition();
        RegisterRenderItem(particle->getRenderItem());
        particles.push_back(particle);
        break;
    }
    default:
        break;
    }
}

void onCollision(physx::PxActor* actor1, physx::PxActor* actor2)
{
    PX_UNUSED(actor1);
    PX_UNUSED(actor2);
}

int main(int, const char* const*)
{
#ifndef OFFLINE_EXECUTION 
    extern void renderLoop();
    renderLoop();
#else
    const PxU32 frameCount = 100;
    initPhysics(false);
    for (PxU32 i = 0; i < frameCount; i++)
        stepPhysics(false);
    cleanupPhysics(false);
#endif

    return 0;
}
