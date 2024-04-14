#include "Trash.h"
#include <iomanip>
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

    m_position.x = (m_hitbox.x - bounds.x) / (bounds.width - m_hitbox.width);

    if (m_hitbox.y + m_hitbox.height >= bounds.y + bounds.height ||
        m_hitbox.x + m_hitbox.width >= bounds.x + bounds.width)
        return false;

    return true;
};

void Trash::correct_position(const Orientation::Axis axis,
                             const Rlib::Rectangle& bounds) {
    switch (axis) {
        using enum Orientation::Axis;
    case HORIZONTAL:
        m_hitbox.x = bounds.x + m_position.x * (bounds.width - m_hitbox.width);
        break;

    case VERTICAL:
        m_hitbox.y =
            bounds.y + m_position.y * (bounds.height - m_hitbox.height);
        break;

    default:
        break;
    }
};

void Trash::draw() {
    if (m_type != Type::MAX_TYPES)
        m_hitbox.Draw(m_color);
};

Trash Trash::create_random(const Rlib::Rectangle& bounds) {
    using enum Type;
    constexpr int hitbox_size{100};

    return {static_cast<Type>(Random::get(0, static_cast<int>(MAX_TYPES) - 1)),
            Random::get(100, 600),
            {static_cast<float>(Random::get(
                 (int)bounds.x, bounds.x + bounds.width - hitbox_size)),
             0, hitbox_size, hitbox_size}};
};
