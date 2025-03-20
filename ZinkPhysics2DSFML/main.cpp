#include <SFML/Graphics.hpp>
#include "RigidBody.h"
#include "Vector2D.h"
#include <cstdlib>
#include <ctime>

const float GRAVITY = 0.5f;
const float DT = 0.01f;
const float MAX_VELOCITY = 25.0f;

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
            float restitution = 0.9f;
            float impulse = (2 * velocityAlongNormal) / (ball1.invMass + ball2.invMass);
            ball1.velocity += normal * (-impulse * ball1.invMass * restitution);
            ball2.velocity += normal * (impulse * ball2.invMass * restitution);
            float dampingFactor = 0.5f;
            ball1.velocity *= dampingFactor;
            ball2.velocity *= dampingFactor;
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

int main() {
    srand(static_cast<unsigned int>(time(0)));
    sf::RenderWindow window(sf::VideoMode(800, 600), "2D Physics Engine");

    Vector2D ball1Position(800, 100);
    RigidBody ball1(1.0f, ball1Position, RigidBody::ShapeType::Circle, 0.0f, 0.0f);
    ball1.radius = 20.0f;

    Vector2D ball2Position(300, 150);
    RigidBody ball2(1.0f, ball2Position, RigidBody::ShapeType::Circle, 0.0f, 0.0f);
    ball2.radius = 20.0f;

    Vector2D gravity(0, GRAVITY * ball1.mass);

    ball1.velocity = Vector2D(randomFloat(-50.0f, 50.0f), randomFloat(-50.0f, 50.0f));
    ball2.velocity = Vector2D(randomFloat(-50.0f, 50.0f), randomFloat(-50.0f, 50.0f));

    sf::CircleShape ball1Shape(ball1.radius);
    ball1Shape.setOrigin(ball1.radius, ball1.radius);
    ball1Shape.setFillColor(sf::Color::Red);

    sf::CircleShape ball2Shape(ball2.radius);
    ball2Shape.setOrigin(ball2.radius, ball2.radius);
    ball2Shape.setFillColor(sf::Color::Blue);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        ball1.applyForce(gravity);
        ball2.applyForce(gravity);

        ball1.update(DT);
        ball2.update(DT);

        resolveCollision(ball1, ball2);

        checkBounds(ball1, window);
        checkBounds(ball2, window);

        if (ball1.velocity.length() > MAX_VELOCITY) {
            ball1.velocity = ball1.velocity.normalized() * MAX_VELOCITY;
        }

        if (ball2.velocity.length() > MAX_VELOCITY) {
            ball2.velocity = ball2.velocity.normalized() * MAX_VELOCITY;
        }

        float scaleFactor = 1.5f;
        ball1Shape.setScale(scaleFactor, scaleFactor);
        ball2Shape.setScale(scaleFactor, scaleFactor);

        ball1Shape.setPosition(ball1.position.x, ball1.position.y);
        ball2Shape.setPosition(ball2.position.x, ball2.position.y);

        window.clear(sf::Color::White);
        window.draw(ball1Shape);
        window.draw(ball2Shape);
        window.display();
    }

    return 0;
}