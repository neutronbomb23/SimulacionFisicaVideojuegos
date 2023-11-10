#pragma once
#include <PxPhysicsAPI.h>
#include "RenderUtils.hpp"
using namespace physx;
class Particle
{
private:
	float velR = 940;// velocidad real
	float velS = 45;// en m/s (velocidad simulada)
	float damping; // por defecto sería 0.9	
	float _remaining_time;
	float lifetime = 0.0f;

	Vector3 gR = Vector3(0, -9.8f, 0);// gravedad real
	Vector3 gS = Vector3(0, -0.02245925758, 0);// gravedad simulada
	Vector3 vel;
	Vector3 acel;
	Vector3 _force_accum = Vector3(0, 0, 0);

	RenderItem* rend;
	PxTransform trans;
	
	bool dest = false;

public:
	float startTime = 0;
	float inverse_mass = 250;// masa inversa 
	float mass = 0.004;// masa real 4 gramos
	float inverse_massS = 883.600;// masa inversa simulada (masa real simulada 1.13 gramos)
	float massS = 1/*0.0011*/;// masa simulada 1,1 gramos

	// Accumulated force
	Vector3 force;
	Particle(PxShape* _shape, PxTransform trans_, Vector3 v, Vector3 acc, Vector3 gsim, float damp, const Vector4& _color, 
		float Mass = 0.004, float VelR = 940, float VelS = 45);
	Particle(PxTransform pos, Vector3 dir/* int n = -1*/);
	~Particle();
	bool update(float t = 0.5);
	void render();
	void integrate(float t);

	float getMass() { return massS; };
	PxTransform* getTransform() { return &trans; };
	RenderItem* getRenderItem() { return rend; };
	bool getDest() { return dest; };
	Vector3 getVel() { return vel; }
	float getVelR() { return velR; }
	float getVelS() { return velS; }

	// Clears accumulated force
	void clearForce();

	// eleimina fuerza o fuerzas acumuladas
	inline void clearAccum() {
		_force_accum *= 0.0;
	}

	// Add force to apply in next integration only
	inline void addForce(Vector3 f) {
		_force_accum += f;
	}
};

