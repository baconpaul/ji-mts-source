
#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class JIMTSSourceEditor : public juce::AudioProcessorEditor
{
public:
  JIMTSSourceEditor(JIMTSSourceProcessor &);
    ~JIMTSSourceEditor();

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    std::unique_ptr<juce::Component> jiComponent;

    // Do this later
    std::unique_ptr<juce::Timer> idleTimer;
    void idle();

private:

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
  JIMTSSourceProcessor & processor;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (JIMTSSourceEditor)
};
