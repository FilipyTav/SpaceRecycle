#pragma once

#include "Player.h"
#include "Star.h"
#include "Trash.h"
#include "Utils/Defines.h"
#include "Utils/Globals.h"
#include "Utils/Timer.h"
#include <algorithm>
#include <iostream>
#include <raylib-cpp.hpp>
#include <raylib.h>
#include <string>
#include <vector>

struct InfoSquare {
    Rlib::Rectangle rec{};
    std::string text{};

    Rlib::Color fg{};
    Rlib::Color bg{};

    int font_size{};
    bool is_sprite{false};

    SpriteSheet spritesheet{};

    // As sprite
    InfoSquare(const std::string path, Shy<int> spritesheet_size,
               const Rlib::Color bg);

    // As text
    InfoSquare(const std::string_view text, const Rlib::Color bg,
               const Rlib::Color fg, const int font_size);

    // @param size = in pixels
    void update_position(Shy<float> position, Shy<float> size = {});

    void draw(const int sprite_index = -1);
};

struct MainBackground {
    Rlib::Rectangle container{};

    // Bg stars
    std::vector<Star> stars{};
    Timer stars_timer{1};

    Rlib::Color color{BLACK};

    MainBackground();

    void draw();

    void update(const float dt);
};

struct Sidebar {
    Rlib::Rectangle container{};

    InfoSquare score{"", DARKBLUE, WHITE, 30};
    InfoSquare lives{
        Config::General::assets_path + "images/hearts.png", {2, 1}, BLANK};
    InfoSquare lives_text{"Lives: ", DARKBLUE, WHITE, 30};

    void draw(const Player& player);
};

class Game {
  private:
    // The areas on the screen
    // Rlib::Rectangle m_bg{};
    MainBackground m_bg{};
    Sidebar m_sidebar{};

    General::GameScreen m_current_screen{};

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

    const Rlib ::Rectangle& get_bg_rec() const;
    void set_bg_rec(const Rlib ::Rectangle& value);

    const Rlib ::Rectangle& get_sidebar_rec() const;
    void set_sidebar_rec(const Rlib ::Rectangle& value);

    MYSETTER(const bool, paused, m_paused);
    MYGETTERSETTER(const General::GameScreen, current_screen, m_current_screen);

    const bool is_paused();

    // Time between enemy spawns
    Timer enemy_spawn_t{1};

    bool did_lose();
    bool did_win();

    void update_size(const Rlib::Window& window);

    void update(const float dt, Player& player);

    void reset_enemies(const int amount);

    void draw(const Player& player);

    void reset();

    void handle_input(const Rlib::Window& window, bool* exit_request,
                      bool* running, Player& spaceship);
};
