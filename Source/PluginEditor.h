/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class FirfilterAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    FirfilterAudioProcessorEditor (FirfilterAudioProcessor&);
    ~FirfilterAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    


private:
    Slider filterCutoffDial;
    ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> filterCutoffValue;
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    FirfilterAudioProcessor& processor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FirfilterAudioProcessorEditor)
};
