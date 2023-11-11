#include "VortexForceGenerator.h"

void VortexForceGenerator::updateForce(Particle* p, double t) {
	// Check that the particle has Finite Mass
    if (fabs(p->inverse_massS) == 0)return;
    Camera* camera = GetCamera(); 
    
	
    Vector3 c = Vector3(camera->getTransform().p.x + 150, camera->getTransform().p.y - 150, camera->getTransform().p.z - 150);
	
   
	
	Vector3 pos = p->getTransform()->p;
	Vector3 vortexForce = K * calc(c, pos);
	p->addForce(vortexForce);
}

Vector3 VortexForceGenerator::calc(Vector3 c, Vector3 p) {
	 //Reducir el valor de K para disminuir la velocidad del torbellino
	const float K = 2.0f; 

	 //Calcula la dirección tangencial del viento
	Vector3 direction = Vector3(-(p.z - c.z), 50 - (p.y - c.y), p.x - c.x);

	 //Ajusta la fuerza del viento en base a la distancia del centro
	return K * direction;
}

////TORBELLINO ES ESPIRAL
//Vector3 VortexForceGenerator::calc(Vector3 c, Vector3 p) {
//    const float K = 50.0f; // Constante de fuerza del torbellino
//
//  /*   Calcula la diferencia en posición y la distancia al centro del torbellino*/
//    Vector3 diff = p - c;
//    float distance = diff.magnitude();
//
//     /*Evita la división por cero y aplica una fuerza decreciente con la distancia*/
//    if (distance < 1.0f) {
//        distance = 1.0f; // Establece un mínimo para evitar divisiones por cero
//    }
//    float strength = K / distance; // La fuerza disminuye con la distancia
//
//    /* Aplica la fuerza en una dirección tangencial*/
//    return strength * Vector3(-(p.y - c.y), (p.z - c.z), -(p.x - c.x));
//}
