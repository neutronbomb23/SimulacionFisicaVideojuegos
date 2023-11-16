#include "VortexForceGenerator.h"

// Updates the force on a particle according to the vortex behavior
void VortexForceGenerator::updateForce(Particle* p, double t) {
    // Check that the particle has finite mass to avoid division by zero
    if (fabs(p->inverse_massS) == 0) return;

    // Obtain the camera instance to get its position
    Camera* camera = GetCamera();

    // Calculate the center of the vortex relative to the camera's position
    Vector3 c = Vector3(camera->getTransform().p.x + 150, camera->getTransform().p.y - 150, camera->getTransform().p.z - 150);

    // Get the position of the particle
    Vector3 pos = p->getTransform()->p;

    // Calculate the vortex force and apply it to the particle
    Vector3 vortexForce = K * calc(c, pos);
    p->addForce(vortexForce);
}

//Vector3 VortexForceGenerator::calc(Vector3 c, Vector3 p) {
//	 //Reducir el valor de K para disminuir la velocidad del torbellino
//	const float K = 0.01f; 
//
//	 //Calcula la dirección tangencial del viento
//	Vector3 direction = Vector3(-(p.z - c.z), 50 - (p.y - c.y), p.x - c.x);
//
//	 //Ajusta la fuerza del viento en base a la distancia del centro
//	return K * direction;
//}


//Torbellino en espiral
// Calculates the vortex force based on the position of the center and the particle
Vector3 VortexForceGenerator::calc(Vector3 c, Vector3 p) {
    const float K = 50.0f; // Constant for vortex force strength

    // Calculate the difference in position and the distance to the vortex center
    Vector3 diff = p - c;
    float distance = diff.magnitude();

    // Prevent division by zero and apply force decreasing with distance
    if (distance < 1.0f) {
        distance = 1.0f; // Set a minimum to avoid divisions by zero
    }
    float strength = K / distance; // Force decreases as the distance increases

    // Apply the force in a tangential direction to create a vortex effect
    return strength * Vector3(-(p.y - c.y), (p.z - c.z), -(p.x - c.x));
}