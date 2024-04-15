#pragma once

#include "Trash.h"
#include "Utils/Defines.h"
#include "Utils/Globals.h"
#include "Utils/Timer.h"
#include <iostream>
#include <raylib-cpp.hpp>
#include <raylib.h>
#include <vector>

namespace Rlib = raylib;

class Game {
  private:
    // The areas on the screen
    Rlib::Rectangle m_bg_rec{};
    Rlib::Rectangle m_score_rec{};

    // State
    bool m_lost{};
    bool m_won{};

    // If paused
    bool m_paused{false};

    // All the rendered enemies
    std::vector<Trash> m_enemies{};

  public:
    Game();
    Game(Game&&) = default;
    Game(const Game&) = default;
    Game& operator=(Game&&) = default;
    Game& operator=(const Game&) = default;
    ~Game() = default;

    MYGETTERSETTER(const Rlib::Rectangle&, bg_rec, m_bg_rec);

    MYSETTER(const bool, paused, m_paused);

    const bool is_paused();

    // Time between enemy spawns
    Timer enemy_spaw_t{1};

    bool did_lose();
    bool did_win();

    void update_size(const Rlib::Window& window);

    void update(const float dt);

    void reset_enemies(const int amount);

    void draw();
};
