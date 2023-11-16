#include "ParticleGenerator.h"

// Constructor: Initializes the particle system
ParticleGenerator::ParticleGenerator() {
    particleSystem = new ParticleSystem();
    generationTimer = 0.0f;
    isGenerating = true;
}

// Destructor: Cleans up the allocated particle system
ParticleGenerator::~ParticleGenerator() {
    delete particleSystem;
}

// Update the particle generator and system each frame
void ParticleGenerator::updateEveryFrame(float timeElapsed) {
    generationTimer += timeElapsed;
    if (generationTimer >= 0.0000000001f && isGenerating) {
        createParticle();
        generationTimer = 0.0f;
    }
    particleSystem->performUpdate(timeElapsed);
}

// Generate random 3D vector within a range
Vector3 ParticleGenerator::randomVectorInRange(int minSize, int maxSize) {
    Vector3 randomVec;
    randomVec.x = (rand() % (maxSize - minSize + 1)) + minSize - (maxSize / 2);
    randomVec.y = (rand() % (maxSize - minSize + 1)) + minSize - (maxSize / 2);
    randomVec.z = (rand() % (maxSize - minSize + 1)) + minSize - (maxSize / 2);
    return randomVec;
}

// Generate a random color
Vector4 ParticleGenerator::generateRandomColor() {
    Vector4 color;
    color.x = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    color.y = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    color.z = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    color.w = 1.0f; // Alpha value
    return color;
}

// Check if particle generation is enabled
bool ParticleGenerator::isGeneratingParticles() {
    return isGenerating;
}

// Generate a new particle
void ParticleGenerator::createParticle() {
    if (isGenerating) {
        Camera* camera = GetCamera();

        // Posición y velocidad con rango mayor
        Vector3 rPos = randomVectorInRange(-50, 50);
        Vector3 rVel = randomVectorInRange(-10, 10);

        PxTransform tr = PxTransform(Vector3(camera->getTransform().p.x + rPos.x,
            camera->getTransform().p.y + rPos.y,
            camera->getTransform().p.z + rPos.z));

        Vector3 vel = Vector3(camera->getDir().x + rVel.x, camera->getDir().y + rVel.y, camera->getDir().z + rVel.z);

        // Otros parámetros
        Vector4 color = generateRandomColor();
        double damp = 0.9f;
        Vector3 acc = Vector3(0, 0, 0);
        Vector3 gS = Vector3(0, -11.0, 0);

        PxGeometry* geom = chooseRandomGeometry(); // Obtiene una forma geométrica aleatoria
        PxShape* s = CreateShape(*geom); // Crea una forma con la geometría seleccionada

        Particle* p = new Particle(s, tr, vel, acc, gS, damp, color);
        //p->massS = s->getGeometry().sphere.radius * 10; // Ajusta la masa basándose en el radio si es una esfera 
        // DA ERROR! PREGUNTAR A RAÚL
        // Asigna una masa aleatoria a la partícula // Ahora ya no da error lo de abajo
        float randomMassva = randomMass(0.01f, 100.0f);
        p->massS = randomMassva;

        particleSystem->addNewParticle(p);
        RegisterRenderItem(p->getRenderItem());

        delete geom; // Limpia la memoria de la geometría
        isGenerating = true;
    }
}

// Toggle particle generation on or off
void ParticleGenerator::toggleGeneration() {
    isGenerating = !isGenerating;
}

// Generate a random geometry for the particles
PxGeometry* ParticleGenerator::chooseRandomGeometry() {
    int shapeType = rand() % 5; // Generate a number between 0 and 4
    switch (shapeType) {
    case 0: // Sphere
        return new PxSphereGeometry(static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 2.0f)));
    case 1: // Box
        return new PxBoxGeometry(
            static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 2.0f)),
            static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 2.0f)),
            static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 2.0f))
        );
    case 2: // Capsule
        return new PxCapsuleGeometry(
            static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 2.0f)),
            static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 2.0f))
        );
    case 3: // Cone (approximated with a box with a shorter dimension)
        return new PxBoxGeometry(
            static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 2.0f)),
            static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 4.0f)), // Shorter dimension for the "tip"
            static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 2.0f))
        );
    default:
        return new PxSphereGeometry(1.0f); // Default case
    }
}

// Generate a random mass within a specified range
float ParticleGenerator::randomMass(float minMass, float maxMass) {
    float random = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    return minMass + random * (maxMass - minMass);
}
