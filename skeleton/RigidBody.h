#pragma once
#include "core.hpp"
#include "RenderUtils.hpp"
#include "PxRigidBody.h"
#include "PxShape.h"
#include <algorithm>  

#include <random>  // Incluir la biblioteca para generación de números aleatorios
#include <PxPhysicsAPI.h>
#include <PxPhysics.h>

using namespace physx;

enum Shape { s_cube, s_sphere, s_rect, s_capsule };


class RigidBody {
protected:
	PxTransform transform;
	PxRigidDynamic* solid = nullptr;
	PxShape* shape = nullptr;
	RenderItem* render = nullptr;
	PxScene* scn = nullptr;

	Shape shapeType;
	float lifeTime; 
	bool alive;
	bool time;
	float moveSpeed = 5.0f; // Velocidad de movimiento
	float moveLimit = 30.0f; // Límite de movimiento a cada lado
	float currentMove = 0.0f; // Distancia actual movida desde el punto inicial
	bool movingRight = true; // Indica si el objeto está moviéndose hacia la derecha
	

public:
	int hit;
	void HitPlus() {
		hit++;
	}
	enum Direction { LEFT, RIGHT };  // Enumeración para la dirección
	Direction movementDirection;     // Variable para almacenar la dirección actual
	RigidBody(PxScene* scene, PxPhysics* physics,
		const Vector3& Position, const Vector3& Velocity = Vector3(0, 0, 0), const Vector3& Inertia = Vector3(0, 0, 0),
		double Mass = 1, double LifeTime = 30,
		Shape Shape = s_cube, Vector4 Color = Vector4(0, 0, 0, 1), int size = 1):lifeTime(LifeTime), alive(true), shapeType(Shape), scn(scene) {
		hit = 0;
		time = true;
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
		case s_capsule:
			shape = CreateShape(PxCapsuleGeometry(size, size));
			break;
		case s_sphere:
			shape = CreateShape(PxSphereGeometry(size));
			break;
		case s_cube:
			shape = CreateShape(PxBoxGeometry(size, size, size));
			break;
		case s_rect:
			shape = CreateShape(PxBoxGeometry(size, 2* size, size));
			break;
		}

		solid->attachShape(*shape);

		PxRigidBodyExt::updateMassAndInertia(*solid, 0.15);
		scene->addActor(*solid);
		render = new RenderItem(shape, solid, Color);
		RegisterRenderItem(render);
	}
	void updateMovement(float deltaTime) {
		// Suponiendo que tienes variables para controlar los límites de movimiento
		float leftLimit = 50.0f;
		float rightLimit = 100.0f;

		Vector3 currentPosition = getPosition();

		// Asegurarse de que el cubo no exceda los límites
		if (currentPosition.x <= leftLimit) {
			movingRight = true; // Cambia la dirección a la derecha
		}
		else if (currentPosition.x >= rightLimit) {
			movingRight = false; // Cambia la dirección a la izquierda
		}

		// Mover el cubo en la dirección actual
		if (movingRight) {
			currentPosition.x += moveSpeed * deltaTime;
		}
		else {
			currentPosition.x -= moveSpeed * deltaTime;
		}

		// Actualizar la posición del cubo
		setPosition(currentPosition);
	}

	void setRandomMass(float minMass, float maxMass) {
		// Crear motor de generación de números aleatorios
		std::random_device rd;
		std::mt19937 gen(rd());

		// Definir distribución para el rango de masas
		std::uniform_real_distribution<> dis(minMass, maxMass);

		// Generar masa aleatoria y establecerla
		float randomMass = static_cast<float>(dis(gen));
		solid->setMass(randomMass);

		// Actualizar inercia basada en la nueva masa
		PxRigidBodyExt::updateMassAndInertia(*solid, 0.15);
	}

	~RigidBody() {
		
	}

	// Método para establecer la dirección
	void setDirection(Direction dir) {
		movementDirection = dir;
	}

	void setPosition(const Vector3& newPosition) {
		transform = PxTransform(newPosition.x, newPosition.y, newPosition.z);
		if (solid) {
			solid->setGlobalPose(transform);
		}
	}

	// Método para mover el cuerpo rígido
	void move(float deltaTime) {
		Vector3 currentPosition = getPosition();
		float speed = 5.0f;  // Velocidad de movimiento, ajusta según sea necesario

		// Mover hacia la izquierda o la derecha según la dirección
		if (movementDirection == LEFT) {
			currentPosition.x -= speed * deltaTime;
		}
		else if (movementDirection == RIGHT) {
			currentPosition.x += speed * deltaTime;
		}

		// Ajustar la posición
		setPosition(currentPosition);
	}

	void integrate(float t) {
		transform = solid->getGlobalPose();
		if(time)lifeTime -= t;
		if (lifeTime <= 0 || fueraLimites()) 
			alive = false;
	};
	void moveHorizontal(float deltaTime, float speed, float leftLimit, float rightLimit) {
	
	}

	Vector3 getPosition() { return solid->getGlobalPose().p; }
	Vector3 getLinearVelocity() { return solid->getLinearVelocity(); }
	float getMass() { return solid->getMass(); }
	float getInvMass() { return solid->getInvMass(); }
	double getLifeTime() { return lifeTime; }

	void moveTowardsNegativeZ(float deltaTime) {
		Vector3 currentPosition = getPosition();
		currentPosition.z += moveSpeed * deltaTime; // Mover hacia Z negativo
		setPosition(currentPosition);
	}

	// Método para establecer una velocidad aleatoria en el eje Z
	void setRandomZVelocity(float minSpeed, float maxSpeed) {
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<> dis(minSpeed, maxSpeed);

		float randomSpeed = static_cast<float>(dis(gen));
		Vector3 currentVelocity = getLinearVelocity();
		currentVelocity.z = -randomSpeed; // Asegurarse de que la velocidad sea negativa (hacia la cámara)
		solid->setLinearVelocity(currentVelocity);
	}

	void changeColor(const Vector4& newColor)
	{
		if (render != nullptr) {
			render->color = newColor;
		}
	}

	bool isAlive() { return alive; }
	Vector4 getColor() { return render->color; }
	Shape getShape() { return shapeType; }
	void setTime(bool t = true) { time = t; }
	void setLifeTime(float t) { lifeTime = t; }

	void addForce(Vector3 force) { solid->addForce(force); }
	bool fueraLimites() {
		bool y = transform.p.y <= -10;
		bool fueraSuelo = transform.p.x >= 470 || transform.p.z >= 470 || transform.p.x <= -50 || transform.p.z <= -50;
		return (y || fueraSuelo);
	}
};

