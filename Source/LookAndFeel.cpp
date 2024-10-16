/*
  ==============================================================================

    LookAndFeel.cpp
    Created: 14 Oct 2024 9:51:17pm
    Author:  Simon Lind

  ==============================================================================
*/

#include "LookAndFeel.h"

RotaryKnobLookAndFeel::RotaryKnobLookAndFeel() {
    setColour(juce::Label::textColourId, Colors::Knob::label);
    setColour(juce::Slider::textBoxTextColourId, Colors::Knob::label);
}

void RotaryKnobLookAndFeel::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height,
                                             float sliderPos, float rotaryStartAngle, float rotaryEndAngle, juce::Slider &slider) {
    
}
