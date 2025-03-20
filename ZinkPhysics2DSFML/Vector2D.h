#ifndef VECTOR2D_H
#define VECTOR2D_H

#include <cmath>
#include <iostream>
#include <algorithm>

class Vector2D {
public:
    float x, y;

    Vector2D() : x(0), y(0) {}

    Vector2D(float x, float y) : x(x), y(y) {}

    Vector2D operator+=(const Vector2D& other) {
        x += other.x;
        y += other.y;
        return *this;
    }

    Vector2D operator-=(const Vector2D& other) {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    Vector2D operator*=(float scalar) {
        x *= scalar;
        y *= scalar;
        return *this;
    }

    Vector2D operator/=(float scalar) {
        if (scalar == 0) {
            throw std::invalid_argument("Division by zero in Vector2D operator/=");
        }
        x /= scalar;
        y /= scalar;
        return *this;
    }

    Vector2D operator+(const Vector2D& other) const {
        return Vector2D(x + other.x, y + other.y);
    }

    Vector2D operator-(const Vector2D& other) const {
        return Vector2D(x - other.x, y - other.y);
    }

    Vector2D operator*(float scalar) const {
        return Vector2D(x * scalar, y * scalar);
    }

    Vector2D operator/(float scalar) const {
        if (scalar == 0) {
            throw std::invalid_argument("Division by zero in Vector2D operator/");
        }
        return Vector2D(x / scalar, y / scalar);
    }

    float length() const {
        return std::sqrt(x * x + y * y);
    }

    float lengthSquared() const {
        return x * x + y * y;
    }

    void normalize() {
        float len = length();
        if (len > 0) {
            x /= len;
            y /= len;
        }
    }

    Vector2D normalized() const {
        float len = length();
        if (len > 0) {
            return *this / len;
        }
        return *this;
    }

    Vector2D operator-() const {
        return Vector2D(-x, -y);
    }


    float dot(const Vector2D& other) const {
        return x * other.x + y * other.y;
    }

    Vector2D perpendicular() const {
        return Vector2D(-y, x);
    }

    Vector2D project(const Vector2D& other) const {
        float dotProduct = dot(other);
        float otherLengthSquared = other.lengthSquared();
        if (otherLengthSquared > 0) {
            return other * (dotProduct / otherLengthSquared);
        }
        return Vector2D(0, 0);
    }

    float angle(const Vector2D& other) const {
        float dotProd = dot(other);
        float lenProd = length() * other.length();
        if (lenProd != 0) {
            return std::acos(dotProd / lenProd);
        }
        return 0.0f;
    }

    Vector2D rotate(float angle) const {
        float cosA = std::cos(angle);
        float sinA = std::sin(angle);
        return Vector2D(x * cosA - y * sinA, x * sinA + y * cosA);
    }

    Vector2D reflect(const Vector2D& normal) const {
        float dotProd = dot(normal);
        return *this - normal * (2 * dotProd);
    }

    Vector2D refract(const Vector2D& normal, float eta) const {
        float dotProd = dot(normal);
        float k = 1.0f - eta * eta * (1.0f - dotProd * dotProd);
        if (k < 0.0f) {
            return Vector2D(0, 0);
        }
        else {
            return *this * eta - normal * (eta * dotProd + std::sqrt(k));
        }
    }

    friend std::ostream& operator<<(std::ostream& os, const Vector2D& vec) {
        os << "(" << vec.x << ", " << vec.y << ")";
        return os;
    }

    Vector2D rotateAroundPoint(const Vector2D& point, float angle) const {
        Vector2D translated = *this - point;
        translated = translated.rotate(angle);
        return translated + point;
    }

    float angleBetween(const Vector2D& other) const {
        return std::acos(dot(other) / (length() * other.length()));
    }

    void clamp(const Vector2D& min, const Vector2D& max) {
        x = std::max(min.x, std::min(x, max.x));
        y = std::max(min.y, std::min(y, max.y));
    }

    Vector2D clamped(const Vector2D& min, const Vector2D& max) const {
        return Vector2D(
            std::max(min.x, std::min(x, max.x)),
            std::max(min.y, std::min(y, max.y))
        );
    }
};

#endif
