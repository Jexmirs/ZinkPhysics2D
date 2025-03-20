#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include "Vector2D.h"
#include <cmath>
#include <string>

class RigidBody {
public:
    Vector2D position;
    Vector2D velocity;
    Vector2D acceleration;
    float mass;
    float invMass;
    float angle;
    float angularVelocity;
    float inertia;
    float invInertia;
    float radius;
    float dragCoefficient;

    enum class ShapeType { Circle, Rectangle };
    ShapeType shapeType;

    RigidBody(float mass, const Vector2D& position, ShapeType shape = ShapeType::Circle, float angle = 0.0f, float dragCoefficient = 0.1f)
        : mass(mass), position(position), velocity(0, 0), acceleration(0, 0),
        angle(angle), angularVelocity(0), inertia(1.0f), invInertia(1.0f), radius(0.0f), dragCoefficient(dragCoefficient),
        shapeType(shape) {

        if (mass != 0) {
            invMass = 1.0f / mass;
        }
        else {
            invMass = 0.0f;
        }

        if (shape == ShapeType::Circle) {
            radius = 1.0f;
            updateInertiaForShape();
        }
    }

    void applyForce(const Vector2D& force) {
        acceleration += force * invMass;
    }

    void applyTorque(float torque) {
        angularVelocity += torque * invInertia;
    }

    void applyGravity(const Vector2D& gravity) {
        applyForce(gravity * mass);
    }

    bool checkCollision(const RigidBody& other) {
        float dist = (position - other.position).length();
        if (shapeType == ShapeType::Circle && other.shapeType == ShapeType::Circle) {
            return dist < (radius + other.radius);
        }
        return false;
    }

    void resolveCollision(RigidBody& other) {
        if (checkCollision(other)) {
            Vector2D normal = (other.position - position).normalized();
            Vector2D relativeVelocity = velocity - other.velocity;
            float velocityAlongNormal = relativeVelocity.dot(normal);
            if (velocityAlongNormal > 0) return;

            float e = 0.9f;
            float j = -(1 + e) * velocityAlongNormal;
            j /= invMass + other.invMass;

            Vector2D impulse = normal * j;
            applyForce(impulse);
            other.applyForce(-impulse);

            Vector2D tangent = relativeVelocity - normal * relativeVelocity.dot(normal);
            float friction = 0.5f;
            Vector2D frictionImpulse = tangent * friction;
            applyForce(frictionImpulse);
            other.applyForce(-frictionImpulse);
        }
    }

    struct State {
        Vector2D position;
        Vector2D velocity;
    };

    struct Derivative {
        Vector2D dx;
        Vector2D dv;
    };

    Derivative evaluate(const State& state, float dt, const Derivative& d) {
        State newState;
        newState.position = state.position + d.dx * dt;
        newState.velocity = state.velocity + d.dv * dt;

        Derivative output;
        output.dx = newState.velocity;
        output.dv = acceleration;
        return output;
    }

    void integrateRK4(float dt) {
        State state = { position, velocity };

        Derivative a = evaluate(state, 0.0f, Derivative());
        Derivative b = evaluate(state, dt * 0.5f, a);
        Derivative c = evaluate(state, dt * 0.5f, b);
        Derivative d = evaluate(state, dt, c);

        Vector2D dxdt = (a.dx + 2.0f * (b.dx + c.dx) + d.dx) / 6.0f;
        Vector2D dvdt = (a.dv + 2.0f * (b.dv + c.dv) + d.dv) / 6.0f;

        position += dxdt * dt;
        velocity += dvdt * dt;

        acceleration = Vector2D(0, 0);
        angle += angularVelocity * dt;
    }

    void update(float deltaTime) {
        integrateRK4(deltaTime);
    }

    void radiusFromSize(const Vector2D& size) {
        if (shapeType == ShapeType::Circle) {
            radius = size.x / 2.0f;
        }
        else if (shapeType == ShapeType::Rectangle) {
            radius = std::sqrt((size.x / 2.0f) * (size.x / 2.0f) + (size.y / 2.0f) * (size.y / 2.0f));
        }
        mass = radius;
        invMass = 1.0f / mass;
    }

    float getKineticEnergy() {
        return 0.5f * mass * velocity.lengthSquared() + 0.5f * inertia * angularVelocity * angularVelocity;
    }

    void updateInertiaForShape() {
        if (shapeType == ShapeType::Circle) {
            inertia = 0.5f * mass * radius * radius;
        }
        else if (shapeType == ShapeType::Rectangle) {
            inertia = (1.0f / 12.0f) * mass * (radius * radius + radius * radius);
        }
        invInertia = 1.0f / inertia;
    }
};

#endif
