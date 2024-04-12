#pragma once

#include "Utils/Globals.h"
#include "Utils/Random.h"
#include <raylib-cpp.hpp>
#include <unordered_map>

class Trash {
  private:
    using Type = TrashInfo::Type;
    Type m_type{};

    int y_speed{};

    // TODO:: Change this for the sprite
    Rlib::Color m_color{};

    Rlib::Rectangle m_hitbox{};

  public:
    Trash(const Type type, const int speed);
    Trash(const Type type, const int speed, const Rlib::Rectangle& bounds);
    Trash(Trash&&) = default;
    Trash(const Trash&) = default;
    Trash& operator=(Trash&&) = default;
    Trash& operator=(const Trash&) = default;
    ~Trash() = default;

    void move(const float dt);

    // Returns true if the hitbox hits the bottom of the screen
    bool update(const float dt, const Rlib::Rectangle& bounds);

    void draw();

    static Trash create_random(const Rlib::Rectangle& bounds);
};
