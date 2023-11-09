#include <ctype.h>
#include <PxPhysicsAPI.h>
#include <vector>
#include <iostream>
#include <list>
#include "core.hpp"
#include "RenderUtils.hpp"
#include "callbacks.hpp"
#include "particle.h"
#include "ParticleGenerator.h"
#include "ParticleSystem.h"

using namespace std;
using namespace physx;

std::string display_text = "This is a test"; // Texto para visualización

// Instancias globales de PhysX
PxDefaultAllocator		gAllocator;
PxDefaultErrorCallback	gErrorCallback;
PxFoundation* gFoundation = NULL;
PxPhysics* gPhysics = NULL;
PxMaterial* gMaterial = NULL;
PxPvd* gPvd = NULL;
ParticleSystem* pS;
ParticleGenerator* partGen;
list<particle*> particles;  // Lista de partículas
PxDefaultCpuDispatcher* gDispatcher = NULL;
PxScene* gScene = NULL;
ContactReportCallback gContactReportCallback;

// Función para inicializar el motor de física de PhysX
void initPhysics(bool interactive)
{
    PX_UNUSED(interactive);

    gFoundation = PxCreateFoundation(PX_FOUNDATION_VERSION, gAllocator, gErrorCallback);

    // Configuración para visualización y depuración de PhysX
    gPvd = PxCreatePvd(*gFoundation);
    PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
    gPvd->connect(*transport, PxPvdInstrumentationFlag::eALL);

    gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale(), true, gPvd);
    gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.6f);

    partGen = new ParticleGenerator();  // Inicializa el generador de partículas
    pS = new ParticleSystem();
    // Configura la descripción de la escena
    PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
    sceneDesc.gravity = PxVec3(0.0f, -9.8f, 0.0f);
    gDispatcher = PxDefaultCpuDispatcherCreate(2);
    sceneDesc.cpuDispatcher = gDispatcher;
    sceneDesc.filterShader = contactReportFilterShader;
    sceneDesc.simulationEventCallback = &gContactReportCallback;
    gScene = gPhysics->createScene(sceneDesc);
}

// Función para configurar la actualización de la física en cada paso
void stepPhysics(bool interactive, double t)
{
    PX_UNUSED(interactive);
    partGen->update(t);  // Actualiza el generador de partículas

    // Actualiza y limpia las partículas destruidas
    auto it = particles.begin();
    while (it != particles.end()) {
        auto aux = it;
        ++aux;
        if ((*it)->getDestroyed()) {
            delete* it;
            particles.erase(it);
        }
        else (*it)->update(t);
        it = aux;
    }

    // Simula la escena y obtiene resultados
    gScene->simulate(t);
    gScene->fetchResults(true);
}

// Función para liberar los recursos y limpiar datos
void cleanupPhysics(bool interactive)
{
    PX_UNUSED(interactive);

    gScene->release();
    gDispatcher->release();
    gPhysics->release();
    PxPvdTransport* transport = gPvd->getTransport();
    gPvd->release();
    transport->release();
    for (auto& i : particles) delete i;
    particles.clear();
    gFoundation->release();
}

// Función llamada cuando se presiona una tecla
void keyPress(unsigned char key, const PxTransform& camera)
{
    
    PX_UNUSED(camera);
    Camera* cam = GetCamera();
    switch (toupper(key))
    {
    case 'F':  // Dispara un FIREBALL (bola de fuego) en la dirección de la cámara
    {
        double damp = 0.9f;
        float vel = 10.0f;
        Vector4 color = Vector4(1, 0, 0, 0.5);  // Color rojo
        float r = 0.1f;
        particle* p = new particle(cam->getTransform(), cam->getDir() * vel, Vector3(0, 0, 0), Vector3(0, -0.6, 0), 1.0f, damp, color, r);
        particles.push_back(p);
        break;
    }
    case 'G':  // Crea una chispa en la posición de la cámara
    {
        particle* spark = new particle(cam->getTransform());
        spark->setupSpark();
        particles.push_back(spark);
        break;
    }
    case 'H':  // Crea un efecto de humo en la posición de la cámara
    {
        particle* smoke = new particle(cam->getTransform());
        smoke->setupSmoke();
        particles.push_back(smoke);
        break;
    }
    case 'I':
    {
        particle* waterBubble = new particle(cam->getTransform());
        waterBubble->setupBubble();
        particles.push_back(waterBubble);
        break;
       
    }
    case 'J':
    {
        particle* meteor = new particle(cam->getTransform());
        meteor->setupShootingStar();
        particles.push_back(meteor);
        break;
    }
    case '6':
    {
        PxTransform pos = cam->getTransform();
        Vector3 vel = { 0.0, 10.0, 0.0 };
        Vector3 null = {0.0, 0.0, 0.0};
        partGen->generateFire(pos, vel, null, null, 0.0, 0.9, Vector4(1.0, 0.0, 0.0, 1.0), 1.0, 3);
    }
    default:
        break;
    }
}

// Función para manejar colisiones
void onCollision(physx::PxActor* actor1, physx::PxActor* actor2)
{
    PX_UNUSED(actor1);
    PX_UNUSED(actor2);
    // Actualmente vacía, puede ser ampliada para manejar eventos de colisión específicos
}

int main(int, const char* const*)
{
#ifndef OFFLINE_EXECUTION 
    extern void renderLoop();  // Función externa para el bucle de renderizado
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
