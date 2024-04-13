#include "Game.h"

void Game::update_size(const Rlib::Window& window) {
    this->set_bg_rec({0, 0, static_cast<float>(window.GetWidth() * .70),
                      static_cast<float>(window.GetHeight())});

    for (Trash& a : m_enemies) {
    }
};
