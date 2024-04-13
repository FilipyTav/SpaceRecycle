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
    Game() = default;
    Game(Game&&) = default;
    Game(const Game&) = default;
    Game& operator=(Game&&) = default;
    Game& operator=(const Game&) = default;
    ~Game() = default;

    MYGETTERSETTER(const Rlib::Rectangle&, bg_rec, m_bg_rec);

    bool did_lose() { return m_lost; };
    bool did_win() { return m_won; };

    void update_size(const Rlib::Window& window);

    void update(const float dt) {
        for (auto& enemy : m_enemies) {
            enemy.update(dt, m_bg_rec);
        }
    };

    void reset_enemies(const int amount) {
        m_enemies.resize(amount, {TrashInfo::Type::MAX_TYPES});
        m_enemies[0] = Trash::create_random(m_bg_rec);
    };

    void draw() {
        m_bg_rec.Draw(BLACK);

        for (auto& enemy : m_enemies) {
            enemy.draw();
        }
    };
};
