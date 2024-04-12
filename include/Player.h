#pragma once

#include "Utils/Defines.h"
#include "Utils/Globals.h"
#include <raylib-cpp.hpp>
#include <raylib.h>

class Player {
  private:
    using Direction = Orientation::Direction;
    using Axis = Orientation::Axis;

    TrashInfo::Type m_type{};
    Rlib::Color m_color{};

    Rlib::Rectangle m_hitbox{};
    MYSETTER(const Rlib::Rectangle, hitbox, m_hitbox);

    void correct_position(const Rlib::Rectangle& bounds, const Axis axis);

  public:
    Player(const Rlib::Rectangle& hitbox, const TrashInfo::Type type,
           const float speed = 400);
    Player(Player&&) = default;
    Player(const Player&) = default;
    Player& operator=(Player&&) = default;
    Player& operator=(const Player&) = default;
    ~Player() = default;

    // In pixels
    float speed{};

    MYGETTER(const Rlib::Rectangle, hitbox, m_hitbox);

    void update(const Rlib::Rectangle& bounds);

    void move(const Direction direction, const Rlib::Rectangle& bounds,
              const float dt = 0);

    void draw() { m_hitbox.Draw(m_color); };

    void set_type(const TrashInfo::Type type);

    void handle_input(const Rlib::Rectangle& bounds, const float dt);
};
