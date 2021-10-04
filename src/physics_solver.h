#ifndef PHYSICS_SOLVER_H
#define PHYSICS_SOLVER_H

#include <vector>
#include "particle.h"

namespace sf { class RenderWindow; }

class PhysicsSolver
{
private:
    static constexpr uint maxParticles = 1000u;

    std::vector<Particle> particles;
    sf::VertexArray particlesVA;        // for rendering particles

    void trimParticles();
public:
    PhysicsSolver();

    void update(float dt);
    void renderParticles(sf::RenderWindow* wnd);
    void addParticle(ParticleType type, sf::Vector2f position = sf::Vector2f(0.0f, 0.0f));
    ParticleInfo getParticleInfo(ParticleType type);
};


#endif // PHYSICS_SOLVER_H
