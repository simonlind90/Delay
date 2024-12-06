/*
  ==============================================================================

    Measurement.h
    Created: 6 Dec 2024 9:20:10am
    Author:  Simon Lind

  ==============================================================================
*/

#pragma once

#include <atomic>

struct Measurement {
    void reset() noexcept {
        value.store(0.0f);
    }
    void updateIfGreater(float newValue) noexcept {
        auto oldValue = value.load();
        while (newValue > oldValue && !value.compare_exchange_weak(oldValue, newValue));
    }
    
    float readAndReset() noexcept {
        return value.exchange(0.0f);
    }
    
    std::atomic<float> value;
};
