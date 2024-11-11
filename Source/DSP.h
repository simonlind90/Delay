/*
  ==============================================================================

    DSP.h
    Created: 11 Nov 2024 8:57:24pm
    Author:  Simon Lind

  ==============================================================================
*/

#pragma once

#include <cmath>

inline void panningEqualPower(float panning, float& left, float& right) {
    float x = 0.7853981633974483f * (panning + 1.0f); // π/4 (x + 1) { -1 <= x <= 1 }
    left = std::cos(x);
    right = std::sin(x);
}
