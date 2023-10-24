#pragma once
#include "RenderUtils.hpp"
#include <iostream>
using namespace physx;
// La clase particle representa una partícula en la simulación.
class particle
{
private:
    Vector3 v;        
    Vector4 color;        
    PxTransform p; 
    RenderItem* rend; 
    Vector3 acceleration, gravity;    
    float radius;
    float damp;     
    float mass;
    float lifetime = 0.0f;
    bool dest = false; 

public:
    particle(PxTransform pos, Vector3 vel, Vector3 acc, Vector3 grav, float weight, float damping, Vector4 c, float radius);
    particle(PxTransform pos);

    ~particle();

    void update(double t);
    void setColor(const Vector4& newColor);
    void integrate(double t);
    Vector3 RandVect(int size);
   
    void setupLaser();
    PxTransform* getPos() { return &p; }
    bool getDestroyed() { return dest; }
    RenderItem* getRend() const { return rend; }
    void setupFireball();
    void setupCanonball();
    // Configura la partícula  de humo
    void setupSmoke() {
        v = RandVect(5); // Humo tiende a moverse lentamente y de manera errática
        acceleration = Vector3(0, 0, 0); // No tiene aceleración inicial
        gravity = Vector3(0, 0.05, 0);  // Un ligero empuje hacia arriba debido a la flotación del humo
        mass = 0.5f;    // Puede ser más ligero que una partícula láser
        damp = 0.98f;   // Poco de amortiguamiento para que el humo se disipe
        color = Vector4(0.5, 0.5, 0.5, 0.7); // Un color gris claro para representar humo
        radius = 0.5f;  // Puede ser más grande ya que el humo tiende a dispersarse
       
    }

    // Configura la partícula con propiedades de chispa
    void setupSpark() {
        v = RandVect(50); // Mayor velocidad inicial que el láser
        acceleration = Vector3(0, 0, 0); // No hay aceleración en particular
        gravity = Vector3(0, -0.5, 0); // Una gravedad un poco más fuerte para que caiga rápidamente
        mass = 0.5f; // Más ligera que el láser
        damp = 0.95f; // Un factor de amortiguamiento similar
        color = Vector4(1, 0.7, 0.3, 1); // Un color anaranjado brillante para representar una chispa
        radius = 0.1f; // Un radio más pequeño que el láser
        lifetime = 0.0f; // Reiniciamos el tiempo de vida
    }
    // Configura la partícula como una burbuja subacuática
    void setupBubble() {
        v = Vector3(0, RandFloat(0.1f, 1.0f), 0); // Ascendente lentamente. La función RandFloat() generaría un valor flotante aleatorio entre los dos valores proporcionados. Necesitarías implementar esta función o utilizar una existente.
        acceleration = Vector3(0, 0.05, 0); // Aceleración hacia arriba debido al flotamiento.
        gravity = Vector3(0, 0, 0); // Las burbujas no se ven afectadas por la gravedad de la misma manera que otras partículas, ya que su aceleración de flotamiento contrarresta la gravedad.
        mass = 0.1f; // Las burbujas son ligeras.
        damp = 0.98f; // Amortiguamiento para representar la resistencia del agua.
        color = Vector4(0.8, 0.8, 0.8, 0.4); // Color transparente con reflejos. Podrías querer implementar reflejos de una manera más compleja dependiendo de la iluminación y el entorno.
        radius = RandFloat(0.05f, 0.5f); // El radio varía desde pequeño a grande. De nuevo, necesitarías una función RandFloat() para generar valores aleatorios.
    }

    // eStrella fugaz meteorito
    void setupShootingStar() {
        v = Vector3(RandFloat(-1.0f, 1.0f), RandFloat(-1.0f, 1.0f), RandFloat(-1.0f, 1.0f)).getNormalized() * RandFloat(50.0f, 650.0f); // Velocidad muy rápida en una dirección aleatoria. 
        acceleration = Vector3(0, 0, -40.0); // Aceleración muy pequeña
        gravity = Vector3(0, 0, 0); // Estrellas fugaces o meteoritos no se ven afectados por la gravedad de manera tradicional en el espacio.
        mass = 2.0f; // Más pesado que burbujas o chispas.
        damp = 1.0f; // Sin amortiguamiento en el espacio.
        color = Vector4(1, 1, 1, 1); // Color blanco brillante.
        radius = RandFloat(0.5f, 2.0f); // Radio varía de medio a grande.
    }

    //RandFloat()
    float RandFloat(float a, float b) {
        return a + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (b - a)));
    }

};
