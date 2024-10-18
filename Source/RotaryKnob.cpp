/*
  ==============================================================================

    RotaryKnob.cpp
    Created: 22 Sep 2024 9:56:33pm
    Author:  Simon Lind

  ==============================================================================
*/

#include <JuceHeader.h>
#include "RotaryKnob.h"
#include "LookAndFeel.h"

//==============================================================================
RotaryKnob::RotaryKnob(const juce::String& text,
                       juce::AudioProcessorValueTreeState& apvts,
                       const juce::ParameterID& parameterID,
                       bool drawFromMiddle): attachment(apvts, parameterID.getParamID(), slider) {
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    slider.getProperties().set("drawFromMiddle", drawFromMiddle);
    slider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 70, 16);
    slider.setBounds(0, 0, 70, 86);
    addAndMakeVisible(slider);
    
    label.setText(text, juce::NotificationType::dontSendNotification);
    label.setJustificationType(juce::Justification::horizontallyCentred);
    label.setBorderSize(juce::BorderSize<int>{ 0, 0, 2, 0 });
    label.attachToComponent(&slider, false);
    addAndMakeVisible(label);
    
    float pi = juce::MathConstants<float>::pi;
    slider.setRotaryParameters(1.25f * pi, 2.75f * pi, true);
    
    setSize(70, 110);
    
    setLookAndFeel(RotaryKnobLookAndFeel::get());
}

RotaryKnob::~RotaryKnob()
{
}

void RotaryKnob::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    slider.setTopLeftPosition(0, 24);
}
