#include "Particle.h"

Particle::Particle(PxShape* _shape, PxTransform trans_, Vector3 v, Vector3 acc, Vector3 gsim,  float damp, const Vector4& _color, 
    float Mass, float VelR, float VelS):
acel(acc), damping(damp), trans(trans_), vel(v), gS(gsim), mass(Mass), velR(VelR), velS(VelS){
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
    rend->transform = &trans;
    RegisterRenderItem(rend);
}

Particle::~Particle() {
	DeregisterRenderItem(rend);
	delete rend;
}


bool Particle::update(float t) {
    lifetime += t;
    if (lifetime >= 10.0f)dest = true;
    integrate(t);
    render();
    return _remaining_time > 0.0;
}

void Particle::render() {
	rend->transform = &trans;
}

void Particle::integrate(float t)
{
   
    Vector3 resulting_accel = _force_accum * inverse_massS;
    vel += resulting_accel * t; // Ex. 1.3 --> add acceleration
    vel *= powf(damping, t); // Exercise 1.3 --> add damping
    (&trans)->p += vel * t;
    _remaining_time -= t;
    // Clear accum
    clearAccum();
}
