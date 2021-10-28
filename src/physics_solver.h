#ifndef PHYSICS_SOLVER_H
#define PHYSICS_SOLVER_H

#include <vector>
#include <mutex>
#include <array>
#include "particle.h"

namespace sf { class RenderWindow; }

class PhysicsSolver
{
private:
    static constexpr uint maxParticles = 1000u;
    std::mutex mtx;

    //std::vector<Particle> particles;
    Particle particles[maxParticles];
    sf::VertexArray particlesVA;        // for rendering particles

    void trimParticles();
    void solveParticle(uint idx, float dt);
    void solveParticleConcurrent(uint idx, uint fromParticle,float dt);
    uint countParticles() const;
public:
    PhysicsSolver();

    bool particleTrails = false;

    void update(float dt);
    void renderParticles(sf::RenderWindow* wnd);
    void addParticle(ParticleType type, sf::Vector2f position = sf::Vector2f(0.0f, 0.0f));
    void clearParticles();
    ParticleInfo getParticleInfo(ParticleType type);
};


#endif // PHYSICS_SOLVER_H
