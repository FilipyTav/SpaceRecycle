#include "Game.h"
#include "Utils/Globals.h"
#include <iostream>
#include <raylib.h>

Game::Game() {
    // 1 seconds between enemy spawns in the beginning
    enemy_spawn_t.start(2);
    m_difficulty_timer.start(10);

    enemies_sprites = {Config::General::assets_path + "/images/trashes.png",
                       {1, 5}};
};

void Game::update_size(const Rlib::Window& window) {
    switch (m_current_screen) {
        using enum General::GameScreen;

    case TITLE: {
        // Main background
        // Needs to be set, otherwise it will cause a segfault
        {
            this->set_bg_rec({0, 0, static_cast<float>(window.GetWidth()),
                              static_cast<float>(window.GetHeight())});
        }

        // Title screen
        {
            m_title_screen.gamename.update_position(
                {m_bg.container.x + m_bg.container.width / 2,
                 m_bg.container.y + m_bg.container.height * .20f});

            m_title_screen.play_btn.set_size(
                {150, m_title_screen.play_btn.font_size * 1.5f});
            m_title_screen.play_btn.update_position(
                {m_bg.container.x + m_bg.container.width / 2 -
                     m_title_screen.play_btn.rec.width / 2,
                 m_bg.container.y + m_bg.container.height * .40f});

            m_title_screen.instructions_btn.set_size(
                {150, m_title_screen.instructions_btn.font_size * 1.5f});
            m_title_screen.instructions_btn.update_position(
                {m_bg.container.x + m_bg.container.width / 2 -
                     m_title_screen.instructions_btn.rec.width / 2,
                 m_title_screen.play_btn.rec.y + 80});

            m_title_screen.exit_btn.set_size(
                {150, m_title_screen.exit_btn.font_size * 1.5f});
            m_title_screen.exit_btn.update_position(
                {m_bg.container.x + m_bg.container.width / 2 -
                     m_title_screen.exit_btn.rec.width / 2,
                 m_title_screen.instructions_btn.rec.y + 80});
        }

    } break;

    case INSTRUCTIONS:
        // Bounds
        {
            m_inst_screen.bg.set_size(
                {window.GetWidth() * .75f, window.GetHeight() * .80f});
            m_inst_screen.bg.update_position(
                {window.GetWidth() / 2.f - m_inst_screen.bg.rec.width / 2,
                 window.GetHeight() / 2.f - m_inst_screen.bg.rec.height / 2});
        }

        // Buttons
        {
            m_inst_screen.close_btn.set_size(
                {static_cast<float>(m_inst_screen.bg.font_size),
                 static_cast<float>(m_inst_screen.bg.font_size)});
            m_inst_screen.close_btn.update_position(
                {m_inst_screen.bg.rec.x + m_inst_screen.bg.rec.width -
                     m_inst_screen.close_btn.rec.width,
                 m_inst_screen.bg.rec.y});
        }

        // Main info
        {
            m_inst_screen.controls.font_size = window.GetHeight() * .05f;
            m_inst_screen.controls.update_position(
                {m_inst_screen.bg.rec.x + m_inst_screen.bg.rec.width / 2,
                 m_inst_screen.bg.rec.y + (m_inst_screen.bg.rec.height / 2) -
                     m_inst_screen.bg.rec.height * .40f});

            m_inst_screen.types.font_size = window.GetHeight() * .05f;
            m_inst_screen.types.update_position(
                {m_inst_screen.bg.rec.x + m_inst_screen.bg.rec.width / 2,
                 m_inst_screen.bg.rec.y + (m_inst_screen.bg.rec.height / 2) +
                     m_inst_screen.bg.rec.height * .10f});
        }
        break;

    case GAMEPLAY: {
        // Main background
        {
            this->set_bg_rec({0, 0, static_cast<float>(window.GetWidth() * .70),
                              static_cast<float>(window.GetHeight())});
        }

        // Sidebar
        {
            this->set_sidebar_rec({m_bg.container.x + m_bg.container.width, 0,
                                   static_cast<float>(window.GetWidth() * .30),
                                   static_cast<float>(window.GetHeight())});

            m_sidebar.score.update_position(
                {m_sidebar.container.x + (m_sidebar.container.width * .05f),
                 m_sidebar.container.GetHeight() * .10f},
                {m_sidebar.container.width * .90f,
                 m_sidebar.score.font_size * 1.5f});

            m_sidebar.lives_text.update_position(
                {m_sidebar.container.x + (m_sidebar.container.width * .05f),
                 window.GetHeight() * .30f},
                {m_sidebar.container.width * .90f,
                 m_sidebar.lives_text.font_size * 1.5f});

            m_sidebar.lives.update_position(
                {m_sidebar.container.x + (m_sidebar.container.width * .05f),
                 window.GetHeight() * .40f},
                Config::Sprites::hearts_size);
        }

    default:
        break;
    }
    }
};

