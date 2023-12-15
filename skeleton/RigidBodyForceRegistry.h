#pragma once
#include <map>
#include "RigidBody.h"
#include "ForceGenerator.h"

using namespace std;
typedef pair<ForceGenerator*, RigidBody*> FRBPair;

// The RigidBodyForceRegistry class manages associations between force generators and rigid bodies.
// It allows for the application of forces to various rigid bodies in a simulation.
class RigidBodyForceRegistry : public multimap<ForceGenerator*, RigidBody*> {
public:
    // Update all forces in the registry for a given duration.
    // This method is typically called each simulation step.
    void updateForces(double duration) {
        for (auto it = begin(); it != end(); it++) {
            // Update force on the associated rigid body
            it->first->updateForce(it->second, duration);
        }
    }



    // Delete a specific force generator associated with a specific rigid body.
    void deleteForceRegistry(ForceGenerator* f, RigidBody* rb)
    {
        for (auto it = begin(); it != end(); ) {
            if (it->first == f && it->second == rb) {
                delete it->first; // Delete the force generator
                it = erase(it);   // Remove the association from the registry
            }
            else {
                it++;
            }
        }
    }  /// nada esto es para eliminar las fuerzas, pero no va tan bien

    // Add a new force generator and rigid body pair to the registry.
    void addRegistry(ForceGenerator* fg, RigidBody* rb)
    {
        FRBPair pair = { fg, rb };
        insert(pair);
    }

    // Delete all force generators associated with a specific rigid body and remove them from the registry.
    void deleteRigidBodyRegistry(RigidBody* rb)
    {
        for (auto it = begin(); it != end(); ) {
            if (it->second == rb) {
                delete it->first; // Delete the force generator
                it = erase(it);   // Remove the association from the registry
            }
            else {
                it++;
            }
        }
    }


    // Clear all entries in the registry.
    void clear() { if (!(*this).empty())(*this).clear(); }

    // Get a constant reference to the internal multimap registry.
    multimap<ForceGenerator*, RigidBody*> const& getRegistry() { return *this; }
};
