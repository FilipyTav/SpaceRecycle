#pragma once

#include "Utils/Globals.h"
#include "Utils/Random.h"

struct Star {
    Rlib::Rectangle rec{};

    // In pixels
    int size{};
    int speed{};

    Rlib::Color color{WHITE};

    void update(const float dt) { rec.y += speed * dt; };
    void draw() { rec.Draw(color); };

    Star static create_random(const Rlib::Rectangle& bounds) {
        const int sz{Random::get(1, 10)};

        return {{static_cast<float>(
                     Random::get((int)bounds.x, bounds.x + bounds.width - sz)),
                 0, static_cast<float>(sz), static_cast<float>(sz)},
                sz,
                Random::get(50, 150)};
    };
};
