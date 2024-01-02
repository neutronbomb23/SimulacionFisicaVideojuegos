#pragma once
#include <map>
#include "RigidBody.h"
#include "ForceGenerator.h"

using namespace std;
typedef pair<ForceGenerator*, RigidBody*> FRBPair;

class RigidBodyForceRegistry : public multimap<ForceGenerator*, RigidBody*> {
 public:
    void updateForces(double duration) {
        for (auto it = begin(); it != end(); it++)
            it->first->updateForce(it->second, duration);
    }
    void addRegistry(ForceGenerator* fg, RigidBody* rb)
    {
        FRBPair pair = { fg, rb };
        insert(pair);
    }
    void deleteRigidBodyRegistry(RigidBody* rb)
    {
        for (auto it = begin(); it != end(); ) {
            if (it->second == rb) { delete it->first; it = erase(it); }
            else { it++; }
        }
    }
    void deleteForceRegistry(ForceGenerator* f, RigidBody* rb)
    {
        for (auto it = begin(); it != end(); ) {
            if (it->first == f && it->second == rb) { delete it->first; it = erase(it); }
            else { it++; }
        }
    }
    void clear() { if (!(*this).empty())(*this).clear(); }
    multimap<ForceGenerator*, RigidBody*> const& getRegistry() { return *this; }
};
