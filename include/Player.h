#pragma once

#include "BoundingBox.hpp"
#include "Trash.h"
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

    int m_score{0};

    int m_health{};
    int m_max_health{};

  public:
    Player(const Rlib::Rectangle& hitbox, const TrashInfo::Type type,
           const float speed = 400, const int health = 3);
    Player(Player&&) = default;
    Player(const Player&) = default;
    Player& operator=(Player&&) = default;
    Player& operator=(const Player&) = default;
    ~Player() = default;

    // In pixels
    float speed{};

    MYGETTER(const Rlib::Rectangle, hitbox, m_hitbox);

    void update(const Rlib::Rectangle& bounds);
    void update_position(const Rlib::Rectangle& bounds);

    void move(const Direction direction, const Rlib::Rectangle& bounds,
              const float dt = 0);

    void draw() const { m_hitbox.Draw(m_color); };

    void set_type(const TrashInfo::Type type);

    void handle_input(const Rlib::Rectangle& bounds, const float dt);

    bool is_enemy_colliding(const Rlib::Rectangle& enemy_hitbox) const;

    bool is_correct_enemy(const Trash& enemy) const;

    // If the enemy collected has the same type as the spaceship
    bool is_same_type(const Trash& enemy) const;

    MYGETTER(const int, points, m_score);
    MYGETTER(const int, hp, m_health);

    void modify_points(const Math::Operations op, const int amount);
    void modify_health(const Math::Operations op, const int amount);

    bool is_alive();

    void reset();

    void place_in_middle(const Rlib::Rectangle bounds);

    void place_at_bottom(const Rlib::Rectangle bounds);
};
