#pragma once

#include "Utils/Defines.h"
#include "raylib-cpp.hpp"

struct Timer {
  private:
    // In seconds
    double m_start_time{};
    double m_lifetime{};

    // Default value for the timer, in seconds
    double m_reset_value{};

  public:
    Timer(const double default_lifetime) : m_reset_value{default_lifetime} {};

    void start(const double life_time) {
        m_start_time = GetTime();
        m_lifetime = life_time;

        m_reset_value = m_lifetime;
    };

    bool is_done() { return GetTime() - m_start_time >= m_lifetime; }

    double get_elapsed() { return GetTime() - m_start_time; }

    void reset() { this->start(m_reset_value); }

    MYGETTER(const double, lifetime, m_lifetime);
};
