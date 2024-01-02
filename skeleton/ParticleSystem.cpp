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
    //addGravity(p);
    //addWind(p);
    //addVortex(p);
    //addBurst(p);
}

void ParticleSystem::addFirework(Firework* f) {
    fireworks.push_back(f);
}

void ParticleSystem::addGravity(Particle* p) {
    /*gS = gR * powf((velS / velR), 2);
   inverse_massS = (powf(velS, 2) * inverse_mass) / powf(velR, 2);*/// m1 = ((v1^2)*m2)/(v2^2) Por igualación de 
   //energías y teniendo en cuenta que es la masa inversa
    float velS = p->getVelS();// en m/s (velocidad simulada)
    float velR = p->getVelR();// en m/s (velocidad real)
    Vector3 gR = Vector3(0, -9.8f, 0);// gravedad real
    //Vector3 gS = Vector3(0, -0.02245925758, 0);// gravedad simulada
    Vector3 gS = gR * powf((velS / velR), 2);// gravedad simulada

    GravityForceGenerator* g = new GravityForceGenerator(gS);
    partRgis->addRegistry(g, p);// añadimos la fuerza a la particula
}

void ParticleSystem::addWind(Particle* p) {
    Vector3 wind = Vector3(-10, 0, 0);
    float k1 = 0.25 /*coeficiente de rozamiento con el aire*/, k2 = 0.1;
    w = new WindForceGenerator(wind, k1, k2);
    partRgis->addRegistry(w, p);// añadimos la fuerza a la particula
}

void ParticleSystem::addVortex(Particle* p) {
    Vector3 wind = Vector3(-10, 0, 0);
    float k = 0.01 /*constante de fuerza del torbellino*/;
    VortexForceGenerator* v = new VortexForceGenerator(k, wind);
    partRgis->addRegistry(v, p);// añadimos la fuerza a la particula
}

void ParticleSystem::addBurst(Particle* p) {
    float k = 1000 /*constante de fuerza de la explosion*/;
    float r = 1500;
    float t = 2;
    BurstForceGenerator* b = new BurstForceGenerator(k, r, t);
    partRgis->addRegistry(b, p);// añadimos la fuerza a la particula
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
        // First one standard spring uniting 2 particles
        tr.p = Vector3(10, 30, 20);
        p1 = new Particle(s, tr, { 0.0, 0.0, 0.0 }, { 0.0, 0.0, 0.0 }, gS, 0.85, color, this);
        tr.p = Vector3(20, 30, 10);
        p2 = new Particle(s, { 0.0, 0.0, 0.0 }, { 0.0, 0.0, 0.0 }, { 0.0, 0.0, 0.0 }, gS, 0.85, color, this);
        //p2->setMass(2.0);
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
        //Then one spring with one fixed side
        tr.p = Vector3(10, 60, 20);
        p3 = new Particle(s, tr, { 0.0, 0.0, 0.0 }, { 0.0, 0.0, 0.0 }, gS, 0.85, color, this);
        //AnchoredSpringFG* f3 = new AnchoredSpringFG(0.1, 10, { 10.0,20.0,0.0 });
        tr.p = Vector3(20, 60, 10);
        //f3 = new AnchoredSpringFG(0, 10, tr.p);
        f3 = new AnchoredSpringFG(0.3, 10, tr.p);
        particles.push_back(f3->getOther());
        //_force_generators.push_back(f3);
        RegisterRenderItem(p3->getRenderItem());
        //addGravity(p3);
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
    case GOMAELASTICA:
        p1 = new Particle(s, tr, { 0.0, 0.0, 0.0 }, { 0.0, 0.0, 0.0 }, gS, 0.85, color, this);
        tr.p += Vector3(0, 15, 0);
        p2 = new Particle(s, { 10.0, 10.0, 0.0 }, { 0.0, 0.0, 0.0 }, { 0.0, 0.0, 0.0 }, gS, 0.85, color, this);
        p2->setMass(2.0);
        f1 = new SpringForceGenerator(1, 10, p2, true);
        partRgis->addRegistry(f1, p1);
        f2 = new SpringForceGenerator(1, 10, p1, true);
        partRgis->addRegistry(f2, p2);
        RegisterRenderItem(p1->getRenderItem());
        RegisterRenderItem(p2->getRenderItem());
        particles.push_back(p1);
        particles.push_back(p2);
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
    //if(f1 != nullptr)f1->setK(k);
    //if(f2 != nullptr)f2->setK(k);
    //if (f3 != nullptr) { f3->setK(k); }
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