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

void resolveCollision(RigidBody& ball1, RigidBody& ball2) {
    Vector2D diff = ball2.position - ball1.position;
    float distance = diff.length();

    if (distance < ball1.radius + ball2.radius) {
        Vector2D normal = diff.normalized();
        Vector2D relativeVelocity = ball2.velocity - ball1.velocity;

        float velocityAlongNormal = relativeVelocity.dot(normal);

        if (velocityAlongNormal < 0) {
            float restitution = 1.0f;

            float impulse = -(1 + restitution) * velocityAlongNormal;
            impulse /= (ball1.invMass + ball2.invMass);

            Vector2D impulseVector = normal * impulse;

            ball1.velocity += impulseVector * ball1.invMass;
            ball2.velocity -= impulseVector * ball2.invMass;
        }
    }
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

int main() {
    srand(static_cast<unsigned int>(time(0)));
    sf::RenderWindow window(sf::VideoMode(800, 600), "2D Physics Engine");

    std::vector<RigidBody> objects;
    std::vector<sf::CircleShape> shapes;

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

    Chart velocityChart(10, 10, 200, 100, "Velocity Chart", sf::Color::Blue);
    Chart performanceChart(10, 120, 200, 100, "Performance Chart", sf::Color::Green);
    Chart positionChart(220, 10, 200, 100, "Position Chart", sf::Color::Red);
    Chart accelerationChart(220, 120, 200, 100, "Acceleration Chart", sf::Color::Yellow);
    Chart forceChart(440, 10, 200, 100, "Force Chart", sf::Color::Magenta);

    sf::Clock clock;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

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
                resolveCollision(objects[i], objects[j]);
            }
        }

        for (auto& ball : objects) {
            if (ball.velocity.length() > MAX_VELOCITY) {
                ball.velocity = ball.velocity.normalized() * MAX_VELOCITY;
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

        window.display();
    }

    return 0;
}
