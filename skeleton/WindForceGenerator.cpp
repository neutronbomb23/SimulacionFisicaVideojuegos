#include "WindForceGenerator.h"

// Constructor
WindForceGenerator::WindForceGenerator(const Vector3& w, const float k1, const float k2)
    : ParticleDragGenerator(k1, k2), _wind(w) {}  // Initialize the base class and wind vector

// Updates the force on a particle considering the wind force
void WindForceGenerator::updateForce(Particle* particle, double t) {
    if (fabs(particle->inverse_massS) < 1e-10) return;  // Avoid division by zero
    Vector3 v = particle->getVel();  // Get particle velocity
    Vector3 windForce = _k1 * (_wind - v);  // Calculate wind force (linear force model)
    particle->addForce(windForce);  // Apply the wind force to the particle
}

// Normalizes a vector to unit length
void WindForceGenerator::normalize(Vector3& vec) {
    float length = sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);  // Calculate vector length
    if (length != 0) {
        vec.x /= length;  // Normalize x component
        vec.y /= length;  // Normalize y component
        vec.z /= length;  // Normalize z component
    }
}

// Sets the wind direction to a random vector
void WindForceGenerator::setWindDirectionRandom() {
    Vector3 randomDirection(
        static_cast<float>(rand()) / RAND_MAX * 2.0f - 1.0f,  // Random X
        static_cast<float>(rand()) / RAND_MAX * 2.0f - 1.0f,  // Random Y
        static_cast<float>(rand()) / RAND_MAX * 2.0f - 1.0f   // Random Z
    );
    normalize(randomDirection);  // Normalize to unit vector
    _wind = randomDirection;  // Set wind direction
}

// Calculates the absolute value of a vector
Vector3 WindForceGenerator::abs(Vector3 a) {
    Vector3 b = Vector3(0, 0, 0);
    b = Vector3(pow(a.x, 2), pow(a.y, 2), pow(a.z, 2));  // Square each component
    b = Vector3(sqrtf(b.x), sqrtf(b.y), sqrtf(b.z));  // Take square root of each component
    return b;  // Return the absolute value vector
}

// Calculates the cross product of two vectors
Vector3 WindForceGenerator::prodVect(Vector3 a, Vector3 b) {
    return Vector3((a.y * b.z - a.z * b.y), (a.x * b.z - a.z * b.x), (a.x * b.y - a.y * b.x));
}