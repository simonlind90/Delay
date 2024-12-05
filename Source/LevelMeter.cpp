/*
  ==============================================================================

    LevelMeter.cpp
    Created: 5 Dec 2024 9:21:23pm
    Author:  Simon Lind

  ==============================================================================
*/

#include <JuceHeader.h>
#include "LevelMeter.h"
#include "LookAndFeel.h"

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
    const auto bounds = getLocalBounds();
    g.fillAll(Colors::LevelMeter::background);
    
    g.setFont(Fonts::getFont(10.0f));
    for (float db = maxdB; db >= mindB; db -= stepdB) {
        int y = positionForLevel(db);
        
        g.setColour(Colors::LevelMeter::tickLine);
        g.fillRect(0, y, 16, 1);
        
        g.setColour(Colors::LevelMeter::tickLabel);
        g.drawSingleLineText(juce::String(int(db)), bounds.getWidth(), y + 3, juce::Justification::right);
    }
}

void LevelMeter::resized()
{
    maxPos = 4.0f;
    minPos = float(getHeight()) - 4.0f;
}

void LevelMeter::timerCallback() {
    DBG("left: " << measurementL.load() << ", right: " << measurementR.load());
}
