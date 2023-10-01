#pragma once
#include "RenderUtils.hpp"
#include <iostream>

// La clase Particle representa una part�cula en la simulaci�n.
class Particle
{
private:
    Vector3 velocity;        // Velocidad de la part�cula.
    physx::PxTransform position; // Posici�n actual de la part�cula en el mundo.
    RenderItem* renderItem;  // Representaci�n gr�fica de la part�cula para su renderizaci�n.
    Vector3 acceleration;    // Aceleraci�n experimentada por la part�cula.
    float dampingFactor;     // Factor para controlar la reducci�n de velocidad (efecto de amortiguaci�n).
    float weight;            // Masa de la part�cula.
    bool isDestroyed = false; // Bandera para verificar si la part�cula debe ser destruida.

public:
    Particle(physx::PxTransform initPos, Vector3 initVel, Vector3 initAccel, float weightVal, float dampingVal);
    ~Particle();

    // Integra el movimiento de la part�cula usando sus propiedades actuales.
    void updateMovement(double timeDelta);

    // Realiza un movimiento de disparo vertical para la part�cula.
    void performVerticalShoot(double timeDelta);

    // M�todos de acceso (getters) y establecimiento (setters) para los miembros de la clase.
    RenderItem* getRenderItem() const;
    physx::PxTransform* getPosition();
    bool shouldBeDestroyed() const;
    void markForDestruction();
};