bool Game::did_lose() { return m_lost; };

bool Game::did_win() { return m_won; };

void Game::update(const float dt, Player& player, const Rlib::Window& window,
                  bool* game_running) {
    switch (m_current_screen) {
        using enum General::GameScreen;
    case TITLE: {
        this->set_current_screen(m_title_screen.update(GetMousePosition(),
                                                       game_running, dt,
                                                       m_bg.container),
                                 window);
    } break;

    case INSTRUCTIONS:
        this->set_current_screen(m_inst_screen.update(GetMousePosition()));
        break;

    case GAMEPLAY: {
        // Enemies
        {
            if (enemy_spawn_t.is_done()) {
                m_enemies.push_back(Trash::create_random(m_bg.container));

                enemy_spawn_t.reset();
            }

            if (m_difficulty_timer.is_done()) {
                m_difficulty_timer.reset();
                enemy_spawn_t.start(enemy_spawn_t.get_lifetime() - .1);

                std::cout << "Spawn time: " << enemy_spawn_t.get_lifetime()
                          << '\n';
            }

            for (int i = 0; i < m_enemies.size(); i++) {
                auto& enemy{m_enemies[i]};

                if (!enemy.update(dt, m_bg.container)) {
                    m_enemies.erase(m_enemies.begin() + i);
                } else if (player.is_enemy_colliding(enemy.get_hitbox())) {
                    if (player.is_same_type(enemy)) {
                        player.modify_points(Math::Operations::ADD,
                                             enemy.get_value());
                    } else
                        player.modify_health(Math::Operations::SUB, 1);

                    if (!player.is_alive())
                        m_lost = true;

                    // It does not work if those 2 lines are inverted.
                    // Why?
                    // enemy.set_visible(false);
                    m_enemies.erase(m_enemies.begin() + i);
                }
            }
        }

        m_sidebar.score.text = TextFormat("Pontos: %i", player.get_points());
        m_sidebar.lives.text = TextFormat("HP: %i", player.get_hp());
    } break;

    default:
        break;
    }
    m_bg.update(dt);
};

void Game::reset_enemies(const int amount) {
    m_enemies.clear();
    m_enemies.reserve(amount);
    m_enemies.resize(1, {Trash::create_random(m_bg.container)});

    enemy_spawn_t.reset();
};

void Game::draw(const Player& player) {
    switch (m_current_screen) {
        using enum General::GameScreen;

    case TITLE:
        m_bg.container.Draw(BLACK);
        m_title_screen.draw();
        break;

    case INSTRUCTIONS:
        m_inst_screen.draw();
        break;

    case GAMEPLAY: {
        m_bg.draw();
        m_sidebar.draw(player);

        for (auto& enemy : m_enemies) {
            enemy.draw();
            enemies_sprites.draw(enemy.get_sprite_box(),
                                 static_cast<int>(enemy.get_type()));
        }
    } break;

    default:
        break;
    }
};

const bool Game::is_paused() { return m_paused; };

void Game::reset() {
    this->reset_enemies(10);
    m_difficulty_timer.reset();

    m_lost = false;
    m_won = false;

    m_paused = false;
};

const Rlib ::Rectangle& Game::get_bg_rec() const { return m_bg.container; }

