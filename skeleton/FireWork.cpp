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
    float num = 1 - 0.25 * gen;
    vel = dir * 10 * num;
    rend->color = Vector4(1, num, num, 1);
    rend->shape = CreateShape(physx::PxSphereGeometry(num * 0.5));
    RegisterRenderItem(rend);
}

Firework::~Firework()
{
    Vector3 dir;
    float angle;
    int r = (rand() % 10) + 1;
    if (gen == 3) {
        int r = (rand() % 5) + 1;
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
    if (lifetime >= 1.0) 
        dest = true;
}


void Firework::integrate(double t)
{
    trans.p += vel * t;
    vel += (gS + acel) * t;
    vel *= pow(damping, t);
}