/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "JIMath.h"

struct JIComponent : juce::Component
{
    void paint(juce::Graphics &g) override
    {
        int JIRadius{30};
        float ctrDistance{JIRadius * 1.6f};

        float vDistance = sqrt(3) * ctrDistance;
        float hDistance = 3.4 / 2.0 * ctrDistance;

        auto ctrX = getWidth() / 2;
        auto ctrH = getHeight() / 2;

        auto nV = std::ceil(getHeight() / vDistance);
        auto nW = std::ceil(getWidth() / hDistance);

        // this overdoes it a bit
        for (int v = -nV-1; v < nV + 1; ++v)
        {
            float off = (v%2 == 0 ? 0.f : hDistance * 0.5f);
            float y = -v * vDistance + ctrH;
            if (y < 0 || y > getHeight())
                continue;
            for (int w = -nW-1; w < nW + 1; ++w)
            {
                float x = w * hDistance + ctrX + off;
                if (x < 0 || x > getWidth())
                    continue;

                g.setColour(juce::Colours::white);
                g.drawEllipse(x-JIRadius,y-JIRadius,2*JIRadius, 2*JIRadius, 1);

                auto fifths = w;
                auto thirds = v;

                if (v < 0)
                {
                    fifths -= (v-1)/2;
                }
                else
                {
                    fifths -= v/2;
                }
                auto [n,d] = calculateCell(fifths, thirds);
                auto s = std::to_string(n) + "/" + std::to_string(d);
                // auto s = std::to_string(fifths) + "F " + std::to_string(thirds) + "T";
                g.drawSingleLineText(s, x, y, juce::Justification::horizontallyCentred);
            }
        }
    }

    std::pair<uint64_t, uint64_t> calculateCell(int fifths, int thirds)
    {
        JIMath jim;
        uint64_t n{1}, d{1};

        while(thirds > 0)
        {
            auto [nn,dd] = jim.fracMul(n, d, 5, 4);
            n = nn;
            d = dd;
            thirds --;
        }

        while(thirds < 0)
        {
            auto [nn,dd] = jim.fracDiv(n, d, 5, 4);
            n = nn;
            d = dd;
            thirds ++;
        }

        while(fifths > 0)
        {
            auto [nn,dd] = jim.fracMul(n, d, 3, 2);
            n = nn;
            d = dd;
            fifths --;
        }

        while(fifths < 0)
        {
            auto [nn,dd] = jim.fracDiv(n, d, 3, 2);
            n = nn;
            d = dd;
            fifths ++;
        }



        auto g = std::gcd(n,d);
        n = n / g;
        d = d / g;

        return {n,d};
    }
};

//==============================================================================
JIMTSSourceEditor::JIMTSSourceEditor(JIMTSSourceProcessor &p)
    : juce::AudioProcessorEditor(&p), processor(p)
{
    jiComponent = std::make_unique<JIComponent>();
    addAndMakeVisible(*jiComponent);

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize(900, 600);
    setResizable(true, true);
}

JIMTSSourceEditor::~JIMTSSourceEditor() {}

//==============================================================================
void JIMTSSourceEditor::paint(juce::Graphics &g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

void JIMTSSourceEditor::idle() {}

void JIMTSSourceEditor::resized()
{
    jiComponent->setBounds(getLocalBounds());
}
