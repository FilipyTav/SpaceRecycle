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

    InfoSquare(const std::string path, Shy<int> spritesheet_size,
               const Rlib::Color bg)
        : bg{bg} {
        spritesheet = SpriteSheet{path, spritesheet_size};
        is_sprite = true;
    };

    InfoSquare(const std::string_view text, const Rlib::Color bg,
               const Rlib::Color fg, const int font_size)
        : text{text}, bg{bg}, fg{fg}, font_size{font_size} {
        is_sprite = false;
    };

    // @param size = in pixels
    void update_position(Shy<float> position, Shy<float> size = {}) {
        if (!size) {
            size = {rec.width, rec.height};
        }

        rec = Rlib::Rectangle{{position.x, position.y}, {size.x, size.y}};
    };

    void draw(const int sprite_index = -1) {
        rec.Draw(bg);

        if (is_sprite)
            spritesheet.draw(rec, sprite_index);
        else
            DrawText(text.c_str(),
                     // Center text
                     (rec.x +
                      (rec.width - MeasureText(text.c_str(), font_size)) / 2.f),
                     (rec.y + (rec.height - font_size) / 2.f), font_size, fg);
    };
};

struct MainBackground {
    Rlib::Rectangle container{};

    // Bg stars
    std::vector<Star> stars{};
    Timer stars_timer{1};

    Rlib::Color color{BLACK};

    MainBackground() { stars_timer.start(.3); };

    void draw() {
        container.Draw(color);
        for (auto& star : stars) {
            star.draw();
        }
    };

    void update(const float dt) {
        for (int i = 0; i < stars.size(); i++) {
            stars[i].update(dt);

            if (!is_rec_inside(container, stars[i].rec))
                stars.erase(stars.begin() + i);
        }

        if (stars_timer.is_done()) {
            stars.push_back(Star::create_random(container));
            stars_timer.reset();
        }
    };
};

struct Sidebar {
    Rlib::Rectangle container{};

    InfoSquare score{"", DARKBLUE, WHITE, 30};
    InfoSquare lives{
        Config::General::assets_path + "images/hearts.png", {2, 1}, BLANK};

    void draw(const Player& player) {
        container.Draw(GRAY);
        score.draw();

        for (int i = 0; i < player.get_max_hp(); i++) {
            lives.draw(1);
            lives.update_position({lives.rec.x + lives.rec.width, lives.rec.y});
        }

        lives.update_position(
            {lives.rec.x - player.get_max_hp() * lives.rec.width, lives.rec.y});
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
