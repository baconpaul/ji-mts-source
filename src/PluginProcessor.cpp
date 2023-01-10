/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "libMTSMaster.h"



//==============================================================================
SimpleMTSMain::SimpleMTSMain()
    : juce::AudioProcessor (juce::AudioProcessor::BusesProperties()
                      .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
        )
{
    auto initS = Tunings::evenTemperament12NoteScale();
    setSCL( initS.rawText, false );
    setKBM( "", false );
    retune();

    if (MTS_CanRegisterMaster())
    {
        MTS_RegisterMaster();
        registeredMTS = true;
        repushTuning = true;
    }
}

SimpleMTSMain::~SimpleMTSMain()
{
    if (registeredMTS)
        MTS_DeregisterMaster();
}

//==============================================================================
const juce::String SimpleMTSMain::getName() const
{
    return JucePlugin_Name;
}

bool SimpleMTSMain::acceptsMidi() const
{
    return false;
}

bool SimpleMTSMain::producesMidi() const
{
    return false;
}

bool SimpleMTSMain::isMidiEffect() const
{
    return false;
}

double SimpleMTSMain::getTailLengthSeconds() const
{
    return 0.0;
}

int SimpleMTSMain::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int SimpleMTSMain::getCurrentProgram()
{
    return 0;
}

void SimpleMTSMain::setCurrentProgram (int index)
{
}

const juce::String SimpleMTSMain::getProgramName (int index)
{
    return {};
}

void SimpleMTSMain::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void SimpleMTSMain::prepareToPlay (double sampleRate, int samplesPerBlock)
{
}

void SimpleMTSMain::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

bool SimpleMTSMain::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    return true;
}

void SimpleMTSMain::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    if (!registeredMTS)
        return;

    numClients = MTS_GetNumClients();


}

//==============================================================================
bool SimpleMTSMain::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* SimpleMTSMain::createEditor()
{
    return new SimpleMTSMainEditor (*this);
}

//==============================================================================
void SimpleMTSMain::getStateInformation (juce::MemoryBlock& destData)
{
    auto xml = std::make_unique<juce::XmlElement>("state");

    xml->deleteAllChildElementsWithTagName( "tuningState" );
    auto txml = xml->createNewChildElement( "tuningState" );
    auto sclx = txml->createNewChildElement( "scl" );
    sclx->addTextElement(currentSCLString);
    auto kbmx = txml->createNewChildElement( "kbm" );
    kbmx->addTextElement(currentKBMString);

    copyXmlToBinary (*xml, destData);
}

void SimpleMTSMain::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));

    if (xmlState.get() != nullptr)
    {
        auto txml = xmlState->getChildByName( "tuningState" );
        if( txml )
        {
            bool needsRT = false;

            auto sclx = txml->getChildByName( "scl" );
            auto kbmx = txml->getChildByName( "kbm" );
            std::string s = "";

            if( sclx && sclx->getFirstChildElement() && sclx->getFirstChildElement()->isTextElement() )
            {
                s = sclx->getFirstChildElement()->getText().toStdString();
                if( s.size() > 1 )
                {
                    needsRT = true;
                    setSCL( s, false );
                }
            }
            
            std::string k = "";
            if( kbmx && kbmx->getFirstChildElement() && kbmx->getFirstChildElement()->isTextElement() )
            {
                k = kbmx->getFirstChildElement()->getText().toStdString();
                if( k.size() > 1 )
                {
                    needsRT = true;
                    setKBM( k, false );
                }
            }

            if( needsRT )
            {
                retune();
            }
        }
    }
}


void SimpleMTSMain::setSCL( juce::String SCL, bool dretune )
{
    currentSCLString = SCL;
    if( dretune )
        retune();
}

void SimpleMTSMain::setKBM( juce::String KBM, bool dretune )
{
    currentKBMString = KBM;
    if( dretune )
        retune();
}

void SimpleMTSMain::retune() {
    try
    {
        if (currentKBMString.isEmpty())
            currentKBMString = Tunings::tuneA69To(440).rawText;
        tuning = Tunings::Tuning(Tunings::parseSCLData(currentSCLString.toStdString()),
                                 Tunings::parseKBMData(currentKBMString.toStdString()));
        repushTuning = true;

        repushTuning = false;
        for (int i=0; i<128; ++i)
        {
            MTS_SetNoteTuning(tuning.frequencyForMidiNote(i), i);
        }
        MTS_SetScaleName(tuning.scale.name.c_str());
    }
    catch (const Tunings::TuningError &e)
    {
        // This is just sample code
        std::cout << "TUNING ERROR " << e.what() << std::endl;
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SimpleMTSMain();
}
