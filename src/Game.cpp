#include "Game.h"
#include "Utils/Globals.h"
#include <iostream>
#include <raylib.h>

Game::Game() {
    // 1 seconds between enemy spawns in the beginning
    enemy_spaw_t.start(1);
};

void Game::update_size(const Rlib::Window& window) {
    this->set_bg_rec({0, 0, static_cast<float>(window.GetWidth() * .70),
                      static_cast<float>(window.GetHeight())});

    // this->set_sidebar_rec({m_bg_rec.x + m_bg_rec.width, 0,
    //                        static_cast<float>(window.GetWidth() * .30),
    //                        static_cast<float>(window.GetHeight())});

    // Sidebar
    {
        m_sidebar.container =
            Rlib::Rectangle{m_bg_rec.x + m_bg_rec.width, 0,
                            static_cast<float>(window.GetWidth() * .30),
                            static_cast<float>(window.GetHeight())};

        m_sidebar.score.rec = Rlib::Rectangle{
            m_bg_rec.x + m_bg_rec.width + (m_sidebar.container.width * .05f),
            window.GetHeight() * .10f, m_sidebar.container.width * .9f,
            (float)m_sidebar.score.font_size};
    }

    for (Trash& trash : m_enemies) {
        trash.correct_position(Orientation::Axis::HORIZONTAL, m_bg_rec);
        // trash.correct_position(Orientation::Axis::VERTICAL, m_bg_rec);
    }
};

bool Game::did_lose() { return m_lost; };

bool Game::did_win() { return m_won; };

void Game::update(const float dt, Player& player) {
    if (enemy_spaw_t.is_done()) {
        m_enemies.push_back(Trash::create_random(m_bg_rec));

        enemy_spaw_t.reset();
    }

    for (int i = 0; i < m_enemies.size(); i++) {
        auto& enemy{m_enemies[i]};

        if (!enemy.update(dt, m_bg_rec)) {
            m_enemies.erase(m_enemies.begin() + i);
        } else if (player.is_enemy_colliding(enemy.get_hitbox())) {
            if (player.is_same_type(enemy)) {
                player.modify_points(Math::Operations::ADD, enemy.get_value());
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

    m_sidebar.score.text = TextFormat("Score: %i", player.get_points());

    std::cout << "Enemies: " << m_enemies.size() << '\n';
};

void Game::reset_enemies(const int amount) {
    m_enemies.reserve(amount);
    m_enemies.resize(1, {Trash::create_random(m_bg_rec)});

    enemy_spaw_t.reset();
};

void Game::draw() {
    m_bg_rec.Draw(BLACK);
    m_sidebar.draw();

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
