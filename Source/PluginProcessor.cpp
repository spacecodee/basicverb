#include "PluginProcessor.h"

#include "PluginEditor.h"

//==============================================================================
AudioPluginAudioProcessor::AudioPluginAudioProcessor()
    : AudioProcessor(
          BusesProperties()
#if !JucePlugin_IsMidiEffect
#if !JucePlugin_IsSynth
          .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
          .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
      ),
      parameters(*this, nullptr, "Parameters", createParameterLayout())
{
    // Constructor is now empty or can include other initialization code
}

AudioPluginAudioProcessor::~AudioPluginAudioProcessor()
{
}

//==============================================================================
const juce::String AudioPluginAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool AudioPluginAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
  return true;
#else
    return false;
#endif
}

bool AudioPluginAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
  return true;
#else
    return false;
#endif
}

bool AudioPluginAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
  return true;
#else
    return false;
#endif
}

double AudioPluginAudioProcessor::getTailLengthSeconds() const { return 0.0; }

int AudioPluginAudioProcessor::getNumPrograms()
{
    return 1; // NB: some hosts don't cope very well if you tell them there are 0
    // programs,
    // so this should be at least 1, even if you're not really implementing
    // programs.
}

int AudioPluginAudioProcessor::getCurrentProgram() { return 0; }

void AudioPluginAudioProcessor::setCurrentProgram(int index)
{
    juce::ignoreUnused(index);
}

const juce::String AudioPluginAudioProcessor::getProgramName(int index)
{
    juce::ignoreUnused(index);
    return {};
}

void AudioPluginAudioProcessor::changeProgramName(int index,
                                                  const juce::String& newName)
{
    ignoreUnused(index, newName);
}

//==============================================================================
void AudioPluginAudioProcessor::prepareToPlay(double sampleRate,
                                              int samplesPerBlock)
{
    // Set up the reverb
    reverb.setSampleRate(sampleRate);

    // Initialize reverb parameters
    updateReverbParameters();

    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    juce::ignoreUnused(sampleRate, samplesPerBlock);
}

// Add this helper method to your class
void AudioPluginAudioProcessor::updateReverbParameters()
{
    reverbParams.roomSize = getRoomSize();
    reverbParams.damping = getDamping();
    reverbParams.wetLevel = getWetLevel();
    reverbParams.dryLevel = getDryLevel();
    reverbParams.width = getWidth();
    reverbParams.freezeMode = getFreezeMode();

    reverb.setParameters(reverbParams);
}

void AudioPluginAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

bool AudioPluginAudioProcessor::isBusesLayoutSupported(
    const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
  juce::ignoreUnused(layouts);
  return true;
#else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono() &&
        layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
#if !JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
#endif

    return true;
#endif
}

void AudioPluginAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer,
                                             juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    const auto totalNumInputChannels = getTotalNumInputChannels();
    const auto totalNumOutputChannels = getTotalNumOutputChannels();

    // Clear any unused output channels
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    // Update reverb parameters (in case they've been changed by automation)
    updateReverbParameters();

    // Apply reverb processing
    if (totalNumInputChannels == 1)
    {
        // Mono input
        reverb.processMono(buffer.getWritePointer(0), buffer.getNumSamples());
    }
    else if (totalNumInputChannels >= 2)
    {
        // Stereo input (or more)
        reverb.processStereo(buffer.getWritePointer(0), buffer.getWritePointer(1),
                             buffer.getNumSamples());
    }
}

//==============================================================================
bool AudioPluginAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* AudioPluginAudioProcessor::createEditor()
{
    return new AudioPluginAudioProcessorEditor(*this);
}

//==============================================================================
void AudioPluginAudioProcessor::getStateInformation(
    juce::MemoryBlock& destData)
{
    const auto state = parameters.copyState();
    const std::unique_ptr xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void AudioPluginAudioProcessor::setStateInformation(const void* data,
                                                    const int sizeInBytes)
{
    const std::unique_ptr xmlState(
        getXmlFromBinary(data, sizeInBytes));

    if (xmlState.get() != nullptr)
        parameters.replaceState(juce::ValueTree::fromXml(*xmlState));

    updateReverbParameters();
}

juce::AudioProcessorValueTreeState::ParameterLayout
AudioPluginAudioProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "roomSize", "Room Size", 0.0f, 1.0f, 0.5f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("damping", "Damping",
                                                           0.0f, 1.0f, 0.5f));
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "wetLevel", "Wet Level", 0.0f, 1.0f, 0.33f));
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "dryLevel", "Dry Level", 0.0f, 1.0f, 0.4f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("width", "Width", 0.0f,
                                                           1.0f, 1.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "freezeMode", "Freeze Mode", 0.0f, 1.0f, 0.0f));

    return layout;
}

// Add this at the end of the file, after all the class implementations:

//==============================================================================
// This creates new instances of the plugin.
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AudioPluginAudioProcessor();
}
