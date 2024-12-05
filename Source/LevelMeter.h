/*
  ==============================================================================

    LevelMeter.h
    Created: 5 Dec 2024 9:21:23pm
    Author:  Simon Lind

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class LevelMeter  : public juce::Component, private juce::Timer
{
public:
    LevelMeter(std::atomic<float>& measurementL,
               std::atomic<float>& measurementR);
    ~LevelMeter() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    void timerCallback() override;
    std::atomic<float>& measurementL;
    std::atomic<float>& measurementR;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LevelMeter)
};
