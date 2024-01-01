#include "RigidBodyGenerator.h"
default_random_engine generatorRB;

void RigidBodyGenerator::update(float t) {
    NxtGen -= t;
}

list<RigidBody*> RigidBodyGenerator::generateBodies() {
    list<RigidBody*> listRB;
    if (particulaNormal == nullptr) {
        cerr << "Error: RigidBody template is nullptr in RigidBodyGenerator." << endl;
        return listRB;
    }
    if (active && NxtGen <= 0) {
        normal_distribution<float> d(median, var);

        for (int i = 0; i < nGen; i++) {
            Vector3 position = pos + Vector3(d(generatorRB), d(generatorRB), d(generatorRB));
            Vector3 velocity = vel + Vector3(d(generatorRB), d(generatorRB), d(generatorRB));

            RigidBody* rb = new RigidBody(scene, physics, position, velocity, Vector3(0, 0, 0), particulaNormal->getMass(), particulaNormal->getLifeTime(), particulaNormal->getShape(), particulaNormal->getColor());

            listRB.push_back(rb);
        }
        NxtGen = timeforNext;
    }
    return listRB;
}