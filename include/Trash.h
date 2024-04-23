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

    Rlib::Color m_color{};
    Rlib::Rectangle m_sprite_box{};
    Rlib::Rectangle m_hitbox{};

    friend class Game;
    MYSETTER(const Rlib::Rectangle&, hitbox, m_hitbox);

    // In percentage from the start of the container
    Shy<float> m_position{};

    bool m_visible{true};

    // How many points this is worth
    int m_value{};

  public:
    Trash(const Type type, const int speed = 300);
    Trash(const Type type, const int speed, const Rlib::Rectangle& hitbox);
    Trash(Trash&&) = default;
    Trash(const Trash&) = default;
    Trash& operator=(Trash&&) = default;
    Trash& operator=(const Trash&) = default;
    ~Trash() = default;

    MYGETTER(const Rlib::Rectangle&, hitbox, m_hitbox);
    MYGETTER(const Rlib::Rectangle&, sprite_box, m_sprite_box);

    void move(const float dt);

    // Returns false if the hitbox gets out of bounds
    bool update(const float dt, const Rlib::Rectangle& bounds);

    void draw();

    // bounds: the rectangle that contains the object - used to determine the
    // position of the hitbox
    static Trash create_random(const Rlib::Rectangle& bounds);

    void correct_position(const Orientation::Axis axis,
                          const Rlib::Rectangle& bounds);

    MYGETTER(const Type, type, m_type);
    MYGETTERSETTER(const bool, visible, m_visible);

    MYGETTER(const int, value, m_value);
};
