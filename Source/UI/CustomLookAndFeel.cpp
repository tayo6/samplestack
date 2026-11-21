#include "CustomLookAndFeel.h"
#include "SampleStackTheme.h"

namespace simpler = SampleStackTheme::simpler;

SampleStackLookAndFeel::SampleStackLookAndFeel()
{
    // Simpler's UI font; Segoe UI ships with Windows 7. Other systems fall back.
    setDefaultSansSerifTypefaceName ("Segoe UI");

    setColour (juce::ResizableWindow::backgroundColourId, simpler::panel());

    setColour (juce::Label::textColourId, simpler::text());
    setColour (juce::Label::textWhenEditingColourId, simpler::text());
    setColour (juce::Label::outlineColourId, juce::Colours::transparentBlack);

    setColour (juce::TextEditor::backgroundColourId, simpler::comboFill());
    setColour (juce::TextEditor::textColourId, simpler::text());
    setColour (juce::TextEditor::highlightColourId, simpler::blueSelect());
    setColour (juce::TextEditor::highlightedTextColourId, simpler::text());
    setColour (juce::TextEditor::outlineColourId, simpler::fieldBorder());
    setColour (juce::TextEditor::focusedOutlineColourId, simpler::fieldBorder());
    setColour (juce::CaretComponent::caretColourId, simpler::text());

    setColour (juce::TextButton::buttonColourId, simpler::field());
    setColour (juce::TextButton::buttonOnColourId, simpler::orange());
    setColour (juce::TextButton::textColourOffId, simpler::text());
    setColour (juce::TextButton::textColourOnId, simpler::text());
    setColour (juce::ToggleButton::textColourId, simpler::text());

    setColour (juce::ComboBox::backgroundColourId, simpler::comboFill());
    setColour (juce::ComboBox::textColourId, simpler::text());
    setColour (juce::ComboBox::outlineColourId, simpler::fieldBorder());
    setColour (juce::ComboBox::arrowColourId, simpler::text());

    setColour (juce::PopupMenu::backgroundColourId, simpler::comboFill());
    setColour (juce::PopupMenu::textColourId, simpler::text());
    setColour (juce::PopupMenu::highlightedBackgroundColourId, simpler::blueSelect());
    setColour (juce::PopupMenu::highlightedTextColourId, simpler::text());

    setColour (juce::Slider::textBoxTextColourId, simpler::text());
    setColour (juce::Slider::textBoxBackgroundColourId, juce::Colours::transparentBlack);
    setColour (juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
}

//==============================================================================
// Helpers

juce::Font SampleStackLookAndFeel::uiFont (float height)
{
    return juce::Font (height);
}

void SampleStackLookAndFeel::drawField (juce::Graphics& graphics,
                                        juce::Rectangle<float> bounds,
                                        juce::Colour fill)
{
    // Simpler has no rounded corners: flat fill with a 1px dark border.
    graphics.setColour (fill);
    graphics.fillRect (bounds);
    graphics.setColour (simpler::fieldBorder());
    graphics.drawRect (bounds, 1.0f);
}

juce::Colour SampleStackLookAndFeel::getAccentColour (const juce::Component& component)
{
    const auto& properties = component.getProperties();
    if (properties.contains ("accent"))
        return juce::Colour ((juce::uint32) (juce::int64) properties["accent"]);

    return simpler::orange();
}

void SampleStackLookAndFeel::drawArrowHead (juce::Graphics& graphics, float tipX, float y,
                                            bool pointsRight)
{
    const float direction = pointsRight ? -1.0f : 1.0f;
    juce::Path head;
    head.startNewSubPath (tipX + direction * 3.5f, y - 3.5f);
    head.lineTo (tipX, y);
    head.lineTo (tipX + direction * 3.5f, y + 3.5f);
    graphics.strokePath (head, juce::PathStrokeType (1.4f));
}

void SampleStackLookAndFeel::drawModeIcon (juce::Graphics& graphics, const juce::String& icon,
                                           juce::Rectangle<float> bounds, juce::Colour ink)
{
    graphics.setColour (ink);
    const float left = bounds.getX();
    const float right = bounds.getRight();
    const float centreY = bounds.getCentreY();

    if (icon == "oneShot")                       // arrow into a bar
    {
        const float bar = right - 1.0f;
        graphics.drawLine (left, centreY, bar - 1.0f, centreY, 1.4f);
        drawArrowHead (graphics, bar - 1.0f, centreY, true);
        graphics.drawLine (bar, centreY - 5.0f, bar, centreY + 5.0f, 1.6f);
    }
    else if (icon == "loopFwd")                  // two arrows, same direction
    {
        for (const float offset : { -3.5f, 3.5f })
        {
            graphics.drawLine (left, centreY + offset, right, centreY + offset, 1.4f);
            drawArrowHead (graphics, right, centreY + offset, true);
        }
    }
    else if (icon == "loopAlt")                  // two arrows, opposite directions
    {
        graphics.drawLine (left, centreY - 3.5f, right, centreY - 3.5f, 1.4f);
        drawArrowHead (graphics, right, centreY - 3.5f, true);
        graphics.drawLine (left, centreY + 3.5f, right, centreY + 3.5f, 1.4f);
        drawArrowHead (graphics, left, centreY + 3.5f, false);
    }
}

void SampleStackLookAndFeel::drawTabShape (juce::Graphics& graphics,
                                           juce::Rectangle<float> bounds,
                                           const juce::String& text,
                                           bool isFront, bool isMouseOver, bool isMouseDown,
                                           bool dot, bool dotOn, bool arrow)
{
    auto fill = isFront ? simpler::tabActive() : simpler::tabInactive();
    if (isMouseOver && ! isFront)
        fill = fill.brighter (0.08f);
    if (isMouseDown)
        fill = fill.darker (0.08f);

    juce::Path shape;
    shape.addRoundedRectangle (bounds.getX(), bounds.getY(), bounds.getWidth(), bounds.getHeight(),
                               4.0f, 4.0f, true, true, false, false);
    graphics.setColour (fill);
    graphics.fillPath (shape);

    auto textArea = bounds;
    if (dot)
    {
        auto led = textArea.removeFromLeft (14.0f).withSizeKeepingCentre (6.0f, 6.0f);
        graphics.setColour (dotOn ? simpler::orange() : simpler::panelDark());
        graphics.fillEllipse (led);
    }

    if (arrow)
    {
        auto area = textArea.removeFromRight (14.0f);
        juce::Path triangle;
        const float cx = area.getCentreX();
        const float cy = area.getCentreY();
        triangle.addTriangle (cx - 4.0f, cy, cx + 3.0f, cy - 4.0f, cx + 3.0f, cy + 4.0f);
        graphics.setColour (simpler::text());
        graphics.fillPath (triangle);
    }

    graphics.setColour (simpler::text());
    graphics.setFont (uiFont (13.0f));
    graphics.drawFittedText (text, textArea.toNearestInt(), juce::Justification::centred, 1);
}

//==============================================================================
// Buttons

juce::Font SampleStackLookAndFeel::getTextButtonFont (juce::TextButton&, int)
{
    return uiFont();
}

void SampleStackLookAndFeel::drawButtonBackground (juce::Graphics& graphics,
                                                   juce::Button& button,
                                                   const juce::Colour&,
                                                   bool isHighlighted,
                                                   bool isDown)
{
    const auto& properties = button.getProperties();

    if ((bool) properties.getWithDefault ("tab", false))
    {
        // The tab draws its own text, so drawButtonText() skips tab buttons.
        drawTabShape (graphics, button.getLocalBounds().toFloat().reduced (1.0f, 0.0f),
                      button.getButtonText(), button.getToggleState(),
                      isHighlighted, isDown,
                      (bool) properties.getWithDefault ("dot", false),
                      (bool) properties.getWithDefault ("dotOn", false),
                      (bool) properties.getWithDefault ("arrow", false));
        return;
    }

    auto fill = button.getToggleState() ? getAccentColour (button) : simpler::field();
    if (! button.isEnabled())
        fill = fill.withMultipliedSaturation (0.4f).withAlpha (0.6f);
    if (isHighlighted)
        fill = fill.brighter (0.06f);
    if (isDown)
        fill = fill.darker (0.12f);

    drawField (graphics, button.getLocalBounds().toFloat().reduced (0.5f), fill);
}

void SampleStackLookAndFeel::drawButtonText (juce::Graphics& graphics,
                                             juce::TextButton& button,
                                             bool isHighlighted,
                                             bool isDown)
{
    juce::ignoreUnused (isHighlighted, isDown);

    if ((bool) button.getProperties().getWithDefault ("tab", false))
        return;

    graphics.setFont (getTextButtonFont (button, button.getHeight()));
    graphics.setColour (simpler::text().withAlpha (button.isEnabled() ? 1.0f : 0.45f));
    graphics.drawFittedText (button.getButtonText(), button.getLocalBounds().reduced (3, 0),
                             juce::Justification::centred, 1);
}

void SampleStackLookAndFeel::drawToggleButton (juce::Graphics& graphics,
                                               juce::ToggleButton& button,
                                               bool isHighlighted,
                                               bool isDown)
{
    drawButtonBackground (graphics, button, {}, isHighlighted, isDown);

    const auto icon = button.getProperties().getWithDefault ("icon", "").toString();
    const float alpha = button.isEnabled() ? 1.0f : 0.4f;

    if (icon.isNotEmpty())
    {
        drawModeIcon (graphics, icon, button.getLocalBounds().toFloat().reduced (6.0f, 5.0f),
                      simpler::text().withAlpha (alpha));
        return;
    }

    graphics.setColour ((button.getToggleState() ? simpler::text() : simpler::textDim())
                            .withAlpha (alpha));
    graphics.setFont (uiFont (12.0f));
    graphics.drawFittedText (button.getButtonText(), button.getLocalBounds(),
                             juce::Justification::centred, 1);
}

//==============================================================================
// Labels and text entry

juce::Font SampleStackLookAndFeel::getLabelFont (juce::Label&)
{
    return uiFont();
}

void SampleStackLookAndFeel::drawLabel (juce::Graphics& graphics, juce::Label& label)
{
    const auto bounds = label.getLocalBounds();
    const bool insideSlider = dynamic_cast<juce::Slider*> (label.getParentComponent()) != nullptr;
    const bool asField = (bool) label.getProperties().getWithDefault ("field", false);

    if (asField)
        drawField (graphics, bounds.toFloat().reduced (0.5f), simpler::field());

    if (label.isBeingEdited())
        return;

    graphics.setFont (getLabelFont (label));
    graphics.setColour (label.findColour (juce::Label::textColourId)
                            .withMultipliedAlpha (label.isEnabled() ? 1.0f : 0.5f));

    const auto justification = (insideSlider || asField) ? juce::Justification::centred
                                                         : label.getJustificationType();
    graphics.drawFittedText (label.getText(), bounds.reduced (2, 0), justification, 1, 1.0f);
}

void SampleStackLookAndFeel::fillTextEditorBackground (juce::Graphics& graphics, int width,
                                                       int height, juce::TextEditor& editor)
{
    juce::ignoreUnused (width, height);
    graphics.fillAll (editor.findColour (juce::TextEditor::backgroundColourId));
}

void SampleStackLookAndFeel::drawTextEditorOutline (juce::Graphics& graphics, int width,
                                                    int height, juce::TextEditor& editor)
{
    graphics.setColour (editor.findColour (juce::TextEditor::outlineColourId));
    graphics.drawRect (0, 0, width, height, 1);
}

//==============================================================================
// Sliders

void SampleStackLookAndFeel::drawLinearSlider (juce::Graphics& graphics,
                                               int x, int y, int width, int height,
                                               float sliderPos, float minSliderPos,
                                               float maxSliderPos,
                                               juce::Slider::SliderStyle style,
                                               juce::Slider& slider)
{
    if (style == juce::Slider::LinearBar || style == juce::Slider::LinearBarVertical)
    {
        // Value field ("0.0 dB", "C3", "100 %"). The slider's own text box (shown with
        // setTextBoxStyle (TextBoxLeft, ...)) draws the number through drawLabel().
        const auto bounds = slider.getLocalBounds().toFloat();
        auto fill = simpler::field();
        if (slider.isMouseOverOrDragging())
            fill = fill.brighter (0.06f);
        drawField (graphics, bounds.reduced (0.5f), fill);

        if ((bool) slider.getProperties().getWithDefault ("arrow", false))
        {
            const float cy = bounds.getCentreY();
            const float rx = bounds.getRight() - 4.0f;
            juce::Path triangle;
            triangle.addTriangle (rx - 6.0f, cy, rx, cy - 4.5f, rx, cy + 4.5f);   // points left
            graphics.setColour (simpler::panelDark());
            graphics.fillPath (triangle);
        }
        return;
    }

    if (style == juce::Slider::LinearVertical)
    {
        // Thin level line with a hollow triangle thumb (Simpler's Vol slider).
        const float cx = (float) x + (float) width * 0.5f;
        const float top = (float) y;
        const float bottom = (float) (y + height);

        graphics.setColour (simpler::text());
        graphics.drawLine (cx, top, cx, sliderPos, 2.0f);
        graphics.setColour (simpler::panelDark());
        graphics.drawLine (cx, sliderPos, cx, bottom, 1.0f);

        juce::Path thumb;
        thumb.addTriangle (cx - 7.0f, sliderPos, cx + 7.0f, sliderPos - 6.0f,
                           cx + 7.0f, sliderPos + 6.0f);
        graphics.setColour (simpler::panelDark());
        graphics.strokePath (thumb, juce::PathStrokeType (1.2f));
        return;
    }

    LookAndFeel_V4::drawLinearSlider (graphics, x, y, width, height, sliderPos,
                                      minSliderPos, maxSliderPos, style, slider);
}

void SampleStackLookAndFeel::drawRotarySlider (juce::Graphics& graphics,
                                               int x, int y, int width, int height,
                                               float sliderPosProportional,
                                               float rotaryStartAngle,
                                               float rotaryEndAngle,
                                               juce::Slider& slider)
{
    juce::ignoreUnused (slider);

    const float diameter = (float) juce::jmin (width, height) * 0.72f;
    const float centreX = (float) x + (float) width * 0.5f;
    const float centreY = (float) y + (float) height * 0.44f;
    const float radius = diameter * 0.5f;
    const float angle = rotaryStartAngle
        + sliderPosProportional * (rotaryEndAngle - rotaryStartAngle);

    graphics.setColour (SampleStackTheme::panelRaised());
    graphics.fillEllipse (centreX - radius, centreY - radius, diameter, diameter);

    graphics.setColour (SampleStackTheme::border());
    graphics.drawEllipse (centreX - radius, centreY - radius, diameter, diameter, 1.0f);

    juce::Path track;
    track.addCentredArc (centreX, centreY, radius * 0.78f, radius * 0.78f,
                         0.0f, rotaryStartAngle, rotaryEndAngle, true);
    graphics.setColour (SampleStackTheme::border());
    graphics.strokePath (track, juce::PathStrokeType (3.0f));

    juce::Path valueArc;
    valueArc.addCentredArc (centreX, centreY, radius * 0.78f, radius * 0.78f,
                            0.0f, rotaryStartAngle, angle, true);
    graphics.setColour (SampleStackTheme::preSample());
    graphics.strokePath (valueArc, juce::PathStrokeType (3.0f));

    juce::Path pointer;
    pointer.addRoundedRectangle (-1.5f, -radius * 0.62f, 3.0f, radius * 0.28f, 1.2f);
    graphics.setColour (SampleStackTheme::text());
    graphics.fillPath (pointer, juce::AffineTransform::rotation (angle).translated (centreX, centreY));
}

//==============================================================================
// Combo box

juce::Font SampleStackLookAndFeel::getComboBoxFont (juce::ComboBox&)
{
    return uiFont();
}

void SampleStackLookAndFeel::drawComboBox (juce::Graphics& graphics, int width, int height,
                                           bool isButtonDown, int, int, int, int,
                                           juce::ComboBox& box)
{
    const auto bounds = juce::Rectangle<float> (0.0f, 0.0f, (float) width, (float) height)
                            .reduced (0.5f);
    auto fill = box.findColour (juce::ComboBox::backgroundColourId);
    if (isButtonDown)
        fill = fill.darker (0.1f);
    drawField (graphics, bounds, fill);

    juce::Path arrow;
    const float cx = bounds.getRight() - 9.0f;
    const float cy = bounds.getCentreY();
    arrow.addTriangle (cx - 4.5f, cy - 2.5f, cx + 4.5f, cy - 2.5f, cx, cy + 3.5f);
    graphics.setColour (box.findColour (juce::ComboBox::arrowColourId)
                            .withAlpha (box.isEnabled() ? 1.0f : 0.4f));
    graphics.fillPath (arrow);
}

void SampleStackLookAndFeel::positionComboBoxText (juce::ComboBox& box, juce::Label& label)
{
    label.setBounds (3, 1, box.getWidth() - 18, box.getHeight() - 2);
    label.setFont (getComboBoxFont (box));
    label.setJustificationType (juce::Justification::centredLeft);
}

//==============================================================================
// Tabs

int SampleStackLookAndFeel::getTabButtonBestWidth (juce::TabBarButton& button, int)
{
    int width = uiFont (13.0f).getStringWidth (button.getButtonText()) + 28;
    if ((bool) button.getProperties().getWithDefault ("dot", false))
        width += 12;
    if ((bool) button.getProperties().getWithDefault ("arrow", false))
        width += 12;
    return width;
}

void SampleStackLookAndFeel::drawTabAreaBehindFrontButton (juce::TabbedButtonBar&,
                                                           juce::Graphics&, int, int)
{
}

void SampleStackLookAndFeel::drawTabButton (juce::TabBarButton& button, juce::Graphics& graphics,
                                            bool isMouseOver, bool isMouseDown)
{
    const auto& properties = button.getProperties();
    drawTabShape (graphics, button.getLocalBounds().toFloat().reduced (1.0f, 0.0f),
                  button.getButtonText(), button.isFrontTab(), isMouseOver, isMouseDown,
                  (bool) properties.getWithDefault ("dot", false),
                  (bool) properties.getWithDefault ("dotOn", false),
                  (bool) properties.getWithDefault ("arrow", false));
}
