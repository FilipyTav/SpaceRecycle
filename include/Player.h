#pragma once

#include "Game.h"
#include "Utils/Defines.h"
#include "Utils/Globals.h"
#include <raylib-cpp.hpp>
#include <raylib.h>

namespace Rlib = raylib;

class Player {
  private:
    Rlib::Rectangle m_hitbox{};
    MYSETTER(const Rlib::Rectangle, hitbox, m_hitbox);

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

    void update(const Game& game) {
        m_hitbox.y = game.get_bg_rec().y + game.get_bg_rec().height -
                     m_hitbox.height - 30;
    };

    void move(const Direction direction, const float dt = 0) {
        switch (direction) {
            using enum Direction;
        case UP:
        case DOWN:
            break;

        case LEFT:
            if (m_hitbox.x > 0)
                m_hitbox.x -= speed * dt;
            break;

        case RIGHT:
            if (m_hitbox.x + m_hitbox.width < 600)
                m_hitbox.x += speed * dt;
            break;

        default:
            break;
        }
    };

    void handle_input(const float dt) {
        if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))
            this->move(Direction::LEFT, dt);

        if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT))
            this->move(Direction::RIGHT, dt);
    };
};
