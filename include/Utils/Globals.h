#pragma once

#include <raylib-cpp.hpp>
#include <unordered_map>

namespace Rlib = raylib;

namespace Orientation {
enum class Direction { UP, DOWN, LEFT, RIGHT, MAX_DIRECTIONS };
enum class Axis { HORIZONTAL, VERTICAL, MAX_AXES };
} // namespace Orientation

namespace TrashInfo {
enum class Type {
    PAPER,   // Blue
    GLASS,   // Green
    PLASTIC, // Red
    METAL,   // Yellow
    ORGANIC, // Brown

    MAX_TYPES
};

using enum Type;
inline std::unordered_map<Type, Rlib::Color> colors_map{{PAPER, BLUE},
                                                        {GLASS, GREEN},
                                                        {PLASTIC, RED},
                                                        {METAL, YELLOW},
                                                        {ORGANIC, BROWN}};
} // namespace TrashInfo
