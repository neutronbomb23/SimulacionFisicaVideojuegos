#include "Particle.h"

// Constructor: Inicializa la part�cula con los valores dados.
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

// M�todo para calcular la nueva posici�n y velocidad de la part�cula en funci�n de la aceleraci�n actual y el tiempo transcurrido.
void Particle::updateMovement(double timeDelta)
{
    velocity += acceleration * timeDelta;       // Actualiza la velocidad bas�ndose en la aceleraci�n.
    velocity *= pow(dampingFactor, timeDelta);  // Aplica la amortiguaci�n a la velocidad.
    position.p += velocity * timeDelta;         // Actualiza la posici�n bas�ndose en la nueva velocidad.
}

// M�todo que actualmente duplica el comportamiento de updateMovement(). Si hay un comportamiento distinto para el disparo vertical, deber�a implementarse aqu�.
void Particle::performVerticalShoot(double timeDelta)
{
    velocity += acceleration * timeDelta;
    velocity *= pow(dampingFactor, timeDelta);
    position.p += velocity * timeDelta; // B�sicamente el integerate de L�a colgado en las diapos
}

// M�todos de acceso (getters) y establecimiento (setters) para los miembros de la clase.

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
                