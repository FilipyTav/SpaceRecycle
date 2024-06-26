#pragma once

#include <cassert>
#include <cmath>
#include <raylib-cpp.hpp>
#include <raylib.h>
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
    Rlib::Color tex_color{};

    // Amount of sprites on each axis
    Shy<int> size{};

    Rlib::Rectangle sprite_rec{};
    int index{0};

    int frames_speed{};
    int frame_counter{};

    SpriteSheet(){};

    SpriteSheet(const std::string path, const Shy<int> size,
                const int frame_speed = 5)
        : size{size}, frames_speed{frame_speed} {
        texture.Load(path);

        sprite_rec =
            Rlib::Rectangle{{0, 0},
                            {texture.width / static_cast<float>(size.x),
                             texture.height / static_cast<float>(size.y)}};

        // Placeholder color
        tex_color = Rlib::Color{156, 22, 125, 255};
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

        sprite_rec.x += sprite_rec.width;

        if (sprite_rec.x >= texture.width) {
            sprite_rec.x = 0;
            sprite_rec.y += sprite_rec.height;
        }

        if (sprite_rec.y >= texture.height) {
            sprite_rec.y = 0;
        }
    };

    bool is_ready() {
        // 60 fps
        return frame_counter >= (60 / frames_speed);
    };

    void update() { frame_counter++; };

    void reset_frame_counter() { frame_counter = 0; };

    // @param i = which index to use. < 0 means it should use the internal index
    // @param color = Replaces placeholder color with this one
    void draw(const Rlib::Rectangle& dest_rec, int i = -1,
              const Rlib::Color color = Rlib::BLANK) {
        if (color != Rlib::BLANK) {
            this->change_sprite_color(color);
        }

        if (i < 0) {
            texture.Draw(sprite_rec, dest_rec);
        } else {
            assert(i >= 0 && i < size.x * size.y &&
                   "SpriteSheet::draw() invalid index");

            texture.Draw(this->get_rec_by_index(i), dest_rec);
        }
    };

    void change_sprite_color(const Rlib::Color color) {
        SetTraceLogLevel(LOG_NONE);
        /*
         * 2 hours to do this
         * almost no docs or info about it
         * also didnt pay attention to the cheatsheet
         *
         * See:
         * https://raylib.handmade.network/forums/t/7285-get_pixel_color_from_texture2d
         */
        Rlib::Image a = texture.GetData();

        ImageColorReplace(&a, tex_color, color);
        tex_color = color;

        // Save changes
        Color* pixels =
            LoadImageColors(a); // Load pixel data from image (RGBA 32bit)
        texture.Update(pixels);

        SetTraceLogLevel(LOG_ALL);
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

inline Shy<int> size{70, 70};

using enum Type;
inline std::unordered_map<Type, Rlib::Color> colors_map{
    {PAPER, {0, 173, 230}},    // #00adef
    {GLASS, {0, 166, 82}},     // #00a652
    {PLASTIC, {237, 27, 36}},  // #ed1b24
    {METAL, {255, 247, 70}},   // #fff746
    {ORGANIC, {145, 76, 41}}}; // #914C29

inline std::unordered_map<Type, const int> values_map{
    {PAPER, 1}, {GLASS, 5}, {PLASTIC, 10}, {METAL, 15}, {ORGANIC, 20}};
} // namespace TrashInfo

namespace Config {
namespace General {
inline const std::string root_path =
    std::string(GetApplicationDirectory()) + "../";

inline const std::string assets_path = root_path + "assets/";

inline Shy<int> spaceship_hitbox{50, 50};
inline Shy<int> spaceship_size{90, 90};

inline const std::string game_name = "SpaceRecycle";
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

namespace General {
enum class GameScreen { TITLE, INSTRUCTIONS, GAMEPLAY, MAX_SCREENS };
}

inline Shy<float> get_middle_rec(const Rlib::Rectangle& rec) {
    return {rec.x + rec.width / 2, rec.y + rec.height / 2};
};

inline bool is_rec_inside(const Rlib::Rectangle& container,
                          const Rlib::Rectangle& contained) {
    return contained.x + contained.width <= container.x + container.width &&
           contained.y + contained.height <= container.y + container.height;
};
