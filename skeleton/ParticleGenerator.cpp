#include "ParticleGenerator.h"

// Constructor: Inicializa el generador de partículas con un nuevo sistema de partículas
ParticleGenerator::ParticleGenerator() {
    partSyst = new ParticleSystem();
    timeSinceLastGeneration = 0.0f;
    generationInterval = 1.0f;  // Genera partículas cada segundo
}

// Destructor: Libera la memoria del sistema de partículas
ParticleGenerator::~ParticleGenerator() {
    delete partSyst;
}

// Actualiza el generador y verifica si es momento de generar una nueva partícula
void ParticleGenerator::update(float t) {
    timeSinceLastGeneration += t;
    if (timeSinceLastGeneration >= generationInterval) {
        generate();
        timeSinceLastGeneration = 0.0f; // Restablece el contador
    }
    partSyst->update(t);
}

// Genera un vector con valores aleatorios entre -size y size
Vector3 ParticleGenerator::UniformDistribution(int size) {
    Vector3 rnd;
    rnd.x = rand() % size - size;
    rnd.y = rand() % size - size;
    rnd.z = rand() % size - size;
    return rnd;
}



// Genera una dirección aleatoria normalizada
Vector3 ParticleGenerator::UniformDirection() {
    float theta = (rand() / static_cast<float>(RAND_MAX)) * 2.0 * 3.14159265358979323846;
    float phi = acos(2.0 * (rand() / static_cast<float>(RAND_MAX)) - 1.0);
    Vector3 direction;
    direction.x = sin(phi) * cos(theta);
    direction.y = sin(phi) * sin(theta);
    direction.z = cos(phi);
    return direction;
}

// Genera un vector con valores aleatorios siguiendo una distribución normal
Vector3 ParticleGenerator::NormalDistribution(float m, float d) {
    Vector3 rnd;
    rnd.x = RandomNormal(m, d);
    rnd.y = RandomNormal(m, d);
    rnd.z = RandomNormal(m, d);
    return rnd;
}

// Genera un valor float aleatorio basado en una distribución normal
float ParticleGenerator::RandomNormal(float mean, float stddev) {
    float u1 = 1.0 - rand() / static_cast<float>(RAND_MAX);
    float u2 = 1.0 - rand() / static_cast<float>(RAND_MAX);
    float z = sqrt(-2.0 * log(u1)) * cos(2.0 * 3.14159265358979323846 * u2);
    return mean + stddev * z;
}



// Genera un color aleatorio de una lista predefinida
Vector4 ParticleGenerator::RandomColor() {
    int rndColor = rand() % 5;
    switch (rndColor) {
    case 0: return Vector4(1, 1, 0, 1);  // Amarillo
    case 1: return Vector4(0, 0, 0, 1);  // Negro
    case 2: return Vector4(1, 1, 1, 1);  // Blanco
    case 3: return Vector4(1, 0, 0, 1);  // Rojo
    case 4: return Vector4(0.6, 0.3, 0.2, 1);  // Marrón
    default: return Vector4(1, 1, 1, 1);  // Blanco (default)
    }
}

// Genera partículas o fuegos artificiales y los agrega al sistema de partículas
void ParticleGenerator::generate() {
    Camera* camera = GetCamera();
    Vector3 rPos = UniformDistribution(20);
    PxTransform pos = PxTransform(Vector3(camera->getTransform().p.x + rPos.x,
        camera->getTransform().p.y + rPos.y, camera->getTransform().p.z + rPos.z));
    Vector3 rVel = UniformDistribution(50);
    Vector3 velDirection = UniformDirection();
    Vector3 vel = velDirection + rVel;
    double damp = 0.9f;
    Vector3 gravity = Vector3(0, -10, 0);
    Vector3 acceleration = Vector3(0, 0, 0);
    float r = 0.1f;
    Vector4 color;
    float weight = 1.0f;

    color = RandomColor();

    int rndgen = rand() % 3;

    if(rndgen %2 == 0) {
        r = 0.1f;
        vel = Vector3(0, 15, 0);
        gravity = Vector3(0, 0, 0);
        Firework* f = new Firework(pos, vel, acceleration, gravity, weight, damp, color, r, partSyst);
        partSyst->addFirework(f);
    }
    else {
       camera = GetCamera();
       particle* humito = new particle(camera->getTransform());
       humito->setupSmoke();
       partSyst->addParticle(humito);
    }
}
