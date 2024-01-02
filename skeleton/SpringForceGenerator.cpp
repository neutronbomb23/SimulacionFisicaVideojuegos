#include "SpringForceGenerator.h"

SpringForceGenerator::SpringForceGenerator(double k, double resting_length, Particle* other, bool gomaElastica)
	: _K(k), _resting_length(resting_length), _other(other), gomaEl(gomaElastica) {}

void SpringForceGenerator::updateForce(Particle* particle, double t) {
	Vector3 relative_pos_vector = _other->getTransform()->p - particle->getTransform()->p;
		// Particle is the particle to apply the force
		Vector3 force;
		//if (!gomaEl || mayorIgual(relative_pos_vector, _resting_length)) {
		//	// normalize: Normalize the relative_pos_vector and returns its length.
		//	const float length = relative_pos_vector.normalize();
		//	const float delta_x = length - _resting_length;

		//	force = relative_pos_vector * delta_x * _K;
		//}
		//else force = -(particle->getVel());
			// normalize: Normalize the relative_pos_vector and returns its length.
			const float length = relative_pos_vector.normalize();
			float delta_x = length - _resting_length;
			if (gomaEl && !mayorIgual(relative_pos_vector, _resting_length))delta_x = -delta_x;

			force = relative_pos_vector * delta_x * _K;
			//if (gomaEl && !mayorIgual(relative_pos_vector, _resting_length))force = -force;
	particle->addForce(force);
}