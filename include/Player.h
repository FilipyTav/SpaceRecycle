#pragma once

#include "Game.h"
#include "Utils/Defines.h"
#include <raylib-cpp.hpp>

namespace Rlib = raylib;

class Player {
  private:
    Rlib::Rectangle m_hitbox{};
    MYSETTER(const Rlib::Rectangle, hitbox, m_hitbox);

  public:
    Player(const Rlib::Rectangle& hitbox) : m_hitbox{hitbox} {};
    Player(Player&&) = default;
    Player(const Player&) = default;
    Player& operator=(Player&&) = default;
    Player& operator=(const Player&) = default;
    ~Player() = default;

    MYGETTER(const Rlib::Rectangle, hitbox, m_hitbox);

    void update(const Game& game) {
        m_hitbox.y =
            game.get_bg_rec().y + game.get_bg_rec().height - m_hitbox.height;
    };
};
