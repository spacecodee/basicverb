#pragma once

#include <juce_audio_processors/juce_audio_processors.h>

//==============================================================================
class AudioPluginAudioProcessor final : public juce::AudioProcessor
{
public:
    //==============================================================================
    AudioPluginAudioProcessor();
    ~AudioPluginAudioProcessor() override;

    //==============================================================================
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;

    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    using AudioProcessor::processBlock;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void changeProgramName(int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

    // Make the AudioProcessorValueTreeState public so the editor can access it
    juce::AudioProcessorValueTreeState parameters;

    // Add these public methods
    // Getter methods for parameter values
    float getRoomSize() const
    {
        return *parameters.getRawParameterValue("roomSize");
    }

    float getDamping() const
    {
        return *parameters.getRawParameterValue("damping");
    }

    float getWetLevel() const
    {
        return *parameters.getRawParameterValue("wetLevel");
    }

    float getDryLevel() const
    {
        return *parameters.getRawParameterValue("dryLevel");
    }

    float getWidth() const { return *parameters.getRawParameterValue("width"); }

    float getFreezeMode() const
    {
        return *parameters.getRawParameterValue("freezeMode");
    }

    // Setter methods for parameter values
    void setRoomSize(const float value)
    {
        parameters.getParameter("roomSize")->setValueNotifyingHost(value);
        updateReverbParameters();
    }

    void setDamping(const float value)
    {
        parameters.getParameter("damping")->setValueNotifyingHost(value);
        updateReverbParameters();
    }

    void setWetLevel(const float value)
    {
        parameters.getParameter("wetLevel")->setValueNotifyingHost(value);
        updateReverbParameters();
    }

    void setDryLevel(const float value)
    {
        parameters.getParameter("dryLevel")->setValueNotifyingHost(value);
        updateReverbParameters();
    }

    void setWidth(const float value)
    {
        parameters.getParameter("width")->setValueNotifyingHost(value);
        updateReverbParameters();
    }

    void setFreezeMode(const float value)
    {
        parameters.getParameter("freezeMode")->setValueNotifyingHost(value);
        updateReverbParameters();
    }

private:
    //==============================================================================
    juce::Reverb reverb;
    juce::Reverb::Parameters reverbParams;

    void updateReverbParameters();
    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioPluginAudioProcessor)
};
