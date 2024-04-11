#pragma once

#include "Utils/Defines.h"
#include <raylib-cpp.hpp>
#include <raylib.h>

namespace Rlib = raylib;

class Game {
  private:
    Rlib::Rectangle m_bg_rec{};
    Rlib::Rectangle m_score_rec{};

  public:
    Game() = default;
    Game(Game&&) = default;
    Game(const Game&) = default;
    Game& operator=(Game&&) = default;
    Game& operator=(const Game&) = default;
    ~Game() = default;

    MYGETTERSETTER(const Rlib::Rectangle&, bg_rec, m_bg_rec);

    void update(const Rlib::Window& window);
};
