#ifndef FLUIDSIMULATION_H
#define FLUIDSIMULATION_H

#include <vector>
#include <SFML/Graphics.hpp>
#include "Vector2D.h"

class FluidParticle {
public:
    Vector2D position;
    Vector2D velocity;
    float density;

    FluidParticle(const Vector2D& pos, const Vector2D& vel, float dens)
        : position(pos), velocity(vel), density(dens) {
    }
};

class FluidSimulation {
public:
    FluidSimulation(int width, int height, float particleSpacing);
    void addParticle(const Vector2D& position, const Vector2D& velocity, float density);
    void update(float dt);
    void render(sf::RenderWindow& window);

private:
    float m_width, m_height;
    float m_particleSpacing;
    std::vector<FluidParticle> m_particles;

    void applyForces(float dt);
    void updateParticles(float dt);
    void resolveCollisions();
};

FluidSimulation::FluidSimulation(int width, int height, float particleSpacing)
    : m_width(static_cast<float>(width)), m_height(static_cast<float>(height)), m_particleSpacing(particleSpacing) {
}

void FluidSimulation::addParticle(const Vector2D& position, const Vector2D& velocity, float density) {
    m_particles.push_back(FluidParticle(position, velocity, density));
}

void FluidSimulation::applyForces(float dt) {
    Vector2D gravity(0.0f, -9.81f);

    for (auto& particle : m_particles) {
        particle.velocity += gravity * dt;
    }
}

void FluidSimulation::updateParticles(float dt) {
    for (auto& particle : m_particles) {
        particle.position += particle.velocity * dt;
    }
}

void FluidSimulation::resolveCollisions() {
    for (auto& particle : m_particles) {
        if (particle.position.y < 0) {
            particle.velocity.y = std::abs(particle.velocity.y);
            particle.position.y = 0;
        }
        if (particle.position.y > m_height) {
            particle.velocity.y = -std::abs(particle.velocity.y);
            particle.position.y = m_height;
        }
        if (particle.position.x < 0) {
            particle.velocity.x = std::abs(particle.velocity.x);
            particle.position.x = 0;
        }
        if (particle.position.x > m_width) {
            particle.velocity.x = -std::abs(particle.velocity.x);
            particle.position.x = m_width;
        }
    }
}

void FluidSimulation::render(sf::RenderWindow& window) {
    for (const auto& particle : m_particles) {
        sf::CircleShape shape(2.0f);
        shape.setFillColor(sf::Color::Blue);
        shape.setPosition(particle.position.x, particle.position.y);
        window.draw(shape);
    }
}

void FluidSimulation::update(float dt) {
    applyForces(dt);
    updateParticles(dt);
    resolveCollisions();
}

#endif // FLUIDSIMULATION_H
