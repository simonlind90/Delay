/*
  ==============================================================================

    LevelMeter.cpp
    Created: 5 Dec 2024 9:21:23pm
    Author:  Simon Lind

  ==============================================================================
*/

#include <JuceHeader.h>
#include "LevelMeter.h"

//==============================================================================
LevelMeter::LevelMeter(std::atomic<float>& measurementL_,
                       std::atomic<float>& measurementR_): measurementL(measurementL_), measurementR(measurementR_)
{
    setOpaque(true);
    startTimerHz(1);
}

LevelMeter::~LevelMeter()
{
}

void LevelMeter::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (juce::FontOptions (14.0f));
    g.drawText ("LevelMeter", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}

void LevelMeter::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}

void LevelMeter::timerCallback() {
    DBG("left: " << measurementL.load() << ", right: " << measurementR.load());
}
