#ifndef Vector2D_H
#define Vector2D_H

#include <utility>

struct Vector2D
{
public: int X, Y;

public: Vector2D(int x = 0, int y = 0) : X(x), Y(y) {}
public: Vector2D operator+(const Vector2D& v) const { return Vector2D(X + v.X, Y + v.Y); }
public: void operator+=(const Vector2D& v) { X += v.X; Y += v.Y; }
public: Vector2D operator-(const Vector2D& v) const { return Vector2D(X - v.X, Y - v.Y); }
public: void operator-=(const Vector2D& v) { X -= v.X; Y -= v.Y; }
public: bool operator==(const Vector2D& v) const { return X == v.X && Y == v.Y; }
public: bool operator!=(const Vector2D& v) const { return X != v.X || Y != v.Y; }
public: bool operator<(const Vector2D& v) const { return X < v.X || (X == v.X && Y < v.Y); }
public: std::pair<int, int> XY() const { return std::make_pair(X, Y); }

public: inline static Vector2D Up() { return Vector2D(0, -1); }
public: inline static Vector2D Down() { return Vector2D(0, 1); }
public: inline static Vector2D Left() { return Vector2D(-1, 0); }
public: inline static Vector2D Right() { return Vector2D(1, 0); }

public: inline int ManhattanDistance(const Vector2D& v) const { return abs(X - v.X) + abs(Y - v.Y); }
};

#endif // !Vector2D_H