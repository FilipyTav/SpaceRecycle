#pragma once

#include "Player.h"
#include "Trash.h"
#include "Utils/Defines.h"
#include "Utils/Globals.h"
#include "Utils/Timer.h"
#include <iostream>
#include <raylib-cpp.hpp>
#include <raylib.h>
#include <string>
#include <vector>

namespace Rlib = raylib;

struct InfoSquare {
    Rlib::Rectangle rec{};
    std::string text{};

    Rlib::Color fg{};
    Rlib::Color bg{};

    int font_size{};

    void draw() {
        rec.Draw(bg);
        DrawText(
            text.c_str(),
            // Center text
            (rec.x + (rec.width - MeasureText(text.c_str(), font_size)) / 2.f),
            (rec.y + (rec.height - font_size) / 2.f), font_size, fg);
    };
};

struct MainBackground {
    Rlib::Rectangle container{};

    void draw() { container.Draw(BLACK); };
};

struct Sidebar {
    Rlib::Rectangle container{};

    InfoSquare score{{}, "", WHITE, DARKBLUE, 30};

    void draw() {
        container.Draw(GRAY);
        score.draw();
    };
};

class Game {
  private:
    // The areas on the screen
    // Rlib::Rectangle m_bg{};
    MainBackground m_bg{};
    Sidebar m_sidebar{};

    // State
    bool m_lost{};
    bool m_won{};

    // If paused
    bool m_paused{false};

    // All the rendered enemies
    std::vector<Trash> m_enemies{};

    // Bg stars
    std::vector<Trash> m_stars{};

  public:
    Game();
    Game(Game&&) = default;
    Game(const Game&) = default;
    Game& operator=(Game&&) = default;
    Game& operator=(const Game&) = default;
    ~Game() = default;

    const Rlib ::Rectangle& get_bg_rec() const;
    void set_bg_rec(const Rlib ::Rectangle& value);

    const Rlib ::Rectangle& get_sidebar_rec() const;
    void set_sidebar_rec(const Rlib ::Rectangle& value);

    MYSETTER(const bool, paused, m_paused);

    const bool is_paused();

    // Time between enemy spawns
    Timer enemy_spaw_t{1};

    bool did_lose();
    bool did_win();

    void update_size(const Rlib::Window& window);

    void update(const float dt, Player& player);

    void reset_enemies(const int amount);

    void draw();

    void reset();
};
