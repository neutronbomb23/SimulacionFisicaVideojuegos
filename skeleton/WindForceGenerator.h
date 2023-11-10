#pragma once
#include "ParticleDragGenerator.h"
class WindForceGenerator :
    public ParticleDragGenerator
{
public:
    WindForceGenerator(const Vector3& w, const float k1 = 0.25, const float k2 = 0);
    virtual void updateForce(Particle* particle, double t);
    void normalize(Vector3& vec);
    void setWindDirectionRandom(); // Método para establecer una dirección de viento aleatoria
    inline void setWind(Vector3 w) { _wind = w; }

protected:
    Vector3 _wind;
    Vector3 abs(Vector3 a);
    Vector3 prodVect (Vector3 a, Vector3 b);
    float d = 0.00119; // Densidad del aire a 25º (1.19 g/L)
    float vol = 1000; // Volumen sobre el que se va a ejercer el viento
    float mass = d * vol; // Masa
};

