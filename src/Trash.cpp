#include "Trash.h"
#include <iostream>

/* ---------- Public methods ---------- */
Trash::Trash(const Type type, const int speed) : m_type{type}, y_speed{speed} {
    m_color = TrashInfo::colors_map[type];
};

Trash::Trash(const Type type, const int speed, const Rlib::Rectangle& hitbox)
    : Trash(type, speed) {
    m_hitbox = hitbox;
};

void Trash::move(const float dt) { m_hitbox.y += y_speed * dt; };

bool Trash::update(const float dt, const Rlib::Rectangle& bounds) {
    this->move(dt);

    if (m_hitbox.y + m_hitbox.height >= bounds.y + bounds.height)
        return true;

    return false;
};

void Trash::draw() { m_hitbox.Draw(m_color); };

Trash Trash::create_random(const Rlib::Rectangle& bounds) {
    using enum Type;
    constexpr int hitbox_size{100};

    return {static_cast<Type>(Random::get(0, static_cast<int>(MAX_TYPES) - 1)),
            Random::get(100, 600),
            {static_cast<float>(Random::get(
                 (int)bounds.x, bounds.x + bounds.width - hitbox_size)),
             0, hitbox_size, hitbox_size}};
};
