#ifndef PHYSICS_SIMULATION_H
#define PHYSICS_SIMULATION_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include "RigidBody.h"
#include "Vector2D.h"
#include "Charts.h"

const float GRAVITY = 0.9f;
const float DT = 0.5f;
const float MAX_VELOCITY = 25.0f;
const int NUM_OBJECTS = 20;

float randomFloat(float min, float max) {
    return min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (max - min)));
}

void checkBounds(RigidBody& ball, const sf::RenderWindow& window) {
    if (ball.position.x - ball.radius < 0) {
        ball.position.x = ball.radius;
        ball.velocity.x = -ball.velocity.x;
    }
    if (ball.position.x + ball.radius > window.getSize().x) {
        ball.position.x = window.getSize().x - ball.radius;
        ball.velocity.x = -ball.velocity.x;
    }
    if (ball.position.y - ball.radius < 0) {
        ball.position.y = ball.radius;
        ball.velocity.y = -ball.velocity.y;
    }
    if (ball.position.y + ball.radius > window.getSize().y) {
        ball.position.y = window.getSize().y - ball.radius;
        ball.velocity.y = -ball.velocity.y;
    }
}

void drawArrow(sf::RenderWindow& window, Vector2D start, Vector2D end, sf::Color color) {
    sf::Vertex line[] = {
        sf::Vertex(sf::Vector2f(start.x, start.y), color),
        sf::Vertex(sf::Vector2f(end.x, end.y), color)
    };
    window.draw(line, 2, sf::Lines);
}

class PhysicsSimulation {
public:
    PhysicsSimulation() {
        srand(static_cast<unsigned int>(time(0)));

        for (int i = 0; i < NUM_OBJECTS; ++i) {
            Vector2D position(randomFloat(50.0f, 750.0f), randomFloat(50.0f, 550.0f));
            RigidBody ball(1.0f, position, RigidBody::ShapeType::Circle, 0.0f, 0.0f);
            ball.radius = 20.0f;
            ball.velocity = Vector2D(randomFloat(-50.0f, 50.0f), randomFloat(-50.0f, 50.0f));
            objects.push_back(ball);

            sf::CircleShape ballShape(ball.radius);
            ballShape.setOrigin(ball.radius, ball.radius);
            ballShape.setFillColor(sf::Color::Red);
            shapes.push_back(ballShape);
        }

        velocityChart = Chart(10, 10, 200, 100, "Velocity Chart", sf::Color::Blue);
        performanceChart = Chart(10, 120, 200, 100, "Performance Chart", sf::Color::Green);
        positionChart = Chart(220, 10, 200, 100, "Position Chart", sf::Color::Red);
        accelerationChart = Chart(220, 120, 200, 100, "Acceleration Chart", sf::Color::Yellow);
        forceChart = Chart(440, 10, 200, 100, "Force Chart", sf::Color::Magenta);
    }

    void update(sf::RenderWindow& window) {
        sf::Clock clock;

        for (auto& ball : objects) {
            Vector2D gravity(0, GRAVITY * ball.mass);
            ball.applyForce(gravity);
            ball.update(DT);
            checkBounds(ball, window);
            velocityChart.addData(ball.velocity.length());
            positionChart.addData(ball.position.y);
            accelerationChart.addData(ball.velocity.length() / DT);
            forceChart.addData(gravity.length());
        }

        for (size_t i = 0; i < objects.size(); ++i) {
            for (size_t j = i + 1; j < objects.size(); ++j) {
                if (objects[i].checkCollision(objects[j])) {
                    objects[i].resolveCollision(objects[j]);
                }
            }
        }

        for (size_t i = 0; i < objects.size(); ++i) {
            if (objects[i].velocity.length() > MAX_VELOCITY) {
                objects[i].velocity = objects[i].velocity.normalized() * MAX_VELOCITY;
            }
        }

        for (size_t i = 0; i < objects.size(); ++i) {
            shapes[i].setPosition(objects[i].position.x, objects[i].position.y);
        }

        performanceChart.addData(clock.getElapsedTime().asSeconds());

        window.clear(sf::Color::White);

        for (auto& shape : shapes) {
            window.draw(shape);
        }

        velocityChart.draw(window);
        performanceChart.draw(window);
        positionChart.draw(window);
        accelerationChart.draw(window);
        forceChart.draw(window);
    }

private:
    std::vector<RigidBody> objects;
    std::vector<sf::CircleShape> shapes;
    Chart velocityChart, performanceChart, positionChart, accelerationChart, forceChart;
};

#endif
