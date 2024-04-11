#pragma once

#include "Utils/Defines.h"
#include "Utils/Globals.h"
#include <raylib-cpp.hpp>
#include <raylib.h>

namespace Rlib = raylib;

class Player {
  private:
    Rlib::Rectangle m_hitbox{};
    MYSETTER(const Rlib::Rectangle, hitbox, m_hitbox);

    void correct_x(const Rlib::Rectangle& bounds) {
        if (m_hitbox.x + m_hitbox.width > bounds.x + bounds.width)
            m_hitbox.x = bounds.x + bounds.width - m_hitbox.width;
        else if (m_hitbox.x < bounds.x)
            m_hitbox.x = bounds.x;
    };

  public:
    Player(const Rlib::Rectangle& hitbox, const float speed = 400)
        : m_hitbox{hitbox}, speed{speed} {};
    Player(Player&&) = default;
    Player(const Player&) = default;
    Player& operator=(Player&&) = default;
    Player& operator=(const Player&) = default;
    ~Player() = default;

    // In pixels
    float speed{};

    MYGETTER(const Rlib::Rectangle, hitbox, m_hitbox);

    void update(const Rlib::Rectangle& bounds) {
        m_hitbox.y = bounds.y + bounds.height - m_hitbox.height - 30;

        this->correct_x(bounds);
    };

    void move(const Direction direction, const Rlib::Rectangle& bounds,
              const float dt = 0) {
        switch (direction) {
            using enum Direction;
        case UP:
        case DOWN:
            break;

        case LEFT:
            m_hitbox.x -= speed * dt;
            this->correct_x(bounds);

            break;

        case RIGHT:
            m_hitbox.x += speed * dt;
            this->correct_x(bounds);

            break;

        default:
            break;
        }
    };

    void handle_input(const Rlib::Rectangle& bounds, const float dt) {
        if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))
            this->move(Direction::LEFT, bounds, dt);

        if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT))
            this->move(Direction::RIGHT, bounds, dt);
    };
};
