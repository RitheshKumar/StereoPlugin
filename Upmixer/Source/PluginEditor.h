/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#ifndef PLUGINEDITOR_H_INCLUDED
#define PLUGINEDITOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "peakBar.cpp"
#include "dragBox.cpp"


//==============================================================================
/**
*/
class UpmixerAudioProcessorEditor  : public AudioProcessorEditor,
                                     private Timer
{
public:
    UpmixerAudioProcessorEditor (UpmixerAudioProcessor&);
    ~UpmixerAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    void timerCallback() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    UpmixerAudioProcessor& processor;


    //GUI declarations
    peakBar correlationBar,
            ppmBarL, ppmBarR;
    ComboBox presetList;
    dragBox widthSetBox, depthSetBox;
    
    Label corrBarLabel;

    ComponentBoundsConstrainer constrainer;
    ComponentDragger dragger;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (UpmixerAudioProcessorEditor)
};


#endif  // PLUGINEDITOR_H_INCLUDED
