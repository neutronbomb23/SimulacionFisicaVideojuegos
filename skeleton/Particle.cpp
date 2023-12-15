#include "Particle.h"
#include "ParticleSystem.h"

Particle::Particle(PxShape* _shape, PxTransform trans_, Vector3 v, Vector3 acc, Vector3 gsim,  float damp, const Vector4& _color, ParticleSystem* partS,
    float Mass, float VelR, float VelS):
acel(acc), damping(damp), trans(trans_), vel(v), gS(gsim), mass(Mass), velR(VelR), velS(VelS), partSys(partS){
   /*inverse_mass = 1 / mass;
    inverse_massS = (powf(velS, 2) * inverse_mass) / powf(velR, 2);
    massS = 1 / inverse_massS;*/
    //massS = 1; // para la gravedad
	rend = new RenderItem();
	rend->color = _color;
	rend->shape = _shape;
	rend->transform = &trans_;
}

Particle::Particle(PxTransform trans_, Vector3 dir)
    : trans(trans_)
{
    rend = new RenderItem();
    int r = rand() % 3;
    if (r == 0) bullet(dir);
    else if (r == 1) laser(dir);
    else fireball(dir);
    rend->transform = &trans;
    RegisterRenderItem(rend);
}

void Particle::fireball(Vector3 dir) {
    vel = dir * 5;// velocidad real 333 m/s
    acel = Vector3(0, 0, 0);
    gS = Vector3(0, -0.002, 0);
    //masa simulada 44,35 gramos aprox (real 10kg)
    damping = 0.9;
    Vector4 color = Vector4(1, 0.3, 0.3, 1);
    rend->color = color;
    rend->shape = CreateShape(PxSphereGeometry(1.2));
}

void Particle::bullet(Vector3 dir) {
    vel = dir * 45;// velocidad simulada 45 m/s
    acel = Vector3(0, 0, 0);
    gS = Vector3(0, -0.02245925758, 0);// gravedad simulada 
    damping = 1;
    //masa simulada 1.13 gr;
    damping = 0.9;
    Vector4 color = Vector4(1, 0.7, 0.7, 1);
    rend->color = color;
    rend->shape = CreateShape(PxSphereGeometry(1));
}

void Particle::laser(Vector3 dir) {
    // velocidad real 3*10^8 m/s
    vel = dir * 25;// velocidad simulada
    acel = Vector3(0, 0, 0);
    gS = Vector3(0, 0, 0);// aproximadamente 0
    //masa simulada infinita
    damping = 1;
    Vector4 color = Vector4(0, 1, 0, 1);
    rend->color = color;
    rend->shape = CreateShape(PxSphereGeometry(0.5));
}

Particle::~Particle() {
    if (partSys != nullptr)partSys->getPartRegis()->deleteParticleRegistry(this);
	DeregisterRenderItem(rend);
	delete rend;
}


bool Particle::update(float t) {
    lifetime += t;
    if (lifetime >= 100.0f)dest = true;
    integrate(t);
    render();
    return _remaining_time > 0.0;
}

void Particle::render() {
	rend->transform = &trans;
}

/*void Particle::integrate(float t)
{
	// Trivial case, infinite mass --> do nothing
	//if (inverse_massS <= 0.0f) return;
	// Update position
	(&trans)->p += vel * t;
	// Update linear velocity
	vel += acel * t;
	vel += gS * t;
	// Impose drag (damping)
	vel *= powf(damping, t);
}*/
void Particle::integrate(float t)
{
    //addForce(gS);
    // Semi-implicit Euler algorithm
    // Get the accel considering the force accum
    Vector3 resulting_accel = _force_accum * inverse_massS;
    vel += resulting_accel * t; // Ex. 1.3 --> add acceleration
    vel *= powf(damping, t); // Exercise 1.3 --> add damping
    (&trans)->p += vel * t;
    _remaining_time -= t;
    // Clear accum
    clearAccum();
}
