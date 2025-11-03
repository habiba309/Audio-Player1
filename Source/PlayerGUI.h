#pragma once
#include <JuceHeader.h>
#include "PlayerAudio.h"

class PlayerGUI : public juce::Component,
    public juce::Button::Listener,
    public juce::Slider::Listener,
    public juce::Timer
{
public:
    PlayerGUI();
    ~PlayerGUI() override;

    void resized() override;
    void paint(juce::Graphics& g) override;
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate);
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill);
    void releaseResources();

private:
    PlayerAudio playerAudio;

    // GUI elements
    juce::TextButton loadButton{ "Load File" };
    juce::TextButton restartButton{ "Restart" };
    juce::TextButton stopButton{ "Stop" };
    juce::TextButton loopButton{ "Loop" };
    juce::TextButton muteButton{ "Mute" };
    juce::TextButton pauseButton{ "Pause" };
    juce::TextButton gotostartButton{ "|< Start" };
    juce::TextButton gotoendButton{ "End >|" };

    bool isLooping = false;
    bool isMuted = false;
    float previousVolume = 1.0f;

    juce::Slider volumeSlider;
    juce::Slider positionSlider;
    juce::Slider speedSlider;
    juce::Label durationLabel;
    juce::Label currentTimeLabel;
    juce::Label speedLabel;

    std::unique_ptr<juce::FileChooser> fileChooser;

    // Event handlers

    void buttonClicked(juce::Button* button) override;
    void sliderValueChanged(juce::Slider* slider) override;
    void timerCallback() override;


    juce::String formatTime(double seconds);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerGUI)
};
