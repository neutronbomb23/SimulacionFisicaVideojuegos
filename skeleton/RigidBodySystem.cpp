#include "RigidBodySystem.h"

// Constructor of RigidBodySystem. Initializes the physics scene and registry.
RigidBodySystem::RigidBodySystem(PxScene* Scene, PxPhysics* Physics)
    : scene(Scene), physics(Physics), numRB(0) {
    forceRegistry = new RigidBodyForceRegistry();
}


// Update method called every frame to update the system's state.
void RigidBodySystem::update(double t) {
    // Iterate over all rigid body generators to generate and update bodies.
    for (RigidBodyGenerator* g : rbGenerators) {
        addRBS(g->generateBodies());
        g->update(t);
    }

    // Update forces acting on rigid bodies.
    forceRegistry->updateForces(t);

    // Integrate physics for each rigid body and mark dead bodies for deletion.
    for (RigidBody* rb : rbs) {
        if (rb->isAlive()) {
            rb->integrate(t);
        }
        else {
            rbsToDelete.push_back(rb);
        }
    }

    // Delete rigid bodies that are no longer in use.
    deleteUnusedRB();
}

// Deletes rigid bodies marked for deletion.
void RigidBodySystem::deleteUnusedRB() {
    for (auto it = rbsToDelete.begin(); it != rbsToDelete.end();) {
        forceRegistry->deleteRigidBodyRegistry(*it);
        rbs.remove(*it);
        delete(*it);
        it = rbsToDelete.erase(it);
        numRB--;
    }
}

// Adds a list of new rigid bodies to the system.
void RigidBodySystem::addRBS(list<RigidBody*> lrb) {
    for (RigidBody* r : lrb) {
        if (rbs.size() < MAXRBS) {
            rbs.push_back(r);
            numRB++;
            // Register forces for each new rigid body.
            for (auto fg : forceGenerators) {
                forceRegistry->addRegistry(fg, r);
            }
        }
        else {
            delete r;
        }
    }
}

// Creates and adds rigid body generators based on the specified type.
void RigidBodySystem::createGenerators(GeneratorType T) {
    Camera* cam = GetCamera();
    Vector3 pos = cam->getEye() + cam->getDir() * 50;
    Vector3 perpendicular = Vector3(0, 1, 0).cross(cam->getDir());
    Vector4 randomColor = generateRandomColor();

    // Create generators for different types of rigid bodies.
    switch (T) {
               // NI PROBAR ESTAS NO VAN
    case g_cone: {
        RigidBody* particulaNormal = new RigidBody(scene, physics, pos, Vector3(-10, 0, 0), Vector3(0, 0, 0), 1, 20, s_cone, randomColor);
        rbGenerators.push_back(new RigidBodyGenerator(scene, physics, particulaNormal, pos, perpendicular * 20, 1, 0.7));
        break;
    }
    case g_pyramid: {
        RigidBody* particulaNormal = new RigidBody(scene, physics, pos, Vector3(-10, 0, 0), Vector3(0, 0, 0), 1, 20, s_pyramid, randomColor);
        rbGenerators.push_back(new RigidBodyGenerator(scene, physics, particulaNormal, pos, perpendicular * 20, 1, 0.7));
        break;
    }
    case g_torus: {
        RigidBody* particulaNormal = new RigidBody(scene, physics, pos, Vector3(-10, 0, 0), Vector3(0, 0, 0), 1, 20, s_torus, randomColor);
        rbGenerators.push_back(new RigidBodyGenerator(scene, physics, particulaNormal, pos, perpendicular * 20, 1, 0.7));
        break;
        }
                // Estas si 
    case g_sphere: {
        RigidBody* particulaNormal = new RigidBody(scene, physics, pos, Vector3(-10, 0, 0), Vector3(0, 0, 0), 1, 20, s_sphere, randomColor);
        rbGenerators.push_back(new RigidBodyGenerator(scene, physics, particulaNormal, pos, perpendicular * 20, 1, 0.7));
        break;
    }
    case g_cube: {
        RigidBody* particulaNormal = new RigidBody(scene, physics, pos, Vector3(-10, 0, 0), Vector3(0, 0, 0), 1, 20, s_cube, randomColor);
        rbGenerators.push_back(new RigidBodyGenerator(scene, physics, particulaNormal, pos, Vector3(0, 0, 0), 1, 0.7));
        break;
    }
    }
}

void RigidBodySystem::createRoadParticles() {
    Vector3 startPosition = Vector3(-100, 0, 0); // Posición de inicio de la carretera
    Vector3 direction = Vector3(1, 0, 0); // Dirección de la carretera
    float particleSpacing = 10.0f; // Espaciado entre partículas
    int numParticles = 20; // Número de partículas para simular la carretera
    Vector4 roadColor = Vector4(1, 0, 0, 1); // Color rojo para la carretera

    for (int i = 0; i < numParticles; i++) {
        Vector3 position = startPosition + direction * (i * particleSpacing);
        RigidBody* roadParticle = new RigidBody(scene, physics, position, Vector3(0, 0, 0), Vector3(0, 0, 0), 1, 20, s_cube, roadColor);
        roadParticle->setMass(0.0f); // Las partículas de la carretera podrían ser estáticas
        rbs.push_back(roadParticle);
    }
}



// Method to shoot a rigid body like a projectile.
void RigidBodySystem::shootRB() {
    Camera* cam = GetCamera();
    Vector4 randomColor = generateRandomColor();
    Vector3 pos = cam->getEye() + cam->getDir();
    Vector3 dir = cam->getDir() * 1005;
    RigidBody* particulaNormal = new RigidBody(scene, physics, pos, dir, Vector3(0, 0, 0), 1, 20, s_cube, randomColor);
    rbs.push_back(particulaNormal);
}
