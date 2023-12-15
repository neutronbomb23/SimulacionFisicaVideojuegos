#pragma once
#include "ForceGenerator.h"
#include "core.hpp"

class SpringForceGenerator : public ForceGenerator
{
public:
	SpringForceGenerator(double k, double resting_length, Particle* other, bool gomaElastica = false);

	virtual void updateForce(Particle* particle, double t);

	void setK(double k) { _K = k; }
	Particle* getOther() { return _other; }

	virtual ~SpringForceGenerator() {}
protected:
	double _K;
	double _resting_length;
	Particle* _other;
	bool gomaEl;
	bool mayorIgual(const Vector3& v1, double d2) const{
		float d1 = sqrt(pow(v1.x, 2) + pow(v1.y, 2) + pow(v1.z, 2));
		return d1 >= d2;
	}
};

