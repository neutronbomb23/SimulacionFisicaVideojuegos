#pragma once
#include <list>
#include <random>
#include <iostream>
#include "RigidBody.h"
#include "RigidBodyForceRegistry.h"

using namespace std;

class RigidBodyGenerator {
private:   
    int nGen;
    Vector3 pos, vel;
    RigidBody* particulaNormal;
    float var = 2, median = 1;
    bool active = true;
    PxScene* scene;
    PxPhysics* physics;
    float timeforNext;
    float NxtGen;


    // Store a list of force generators to be applied to each new rigid body
    list<ForceGenerator*> forceGenerators;

public:
    RigidBodyGenerator(PxScene* Scene, PxPhysics* Physics, RigidBody* rb,
        Vector3 position, Vector3 Vel = Vector3(0, 0, 0),
        int numGen = 1, float tmf = 3.0f) :
        scene(Scene), physics(Physics), particulaNormal(rb), pos(position), vel(Vel),
        nGen(numGen), timeforNext(tmf), NxtGen(tmf), active(true) {
    };

    void update(float t);
    list<RigidBody*> generateBodies();

    // Add a force generator to be applied to all generated rigid bodies
    void addForceGenerator(ForceGenerator* fg) {
        forceGenerators.push_back(fg);
    }
};
