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
    Trash(const Type type) : m_type{type} { m_color = colors_map[type]; };
    Trash(Trash&&) = default;
    Trash(const Trash&) = default;
    Trash& operator=(Trash&&) = default;
    Trash& operator=(const Trash&) = default;
    ~Trash() = default;

    static Trash create_random() {
        using enum Type;

        return {
            static_cast<Type>(Random::get(0, static_cast<int>(MAX_TYPES) - 1))};
    };
};
