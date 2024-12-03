/*
  ==============================================================================

    DelayLine.cpp
    Created: 22 Nov 2024 10:05:55am
    Author:  Simon Lind

  ==============================================================================
*/

#include <JuceHeader.h>
#include "DelayLine.h"

void DelayLine::setMaximumDelayInSamples(int maxLengthInSamples) {
    jassert(maxLengthInSamples > 0);
    int paddedLength = maxLengthInSamples + 1;
    if (bufferLength < paddedLength) {
        bufferLength = paddedLength;
        buffer.reset(new float[size_t(bufferLength)]);
    }
}

void DelayLine::reset() noexcept {
    writeIndex = bufferLength - 1;
    for (size_t i = 0; i < size_t(bufferLength); ++i) {
        buffer[i] = 0.0f;
    }
}

void DelayLine::write(float input) noexcept {
    jassert(bufferLength > 0);
    writeIndex += 1;
    if (writeIndex >= bufferLength) {
        writeIndex = 0;
    }
    buffer[size_t(writeIndex)] = input;
}

float DelayLine::read(float delayInSamples) const noexcept {
    jassert(delayInSamples >= 0.0f);
    jassert(delayInSamples <= bufferLength - 1.0f);
    
    int integerDelay = int(delayInSamples);
    
    int readIndexA = writeIndex - integerDelay;
    if (readIndexA < 0) {
        readIndexA += bufferLength;
    }
    
    int readIndexB = readIndexA - 1;
    if (readIndexB < 0) {
        readIndexB += bufferLength;
    }
    
    float sampleA = buffer[size_t(readIndexA)];
    float sampleB = buffer[size_t(readIndexB)];
    
    float fraction = delayInSamples - float(integerDelay);
    return sampleA + fraction * (sampleB - sampleA);
}
