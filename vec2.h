#pragma once

struct Vec2
{
    double x{};
    double y{};
};


inline Vec2 operator+(Vec2 a, Vec2 b)
{
    Vec2 result{a.x + b.x, a.y + b.y};
    return result;
}

inline Vec2 operator*(Vec2 a, double c)
{
    Vec2 result{a.x * c, a.y * c};
    return result;
}

inline double dot(Vec2 a, Vec2 b)
{
    double result{a.x * b.x + a.y * b.y};
    return result;
}

