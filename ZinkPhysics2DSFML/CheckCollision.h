#ifndef CHECKCOLLISION_H
#define CHECKCOLLISION_H

#include "RigidBody.h"
#include <cmath>
#include <array>

namespace Collision {

    bool checkCircleCollision(const RigidBody& body1, const RigidBody& body2) {
        float distanceSquared = (body1.position - body2.position).lengthSquared();
        float radiusSum = body1.radius + body2.radius;
        return distanceSquared < (radiusSum * radiusSum);
    }

    bool checkRectangleCollisionSAT(const RigidBody& body1, const RigidBody& body2) {
        std::array<Vector2D, 4> body1Vertices = getRectangleVertices(body1);
        std::array<Vector2D, 4> body2Vertices = getRectangleVertices(body2);
        std::array<Vector2D, 8> axes = getAxes(body1, body2);

        for (const auto& axis : axes) {
            if (!overlapOnAxis(body1Vertices, body2Vertices, axis)) {
                return false;
            }
        }

        return true;
    }

    bool checkCircleRectangleCollision(const RigidBody& body1, const RigidBody& body2) {
        float closestX = std::clamp(body1.position.x, body2.position.x - body2.radius, body2.position.x + body2.radius);
        float closestY = std::clamp(body1.position.y, body2.position.y - body2.radius, body2.position.y + body2.radius);

        Vector2D closestPoint(closestX, closestY);
        float distanceSquared = (body1.position - closestPoint).lengthSquared();

        return distanceSquared < (body1.radius * body1.radius);
    }

    std::array<Vector2D, 4> getRectangleVertices(const RigidBody& body) {
        std::array<Vector2D, 4> vertices;
        float halfWidth = body.radius; // Assuming radius is half the width of the rectangle
        float halfHeight = body.radius; // Assuming radius is half the height of the rectangle
        float cosAngle = std::cos(body.angle);
        float sinAngle = std::sin(body.angle);

        vertices[0] = body.position + Vector2D(-halfWidth, -halfHeight).rotate(body.angle);
        vertices[1] = body.position + Vector2D(halfWidth, -halfHeight).rotate(body.angle);
        vertices[2] = body.position + Vector2D(halfWidth, halfHeight).rotate(body.angle);
        vertices[3] = body.position + Vector2D(-halfWidth, halfHeight).rotate(body.angle);

        return vertices;
    }

    std::array<Vector2D, 8> getAxes(const RigidBody& body1, const RigidBody& body2) {
        std::array<Vector2D, 8> axes;
        auto body1Vertices = getRectangleVertices(body1);
        auto body2Vertices = getRectangleVertices(body2);

        for (size_t i = 0; i < 4; i++) {
            Vector2D edge = body1Vertices[(i + 1) % 4] - body1Vertices[i];
            axes[i] = edge.perpendicular().normalized();
        }

        for (size_t i = 0; i < 4; i++) {
            Vector2D edge = body2Vertices[(i + 1) % 4] - body2Vertices[i];
            axes[4 + i] = edge.perpendicular().normalized();
        }

        return axes;
    }

    bool overlapOnAxis(const std::array<Vector2D, 4>& body1Vertices, const std::array<Vector2D, 4>& body2Vertices, const Vector2D& axis) {
        float min1, max1, min2, max2;

        projectVertices(body1Vertices, axis, min1, max1);
        projectVertices(body2Vertices, axis, min2, max2);

        return !(max1 < min2 || max2 < min1);
    }

    void projectVertices(const std::array<Vector2D, 4>& vertices, const Vector2D& axis, float& min, float& max) {
        min = max = axis.dot(vertices[0]);

        for (size_t i = 1; i < vertices.size(); i++) {
            float projection = axis.dot(vertices[i]);
            min = std::min(min, projection);
            max = std::max(max, projection);
        }
    }

    bool checkCollision(const RigidBody& body1, const RigidBody& body2) {
        if (checkCircleCollision(body1, body2)) {
            return true;
        }

        if (body1.shapeType == RigidBody::ShapeType::Rectangle && body2.shapeType == RigidBody::ShapeType::Rectangle) {
            return checkRectangleCollisionSAT(body1, body2);
        }

        if (body1.shapeType == RigidBody::ShapeType::Circle && body2.shapeType == RigidBody::ShapeType::Rectangle) {
            return checkCircleRectangleCollision(body1, body2);
        }

        if (body1.shapeType == RigidBody::ShapeType::Rectangle && body2.shapeType == RigidBody::ShapeType::Circle) {
            return checkCircleRectangleCollision(body2, body1);
        }

        return false;
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
}

#endif
