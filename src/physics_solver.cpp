#include "physics_solver.h"
#include <SFML/Graphics.hpp>
#include "tools.h"
#include "debug.h"
#include "app.h"
#include <thread>


namespace constants
{
    constexpr float electronMass = 0.0001f;
    constexpr float electronCharge = - 1.5f;
    constexpr float electronRadius = 0.1f;
    const sf::Color electronColor = sf::Color::Blue;

    constexpr float protonMass = 1.0f;
    constexpr float protonCharge = -electronCharge;
    constexpr float protonRadius = 3.0f;
    const sf::Color protonColor = sf::Color::Red;

    constexpr float strongForce = 0.1f;
    constexpr float maxSpeed = 100.0f;
    constexpr float maxAcceleration = 500.0f;
    constexpr float maxDistance = 500.0f;
}



PhysicsSolver::PhysicsSolver()
{
    particlesVA = sf::VertexArray(sf::Points);
}

void PhysicsSolver::update(float dt)
{
    sf::Clock clock;

    // clear particlesVA if no trails
    if(!particleTrails)
        particlesVA.clear();

//    // CLEAR SOLVEDFLAGS
//    FOR(UINT I = 0U; I < MAXPARTICLES; I++)
//        SOLVEDFLAGS[I] = FALSE;

    trimParticles();

    for(uint i = 0; i < maxParticles; i++)
    {
        if(particles[i].active)
        {
            solveParticle(i, dt);
            //solveParticleConcurrent(i, dt);
        }
    }

    //Debug::print("Physics frameTime: " + std::to_string(clock.getElapsedTime().asSeconds()));
    Debug::print("Physics fps: " + std::to_string(1.0f / clock.getElapsedTime().asSeconds()));
    Debug::print("Particles: " + std::to_string(countParticles()));
    Debug::print("ParticlesVA: " + std::to_string(particlesVA.getVertexCount()));
}

void PhysicsSolver::renderParticles(sf::RenderWindow* wnd)
{
    wnd->draw(particlesVA);
}

void PhysicsSolver::addParticle(ParticleType type, sf::Vector2f position)
{
    uint i;
    for(i = 0u; i < maxParticles; i++)
    {
        if(!particles[i].active)
            break;
    }

    // no room
    if(i == maxParticles) return;

    particles[i].type = type;
    particles[i].position = position;
    particles[i].velocity = sf::Vector2f(0.0f, 0.0f);
    particles[i].active = true;
}

void PhysicsSolver::clearParticles()
{
    for(uint i = 0u; i < maxParticles; i++)
    {
        particles[i].active = false;
    }

    // remove particle trails
    if(particleTrails)
        particlesVA.clear();
}

void PhysicsSolver::trimParticles()
{
    const sf::Vector2i &windSize = App::getWindowSize();
    for(uint i = 0; i < maxParticles; i++)
    {
        const sf::Vector2f &pos = particles[i].position;
        if(pos.x < 0 || pos.x > windSize.x || pos.y < 0 || pos.y > windSize.y)
            {
                particles[i].active = false;
            }
    }
}

void PhysicsSolver::solveParticle(uint idx, float dt)
{
    ParticleInfo info1 = getParticleInfo(particles[idx].type);

    for(uint j = 0u; j < maxParticles; j++)
    {
        if(!particles[j].active) continue;

        // if particle2 is the same particle or it already has been solved
        if(idx == j || solvedFlags[j])
            continue;

        // get direction vector between them
        sf::Vector2f dir = particles[idx].position - particles[j].position;

        // get distance between them
        float distance = chonk::sfLength(dir);

        // skip this particle if distance is too big
        if(distance > constants::maxDistance) continue;

        // normalize direction vec
        dir = chonk::sfNormalize(dir);

        // get particle2 info
        ParticleInfo info2 = getParticleInfo(particles[j].type);

        float impulse;
        if(distance <= 0.0f) continue;

        if(distance <= info2.radius)
        {
            impulse = constants::strongForce;
        }
        else
        {
            // calculate impulse increment
            impulse = info1.charge * info2.charge / (distance * distance) * dt;
        }

        //// ACCELERATION CLAMP ////
        // calculate particle1 velocity increment
        //particles[i].velocity += chonk::clampVec(impulse / info1.mass * chonk::sfNormalize(r), 0.0f, constants::maxAcceleration);
        // calculate particle2 velocity increment
        //particles[j].velocity += chonk::clampVec(impulse / info2.mass * chonk::sfNormalize(-r), 0.0f, constants::maxAcceleration);


        // calculate particle1 velocity increment
        particles[idx].velocity += impulse / info1.mass * dir;
        // calculate particle2 velocity increment
        particles[j].velocity += impulse / info2.mass * (-dir);
    }

    // clamp velocity length to maxSpeed
    particles[idx].velocity = chonk::clampVec(particles[idx].velocity, 0.0f, constants::maxSpeed);
    // apply velocity
    particles[idx].position += particles[idx].velocity * dt;
    // append particle vertex into VA
    particlesVA.append(sf::Vertex(particles[idx].position, getParticleInfo(particles[idx].type).color));
    // set this particle as solved for this frame
    solvedFlags[idx] = true;
}

