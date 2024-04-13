#pragma once

#include "Trash.h"
#include "Utils/Defines.h"
#include "Utils/Globals.h"
#include <raylib-cpp.hpp>
#include <raylib.h>
#include <vector>

namespace Rlib = raylib;

class Game {
  private:
    Rlib::Rectangle m_bg_rec{};
    Rlib::Rectangle m_score_rec{};

    bool m_lost{};
    bool m_won{};

    std::vector<Trash> m_enemies{};

  public:
    Game(){
        // m_enemies.resize(Config::Trash::max, Trash::create_random(m_bg_rec));
        //
        // for (const Trash& a : m_enemies) {
        // }
    };
    Game(Game&&) = default;
    Game(const Game&) = default;
    Game& operator=(Game&&) = default;
    Game& operator=(const Game&) = default;
    ~Game() = default;

    MYGETTERSETTER(const Rlib::Rectangle&, bg_rec, m_bg_rec);

    bool did_lose() { return m_lost; };
    bool did_win() { return m_won; };

    void update_size(const Rlib::Window& window);
};
