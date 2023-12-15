#include "ParticleGenerator.h"

ParticleGenerator::ParticleGenerator() {
    partSyst = new ParticleSystem();
}

ParticleGenerator::~ParticleGenerator() {
    delete partSyst;
}

void ParticleGenerator::update(float t) {
    timer += t;
    if (timer >= 0.05f) {
        //generate();
        timer = 0.0f;
    }
    partSyst->update(t);
}

Vector3 ParticleGenerator::UniformDistribution(int size) {
    Vector3 rnd;
    rnd.x = rand() % size - (size / 2);
    rnd.y = rand() % size - (size / 2);
    rnd.z = rand() % size - (size / 2);
    return rnd;
}

Vector3 ParticleGenerator::NormalDistribution(float m, float d) {
    Vector3 rnd;

    // Genera números aleatorios en una distribución normal
    float x = RandomNormal(m, d);
    float y = RandomNormal(m, d);
    float z = RandomNormal(m, d);

    rnd.x = x;
    rnd.y = y;
    rnd.z = z;

    return rnd;
}

float ParticleGenerator::RandomNormal(float mean, float stddev) {
    // Genera un número aleatorio con distribución normal usando Box-Muller
    float u1 = 1.0 - rand() / static_cast<float>(RAND_MAX);
    float u2 = 1.0 - rand() / static_cast<float>(RAND_MAX);
    float z = sqrt(-2.0 * log(u1)) * cos(2.0 * 3.14 * u2);
    return mean + stddev * z;
}

void ParticleGenerator::generate(int gen) {
    Camera* camera = GetCamera();

    // posicion con randoms
    Vector3 rPos = UniformDistribution(20);
    //Vector3 rPos = NormalDistribution(0, 5);
    PxTransform tr = PxTransform(Vector3(camera->getTransform().p.x + rPos.x,
        camera->getTransform().p.y + rPos.y, camera->getTransform().p.z + rPos.z));

    // velocidad con randoms
    Vector3 rVel = UniformDistribution(10);
    //Vector3 rVel = NormalDistribution(5, 5);
    Vector3 vel = Vector3(camera->getDir().x + rVel.x, camera->getDir().y + rVel.y, camera->getDir().z + rVel.z);

    //otros parámetros
    Vector4 color;
    double damp = 0.9f; // damping
    //Vector3 gS = Vector3(0, -10, 0);
    Vector3 acc = Vector3(0, 0, 0);
    Vector3 gS = Vector3(0, -9.8, 0) /** powf((velS / velR), 2)*/;// gravedad simulada
    PxShape* s = CreateShape(PxSphereGeometry(1));

    /*int rnd = rand() % 2;
    if (rnd == 0) {
        color = Vector4(0.9, 0, 0, 1);
        Particle* p = new Particle(s, tr, vel, acc, gS, damp, color);
        partSyst->addParticle(p);
        RegisterRenderItem(p->getRenderItem());
    }
    else {
        gS = Vector3(0, 0, 0);
        color = Vector4(1, 1, 1, 1);
        Firework* f = new Firework(tr, vel, acc, gS, damp, color, partSyst, gen);
        partSyst->addFirework(f);
    }*/
    int rnd = rand() % 2;
    if (rnd == 0) 
    {
        color = Vector4(0.9, 0, 0, 1);
        Particle* p = new Particle(s, tr, vel, acc, gS, damp, color);
        p->massS = 10;
        partSyst->addParticle(p);
        RegisterRenderItem(p->getRenderItem());
    }
    else {
        color = Vector4(0.9, 0.4, 0, 1);
        Particle *p = new Particle(s, tr, vel, acc, gS, damp, color);
        p->massS = 1;
        partSyst->addParticle(p);
        RegisterRenderItem(p->getRenderItem());
    }
    
  
}
