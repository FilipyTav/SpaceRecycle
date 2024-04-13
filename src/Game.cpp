#include "Game.h"
#include "Utils/Globals.h"

void Game::update_size(const Rlib::Window& window) {
    this->set_bg_rec({0, 0, static_cast<float>(window.GetWidth() * .70),
                      static_cast<float>(window.GetHeight())});

    for (Trash& trash : m_enemies) {
        trash.correct_position(Orientation::Axis::HORIZONTAL, m_bg_rec);
        // a.correct_position(Orientation::Axis::VERTICAL, m_bg_rec);
    }
};
