#include "Game.h"
#include "Utils/Globals.h"

Game::Game() {
    // 1 seconds between enemy spawns in the beginning
    enemy_spaw_t.start(1);
};

void Game::update_size(const Rlib::Window& window) {
    this->set_bg_rec({0, 0, static_cast<float>(window.GetWidth() * .70),
                      static_cast<float>(window.GetHeight())});

    for (Trash& trash : m_enemies) {
        trash.correct_position(Orientation::Axis::HORIZONTAL, m_bg_rec);
        // a.correct_position(Orientation::Axis::VERTICAL, m_bg_rec);
    }
};

bool Game::did_lose() { return m_lost; };

bool Game::did_win() { return m_won; };

void Game::update(const float dt) {
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

void Game::reset_enemies(const int amount) {
    m_enemies.resize(amount, {TrashInfo::Type::MAX_TYPES});
    m_enemies[0] = Trash::create_random(m_bg_rec);

    enemy_spaw_t.reset();
};

void Game::draw() {
    m_bg_rec.Draw(BLACK);

    for (auto& enemy : m_enemies) {
        enemy.draw();
    }
};

const bool Game::is_paused() { return m_paused; };
