#include "particle.h"

// Constructor: Inicializa una partícula con valores proporcionados
particle::particle(PxTransform pos, Vector3 vel, Vector3 acc, Vector3 grav, float weight, float damping, Vector4 c, float radius)
    : p(pos), v(vel), acceleration(acc), gravity(grav), mass(weight), damp(damping)
{
    rend = new RenderItem();
    rend->color = c;
    rend->shape = CreateShape(physx::PxSphereGeometry(radius));
    rend->transform = &p;
    RegisterRenderItem(rend);
}

// Constructor: Inicializa una partícula en una posición dada como tipo láser
particle::particle(PxTransform pos)
    : p(pos)
{
    setupLaser();
    rend = new RenderItem();
    rend->color = color;
    rend->shape = CreateShape(physx::PxSphereGeometry(radius));
    rend->transform = &p;
    RegisterRenderItem(rend);
}

// Configura la partícula con propiedades de láser
void particle::setupLaser() {
    v = RandVect(25);
    acceleration = Vector3(0, 0, 0);
    gravity = Vector3(0, -0.2, 0);
    mass = 1.0f;
    damp = 0.9f;
    color = Vector4(1, 1, 1, 1);
    radius = 0.3f;
}

// Genera un vector con valores aleatorios entre -size/2 y size/2
Vector3 particle::RandVect(int size) {
    return Vector3(rand() % size - (size / 2), rand() % size - (size / 2), rand() % size - (size / 2));
}

// Destructor: Libera los recursos asociados a la partícula
particle::~particle()
{
    DeregisterRenderItem(rend);
    delete rend;
}

// Actualiza el color de la partícula y el RenderItem asociado
void particle::setColor(const Vector4& newColor) {
    color = newColor;
    if (rend) {
        rend->color = color;
    }
}

// Actualiza la partícula basándose en el paso del tiempo
void particle::update(double t)
{
    integrate(t);
    lifetime += t;
    if (p.p.y < 20 || lifetime >= 2) dest = true;
}

// Actualiza la posición y velocidad de la partícula a lo largo del tiempo t
void particle::integrate(double t)
{
    p.p += v * t;
    v += (gravity + acceleration) * t;
    v *= pow(damp, t);
}
