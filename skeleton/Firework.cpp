#include "Firework.h"
#include "ParticleSystem.h"
#include "ParticleGenerator.h"

Firework::Firework(PxTransform pos, Vector3 vel, Vector3 acc, Vector3 grav, float weight, float damping, Vector4 c, float radius, ParticleSystem* pS, ParticleGenerator* pG, int gen)
    : p(pos), v(vel), acceleration(acc), gravity(grav), mass(weight), damp(damping), partS(pS), generation(gen), partG(pG), r(radius)
{
    rend = new RenderItem();
    rend->color = c;
    rend->shape = CreateShape(physx::PxSphereGeometry(radius));
    rend->transform = &p;
    RegisterRenderItem(rend);
}

Firework::~Firework()
{
    OnDie(2);
    DeregisterRenderItem(rend);
    delete rend;
}

void Firework::OnDie(int n) {
    Camera* camera = GetCamera();
    Vector4 color = RandomColor();
    for (int i = 0; i < n; i++) {
        if (generation == 0) partG->generatePart(p);
        else partG->generateFire(p, v, acceleration, gravity, mass, damp, color, r * 0.9, generation - 1);
    }
}
Vector4 Firework::RandomColor() {
    int rndColor = rand() % 5; // 5 colores diferentes
    switch (rndColor) {
    case 0: return Vector4(1, 1, 0, 1);  // Amarillo
    case 1: return Vector4(0, 0, 0, 1);  // Negro
    case 2: return Vector4(1, 1, 1, 1);  // Blanco
    case 3: return Vector4(1, 0, 0, 1);  // Rojo
    case 4: return Vector4(0.6, 0.3, 0.2, 1);  // Marrón (valores aproximados)
    default: return Vector4(1, 1, 1, 1);  // Blanco por defecto, aunque este caso no debería ocurrir
    }
}


void Firework::update(double t)
{
    integrate(t);
    //tiempo de vida (si es mayor que 5 segundos se borra)  
    lifetime += t;
    //comprobar si se tiene que borrar
    if (p.p.y < 10 || lifetime >= 5) dest = true;
}


void Firework::integrate(double t)
{
    p.p += v * t;
    v += (gravity + acceleration) * t;
    v *= pow(damp, t);
}