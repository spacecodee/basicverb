#include "PluginEditor.h"

#include "PluginProcessor.h"

//==============================================================================
AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor(
    AudioPluginAudioProcessor& p)
    : AudioProcessorEditor(&p), processorRef(p)
{
    // Set up Room Size slider
    roomSizeSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    roomSizeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    addAndMakeVisible(roomSizeSlider);

    roomSizeLabel.setText("Room Size", juce::dontSendNotification);
    roomSizeLabel.attachToComponent(&roomSizeSlider, false);
    roomSizeLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(roomSizeLabel);

    // Set up Damping slider
    dampingSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    dampingSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    addAndMakeVisible(dampingSlider);

    dampingLabel.setText("Damping", juce::dontSendNotification);
    dampingLabel.attachToComponent(&dampingSlider, false);
    dampingLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(dampingLabel);

    // Set up Wet Level slider
    wetLevelSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    wetLevelSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    addAndMakeVisible(wetLevelSlider);

    wetLevelLabel.setText("Wet Level", juce::dontSendNotification);
    wetLevelLabel.attachToComponent(&wetLevelSlider, false);
    wetLevelLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(wetLevelLabel);

    // Set up Dry Level slider
    dryLevelSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    dryLevelSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    addAndMakeVisible(dryLevelSlider);

    dryLevelLabel.setText("Dry Level", juce::dontSendNotification);
    dryLevelLabel.attachToComponent(&dryLevelSlider, false);
    dryLevelLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(dryLevelLabel);

    // Set up Width slider
    widthSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    widthSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    addAndMakeVisible(widthSlider);

    widthLabel.setText("Width", juce::dontSendNotification);
    widthLabel.attachToComponent(&widthSlider, false);
    widthLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(widthLabel);

    // Set up Freeze Mode slider
    freezeModeSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    freezeModeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    addAndMakeVisible(freezeModeSlider);

    freezeModeLabel.setText("Freeze Mode", juce::dontSendNotification);
    freezeModeLabel.attachToComponent(&freezeModeSlider, false);
    freezeModeLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(freezeModeLabel);

    // Connect slider values to audio parameters using attachments
    sliderAttachments.push_back(
        std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
            processorRef.parameters, "roomSize", roomSizeSlider));
    sliderAttachments.push_back(
        std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
            processorRef.parameters, "damping", dampingSlider));
    sliderAttachments.push_back(
        std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
            processorRef.parameters, "wetLevel", wetLevelSlider));
    sliderAttachments.push_back(
        std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
            processorRef.parameters, "dryLevel", dryLevelSlider));
    sliderAttachments.push_back(
        std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
            processorRef.parameters, "width", widthSlider));
    sliderAttachments.push_back(
        std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
            processorRef.parameters, "freezeMode", freezeModeSlider));

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize(600, 300);
}

AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor() = default;

//==============================================================================
void AudioPluginAudioProcessorEditor::paint(juce::Graphics& g)
{
    // Fill the background
    g.fillAll(
        getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

    // Add a title
    g.setColour(juce::Colours::white);
    g.setFont(20.0f);
    g.drawText("Basic Reverb", getLocalBounds().withHeight(30),
               juce::Justification::centred, true);
}

void AudioPluginAudioProcessorEditor::resized()
{
    // Set up layout for the sliders
    constexpr int numSliders = 6;
    const int sliderWidth = getWidth() / numSliders;
    constexpr int sliderHeight = 100;
    constexpr int yPos = 80;

    roomSizeSlider.setBounds(0 * sliderWidth, yPos, sliderWidth, sliderHeight);
    dampingSlider.setBounds(1 * sliderWidth, yPos, sliderWidth, sliderHeight);
    wetLevelSlider.setBounds(2 * sliderWidth, yPos, sliderWidth, sliderHeight);
    dryLevelSlider.setBounds(3 * sliderWidth, yPos, sliderWidth, sliderHeight);
    widthSlider.setBounds(4 * sliderWidth, yPos, sliderWidth, sliderHeight);
    freezeModeSlider.setBounds(5 * sliderWidth, yPos, sliderWidth, sliderHeight);
}
