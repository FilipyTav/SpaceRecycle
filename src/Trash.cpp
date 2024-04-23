#include "Trash.h"
#include "Utils/Globals.h"
#include <cstdlib>

/* ---------- Public methods ---------- */
Trash::Trash(const Type type, const int speed) : m_type{type}, y_speed{speed} {
    m_color = TrashInfo::colors_map[type];
    m_value = TrashInfo::values_map[type];
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

    m_sprite_box.width = TrashInfo::size.x;
    m_sprite_box.height = TrashInfo::size.y;

    m_hitbox.width = TrashInfo::size.x * .5;
    m_hitbox.height = TrashInfo::size.y * .5;

    m_sprite_box.x =
        m_hitbox.x - std::abs(m_hitbox.width - m_sprite_box.width) / 2;
    m_sprite_box.y =
        m_hitbox.y - std::abs(m_hitbox.height - m_sprite_box.height) / 2;

    // m_sprite_box.x = m_hitbox.x;
    // m_sprite_box.y = m_hitbox.y;

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
    if (m_type != Type::MAX_TYPES && m_visible) {
        // Just draw the sprites
        // m_sprite_box.Draw(PURPLE);
        // m_hitbox.Draw(m_color);
    }
};

Trash Trash::create_random(const Rlib::Rectangle& bounds) {
    using enum Type;

    return {static_cast<Type>(Random::get(0, static_cast<int>(MAX_TYPES) - 1)),
            Random::get(100, 400),
            {static_cast<float>(Random::get(
                 (int)bounds.x, bounds.x + bounds.width - TrashInfo::size.x)),
             0, static_cast<float>(TrashInfo::size.x),
             static_cast<float>(TrashInfo::size.y)}};
};
