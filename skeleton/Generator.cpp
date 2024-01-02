	#include "Generator.h"

	void Generator::update(float t) {
		cont -= t;
		for (RigidBody* rb : rbs) {
			if (rb->isAlive()) { rb->integrate(t); }
			else { rbsToDelete.push_back(rb); }
		}
		for (RigidBody* shoot : shoots) {
			if (shoot->isAlive()) {
				shoot->integrate(t);
				// Comprueba si la posición Z del disparo es menor que -20.0
				if (shoot->getPosition().z < -20.0) {
					shootsToDelete.push_back(shoot);
				}
			}
			else {
				shootsToDelete.push_back(shoot);
			}
		}

		for (RigidBody* rb : rbs) {
			rb->updateMovement(t); // Actualizar el movimiento de cada cubo
		}
		for (RigidBody* rb : rbs) {
			if (rb->getPosition().x <= -15) { // Suponiendo que -15 es el umbral
				gameOver(); // El jugador pierde si algún bloque cruza el umbral
				break;
			}
		}

		for (RigidBody* rb : rbsMoving) {
			rb->moveTowardsNegativeZ(t);
			if (rb->getPosition().z > 130) {
				gameOver(); // Finalizar el juego si algún bloque cruza el límite
				break; // Salir del bucle ya que el juego ha terminado
			}
		}
		rbRgis->updateForces(t);
		deleteUnusedRB();
		if (cont <= 0) { 
			Fireworks(t);
			winGame();
		}
		if (win) {
			Fireworks(t);
		}
	}
	void Generator::deleteUnusedRB() {
		for (auto it = rbsToDelete.begin(); it != rbsToDelete.end();) {
			rbRgis->deleteRigidBodyRegistry(*it);
			rbs.remove(*it);
			delete(*it);
			it = rbsToDelete.erase(it);
			numObstacles--;
		}
		for (auto it = shootsToDelete.begin(); it != shootsToDelete.end();) {
			rbRgis->deleteRigidBodyRegistry(*it);
			shoots.remove(*it);
			delete(*it);
			it = shootsToDelete.erase(it);
		}
	}

	void Generator::shootRB() {
		if (playing) {
			Camera* cam = GetCamera();
			Vector3 pos = cam->getEye() + cam->getDir();
			Vector3 dir = Vector3(cam->getDir().x * 1000, cam->getDir().y * 1000, cam->getDir().z * 1000);
			addExplosion();
			RigidBody* rb_ = new RigidBody(scene, physics, pos, dir, Vector3(0, 0, 0), 0, 20, s_sphere, Vector4(1, 0, 0, 1), 4.5);
			shoots.push_back(rb_);
			bulletCount++;	
		}
	}



	bool Generator::nivel1() {
		if (!playing) {
			bulletCount = 0; // Reiniciar el conteo de balas
			cont = 5;       // Reiniciar el temporizador
			if (win) borraWin();
			if (lose) borraFlot();
			playing = true;
			spawnMovingBlocks();
			return true;
		}
		return false;
	}	
	void Generator::winGame() {
		if (!win) {
			win = true;
			playing = false;

			// Eliminar bloques en movimiento
			for (auto rb : rbsMoving) {
				rb->changeColor(Vector4(0.0, 0.0, 0.0, 0.0));
				delete rb;
			}
			rbsMoving.clear();

			// Llamar a deleteUnusedRB para limpiar los bloques regulares marcados para eliminación
			deleteUnusedRB();
		}
	}




	void Generator::borraWin() {
		win = false;
		while (!fireworks.empty()) {
			auto n = fireworks.front();
			fireworks.pop_front();
			delete n;
		}
		while (!particles.empty()) {
			auto n = particles.front();
			particles.pop_front();
			delete n;
		}
	}

	void Generator::gameOver() {
		if (!lose) {
			lose = true;
			playing = false;
			water = new BuoyancyForceGenerator(1200, 0.5, 1500);
			for (auto shoot : shoots)shootsToDelete.push_back(shoot);
			deleteUnusedRB();
			for (auto rb : rbs) 
				rbRgis->addRegistry(water, rb);
		}
	}
	void Generator::borraFlot() {
		while (!rbs.empty()) { rbsToDelete.push_back(rbs.front()); rbs.pop_front(); }
		deleteUnusedRB();
		//	delete water;
		lose = false;
	}

	void Generator::addFirework(Firework* f) 
	{ fireworks.push_back(f); }

	void Generator::addParticle(Particle* p) 
	{ particles.push_back(p); }

	void Generator::shootRBAlternate() {
		if (playing) {
			Camera* cam = GetCamera();
			Vector3 pos = cam->getEye() + cam->getDir();
			Vector3 dir = Vector3(cam->getDir().x * 1500, cam->getDir().y * 1500, cam->getDir().z * 1500); // Velocidad aumentada

			Vector4 color = Vector4(0, 1, 1, 1); // Color cian para diferenciarlo
			RigidBody* rb_ = new RigidBody(scene, physics, pos, dir, Vector3(0, 0, 0), 0, 20, s_sphere, color, 4.5); // Uso del nuevo color
			shoots.push_back(rb_);
			bulletCount++;
		}
	}

	void Generator::Fireworks(float t) {
		Camera* camera = GetCamera();
		double damp = 0.9f; // damping
		Vector3 acc = Vector3(0, 0, 0);
		Vector3 gS = Vector3(0, 0, 0);
		Vector3 vel = Vector3(0, 0, -10);
		//PxTransform tr = PxTransform(0, 20, 0);
		Vector4 color = Vector4(1, 1, 1, 1);
		for (numFires; numFires < fires; numFires++) {
			PxTransform tr = PxTransform(Vector3(camera->getTransform().p.x - numFires * 10,
				camera->getTransform().p.y - numFires * 5, camera->getTransform().p.z - numFires * 10));
			Firework* f = new Firework(tr, vel, acc, gS, damp, color, this);
			addFirework(f);
		}
		auto it = particles.begin();
		while (it != particles.end()) {
			auto aux = it;
			++aux;
			if ((*it)->getDest()) {
				delete* it;
				particles.erase(it);
				numFires--;
				if (numFires < 0)numFires = 0;
			}
			else { (*it)->update(t); }
			it = aux;
		}
		auto it2 = fireworks.begin();
		while (it2 != fireworks.end()) {
			auto aux = it2;
			++aux;
			if ((*it2)->getDest()) {
				delete* it2;
				fireworks.erase(it2);
			}
			else (*it2)->update(t);
			it2 = aux;
		}
	}
