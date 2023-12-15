#pragma once
#include <PxPhysicsAPI.h>
#include "RenderUtils.hpp"
#include "Particle.h"
#include "RigidBody.h"
using namespace std;
class ForceGenerator {
public:
	virtual void updateForce(Particle* particle, double duration = 0) = 0;
	string _name;
	double _t = 0.0; // If starting negative --> eternal
	double _duration = -1e10;
	inline bool updateTime(double t) {
		_t += t;
		return _t < _duration || _duration < 0.0; // Devuelve true si ya era cero o si es positivo
	}
	virtual void ForceGenerator::updateForce(RigidBody* rb, double t) {};
	virtual ~ForceGenerator() {}
};

