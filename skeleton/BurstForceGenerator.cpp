#include "BurstForceGenerator.h"
#include <cmath>

// Rotates a vector based on time
Vector3 rotateDirection(const Vector3& direction, double t) {
    // Simple rotation: rotate around the z-axis based on time
    float angle = t; // Rotate by 't' radians (adjust as needed)
    float cosAngle = cos(angle); // Calculate cosine of the angle
    float sinAngle = sin(angle); // Calculate sine of the angle

    // Perform rotation assuming z-axis rotation
    return Vector3(
        direction.x * cosAngle - direction.y * sinAngle, // New x-coordinate
        direction.x * sinAngle + direction.y * cosAngle, // New y-coordinate
        direction.z // z-coordinate remains unchanged
    );
}

// Updates the force on a particle to simulate a burst effect
void BurstForceGenerator::updateForce(Particle* p, double t) {
    // Check that the particle has finite mass
    if (fabs(p->inverse_massS) == 0) return;

    // Obtain the camera instance to get its position
    Camera* camera = GetCamera();
    Vector3 explosionCenter = Vector3(camera->getTransform().p.x + 850, camera->getTransform().p.y - 850, camera->getTransform().p.z - 850);

    // Get the position of the particle
    Vector3 particlePosition = p->getTransform()->p;

    // Calculate distance from explosion center to particle
    float distance = calculateDistance(explosionCenter, particlePosition);

    // Apply burst force if particle is within the radius
    if (distance <= radius) {
        // Calculate direction from explosion center to particle and normalize
        Vector3 direction = (particlePosition - explosionCenter).getNormalized();

        // Rotate the direction vector over time
        direction = rotateDirection(direction, t);

        // Calculate force magnitude, increasing over time and decreasing with distance
        float forceIncreaseFactor = 1.0f + t * t;
        float forceMagnitude = intensity * forceIncreaseFactor / (distance * distance + 1);

        // Calculate and apply the burst force
        Vector3 burstForce = direction * forceMagnitude;
        p->addForce(burstForce);
    }
}

// Calculates the distance between two points in 3D space
float BurstForceGenerator::calculateDistance(Vector3 center, Vector3 position) {
    // Euclidean distance formula
    return sqrtf(pow(position.x - center.x, 2) + pow(position.y - center.y, 2) + pow(position.z - center.z, 2));
}
