
#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class SimpleMTSMainEditor  : public juce::AudioProcessorEditor
{
public:
    SimpleMTSMainEditor (SimpleMTSMain&);
    ~SimpleMTSMainEditor();

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    std::unique_ptr<juce::Button> loadSCL, loadKBM;

    // Do this later
    std::unique_ptr<juce::Timer> idleTimer;
    void idle();

    enum LoadType { SCL, KBM };
    void doLoad(LoadType t);

    std::unique_ptr<juce::FileChooser> fc;

private:

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    SimpleMTSMain& processor;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SimpleMTSMainEditor)
};
