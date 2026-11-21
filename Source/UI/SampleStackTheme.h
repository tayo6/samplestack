#pragma once

#include <cmath>
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

    // Ableton Simpler-style chrome: grey panel, flat inset fields, dark "screens".
    // Layer colours above are intentionally unchanged so both lanes stay distinct.
    namespace simpler
    {
        inline juce::Colour panel()        { return juce::Colour (0xff909192); }
        inline juce::Colour panelDark()    { return juce::Colour (0xff6b6c6d); }
        inline juce::Colour field()        { return juce::Colour (0xff8b8c8d); }
        inline juce::Colour fieldBorder()  { return juce::Colour (0xff5a5b5c); }
        inline juce::Colour text()         { return juce::Colour (0xff161616); }
        inline juce::Colour textDim()      { return juce::Colour (0xff6f7071); }
        inline juce::Colour tabActive()    { return juce::Colour (0xffb4b5b5); }
        inline juce::Colour tabInactive()  { return juce::Colour (0xff9fa0a0); }
        inline juce::Colour display()      { return juce::Colour (0xff2b2b2b); }
        inline juce::Colour orange()       { return juce::Colour (0xffffad32); }
        inline juce::Colour blueSelect()   { return juce::Colour (0xff6fcbff); }
        inline juce::Colour comboFill()    { return juce::Colour (0xffc6c7c7); }
        inline juce::Colour handle()       { return juce::Colour (0xffa4a4a4); }
        inline juce::Colour rulerText()    { return juce::Colour (0xff9b9b9b); }
        inline juce::Colour rulerTick()    { return juce::Colour (0xff7d7d7d); }
    }

    // Simpler ruler labels: "0:00", "0:00:100", "0:01", "0:01:500" (m:ss[:mmm]).
    inline juce::String formatRulerTime (double seconds)
    {
        const int totalMs = (int) std::llround (seconds * 1000.0);
        if (totalMs <= 0)
            return "0:00";

        const int minutes = totalMs / 60000;
        const int secs = (totalMs / 1000) % 60;
        const int millis = totalMs % 1000;

        auto result = juce::String (minutes) + ":" + juce::String (secs).paddedLeft ('0', 2);
        if (millis != 0)
            result << ":" << juce::String (millis).paddedLeft ('0', 3);
        return result;
    }
}
