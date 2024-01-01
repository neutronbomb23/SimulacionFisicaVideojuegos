#pragma once
#include "core.hpp"
#include "RenderUtils.hpp"
#include "PxRigidBody.h"
#include "PxShape.h"
#include <PxPhysicsAPI.h>
#include <PxPhysics.h>

using namespace physx;

enum Shape { s_cube, s_sphere, s_cone, s_pyramid, s_cylinder, s_torus };

class RigidBody {
protected:
	PxTransform transform;
	PxRigidDynamic* solid;
	PxShape* shape;
	RenderItem* render;

	Shape shapeType;
	float lifeTime; 

	bool alive;

public:
	RigidBody(PxScene* scene, PxPhysics* physics,
		const Vector3& Position, const Vector3& Velocity = Vector3(0, 0, 0), const Vector3& Inertia = Vector3(0, 0, 0),
		double Mass = 1, double LifeTime = 10,
		Shape Shape = s_cube, Vector4 Color = Vector4(0, 0, 0, 1)):lifeTime(LifeTime), alive(true), shapeType(Shape) {

		transform = physx::PxTransform(Position.x, Position.y, Position.z);
		solid = physics->createRigidDynamic(transform);

		solid->setLinearVelocity(Velocity);
		solid->setAngularVelocity(Vector3(0, 0, 0));
		solid->setLinearDamping(0.0);
		solid->setMassSpaceInertiaTensor(Inertia);
		solid->setGlobalPose(transform);
		solid->setMass(Mass);

		switch (shapeType)
		{
			//las demas paso, dan problemas
		case s_sphere:
			shape = CreateShape(PxSphereGeometry(1));
			break;
		case s_cube:
			shape = CreateShape(PxBoxGeometry(1, 1, 1));
			break;
		
		}

		solid->attachShape(*shape);

		PxRigidBodyExt::updateMassAndInertia(*solid, 0.15);
		scene->addActor(*solid);
		render = new RenderItem(shape, solid, Color);
	}

	~RigidBody() {
		DeregisterRenderItem(render);
	}

	void RigidBody::integrate(float t) {
		lifeTime -= t;

		// Retrieve the current position of the rigid body
		PxVec3 pos = solid->getGlobalPose().p;

		// Check if the rigid body is out of bounds in y or x axis, or if its lifetime has expired
		if (lifeTime <= 0 || pos.y <= -100 || abs(pos.x) > 300) { // limites buonding box
			alive = false;
		}
	}
	
	float getMass() { return solid->getMass(); }
	float getInvMass() { return solid->getInvMass(); }
	
	void RigidBody::setMass(float mass) {
		if (mass <= 0) {
			// Si la masa es 0 o negativa, se desactiva la dinámica del cuerpo rígido
			solid->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);
			solid->setMass(0.0f);
		}
		else {
			// Para masa positiva, se ajusta la masa y se recalcula la inercia
			solid->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, false);
			solid->setMass(mass);
			PxRigidBodyExt::updateMassAndInertia(*solid, mass);
		}
	}


	Vector3 getPosition() { return solid->getGlobalPose().p; }
	Vector3 getLinearVelocity() { return solid->getLinearVelocity(); }
	double getLifeTime() { return lifeTime; }
	void setLifeTime(double time) { lifeTime = time; }
	bool isAlive() { return alive; }
	Vector4 getColor() { return render->color; }
	Shape getShape() { return shapeType; }

	void addForce(Vector3 force) { solid->addForce(force); }
};

