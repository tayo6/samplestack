#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

namespace SampleStackTheme
{
    inline juce::Colour background()      { return juce::Colour::fromRGB (35, 37, 39); }
    inline juce::Colour panel()           { return juce::Colour::fromRGB (47, 50, 53); }
    inline juce::Colour panelRaised()     { return juce::Colour::fromRGB (57, 60, 63); }
    inline juce::Colour border()          { return juce::Colour::fromRGB (70, 74, 78); }
    inline juce::Colour text()            { return juce::Colour::fromRGB (238, 240, 242); }
    inline juce::Colour mutedText()       { return juce::Colour::fromRGB (164, 169, 174); }

    // The two sample layers must remain visually distinguishable.
    inline juce::Colour mainSample()      { return juce::Colour::fromRGB (72, 185, 223); }
    inline juce::Colour preSample()       { return juce::Colour::fromRGB (235, 166, 55); }
    inline juce::Colour playhead()        { return juce::Colour::fromRGB (245, 245, 245); }
}
