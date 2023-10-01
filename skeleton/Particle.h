#pragma once
#include "RenderUtils.hpp"
#include <iostream>

// La clase Particle representa una partícula en la simulación.
class Particle
{
private:
    Vector3 velocity;        // Velocidad de la partícula.
    physx::PxTransform position; // Posición actual de la partícula en el mundo.
    RenderItem* renderItem;  // Representación gráfica de la partícula para su renderización.
    Vector3 acceleration;    // Aceleración experimentada por la partícula.
    float dampingFactor;     // Factor para controlar la reducción de velocidad (efecto de amortiguación).
    float weight;            // Masa de la partícula.
    bool isDestroyed = false; // Bandera para verificar si la partícula debe ser destruida.

public:
    Particle(physx::PxTransform initPos, Vector3 initVel, Vector3 initAccel, float weightVal, float dampingVal);
    ~Particle();

    // Integra el movimiento de la partícula usando sus propiedades actuales.
    void updateMovement(double timeDelta);

    // Realiza un movimiento de disparo vertical para la partícula.
    void performVerticalShoot(double timeDelta);

    // Métodos de acceso (getters) y establecimiento (setters) para los miembros de la clase.
    RenderItem* getRenderItem() const;
    physx::PxTransform* getPosition();
    bool shouldBeDestroyed() const;
    void markForDestruction();
};
