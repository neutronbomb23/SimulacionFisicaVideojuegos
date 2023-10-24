#pragma once
#include "RenderUtils.hpp"
#include <iostream>
using namespace physx;
// La clase particle representa una part�cula en la simulaci�n.
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
    // Configura la part�cula  de humo
    void setupSmoke() {
        v = RandVect(5); // Humo tiende a moverse lentamente y de manera err�tica
        acceleration = Vector3(0, 0, 0); // No tiene aceleraci�n inicial
        gravity = Vector3(0, 0.05, 0);  // Un ligero empuje hacia arriba debido a la flotaci�n del humo
        mass = 0.5f;    // Puede ser m�s ligero que una part�cula l�ser
        damp = 0.98f;   // Poco de amortiguamiento para que el humo se disipe
        color = Vector4(0.5, 0.5, 0.5, 0.7); // Un color gris claro para representar humo
        radius = 0.5f;  // Puede ser m�s grande ya que el humo tiende a dispersarse
       
    }

    // Configura la part�cula con propiedades de chispa
    void setupSpark() {
        v = RandVect(50); // Mayor velocidad inicial que el l�ser
        acceleration = Vector3(0, 0, 0); // No hay aceleraci�n en particular
        gravity = Vector3(0, -0.5, 0); // Una gravedad un poco m�s fuerte para que caiga r�pidamente
        mass = 0.5f; // M�s ligera que el l�ser
        damp = 0.95f; // Un factor de amortiguamiento similar
        color = Vector4(1, 0.7, 0.3, 1); // Un color anaranjado brillante para representar una chispa
        radius = 0.1f; // Un radio m�s peque�o que el l�ser
        lifetime = 0.0f; // Reiniciamos el tiempo de vida
    }
    // Configura la part�cula como una burbuja subacu�tica
    void setupBubble() {
        v = Vector3(0, RandFloat(0.1f, 1.0f), 0); // Ascendente lentamente. La funci�n RandFloat() generar�a un valor flotante aleatorio entre los dos valores proporcionados. Necesitar�as implementar esta funci�n o utilizar una existente.
        acceleration = Vector3(0, 0.05, 0); // Aceleraci�n hacia arriba debido al flotamiento.
        gravity = Vector3(0, 0, 0); // Las burbujas no se ven afectadas por la gravedad de la misma manera que otras part�culas, ya que su aceleraci�n de flotamiento contrarresta la gravedad.
        mass = 0.1f; // Las burbujas son ligeras.
        damp = 0.98f; // Amortiguamiento para representar la resistencia del agua.
        color = Vector4(0.8, 0.8, 0.8, 0.4); // Color transparente con reflejos. Podr�as querer implementar reflejos de una manera m�s compleja dependiendo de la iluminaci�n y el entorno.
        radius = RandFloat(0.05f, 0.5f); // El radio var�a desde peque�o a grande. De nuevo, necesitar�as una funci�n RandFloat() para generar valores aleatorios.
    }

    // eStrella fugaz meteorito
    void setupShootingStar() {
        v = Vector3(RandFloat(-1.0f, 1.0f), RandFloat(-1.0f, 1.0f), RandFloat(-1.0f, 1.0f)).getNormalized() * RandFloat(50.0f, 650.0f); // Velocidad muy r�pida en una direcci�n aleatoria. 
        acceleration = Vector3(0, 0, -40.0); // Aceleraci�n muy peque�a
        gravity = Vector3(0, 0, 0); // Estrellas fugaces o meteoritos no se ven afectados por la gravedad de manera tradicional en el espacio.
        mass = 2.0f; // M�s pesado que burbujas o chispas.
        damp = 1.0f; // Sin amortiguamiento en el espacio.
        color = Vector4(1, 1, 1, 1); // Color blanco brillante.
        radius = RandFloat(0.5f, 2.0f); // Radio var�a de medio a grande.
    }

    //RandFloat()
    float RandFloat(float a, float b) {
        return a + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (b - a)));
    }

};
