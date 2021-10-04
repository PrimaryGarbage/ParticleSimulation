#ifndef PARTICLE_H
#define PARTICLE_H
#include <SFML/Graphics.hpp>

enum ParticleType { Electron,  Proton };

struct Particle
{
    ParticleType type;
    sf::Vector2f position;
    sf::Vector2f velocity;
    bool active = true;

    Particle() = delete;
    Particle(ParticleType type, sf::Vector2f position = sf::Vector2f(0.0f, 0.0f)) : type(type), position(position), velocity(sf::Vector2f(0.0f, 0.0f)) {}


};

struct ParticleInfo
{
    float mass;
    float charge;
    float radius;
    sf::Color color;

    ParticleInfo() = delete;
    ParticleInfo(float mass, float charge, float radius,sf::Color color): mass(mass), charge(charge), radius(radius),color(color) {}
};

#endif // PARTICLE_H
