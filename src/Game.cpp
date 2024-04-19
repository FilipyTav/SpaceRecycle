#include "Game.h"
#include "Utils/Globals.h"
#include <raylib.h>

Game::Game() {
    // 1 seconds between enemy spawns in the beginning
    enemy_spawn_t.start(1);
};

void Game::update_size(const Rlib::Window& window) {
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

        // m_sidebar.score.rec = Rlib::Rectangle{
        //     m_sidebar.container.x + (m_sidebar.container.width * .05f),
        //     window.GetHeight() * .10f, m_sidebar.container.width * .9f,
        //     m_sidebar.score.font_size * 1.5f};

        m_sidebar.score.update_position(
            {m_sidebar.container.x + (m_sidebar.container.width * .05f),
             m_sidebar.container.GetHeight() * .10f},
            {m_sidebar.container.width * .90f,
             m_sidebar.score.font_size * 1.5f});

        m_sidebar.lives.update_position(
            {m_sidebar.container.x + (m_sidebar.container.width * .05f),
             window.GetHeight() * .30f},
            Config::Sprites::hearts_size);

        // m_sidebar.lives.rec =
        //     Rlib::Rectangle{, , m_sidebar.container.width * .9f,
        //                     m_sidebar.score.font_size * 1.5f};
    }

    for (Trash& trash : m_enemies) {
        trash.correct_position(Orientation::Axis::HORIZONTAL, m_bg.container);
        // trash.correct_position(Orientation::Axis::VERTICAL,
        // m_bg_rec.container);
    }
};

bool Game::did_lose() { return m_lost; };

bool Game::did_win() { return m_won; };

void Game::update(const float dt, Player& player) {
    m_bg.update(dt);

    // Enemies
    {
        if (enemy_spawn_t.is_done()) {
            m_enemies.push_back(Trash::create_random(m_bg.container));

            enemy_spawn_t.reset();
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

    m_sidebar.score.text = TextFormat("Score: %i", player.get_points());
    m_sidebar.lives.text = TextFormat("HP: %i", player.get_hp());
};

void Game::reset_enemies(const int amount) {
    m_enemies.reserve(amount);
    m_enemies.resize(1, {Trash::create_random(m_bg.container)});

    enemy_spawn_t.reset();
};

void Game::draw(const Player& player) {
    m_bg.draw();
    m_sidebar.draw(player);

    for (auto& enemy : m_enemies) {
        enemy.draw();
    }
};

const bool Game::is_paused() { return m_paused; };

void Game::reset() {
    this->reset_enemies(10);

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
                        bool* running, Player& spaceship) {

    if (window.ShouldClose()) {
        *exit_request = true;
        this->set_paused(true);
    }

    if (*exit_request) {
        if (IsKeyPressed(KEY_Y))
            *running = false;
        else if (IsKeyPressed(KEY_N)) {
            *exit_request = false;
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
};
