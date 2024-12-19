/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "ProtectYourEars.h"

//==============================================================================
DelayAudioProcessor::DelayAudioProcessor()
     : AudioProcessor (BusesProperties()
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                       ),
    params(apvts)
{
    lowCutFilter.setType(juce::dsp::StateVariableTPTFilterType::highpass);
    highCutFilter.setType(juce::dsp::StateVariableTPTFilterType::lowpass);
}

DelayAudioProcessor::~DelayAudioProcessor()
{
}

//==============================================================================
const juce::String DelayAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool DelayAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool DelayAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool DelayAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double DelayAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int DelayAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int DelayAudioProcessor::getCurrentProgram()
{
    return 0;
}

void DelayAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String DelayAudioProcessor::getProgramName (int index)
{
    return {};
}

void DelayAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void DelayAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    params.prepareToPlay(sampleRate);
    params.reset();
    
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = juce::uint32(samplesPerBlock);
    spec.numChannels = 2;
    
    delayLine.prepare(spec);
    
    double numSamples = (Parameters::maxDelayTime / 1000.0) * sampleRate;
    int maxDelayInSamples = int(std::ceil(numSamples));
    delayLine.setMaximumDelayInSamples(maxDelayInSamples);
    delayLine.reset();
    
    feedbackL = 0.0f;
    feedbackR = 0.0f;
    
    lowCutFilter.prepare(spec);
    lowCutFilter.reset();
    
    highCutFilter.prepare(spec);
    highCutFilter.reset();
}

void DelayAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

bool DelayAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    const auto mono = juce::AudioChannelSet::mono();
    const auto stereo = juce::AudioChannelSet::stereo();
    const auto mainIn = layouts.getMainInputChannelSet();
    const auto mainOut = layouts.getMainOutputChannelSet();
    
    //DBG("isBusesLayoutSupported, in: " << mainIn.getDescription()
    //        << ", out: " << mainOut.getDescription());
    
    if (mainIn == mono && mainOut == mono) {
        return true;
    } else if (mainIn == mono && mainOut == stereo) {
        return true;
    } else if (mainIn == stereo && mainOut == stereo) {
        return true;
    }
    return false;
}

void DelayAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, [[maybe_unused]] juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i) {
        buffer.clear (i, 0, buffer.getNumSamples());
    }
    
    params.update();
    float sampleRate = float(getSampleRate());
    auto mainInput = getBusBuffer(buffer, true, 0);
    auto mainInputChannels = mainInput.getNumChannels();
    auto isMainInputStereo = mainInputChannels > 1;
    const float* inputDataL = mainInput.getReadPointer(0);
    const float* inputDataR = mainInput.getReadPointer(isMainInputStereo ? 1 : 0);
    
    auto mainOutput = getBusBuffer(buffer, false, 0);
    auto mainOutputChannels = mainOutput.getNumChannels();
    auto isMainOutputStereo = mainOutputChannels > 1;
    float* outputDataL = mainOutput.getWritePointer(0);
    float* outputDataR = mainOutput.getWritePointer(isMainOutputStereo ? 1 : 0);
    
    if (isMainOutputStereo) {
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample) {
            params.smoothen();
            float delayInSamples = (params.delayTime / 1000.0f) * sampleRate;
            delayLine.setDelay(delayInSamples);
            
            lowCutFilter.setCutoffFrequency(params.lowCut);
            highCutFilter.setCutoffFrequency(params.highCut);
            
            float dryL = inputDataL[sample];
            float dryR = inputDataR[sample];
            
            float mono = (dryL + dryR) * 0.5f;
            
            delayLine.pushSample(0, mono * params.panL + feedbackR);
            delayLine.pushSample(1, mono * params.panR + feedbackL);
            
            float wetL = delayLine.popSample(0);
            float wetR = delayLine.popSample(1);
            
            feedbackL = wetL * params.feedback;
            feedbackL = lowCutFilter.processSample(0, feedbackL);
            feedbackL = highCutFilter.processSample(0, feedbackL);
            
            feedbackR = wetR * params.feedback;
            feedbackR = lowCutFilter.processSample(1, feedbackR);
            feedbackR = highCutFilter.processSample(1, feedbackR);
            
            float mixL = dryL * (1.0f - params.mix) + wetL * params.mix;
            float mixR = dryR * (1.0f - params.mix) + wetR * params.mix;
            
            outputDataL[sample] = mixL * params.gain;
            outputDataR[sample] = mixR * params.gain;
        }
    } else {
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample) {
            params.smoothen();
            
            float delayInSamples = params.delayTime / 1000.0f * sampleRate;
            delayLine.setDelay(delayInSamples);
            
            float dry = inputDataL[sample];
            delayLine.pushSample(0, dry + feedbackL);
            
            float wet = delayLine.popSample(0);
            feedbackL = wet * params.feedback;
            feedbackL = lowCutFilter.processSample(0, feedbackL);
            feedbackL = highCutFilter.processSample(0, feedbackL);
            
            float mix = dry + wet * params.mix;
            outputDataL[sample] = mix * params.gain;
        }
    }
    #if JUCE_DEBUG
    protectYourEars(buffer);
    #endif
}

//==============================================================================
bool DelayAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* DelayAudioProcessor::createEditor()
{
    return new DelayAudioProcessorEditor (*this);
}

//==============================================================================
void DelayAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    copyXmlToBinary(*apvts.copyState().createXml(), destData);
}

void DelayAudioProcessor::setStateInformation (const void* data, int sizeInBytes) {
    std::unique_ptr<juce::XmlElement> xml(getXmlFromBinary(data, sizeInBytes));
    if (xml.get() != nullptr && xml->hasTagName(apvts.state.getType())) {
        apvts.replaceState(juce::ValueTree::fromXml(*xml));
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DelayAudioProcessor();
}

