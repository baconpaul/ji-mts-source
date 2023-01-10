/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include "Tunings.h"
#include <set>

//==============================================================================
/**
*/

class SimpleMTSMain;


class SimpleMTSMain  : public juce::AudioProcessor
{
public:
    //==============================================================================
    SimpleMTSMain();
    ~SimpleMTSMain();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    // == tuning support ==
    void setSCL( juce::String SCL, bool retune = true );
    void setKBM( juce::String KBM, bool retune = true );
    void resetSCLToStandard() {
        auto s = Tunings::evenTemperament12NoteScale();
        setSCL( s.rawText );
    }
    void resetKBMToStandard() {
        setKBM( "" );
    }

    void retune();

    Tunings::Tuning tuning;
    juce::String currentSCLString = "", currentKBMString = "";

    std::atomic<int> numClients{0};
private:

    bool registeredMTS{false};
    std::atomic<bool> repushTuning{false};
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SimpleMTSMain)
};
