#pragma once
#include <map>
#include "ForceGenerator.h"
using namespace std;

typedef pair<ForceGenerator*, Particle*> FRPair;
class ParticleForceRegistry :
    public multimap<ForceGenerator*, Particle*>
{
public:
    void updateForces(double duration) {
        for (auto it = begin(); it != end(); it++)
            it->first->updateForce(it->second, duration);
    }
    void addRegistry(ForceGenerator* fg, Particle* p)
    { 
        FRPair pair = { fg, p };
        insert(pair);
    }
    void deleteParticleRegistry(Particle* p)
    {
        for (auto it = begin(); it != end(); ) {
            if (it->second == p) { delete it->first; it = erase(it); }
            else { it++; }
        }
    }
    void deleteForceRegistry(ForceGenerator* f, Particle* p)
    {
        for (auto it = begin(); it != end(); ) {
            if (it->first == f && it->second == p) { delete it->first; it = erase(it); }
            else { it++; }
        }
    }
    void clear() { if(!(*this).empty())(*this).clear(); }
    multimap<ForceGenerator*, Particle*> const& getRegistry() { return *this; }
};

