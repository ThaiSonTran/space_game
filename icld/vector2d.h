#ifndef VECTOR2D_H
#define VECTOR2D_H

struct Vector2D {
    int x, y;

    Vector2D(int x = 0, int y = 0);

    Vector2D& operator+=(const Vector2D& other);
    Vector2D& operator-=(const Vector2D& other);
    Vector2D& operator*=(double scalar);
    Vector2D& operator/=(double scalar);

    Vector2D operator+(const Vector2D& other) const;
    Vector2D operator-(const Vector2D& other) const;
    Vector2D operator*(double scalar) const;
    Vector2D operator/(double scalar) const;

    bool operator==(const Vector2D& other) const;
    bool operator!=(const Vector2D& other) const;
    double magnitude();
};

#endif
