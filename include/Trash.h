#pragma once

#include "Utils/Defines.h"
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

    friend class Game;
    MYGETTERSETTER(const Rlib::Rectangle&, hitbox, m_hitbox);

    // In percentage from the start of the container
    Shy<float> m_position{};

  public:
    Trash(const Type type, const int speed);
    Trash(const Type type, const int speed, const Rlib::Rectangle& hitbox);
    Trash(Trash&&) = default;
    Trash(const Trash&) = default;
    Trash& operator=(Trash&&) = default;
    Trash& operator=(const Trash&) = default;
    ~Trash() = default;

    void move(const float dt);

    // Returns true if the hitbox hits the bottom of the screen
    bool update(const float dt, const Rlib::Rectangle& bounds);

    void draw();

    // bounds: the rectangle that contains the object - used to determine the
    // position of the hitbox
    static Trash create_random(const Rlib::Rectangle& bounds);

    void correct_position(const Orientation::Axis axis,
                          const Rlib::Rectangle& bounds){};
};
