	#include "Generator.h"
	#include "AnchoredSpringFG.h"

	void Generator::update(float t) {
		cont -= t;
		bool anyFireworkActive = false;
		fireworkActive = anyFireworkActive;
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
			if (rb->getPosition().z > 0) {
				gameOver(); // Finalizar el juego si algún bloque cruza el límite
				break; // Salir del bucle ya que el juego ha terminado
			}
		}
		rbRgis->updateForces(t);
		deleteUnusedRB();
		if (cont <= 0 && !fireworkActive) { // Crear fuegos artificiales si es necesario
			Fireworks(t);
			//gameOver();
			winGame();
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
			cont = 50;       // Reiniciar el temporizador
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
			water = new BuoyancyForceGenerator(1200, 0.5, 1500);
			rbsMoving.clear();
			createFireworks(10);
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
			// Eliminar bloques en movimiento
			for (auto rb : rbsMoving) {
				rb->changeColor(Vector4(0.0, 0.0, 0.0, 0.0));
				delete rb;
			}
			water = new BuoyancyForceGenerator(1200, 0.5, 1500);
			/*for (auto shoot : shoots)shootsToDelete.push_back(shoot);
			deleteUnusedRB();*/
			for (auto rb : rbs)
				rbRgis->addRegistry(water, rb);
		}
	}

	void Generator::borraFlot() {
		// Eliminar el generador de flotación y restablecer el estado
		if (water) {
			delete water;
			water = nullptr;
		}

		// Restablecer el estado de pérdida
		lose = false;

		// Eliminar todos los cuerpos rígidos
		while (!rbs.empty()) {
			rbsToDelete.push_back(rbs.front());
			rbs.pop_front();
		}
		deleteUnusedRB();
	}
	void Generator::addFirework(Firework* f) 
	{ fireworks.push_back(f); }

	void Generator::addParticle(Particle* p) 
	{ particles.push_back(p); }

	void Generator::shootRBAlternate() {
		if (playing) {
			// Obtener la cámara actual
			Camera* cam = GetCamera();

			// Utilizar la dirección de la cámara para el disparo, pero fijar el eje Y a 0
			Vector3 camDirection = cam->getDir();
			camDirection.y = 0; // Fijar el eje Y a 0 para mantener una altura constante
			camDirection.normalize(); // Normalizar la dirección de la cámara

			// Ajustar la magnitud de la velocidad del proyectil
			Vector3 dir = camDirection * 1500; // Multiplicar por la velocidad deseada

			// La posición de spawn del proyectil será la posición actual de la cámara, pero con eje Y fijado en 0
			Vector3 pos = cam->getEye();
			pos.y = 0; // Fijar la altura de spawn en el eje Y a 0

			// Definir el color del proyectil (opcional)
			Vector4 color = Vector4(0, 1, 1, 1); // Color cian

			// Crear y agregar el nuevo proyectil a la lista de disparos
			RigidBody* rb_ = new RigidBody(scene, physics, pos, dir, Vector3(0, 0, 0), 0, 20, s_sphere, color, 4.5);
			shoots.push_back(rb_);
			bulletCount++;
		}
	}



	void Generator::createFireworks(int n) {

		if (!fireworkActive) {
			fireworkActive = true;
			int numberOfFireworks = n;
			// Obtén la posición y la dirección de la cámara
			Camera* camera = GetCamera(); // Asumiendo que tienes una función GetCamera()
			PxVec3 camPosition = camera->getEye();
			PxVec3 camDirection = camera->getDir();

			// Define la distancia frente a la cámara donde aparecerán los fuegos artificiales
			float distanceInFrontOfCamera = 20.0f;

			for (int i = 0; i < numberOfFireworks; i++) {
				// Calcula la posición inicial para cada fuego artificial
				PxVec3 fireworkPosition = camPosition + camDirection * distanceInFrontOfCamera;

				// Crea un nuevo fuego artificial en esta posición
				PxTransform initialPosition = PxTransform(fireworkPosition);
				Vector3 initialDirection = Vector3(0, 1, 0); // Dirección hacia arriba, modifica según sea necesario

				Firework* newFirework = new Firework(initialPosition, initialDirection, 0, this);
				this->fireworks.push_back(newFirework);
			}
		}
	}

	void Generator::createAnchoredSprings() {
		// Definir la posición del anclaje y las propiedades del muelle
		Vector3 anchorPosition(0.0f, 10.0f, 0.0f); // Ejemplo de posición del anclaje
		float springConstant = 10.0f; // Constante del muelle
		float restLength = 5.0f;      // Longitud de reposo

		// Añadir un AnchoredSpring a cada RigidBody
		for (auto rb : rbs) {
			AnchoredSpringFG* spring = new AnchoredSpringFG(springConstant, restLength, anchorPosition);
			rbRgis->addRegistry(spring, rb); // Asumiendo que rbRgis puede manejar AnchoredSprings
		}
	}
	void Generator::shootRBWithForce() {
		if (playing) {
			Camera* cam = GetCamera();
			Vector3 pos = cam->getEye() + cam->getDir();
			Vector3 dir = cam->getDir() * 1500;
			dir.y = 0;

			Vector4 color = Vector4(0, 1, 0, 1); // Color verde
			RigidBody* rb_ = new RigidBody(scene, physics, pos, dir, Vector3(0, 0, 0), 0, 20, s_sphere, color, 4.5);

			// Aplicar una fuerza aleatoria (viento o explosión) a este proyectil
			applyRandomForce(rb_);

			shoots.push_back(rb_);
			bulletCount++;
		}
	}

	void Generator::applyRandomForce(RigidBody* rb) {
		int choice = rand() % 2; // 0 o 1 para elegir aleatoriamente entre viento y explosión

		if (choice == 0) {
			int c = rand() % 4;
			if (c == 0) {
			// Aplicar fuerza de viento
			WindForceGenerator* wind = new WindForceGenerator(Vector3(60, 0, 0), 2005, 0);
			rbRgis->addRegistry(wind, rb);
			}
			else if(c == 1){
			// Aplicar fuerza de viento
			WindForceGenerator* wind = new WindForceGenerator(Vector3(-60, 0, 0), 2005, 0);
			rbRgis->addRegistry(wind, rb);
			}
			else if (c == 2) {
				// Aplicar fuerza de viento
				WindForceGenerator* wind = new WindForceGenerator(Vector3(0, 30, 0), 2005, 0);
				rbRgis->addRegistry(wind, rb);
			}
			else {
				// Aplicar fuerza de viento
				WindForceGenerator* wind = new WindForceGenerator(Vector3(0, -60, 0), 2005, 0);
				rbRgis->addRegistry(wind, rb);
			}
			
		}
		else {
			// Aplicar fuerza de explosión
			BurstForceGenerator* burst = new BurstForceGenerator(1000000, 15000000, 0);
			rbRgis->addRegistry(burst, rb);
		}
	}


	void Generator::applyWind(RigidBody* rb) {
	
	}


	void Generator::applySpecialForce(RigidBody* rb) {
		addExplosion();
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