void PhysicsSolver::solveParticleConcurrent(uint idx, float dt)
{
    uint workerMaxIdx = maxParticles / 2u ;
    uint workerMaxIdx1 = workerMaxIdx;
    uint workerIdx = idx;

    std::thread worker([&]()
    {
        ParticleInfo info1 = getParticleInfo(particles[idx].type);
        for(uint j = 0u; j < workerMaxIdx; j++)
        {
            if(!particles[j].active) continue;

            mtx.lock();
            // if particle2 is the same particle or it already has been solved
            if(idx == j || solvedFlags[j])
            {
                mtx.unlock();
                continue;
            }
            else
            {
                mtx.unlock();
            }

            // get vector between them
            sf::Vector2f r = particles[workerIdx].position - particles[j].position;

            // get distance between them
            float distance = chonk::sfLength(r);

            mtx.lock();
            // get particle2 info
            ParticleInfo info2 = getParticleInfo(particles[j].type);
            mtx.unlock();

            float impulse;
            if(distance <= 0.0f) continue;

            if(distance <= info2.radius)
            {
                impulse = constants::strongForce;
            }
            else
            {
                // calculate impulse increment
                impulse = info1.charge * info2.charge / (distance * distance) * dt;
            }

            //// ACCELERATION CLAMP ////
            // calculate particle1 velocity increment
            //particles[i].velocity += chonk::clampVec(impulse / info1.mass * chonk::sfNormalize(r), 0.0f, constants::maxAcceleration);
            // calculate particle2 velocity increment
            //particles[j].velocity += chonk::clampVec(impulse / info2.mass * chonk::sfNormalize(-r), 0.0f, constants::maxAcceleration);


            // calculate particle1 velocity increment
            particles[workerIdx].velocity += impulse / info1.mass * chonk::sfNormalize(r);
            // calculate particle2 velocity increment
            particles[j].velocity += impulse / info2.mass * chonk::sfNormalize(-r);
        }
    });

    ParticleInfo info1 = getParticleInfo(particles[idx].type);
    for(uint j = workerMaxIdx1; j < maxParticles; j++)
    {
        if(!particles[j].active) continue;

        mtx.lock();
        // if particle2 is the same particle or it already has been solved
        if(idx == j || solvedFlags[j])
        {
            mtx.unlock();
            continue;
        }
        else
        {
            mtx.unlock();
        }


        // get vector between them
        sf::Vector2f r = particles[idx].position - particles[j].position;

        // get distance between them
        float distance = chonk::sfLength(r);

        mtx.lock();
        // get particle2 info
        ParticleInfo info2 = getParticleInfo(particles[j].type);
        mtx.unlock();

        float impulse;
        if(distance <= 0.0f) continue;

        if(distance <= info2.radius)
        {
            impulse = constants::strongForce;
        }
        else
        {
            // calculate impulse increment
            impulse = info1.charge * info2.charge / (distance * distance) * dt;
        }

        //// ACCELERATION CLAMP ////
        // calculate particle1 velocity increment
        //particles[i].velocity += chonk::clampVec(impulse / info1.mass * chonk::sfNormalize(r), 0.0f, constants::maxAcceleration);
        // calculate particle2 velocity increment
        //particles[j].velocity += chonk::clampVec(impulse / info2.mass * chonk::sfNormalize(-r), 0.0f, constants::maxAcceleration);


        // calculate particle1 velocity increment
        particles[idx].velocity += impulse / info1.mass * chonk::sfNormalize(r);
        // calculate particle2 velocity increment
        particles[j].velocity += impulse / info2.mass * chonk::sfNormalize(-r);
    }

    worker.join();

    // clamp velocity length to maxSpeed
    particles[idx].velocity = chonk::clampVec(particles[idx].velocity, 0.0f, constants::maxSpeed);;
    particles[idx].position += particles[idx].velocity * dt;
    particlesVA.append(sf::Vertex(particles[idx].position, getParticleInfo(particles[idx].type).color));
    solvedFlags[idx] = true;
}

uint PhysicsSolver::countParticles() const
{
    uint count = 0u;
    for(uint i = 0u; i < maxParticles; i++)
        if(particles[i].active) count++;
    return count;
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
