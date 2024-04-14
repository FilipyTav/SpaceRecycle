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
    Rlib::Rectangle m_bg_rec{};
    Rlib::Rectangle m_score_rec{};

    bool m_lost{};
    bool m_won{};

    std::vector<Trash> m_enemies{};

  public:
    Game() {
        // 1 seconds between enemy spawns in the beginning
        enemy_spaw_t.start(1);
    };
    Game(Game&&) = default;
    Game(const Game&) = default;
    Game& operator=(Game&&) = default;
    Game& operator=(const Game&) = default;
    ~Game() = default;

    MYGETTERSETTER(const Rlib::Rectangle&, bg_rec, m_bg_rec);

    // Time between enemy spawns
    Timer enemy_spaw_t{1};

    bool did_lose() { return m_lost; };
    bool did_win() { return m_won; };

    void update_size(const Rlib::Window& window);

    void update(const float dt) {
        if (enemy_spaw_t.is_done()) {
            m_enemies.push_back(Trash::create_random(m_bg_rec));

            enemy_spaw_t.reset();
        }

        for (int i = 0; i < m_enemies.size(); i++) {
            if (!m_enemies[i].update(dt, m_bg_rec)) {
                m_enemies.erase(m_enemies.begin() + i);
            };
        }

        std::cout << "Enemies: " << m_enemies.size() << '\n';
    };

    void reset_enemies(const int amount) {
        m_enemies.resize(amount, {TrashInfo::Type::MAX_TYPES});
        m_enemies[0] = Trash::create_random(m_bg_rec);

        enemy_spaw_t.reset();
    };

    void draw() {
        m_bg_rec.Draw(BLACK);

        for (auto& enemy : m_enemies) {
            enemy.draw();
        }
    };
};
