#include "Particle.h"

Particle::Particle(float radio, Vector4 c, Vector3 p)
{
	t = new PxTransform(p);
	
	particle = new RenderItem(CreateShape(PxSphereGeometry(radio)), t, c);
	vel = Vector3(0.0, 0.0003, 0);
}


void Particle::Update() {
	t->p += vel;
}
//#include "Particle.h"
//Particle::Particle(Vector3 pos, Vector3 vel, PxShape* _shape, Vector4 _color, RenderItem* _rend) {
//	vel.x = 2;
//	vel.y = 1;
//	color_ = _color;
//	shape = _shape;
//	renderItem = _rend;
//	_rend = new RenderItem(_shape, _color);
//		
//}
//
//void Particle::UpdateTr() {
//
//	this->pos.p += vel;
//}