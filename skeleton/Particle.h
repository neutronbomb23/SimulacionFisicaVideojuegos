#pragma once
#include "RenderUtils.hpp"
#include <PxPhysicsAPI.h>

using namespace physx;

class Particle
{
public:
	Particle(float radio, Vector4 c = Vector4(1, 1, 0, 1), Vector3 p = Vector3(0.0f, 0.0f, 0.0f));
	void Update();
	~Particle();
private:
	RenderItem* particle;
	PxTransform* t;
	Vector3 vel;
};


//#pragma once
//#include <vector>
//#include "core.hpp"
//#include <PxPhysicsAPI.h>
//#include "RenderUtils.hpp"
//
//using namespace std; 
//using namespace physx;
//class Particle
//{
//public:
//	Particle(Vector3 pos, Vector3 vel, PxShape* _shape, Vector4 _color, RenderItem* _rend);
//	~Particle();
//	void UpdateTr();
//	void Integrate(double t);
//private:
//	Vector3 vel;
//	PxTransform pos;
//	PxShape* shape;
//	Vector4 color_;
//	RenderItem* renderItem;
//};
//
