/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
FirfilterAudioProcessor::FirfilterAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       ),
tree(*this, nullptr,"PARAMETERS",
{std::make_unique<AudioParameterFloat>("cutoff", "Cutoff", NormalisableRange<float>(20.0f, 2000.0f), 600.0f),
}),
lowPassFilter(dsp::FilterDesign<float>::designFIRLowpassWindowMethod(2000.0f, 44100, 21, dsp::WindowingFunction<float>::hamming))
#endif
{
}

FirfilterAudioProcessor::~FirfilterAudioProcessor()
{
}

//==============================================================================
const String FirfilterAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool FirfilterAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool FirfilterAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool FirfilterAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double FirfilterAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int FirfilterAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int FirfilterAudioProcessor::getCurrentProgram()
{
    return 0;
}

void FirfilterAudioProcessor::setCurrentProgram (int index)
{
}

const String FirfilterAudioProcessor::getProgramName (int index)
{
    return {};
}

void FirfilterAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void FirfilterAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    lastSampleRate = sampleRate;
    dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getTotalNumInputChannels();
    
    
    lowPassFilter.reset();
    updateFilter();
    lowPassFilter.prepare(spec);
}

void FirfilterAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool FirfilterAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void FirfilterAudioProcessor::updateFilter()
{
    float freq = *tree.getRawParameterValue("cutoff");
    *lowPassFilter.state = *dsp::FilterDesign<float>::designFIRLowpassWindowMethod(freq, 44100, 21, dsp::WindowingFunction<float>::hamming);

}




void FirfilterAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    
    dsp::AudioBlock<float> block (buffer);
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    updateFilter();
    lowPassFilter.process(dsp::ProcessContextReplacing<float> (block));
}

//==============================================================================
bool FirfilterAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* FirfilterAudioProcessor::createEditor()
{
    return new FirfilterAudioProcessorEditor (*this);
}

//==============================================================================
void FirfilterAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void FirfilterAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new FirfilterAudioProcessor();
}
