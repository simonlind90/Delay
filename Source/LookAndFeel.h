/*
  ==============================================================================

    LookAndFeel.h
    Created: 14 Oct 2024 9:51:17pm
    Author:  Simon Lind

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

namespace Colors {
    const juce::Colour background { 245, 240, 235 };
    const juce::Colour header { 40, 40, 40 };

    namespace Knob {
        const juce::Colour trackBackground { 205, 200, 195 };
        const juce::Colour trackActive { 177, 101, 135 };
        const juce::Colour outline { 255, 250, 245 };
        const juce::Colour gradientTop { 250, 245, 240 };
        const juce::Colour gradientBottom { 240, 235, 230 };
        const juce::Colour dial { 100, 100, 100 };
        const juce::Colour dropShadow { 195, 190, 185 };
        const juce::Colour label { 80, 80, 80 };
        const juce::Colour textBoxBackground { 80, 80, 80 };
        const juce::Colour value { 240, 240, 240 };
        const juce::Colour caret { 255, 255, 255 };
    }
}

class RotaryKnobLookAndFeel: public juce::LookAndFeel_V4 {
public:
    RotaryKnobLookAndFeel();
    static RotaryKnobLookAndFeel* get() {
        static RotaryKnobLookAndFeel instance;
        return &instance;
    }
    
    void drawRotarySlider(juce::Graphics& g, 
                          int x, int y,
                          int width, int height,
                          float sliderPos, float rotaryStartAngle, float rotaryEndAngle,
                          juce::Slider& slider) override;
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RotaryKnobLookAndFeel)
};
