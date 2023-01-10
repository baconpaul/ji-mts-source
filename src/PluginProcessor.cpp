/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "libMTSMaster.h"
#include "JIMath.h"



//==============================================================================
JIMTSSourceProcessor::JIMTSSourceProcessor()
    : juce::AudioProcessor (juce::AudioProcessor::BusesProperties()
                      .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
        )
{
    auto initS = Tunings::evenTemperament12NoteScale();
    setSCL( initS.rawText, false );
    setKBM( "", false );
    retune();

    uint64_t n{1}, d{1};
    JIMath jim;
    for (int i=0; i<7; ++i)
    {
        std::cout << i << " " << n << " / " << d << std::endl;
        auto res = jim.fracMul(n,d,3,2);
        n = res.first;
        d = res.second;
    }

    n = 1; d = 1;
    for (int i=0; i<7; ++i)
    {
        std::cout << i << " " << n << " / " << d << std::endl;
        auto res = jim.fracDiv(n,d,3,2);
        n = res.first;
        d = res.second;
    }


    if (false && MTS_CanRegisterMaster())
    {
        MTS_RegisterMaster();
        registeredMTS = true;
        repushTuning = true;
    }
}

JIMTSSourceProcessor::~JIMTSSourceProcessor()
{
    if (registeredMTS)
        MTS_DeregisterMaster();
}

//==============================================================================
const juce::String JIMTSSourceProcessor::getName() const
{
    return JucePlugin_Name;
}

bool JIMTSSourceProcessor::acceptsMidi() const
{
    return false;
}

bool JIMTSSourceProcessor::producesMidi() const
{
    return false;
}

bool JIMTSSourceProcessor::isMidiEffect() const
{
    return false;
}

double JIMTSSourceProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int JIMTSSourceProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int JIMTSSourceProcessor::getCurrentProgram()
{
    return 0;
}

void JIMTSSourceProcessor::setCurrentProgram (int index)
{
}

const juce::String JIMTSSourceProcessor::getProgramName (int index)
{
    return {};
}

void JIMTSSourceProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void JIMTSSourceProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
}

void JIMTSSourceProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

bool JIMTSSourceProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    return true;
}

void JIMTSSourceProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    if (!registeredMTS)
        return;

    numClients = MTS_GetNumClients();


}

//==============================================================================
bool JIMTSSourceProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor*JIMTSSourceProcessor::createEditor()
{
    return new JIMTSSourceEditor(*this);
}

//==============================================================================
void JIMTSSourceProcessor::getStateInformation (juce::MemoryBlock& destData)
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

void JIMTSSourceProcessor::setStateInformation (const void* data, int sizeInBytes)
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


void JIMTSSourceProcessor::setSCL( juce::String SCL, bool dretune )
{
    currentSCLString = SCL;
    if( dretune )
        retune();
}

void JIMTSSourceProcessor::setKBM( juce::String KBM, bool dretune )
{
    currentKBMString = KBM;
    if( dretune )
        retune();
}

void JIMTSSourceProcessor::retune() {
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
    return new JIMTSSourceProcessor();
}
