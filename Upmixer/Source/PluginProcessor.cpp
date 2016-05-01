/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
UpmixerAudioProcessor::UpmixerAudioProcessor() : _peakVal(new float[2])
{
    PeakProgramMeter::createInstance(pPPM);
    Mono2Stereo::createInstance(pM2S);
}

UpmixerAudioProcessor::~UpmixerAudioProcessor()
{
    PeakProgramMeter::destroyInstance(pPPM);
    Mono2Stereo::destroyInstance(pM2S);
}

//==============================================================================
const String UpmixerAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool UpmixerAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool UpmixerAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

double UpmixerAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int UpmixerAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int UpmixerAudioProcessor::getCurrentProgram()
{
    return 0;
}

void UpmixerAudioProcessor::setCurrentProgram (int index)
{
}

const String UpmixerAudioProcessor::getProgramName (int index)
{
    return String();
}

void UpmixerAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void UpmixerAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    pPPM->initInstance(sampleRate, samplesPerBlock, getTotalNumInputChannels());
    pM2S->initInstance(sampleRate, samplesPerBlock);
}

void UpmixerAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

void UpmixerAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    const int totalNumInputChannels  = getTotalNumInputChannels();
    const int totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (int i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
//    for (int channel = 0; channel < totalNumInputChannels; ++channel)
//    {
//        float* channelData = buffer.getWritePointer (channel);
//
//        // ..do something to the data...
//    }
    
    pPPM->ppmProcess( buffer.getArrayOfReadPointers(), buffer.getNumSamples());
    pM2S->process(buffer.getArrayOfReadPointers(), buffer.getArrayOfWritePointers(), buffer.getNumSamples());
    
    for (int channel=0; channel < buffer.getNumChannels(); channel++) {
        _peakVal[channel] = pPPM->getPeak(channel);
    }
//    _peakVal[0] += 0.5f; //This is a debug tactic
}

//==============================================================================
bool UpmixerAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* UpmixerAudioProcessor::createEditor()
{
    return new UpmixerAudioProcessorEditor (*this);
}

//==============================================================================
void UpmixerAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void UpmixerAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new UpmixerAudioProcessor();
}