void Game::set_bg_rec(const Rlib ::Rectangle& value) {
    m_bg.container = value;
};

const Rlib ::Rectangle& Game::get_sidebar_rec() const {
    return m_sidebar.container;
}

void Game::set_sidebar_rec(const Rlib ::Rectangle& value) {
    m_sidebar.container = value;
};

void Game::handle_input(const Rlib::Window& window, bool* exit_request,
                        bool* menu_request, bool* running, Player& spaceship) {
    switch (m_current_screen) {
        using enum General::GameScreen;
    case TITLE:
        break;

    case INSTRUCTIONS:
        break;

    case GAMEPLAY: {
        if (window.ShouldClose()) {
            *exit_request = true;
            this->set_paused(true);
        }

        if (IsKeyPressed(KEY_ESCAPE)) {
            *menu_request = true;
        }

        if (*exit_request) {
            if (IsKeyPressed(KEY_Y))
                *running = false;
            else if (IsKeyPressed(KEY_N)) {
                *exit_request = false;
                this->set_paused(false);
            }
        } else if (*menu_request) {
            this->set_paused(true);

            if (IsKeyPressed(KEY_Y)) {
                m_current_screen = TITLE;
                *menu_request = false;
                this->set_paused(false);
            } else if (IsKeyPressed(KEY_N)) {
                *menu_request = false;
                this->set_paused(false);
            }
        } else if (this->did_lose()) {
            if (IsKeyPressed(KEY_Y)) {
                this->set_paused(false);
                this->reset();
                spaceship.reset();

                spaceship.place_in_middle(this->get_bg_rec());
                spaceship.place_at_bottom(this->get_bg_rec());
            } else if (IsKeyPressed(KEY_N)) {
                *exit_request = true;
            }
        } else {
            if (IsKeyPressed(KEY_P))
                this->set_paused(!this->is_paused());
        }
    } break;

    default:
        break;
    }
};

/* ---------- InfoSquare methods ---------- */
InfoSquare::InfoSquare(const std::string path, Shy<int> spritesheet_size,
                       const Rlib::Color bg)
    : bg{bg} {
    spritesheet = SpriteSheet{path, spritesheet_size};
    is_sprite = true;
};

InfoSquare::InfoSquare(const std::string_view text, const Rlib::Color bg,
                       const Rlib::Color fg, const int font_size)
    : text{text}, bg{bg}, fg{fg}, font_size{font_size} {
    is_sprite = false;
};

void InfoSquare::update_position(Shy<float> position, Shy<float> size) {
    if (!size) {
        size = {rec.width, rec.height};
    }

    rec = Rlib::Rectangle{{position.x, position.y}, {size.x, size.y}};
};

void InfoSquare::draw(const int sprite_index) {
    rec.Draw(bg);

    if (is_sprite)
        spritesheet.draw(rec, sprite_index);
    else
        DrawText(
            text.c_str(),
            // Center text
            (rec.x + (rec.width - MeasureText(text.c_str(), font_size)) / 2.f),
            (rec.y + (rec.height - font_size) / 2.f), font_size, fg);
};

void InfoSquare::set_size(Shy<float> size) {
    rec.width = size.x;
    rec.height = size.y;
};

/* ---------- MainBackground methods ---------- */
MainBackground::MainBackground(){};

void MainBackground::draw() {
    container.Draw(color);
    for (auto& star : stars) {
        star.draw();
    }
};

void MainBackground::update(const float dt) {
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

/* ---------- Sidebar methods ---------- */
void Sidebar::draw(const Player& player) {
    container.Draw(GRAY);
    score.draw();
    lives_text.draw();

    for (int i = 0; i < player.get_max_hp(); i++) {
        // 0 = empty heart
        // 1 = full heart
        if (i < player.get_hp())
            lives.draw(1);
        else
            lives.draw(0);

        lives.update_position({lives.rec.x + lives.rec.width, lives.rec.y});
    }

    // Resets the rec to its original position
    lives.update_position(
        {lives.rec.x - player.get_max_hp() * lives.rec.width, lives.rec.y});
};
