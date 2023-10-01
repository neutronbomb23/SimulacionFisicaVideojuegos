#include "Particle.h"

// Constructor: Inicializa la partícula con los valores dados.
Particle::Particle(physx::PxTransform initPos, Vector3 initVel, Vector3 initAccel, float weightVal, float dampingVal)
    : position(initPos), velocity(initVel), acceleration(initAccel), weight(weightVal), dampingFactor(dampingVal)
{
    renderItem = new RenderItem();
}

// Destructor: Deregistra y libera la memoria del objeto RenderItem.
Particle::~Particle()
{
    DeregisterRenderItem(renderItem);
    delete renderItem;
}

// Método para calcular la nueva posición y velocidad de la partícula en función de la aceleración actual y el tiempo transcurrido.
void Particle::updateMovement(double timeDelta)
{
    velocity += acceleration * timeDelta;       // Actualiza la velocidad basándose en la aceleración.
    velocity *= pow(dampingFactor, timeDelta);  // Aplica la amortiguación a la velocidad.
    position.p += velocity * timeDelta;         // Actualiza la posición basándose en la nueva velocidad.
}

// Método que actualmente duplica el comportamiento de updateMovement(). Si hay un comportamiento distinto para el disparo vertical, debería implementarse aquí.
void Particle::performVerticalShoot(double timeDelta)
{
    velocity += acceleration * timeDelta;
    velocity *= pow(dampingFactor, timeDelta);
    position.p += velocity * timeDelta; // Básicamente el integerate de Lía colgado en las diapos
}

// Métodos de acceso (getters) y establecimiento (setters) para los miembros de la clase.

RenderItem* Particle::getRenderItem() const
{
    return renderItem;
}

physx::PxTransform* Particle::getPosition()
{
    return &position;
}

bool Particle::shouldBeDestroyed() const
{
    return isDestroyed;
}

void Particle::markForDestruction()
{
    isDestroyed = true;
}
                