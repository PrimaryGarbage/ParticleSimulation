#include "physics_solver.h"
#include <SFML/Graphics.hpp>
#include "tools.h"
#include <array>
#include "debug.h"
#include "app.h"


namespace constants
{
    constexpr float electronMass = 0.0001f;
    constexpr float electronCharge = - 1.0f;
    constexpr float electronRadius = 0.1f;
    const sf::Color electronColor = sf::Color::Blue;

    constexpr float protonMass = 1.0f;
    constexpr float protonCharge = 1.0f;
    constexpr float protonRadius = 5.0f;
    const sf::Color protonColor = sf::Color::Red;

    constexpr float strongForce = 0.1f;
    constexpr float maxSpeed = 100.0f;
    constexpr float maxAcceleration = 500.0f;
}



PhysicsSolver::PhysicsSolver()
{
    particlesVA = sf::VertexArray(sf::Points);
}

void PhysicsSolver::update(float dt)
{

    std::array<bool, maxParticles> solvedMatrix;    // <=====!!!!!!!!!!!!!!!!!! CHANGE THIS
    solvedMatrix.fill(false);
    particlesVA.clear();

    trimParticles();

    sf::Clock clock;
    float time;

    for(uint i = 0; i < particles.size(); i++)
    {
        // get particle1 info
        ParticleInfo info1 = getParticleInfo(particles[i].type);
        // set particle velocity
        for(uint j = 0; j < particles.size(); j++)
        {
            // if particle2 is the same particle or it already has been solved
            if(j == i || solvedMatrix[j]) continue;

            // get vector between them
            sf::Vector2f r = particles[i].position - particles[j].position;

            // get distance between them
            float distance = chonk::sfLength(r);

            // get particle2 info
            ParticleInfo info2 = getParticleInfo(particles[j].type);

            float impulse;
            if(distance <= 0.0f) continue;

            if(distance <= info2.radius)
                // apply strong force if distance is less then particle2 radius
                impulse = constants::strongForce;
            else
                // calculate impulse increment
                impulse = info1.charge * info2.charge / (distance * distance) * dt;

            //// ACCELERATION CLAMP ////
            // calculate particle1 velocity increment
            //particles[i].velocity += chonk::clampVec(impulse / info1.mass * chonk::sfNormalize(r), 0.0f, constants::maxAcceleration);
            // calculate particle2 velocity increment
            //particles[j].velocity += chonk::clampVec(impulse / info2.mass * chonk::sfNormalize(-r), 0.0f, constants::maxAcceleration);


            // calculate particle1 velocity increment
            particles[i].velocity += impulse / info1.mass * chonk::sfNormalize(r);
            // calculate particle2 velocity increment
            particles[j].velocity += impulse / info2.mass * chonk::sfNormalize(-r);
        }
        // set particle i as solved
        solvedMatrix[i] = true;

        // move particle
        particles[i].velocity = chonk::clampVec(particles[i].velocity, -constants::maxSpeed, constants::maxSpeed);
        particles[i].position += particles[i].velocity * dt;

        // append particle vertex
        particlesVA.append(sf::Vertex(particles[i].position, info1.color));
    }

    Debug::print("Particles: " + std::to_string(particles.size()));
    Debug::print("ParticlesVA: " + std::to_string(particlesVA.getVertexCount()));
    Debug::print("Physics frameTime: " + std::to_string(clock.getElapsedTime().asSeconds()));
}

void PhysicsSolver::renderParticles(sf::RenderWindow* wnd)
{
    wnd->draw(particlesVA);
}

void PhysicsSolver::addParticle(ParticleType type, sf::Vector2f position)
{
    if(particles.size() >= maxParticles) return;
    particles.emplace_back(type, position);
}

void PhysicsSolver::trimParticles()
{
    for(uint i = 0; i < particles.size(); i++)
    {
        sf::Vector2f pos = particles[i].position;
        sf::Vector2i windSize = App::getWindowSize();
        if(pos.x < 0 || pos.x > windSize.x || pos.y < 0 || pos.y > windSize.y)
            {
                particles.erase(particles.begin() + i);
            }
    }
}

ParticleInfo PhysicsSolver::getParticleInfo(ParticleType type)
{
    switch(type)
    {
        case ParticleType::Electron:
        {
            return ParticleInfo(constants::electronMass, constants::electronCharge, constants::electronRadius,constants::electronColor);
            break;
        }
        case ParticleType::Proton:
        {
            return ParticleInfo(constants::protonMass, constants::protonCharge, constants::protonRadius,constants::protonColor);
            break;
        }
        default:
        {
            return ParticleInfo(constants::electronMass, constants::electronCharge, constants::electronRadius,constants::electronColor);
            break;
        }
    }
}
