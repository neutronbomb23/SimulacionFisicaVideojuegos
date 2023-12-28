#include "ParticleSystem.h"

ParticleSystem::~ParticleSystem() {
    for (auto& i : particles) delete i; 
    for (auto& j : fireworks) delete j;
    partRgis->clear();
    delete partRgis;
}

void ParticleSystem::update(float t) {
    current += t;
    partRgis->updateForces(t);
    auto it = particles.begin();
    while (it != particles.end()) {
        auto aux = it;
        ++aux;
        if ((*it)->getDest()) {
            partRgis->deleteParticleRegistry((*it));
            delete* it;
            particles.erase(it);
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
    if (current > 5000 && b != nullptr)quitaBurst();
}

void ParticleSystem::addParticle(Particle* p) {
    particles.push_back(p);
 
}

void ParticleSystem::addFirework(Firework* f) {
    fireworks.push_back(f);
}

void ParticleSystem::addGravity(Particle* p) {
  
    float velS = p->getVelS();
    float velR = p->getVelR();
    Vector3 gR = Vector3(0, -9.8f, 0);
    Vector3 gS = gR * powf((velS / velR), 2);
    GravityForceGenerator* g = new GravityForceGenerator(gS);
    partRgis->addRegistry(g, p);
}

void ParticleSystem::addWind(Particle* p) {
    Vector3 wind = Vector3(-10, 0, 0);
    float k1 = 0.25, k2 = 0.1;
    w = new WindForceGenerator(wind, k1, k2);
    partRgis->addRegistry(w, p);
}

void ParticleSystem::addVortex(Particle* p) {
    Vector3 wind = Vector3(-10, 0, 0);
    float k = 0.01 /*constante de fuerza del torbellino*/;
    VortexForceGenerator* v = new VortexForceGenerator(k, wind);
    partRgis->addRegistry(v, p);
}

void ParticleSystem::addBurst(Particle* p) {
    float k = 1000;
    float r = 1500;
    float t = 2;
    BurstForceGenerator* b = new BurstForceGenerator(k, r, t);
    partRgis->addRegistry(b, p);
}

void ParticleSystem::addExplosion() {
    current = 0;
    for (auto& i : particles) addBurst(i);
}
void ParticleSystem::quitaBurst() {
    for (auto& i : particles) partRgis->deleteForceRegistry(b, i);
}

void ParticleSystem::generateSpringDemo(SpringType type) {
    Vector4 color = Vector4(0.9, 0, 0, 1);
    Vector3 gS = Vector3(0, 0, 0) /** powf((velS / velR), 2)*/;// gravedad simulada
    PxShape* s = CreateShape(PxSphereGeometry(1));
    Camera* camera = GetCamera();
    PxTransform tr = PxTransform(camera->getEye() + camera->getDir() * 50);
    BuoyancyForceGenerator* f4;
    Particle* p1;
    Particle* p2;
    Particle* p3;
    Particle* prev;
    Particle* p;
    int num = 2; float BETWEEN_SIZE = 15;

    switch (type)
    {
    case ParticleSystem::SPRING:
        tr.p = Vector3(10, 30, 20);
        p1 = new Particle(s, tr, { 0.0, 0.0, 0.0 }, { 0.0, 0.0, 0.0 }, gS, 0.85, color, this);
        tr.p = Vector3(20, 30, 10);
        p2 = new Particle(s, { 0.0, 0.0, 0.0 }, { 0.0, 0.0, 0.0 }, { 0.0, 0.0, 0.0 }, gS, 0.85, color, this);
        f1 = new SpringForceGenerator(0.04, 10, p2);
        partRgis->addRegistry(f1, p1);
        f2 = new SpringForceGenerator(0.04, 10, p1);
        partRgis->addRegistry(f2, p2);
        RegisterRenderItem(p1->getRenderItem());
        RegisterRenderItem(p2->getRenderItem());
        particles.push_back(p1);
        particles.push_back(p2);
        break;
    case ParticleSystem::ANCHORED:
        //Then one spring with one fixed side // fucking box
        tr.p = Vector3(10, 60, 20);
        p3 = new Particle(s, tr, { 0.0, 0.0, 0.0 }, { 0.0, 0.0, 0.0 }, gS, 0.85, color, this);
        tr.p = Vector3(20, 60, 10);
        f3 = new AnchoredSpringFG(0.3, 10, tr.p);
        particles.push_back(f3->getOther());
        RegisterRenderItem(p3->getRenderItem());
        partRgis->addRegistry(f3, p3);
        particles.push_back(p3);
        break;
    case ParticleSystem::FLOTABILITY:
        s = CreateShape(PxBoxGeometry(1, 0.1, 1));
        prev = new Particle(s, tr, Vector3(0, 0, 0), Vector3(0, 0, 0), gS, 0.85, color, this);
        RegisterRenderItem(prev->getRenderItem());
        particles.push_back(prev);
        s = CreateShape(PxBoxGeometry(1, 1, 1));
        for (int i = 0; i < num; ++i) {
            tr.p += Vector3(0, BETWEEN_SIZE * i, 0);
            p = new Particle(s, tr, Vector3(0, 0, 0), Vector3(0, 0, 0), gS, 0.85, color, this);
            RegisterRenderItem(p->getRenderItem());
            particles.push_back(p);


            SpringForceGenerator* sf = new SpringForceGenerator(0.1, 20, p);
            partRgis->addRegistry(sf, prev);

            sf = new SpringForceGenerator(0.1, 20, prev);
            partRgis->addRegistry(sf, p);

            prev = p;
        }
        break;
    case BUOYANCY:
        activateBuoyancy();
        break;
    default:
       break;
    }
}

void ParticleSystem::addK(int k) {
    for (auto FG : partRgis->getRegistry()) {
        SpringForceGenerator* SF = static_cast<SpringForceGenerator*>(FG.first);
        if (SF != nullptr) {
            SF->setK(k);
        }
    }
}

void ParticleSystem::generateBuoyancy() {
    BuoyancyForceGenerator* water = new BuoyancyForceGenerator(120, 0.5, 1000);
    particles.push_back(water->getPart());
    Vector4 color = Vector4(1, 0, 0, 1);
    PxTransform tr = PxTransform(0, 0, 0);
    Particle* p = new Particle(CreateShape(PxBoxGeometry(1, 1, 1)), tr, Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(0, 0, 0), 1, color, this);
    RegisterRenderItem(p->getRenderItem());
    p->setMass(300);
    particles.push_back(p);
    partRgis->addRegistry(water, p);

    tr = PxTransform(10, 0, 0);
    color = Vector4(1, 1, 0, 1);
    Particle* p1 = new Particle(CreateShape(PxBoxGeometry(1, 1, 1)), tr, Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(0, 0, 0), 1, color, this);
    RegisterRenderItem(p1->getRenderItem());
    p1->setMass(3000);
    particles.push_back(p1);
    partRgis->addRegistry(water, p1);

    tr = PxTransform(20, 0, 0);
    color = Vector4(0, 1, 0, 1);
    Particle* p2 = new Particle(CreateShape(PxBoxGeometry(1, 1, 1)), tr, Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(0, 0, 0), 1, color, this);
    p2->setMass(20);
    RegisterRenderItem(p2->getRenderItem());
    particles.push_back(p2);
    partRgis->addRegistry(water, p2);

    tr = PxTransform(30, 0, 0);
    color = Vector4(0, 1, 1, 1);
    Particle* p3 = new Particle(CreateShape(PxBoxGeometry(1, 1, 1)), tr, Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(0, 0, 0), 1, color, this);
    p3->setMass(120);
    RegisterRenderItem(p3->getRenderItem());
    particles.push_back(p3);
    partRgis->addRegistry(water, p3);
}

void ParticleSystem::generateRedRectangles() {
    Vector4 redColor = Vector4(1, 0, 0, 1); // Color rojo
    Vector3 rectangleSize = Vector3(5, 1, 3); // Dimensiones del rect�ngulo
    Vector3 startPosition(0, 0, 0); // Posici�n inicial de los rect�ngulos

    for (int i = 0; i < 5; ++i) {
        // Calcula la posici�n de cada rect�ngulo
        Vector3 position = startPosition + Vector3(0, i * 2, 0); // Ajusta la separaci�n vertical

        // Crea la forma del rect�ngulo
        PxShape* rectangleShape = CreateShape(PxBoxGeometry(rectangleSize.x, rectangleSize.y, rectangleSize.z));

        // Crea la transformaci�n con la posici�n
        PxTransform transform(position.x, position.y, position.z);

        // Crea la part�cula y la a�ade al sistema
        Particle* rectangle = new Particle(rectangleShape, transform, Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(0, 0, 0), 1.0f, redColor, this);
        RegisterRenderItem(rectangle->getRenderItem()); // Si tienes un sistema de renderizado
        particles.push_back(rectangle);
    }
}