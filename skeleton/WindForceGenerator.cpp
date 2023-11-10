#include "WindForceGenerator.h"
WindForceGenerator::WindForceGenerator(const Vector3& w, const float k1, const float k2)
    : ParticleDragGenerator(k1, k2), _wind(w) {}

void WindForceGenerator::updateForce(Particle* particle, double t) {
    if (fabs(particle->inverse_massS) < 1e-10) return;
    Vector3 v = particle->getVel();
    Vector3 windForce = _k1 * (_wind - v); // Simplificada a solo fuerza lineal
    particle->addForce(windForce);
}

void WindForceGenerator::normalize(Vector3& vec) {
    float length = sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
    if (length != 0) {
        vec.x /= length;
        vec.y /= length;
        vec.z /= length;
    }
}

void WindForceGenerator::setWindDirectionRandom() {
    Vector3 randomDirection(
        static_cast<float>(rand()) / RAND_MAX * 2.0f - 1.0f, // X
        static_cast<float>(rand()) / RAND_MAX * 2.0f - 1.0f, // Y
        static_cast<float>(rand()) / RAND_MAX * 2.0f - 1.0f  // Z
    );
    normalize(randomDirection); // Llama a la función normalize sobrecargada
    _wind = randomDirection;
}
Vector3 WindForceGenerator::abs(Vector3 a) {
	Vector3 b = Vector3(0, 0, 0);
	b = Vector3(pow(a.x, 2), pow(a.y, 2), pow(a.z, 2));
	b = Vector3(sqrtf(b.x), sqrtf(b.y), sqrtf(b.z));
	return b;
}

Vector3 WindForceGenerator::prodVect(Vector3 a, Vector3 b) {
	return Vector3((a.y * b.z - a.z * b.y), (a.x * b.z - a.z * b.x), (a.x * b.y - a.y * b.x));
}