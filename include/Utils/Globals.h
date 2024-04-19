#pragma once

#include <cassert>
#include <cmath>
#include <raylib-cpp.hpp>
#include <unordered_map>

namespace Rlib = raylib;

template <typename T> //
struct Shy {
    T x{};
    T y{};

    bool operator!() { return x == 0 && y == 0; };
};

struct SpriteSheet {
    Rlib::Texture2D texture{};
    Shy<int> size{};

    Rlib::Rectangle sprite_rec{};
    int index{0};

    SpriteSheet(){};

    SpriteSheet(const std::string path, const Shy<int> size) : size{size} {
        texture.Load(path);

        sprite_rec =
            Rlib::Rectangle{{0, 0},
                            {texture.width / static_cast<float>(size.x),
                             texture.height / static_cast<float>(size.y)}};
    };

    const Rlib::Rectangle get_rec_by_index(const int index) const {
        // index = y + x*width
        // [ index ] = [ col ] + row * [ width ]
        // row = ( index - col ) / width

        Shy<float> coords{};

        // col
        coords.x = (index % size.x);

        // row
        coords.y = ((index - coords.x)) / size.x;

        return {{coords.x * sprite_rec.width, coords.y * sprite_rec.height},
                {sprite_rec.width, sprite_rec.height}};
    };

    void rec_advance() {
        index = (index + 1) % (size.x * size.y);

        sprite_rec.x += size.x;

        if (sprite_rec.x == texture.width) {
            sprite_rec.x = 0;
            sprite_rec.y += size.y;
        }

        if (sprite_rec.y == texture.height) {
            sprite_rec.y = 0;
        }
    };

    // @param i = which index to use. < 0 means it should use the internal index
    void draw(const Rlib::Rectangle& dest_rec, int i = -1) const {
        if (i < 0) {
            texture.Draw(sprite_rec, dest_rec);
        } else {
            assert(i >= 0 && i < size.x * size.y &&
                   "SpriteSheet::draw() invalid index");

            texture.Draw(this->get_rec_by_index(i), dest_rec);
        }
    };
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

inline Shy<int> size{50, 50};

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

inline const std::string assets_path = root_path + "assets/";

inline Shy<int> spaceship_size{50, 50};
} // namespace General

namespace Sprites {
inline Shy<float> hearts_size{75, 75};
}

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

inline bool is_rec_inside(const Rlib::Rectangle& container,
                          const Rlib::Rectangle& contained) {
    return contained.x + contained.width <= container.x + container.width &&
           contained.y + contained.height <= container.y + container.height;
};
