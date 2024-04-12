#pragma once

#include "Utils/Random.h"
#include <raylib-cpp.hpp>
#include <unordered_map>

namespace Rlib = raylib;

class Trash {
  public:
    enum class Type {
        PAPER,   // Blue
        GLASS,   // Green
        PLASTIC, // Red
        METAL,   // Yellow
        ORGANIC, // Brown

        MAX_TYPES
    };

  private:
    Type m_type{};

    int y_speed{};

    // TODO:: Change this for the sprite
    Rlib::Color m_color{};

    Rlib::Rectangle m_hitbox{};

    using enum Type;
    std::unordered_map<Type, Rlib::Color> colors_map{{PAPER, BLUE},
                                                     {GLASS, GREEN},
                                                     {PLASTIC, RED},
                                                     {METAL, YELLOW},
                                                     {ORGANIC, BROWN}};

  public:
    Trash(const Type type, const int speed) : m_type{type}, y_speed{speed} {
        m_color = colors_map[type];
    };
    Trash(const Type type, const int speed, const Rlib::Rectangle& bounds)
        : m_type{type}, y_speed{speed}, m_hitbox{bounds} {
        m_color = colors_map[type];
    };
    Trash(Trash&&) = default;
    Trash(const Trash&) = default;
    Trash& operator=(Trash&&) = default;
    Trash& operator=(const Trash&) = default;
    ~Trash() = default;

    void move(const float dt) { m_hitbox.y += y_speed * dt; };

    // Returns true if the hitbox hits the bottom of the screen
    bool update(const float dt, const Rlib::Rectangle& bounds) {
        this->move(dt);

        if (m_hitbox.y + m_hitbox.height >= bounds.y + bounds.height)
            return true;

        return false;
    };

    void draw() { m_hitbox.Draw(m_color); };

    static Trash create_random(const Rlib::Rectangle& bounds) {
        using enum Type;
        constexpr int hitbox_size{100};

        return {
            static_cast<Type>(Random::get(0, static_cast<int>(MAX_TYPES) - 1)),
            Random::get(100, 600),
            {static_cast<float>(Random::get(
                 (int)bounds.x, bounds.x + bounds.width - hitbox_size)),
             0, hitbox_size, hitbox_size}};
    };
};
