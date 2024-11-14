/*
  ==============================================================================

    Tempo.cpp
    Created: 14 Nov 2024 9:36:46pm
    Author:  Simon Lind

  ==============================================================================
*/

#include "Tempo.h"

void Tempo::reset() noexcept {
    bpm = 120.0;
}

void Tempo::update(const juce::AudioPlayHead* playhead) noexcept {
    reset();
    if (playhead == nullptr) {
        return;
    }
    const auto opt = playhead->getPosition();
    if (!opt.hasValue()) {
        return;
    }
    const auto& pos = *opt;
    if (pos.getBpm().hasValue()) {
        bpm = *pos.getBpm();
    }
}
