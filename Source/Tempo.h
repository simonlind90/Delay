/*
  ==============================================================================

    Tempo.h
    Created: 14 Nov 2024 9:36:46pm
    Author:  Simon Lind

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class Tempo {
public:
    void reset() noexcept;
    void update(const juce::AudioPlayHead* playhead) noexcept;
    double getMillisecondsForNoteLength(int index) const noexcept;
    double getTempo() const noexcept {
        return bpm;
    }
private:
    double bpm = 120.0;
};
