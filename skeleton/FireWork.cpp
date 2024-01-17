#include "Firework.h"
#include "ParticleSystem.h"
#include "Generator.h"

Firework::Firework(PxTransform pos, Vector3 vel, Vector3 acc, Vector3 grav, float damp, Vector4 c, ParticleSystem* pS, int generation)
    : trans(pos), vel(vel), acel(acc), gS(grav), damping(damp), partS(pS), gen(generation)
{
    rend = new RenderItem();
    float num = 1 - 0.25 * gen;
    rend->color = Vector4(1, num, num, 1);
    rend->shape = CreateShape(physx::PxSphereGeometry(num));
    srand(time(NULL)); // Inicializa la semilla del generador de números aleatorios
  
    //rend->color = c;
    //rend->shape = CreateShape(physx::PxSphereGeometry(1.0f));
    rend->transform = &trans;
    RegisterRenderItem(rend);
}
Firework::Firework(PxTransform pos, Vector3 vel, Vector3 acc, Vector3 grav, float damp, Vector4 c, Generator* gn, int generation)
    : trans(pos), vel(vel), acel(acc), gS(grav), damping(damp), Gen(gn), gen(generation)
{
    rend = new RenderItem();
    float num = 1 - 0.25 * gen;
    rend->color = Vector4(1, num, num, 1);
    rend->shape = CreateShape(physx::PxSphereGeometry(num * 0.5));
    //rend->color = c;
    //rend->shape = CreateShape(physx::PxSphereGeometry(1.0f));
    rend->transform = &trans;
    RegisterRenderItem(rend);
}

Firework::Firework(PxTransform pos, Vector3 dir, int generation, ParticleSystem* pS):
    gen(generation), trans(pos), partS(pS), acel(Vector3(0, 0, 0)), gS(Vector3(0, 0, 0)), damping(0.9f) {
    rend = new RenderItem();
    rend->transform = &trans;
    float num = 1 - 0.25 * gen;
    vel = dir * 10 * num;
    rend->color = Vector4(1, num, num, 1);
    rend->shape = CreateShape(physx::PxSphereGeometry(num*0.5));
    RegisterRenderItem(rend);
}

Firework::Firework(PxTransform pos, Vector3 dir, int generation, Generator* gn) :
    gen(generation), trans(pos), Gen(gn), acel(Vector3(0, 0, 0)), gS(Vector3(0, 0, 0)), damping(0.9f) {
    rend = new RenderItem();
    rend->transform = &trans;
    float num = 1 + 0.1 * gen;
    vel = dir * 10 * num;
    rend->color = Vector4(1, num, num, 1);
    rend->shape = CreateShape(physx::PxSphereGeometry(num * 0.5));
    RegisterRenderItem(rend);
}

Firework::~Firework()
{
    Vector3 dir;
    float angle;
    int r = (rand() % 7) + 1;
    if (gen == 3) {
        int r = (rand() % 3) + 1;
        for (int i = 0; i < r; i++) {
            angle = (360 / r) * i;
            dir = Vector3(cos(angle * pi / 180), sin(angle * pi / 180), 1);
            Particle* part = new Particle(trans, dir);
            //partS->addParticle(part);
            Gen->addParticle(part);
        }
    }
    else {
        for (int i = 0; i < r; i++) {
            angle = (360 / r) * i;
            dir = Vector3(cos(angle * pi / 180), sin(angle * pi / 180), 1);
            Firework* fire = new Firework(trans, dir, gen + 1, Gen);
            //Firework* fire = new Firework(trans, dir, gen + 1, partS);
            //partS->addFirework(fire);
            int colorChoice = rand() % 4; // Genera un número entre 0 y 3
            Vector4 color;
            switch (colorChoice) {
            case 0: color = Vector4(0.5, 0, 0.5, 1); break; // Morado
            case 1: color = Vector4(1, 0.5, 0.5, 1); break; // Rosa
            case 2: color = Vector4(1, 1, 0, 1); break;    // Amarillo
            case 3: color = Vector4(0, 1, 1, 1); break;    // Cian
            }
            rend->color = color;
            Gen->addFirework(fire);
        }
    }
    DeregisterRenderItem(rend);
    delete rend;
}

void Firework::update(double t)
{
    integrate(t);
    //tiempo de vida (si es mayor que 5 segundos se borra)  
    lifetime += t;
    //comprobar si se tiene que borrar
    if (lifetime >= 0.6) 
        dest = true;
}


void Firework::integrate(double t)
{
    trans.p += vel * t;
    vel += (gS + acel) * t;
    vel *= pow(damping, t);
}