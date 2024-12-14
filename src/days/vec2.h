#pragma once

struct Vec2
{
    int x;
    int y;
};

inline bool operator==(const Vec2& lhs, const Vec2& rhs)
{
    return lhs.x == rhs.x && lhs.y == rhs.y;
};

inline bool operator<(const Vec2& lhs, const Vec2& rhs)
{
    return lhs.x < rhs.x || (lhs.x == rhs.x && lhs.y < rhs.y);
};

inline Vec2 operator+(const Vec2& lhs, const Vec2& rhs)
{
    return Vec2{ lhs.x + rhs.x, lhs.y + rhs.y };
};

inline Vec2 operator-(const Vec2& lhs, const Vec2& rhs)
{
    return Vec2{ lhs.x - rhs.x, lhs.y - rhs.y };
};

inline Vec2 operator/(const Vec2& lhs, int rhs)
{
    return Vec2{ lhs.x / rhs, lhs.y / rhs };
};

inline void operator+=(Vec2& lhs, const Vec2& rhs)
{
    lhs.x += rhs.x;
    lhs.y += rhs.y;
};
