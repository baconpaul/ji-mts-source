/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SimpleMTSMainEditor::SimpleMTSMainEditor(SimpleMTSMain &p)
    : juce::AudioProcessorEditor(&p), processor(p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize(500, 300);

    loadSCL = std::make_unique<juce::TextButton>("Load SCL");
    loadSCL->setBounds(10, 10, 200, 25);
    loadSCL->onClick = [this]() { doLoad(SCL); };
    addAndMakeVisible(*loadSCL);

    loadKBM = std::make_unique<juce::TextButton>("Load KBM");
    loadKBM->setBounds(10, 40, 200, 25);

    loadKBM->onClick = [this]() { doLoad(KBM); };
    addAndMakeVisible(*loadKBM);
}

SimpleMTSMainEditor::~SimpleMTSMainEditor() {}

//==============================================================================
void SimpleMTSMainEditor::paint(juce::Graphics &g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

void SimpleMTSMainEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}

void SimpleMTSMainEditor::idle() {}

void SimpleMTSMainEditor::doLoad(LoadType t)
{
    fc = std::make_unique<juce::FileChooser>(juce::String("Load a ") + (t == SCL ? "SCL" : "KBM"),
                                             juce::File(), (t == SCL ? "*.scl" : "*.kbm"));
    fc->launchAsync(juce::FileBrowserComponent::openMode |
                        juce::FileBrowserComponent::canSelectFiles,
                    [this, t](const juce::FileChooser &f) {
                        auto fl = f.getResults();
                        if (fl.size() == 1)
                        {
                            std::cout << "Would load " << fl[0].getFullPathName() << std::endl;
                            auto s = fl[0].loadFileAsString();
                            if (t == SCL)
                                this->processor.setSCL(s);
                            if (t == KBM)
                                this->processor.setKBM(s);
                        }
                    });
}
