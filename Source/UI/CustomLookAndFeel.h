#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

// Shared visual language for SampleStack controls, styled after the Sample tab
// of Ableton's Simpler: grey panel, flat inset fields, orange "on" state.
// Keep this class independent from the processor and audio engine.
//
// Component properties understood by this LookAndFeel (getProperties().set):
//
//   Button (TextButton / ToggleButton)
//     "tab"    : true               -> draw as a tab (rounded top corners); toggle state = front tab
//     "dot"    : true               -> tab: LED dot left of the text
//     "dotOn"  : true               -> tab: LED dot is lit
//     "arrow"  : true               -> tab: small left-pointing triangle after the text
//     "icon"   : "oneShot" | "loopFwd" | "loopAlt"   -> arrow icon instead of text
//     "accent" : int64 ARGB colour  -> "on" fill colour (default: Simpler orange);
//                                      used to keep the two sample layers distinguishable
//   Slider (LinearBar)
//     "arrow"  : true               -> small drag triangle on the right of the value field
//   Label
//     "field"  : true               -> draw as a grey inset field
class SampleStackLookAndFeel final : public juce::LookAndFeel_V4
{
public:
    SampleStackLookAndFeel();

    // Buttons
    juce::Font getTextButtonFont (juce::TextButton& button, int buttonHeight) override;

    void drawButtonBackground (juce::Graphics& graphics,
                               juce::Button& button,
                               const juce::Colour& backgroundColour,
                               bool shouldDrawButtonAsHighlighted,
                               bool shouldDrawButtonAsDown) override;

    void drawButtonText (juce::Graphics& graphics,
                         juce::TextButton& button,
                         bool shouldDrawButtonAsHighlighted,
                         bool shouldDrawButtonAsDown) override;

    void drawToggleButton (juce::Graphics& graphics,
                           juce::ToggleButton& button,
                           bool shouldDrawButtonAsHighlighted,
                           bool shouldDrawButtonAsDown) override;

    // Labels and text entry
    juce::Font getLabelFont (juce::Label& label) override;
    void drawLabel (juce::Graphics& graphics, juce::Label& label) override;
    void fillTextEditorBackground (juce::Graphics& graphics, int width, int height,
                                   juce::TextEditor& editor) override;
    void drawTextEditorOutline (juce::Graphics& graphics, int width, int height,
                                juce::TextEditor& editor) override;

    // Sliders
    void drawLinearSlider (juce::Graphics& graphics,
                           int x, int y, int width, int height,
                           float sliderPos, float minSliderPos, float maxSliderPos,
                           juce::Slider::SliderStyle style,
                           juce::Slider& slider) override;

    void drawRotarySlider (juce::Graphics& graphics,
                           int x, int y, int width, int height,
                           float sliderPosProportional,
                           float rotaryStartAngle,
                           float rotaryEndAngle,
                           juce::Slider& slider) override;

    // Combo box
    juce::Font getComboBoxFont (juce::ComboBox& box) override;
    void drawComboBox (juce::Graphics& graphics, int width, int height, bool isButtonDown,
                       int buttonX, int buttonY, int buttonWidth, int buttonHeight,
                       juce::ComboBox& box) override;
    void positionComboBoxText (juce::ComboBox& box, juce::Label& label) override;

    // TabbedButtonComponent support (the main view uses toggle TextButtons with the
    // "tab" property, but a real TabbedButtonBar gets the same look)
    int getTabButtonBestWidth (juce::TabBarButton& button, int tabDepth) override;
    void drawTabAreaBehindFrontButton (juce::TabbedButtonBar& bar, juce::Graphics& graphics,
                                       int width, int height) override;
    void drawTabButton (juce::TabBarButton& button, juce::Graphics& graphics,
                        bool isMouseOver, bool isMouseDown) override;

    // Shared drawing helpers (also used by the waveform display)
    static juce::Font uiFont (float height = 12.5f);
    static void drawField (juce::Graphics& graphics, juce::Rectangle<float> bounds,
                           juce::Colour fill);
    static void drawTabShape (juce::Graphics& graphics, juce::Rectangle<float> bounds,
                              const juce::String& text, bool isFront, bool isMouseOver,
                              bool isMouseDown, bool dot, bool dotOn, bool arrow);

private:
    static juce::Colour getAccentColour (const juce::Component& component);
    static void drawArrowHead (juce::Graphics& graphics, float tipX, float y, bool pointsRight);
    static void drawModeIcon (juce::Graphics& graphics, const juce::String& icon,
                              juce::Rectangle<float> bounds, juce::Colour ink);
};
