#pragma once

#include <raylib-cpp.hpp>
#include <unordered_map>

namespace Rlib = raylib;

template <typename T> //
struct Shy {
    T x;
    T y;
};

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

namespace Config {
namespace General {
inline const std::string root_path =
    std::string(GetApplicationDirectory()) + "../";
}

namespace Trash {
// How many can be in the screen at the same time
inline int max{10};

// In seconds
inline int delay{1};
} // namespace Trash
} // namespace Config
