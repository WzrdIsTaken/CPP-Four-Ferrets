// Created by Ben and James on 07/04/2022

#pragma once

#include <Engine/Point2D.hpp>

namespace bcw
{
  struct Vector2 : public ASGE::Point2D
  {
   public:
    // Inherit Point2D's constructor
    using ASGE::Point2D::Point2D;
    ~Vector2() = default;
    float magnitude();
    // WHY WERE THESE NOT HERE BEFORE
    // I AM IN GREAT PAIN
    Vector2 operator-(const Vector2& v) const { return Vector2{ this->x - v.x, this->y - v.y }; }
    Vector2 operator+(const Vector2& v) const { return Vector2{ this->x + v.x, this->y + v.y }; }
    Vector2 operator*(const Vector2& v) const { return Vector2{ this->x * v.x, this->y * v.y }; }
    Vector2 operator/(const Vector2& v) const { return Vector2{ this->x / v.x, this->y / v.y }; }
    friend Vector2 operator/(float value, Vector2 vector)
    {
      return Vector2{ value / vector.x, value / vector.y };
    }
    friend Vector2 operator+(float value, Vector2 vector)
    {
      return Vector2{ vector.x + value, vector.y + value };
    }
    friend Vector2 operator*(float value, Vector2 vector)
    {
      return Vector2{ vector.x *value , vector.y * value };
    }

    // bool operator==(const Vector2& v) const {return (x==v.x && y== v.y);}
    // bool operator!=(const Vector2& v) const {return (x!=v.x && y != v.y);}

    // Normalizes the vector
    [[maybe_unused]] Vector2 normalise();
    // d o t
    [[maybe_unused]] Vector2 dotProduct(Vector2& v2);

    // idk dude james wrote these
    [[maybe_unused]] [[nodiscard]] float maximum();
    [[maybe_unused]] [[nodiscard]] float minimum();

  };

  struct Vector2Int
  {
   public:
    int x;
    int y;

    Vector2Int() = default;
    Vector2Int(int _x, int _y);
    ~Vector2Int() = default;

    // Work out the distance between two Vector2Ints
    [[maybe_unused]] [[nodiscard]] int distance(const bcw::Vector2Int& other) const;

    // Return the vector as a string
    [[maybe_unused]] [[nodiscard]] std::string toString() const;
  };
} // namespace bcw

// Many vector methods here if needed:
// http://www.technologicalutopia.com/sourcecode/xnageometry/vector2.cs.htm
