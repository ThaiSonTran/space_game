#include <stdexcept>
#include <iostream>
#include <cmath>
#include "vector2D.h"

Vector2D::Vector2D(int x, int y) : x(x), y(y) {}

Vector2D& Vector2D::operator+=(const Vector2D& other){
    x += other.x;
    y += other.y;
    return *this;
}

Vector2D& Vector2D::operator-=(const Vector2D& other){
    x -= other.x;
    y -= other.y;
    return *this;
}

Vector2D& Vector2D::operator*=(double scalar){
    x *= scalar;
    y *= scalar;
    return *this;
}

Vector2D& Vector2D::operator/=(double scalar){
    if (scalar == 0){
        throw std::runtime_error("Division by zero");
    }
    x /= scalar;
    y /= scalar;
    return *this;
}

Vector2D Vector2D::operator+(const Vector2D& other) const{
    return Vector2D(x + other.x, y + other.y);
}

Vector2D Vector2D::operator-(const Vector2D& other) const{
    return Vector2D(x - other.x, y - other.y);
}

Vector2D Vector2D::operator*(double scalar) const{
    return Vector2D(x * scalar, y * scalar);
}

Vector2D Vector2D::operator/(double scalar) const {
    if (scalar == 0.0f){
        throw std::runtime_error("Division by zero");
    }
    return Vector2D(x / scalar, y / scalar);
}

bool Vector2D::operator==(const Vector2D& other) const{
    return x == other.x && y == other.y;
}

bool Vector2D::operator!=(const Vector2D& other) const{
    return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, const Vector2D& vec){
    os << "(" << vec.x << ", " << vec.y << ")";
    return os;
}
double Vector2D::magnitude(){
    return sqrt(static_cast<double>(1LL * x * x + 1LL * y * y));
}
