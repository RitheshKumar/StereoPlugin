/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#ifndef PLUGINPROCESSOR_H_INCLUDED
#define PLUGINPROCESSOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "PeakProgramMeter.h"


//==============================================================================
/**
*/
class UpmixerAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    UpmixerAudioProcessor();
    ~UpmixerAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    void processBlock (AudioSampleBuffer&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    //==============================================================================
    float getPeakVal(int channel ){ return _peakVal[channel]; }

private:
    //==============================================================================
    float *_peakVal; //we will store only values for the first 2 channels
    PeakProgramMeter *pPPM;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (UpmixerAudioProcessor)
};


#endif  // PLUGINPROCESSOR_H_INCLUDED
