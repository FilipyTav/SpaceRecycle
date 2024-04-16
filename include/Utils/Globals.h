#pragma once

#include <raylib-cpp.hpp>
#include <unordered_map>

namespace Rlib = raylib;

template <typename T> //
struct Shy {
    T x{};
    T y{};
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

inline std::unordered_map<Type, const int> values_map{
    {PAPER, 1}, {GLASS, 5}, {PLASTIC, 10}, {METAL, 15}, {ORGANIC, 20}};
} // namespace TrashInfo

namespace Config {
namespace General {
inline const std::string root_path =
    std::string(GetApplicationDirectory()) + "../";

inline Shy<int> spaceship_size{50, 50};
} // namespace General

namespace Trash {
// How many can be in the screen at the same time
inline int max{10};

// Delay between consecutive trashes falling - in seconds
inline int delay{1};
} // namespace Trash
} // namespace Config

namespace Math {
enum class Operations {
    ADD,
    SUB,
    MULT,
    DIV,

    MAX_OPERATIONS,
};
}

inline Shy<float> get_middle_rec(const Rlib::Rectangle& rec) {
    return {rec.x + rec.width / 2, rec.y + rec.height / 2};
};
