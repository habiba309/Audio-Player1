#pragma once
#include <JuceHeader.h>
#include <juce_events/juce_events.h>
#include "PlayerAudio.h"

class PlayerGUI : public juce::Component,
    public juce::Button::Listener,
    public juce::Slider::Listener,
    public juce::Timer,
	public juce::ChangeListener
{
public:
    PlayerGUI();
    ~PlayerGUI() override;

    void resized() override;
    void paint(juce::Graphics& g) override;
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate);
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill);
    void releaseResources();
	void changeListenerCallback(juce::ChangeBroadcaster* source) {}
    bool hasFileLoaded() const { return playerAudio.hasFileLoaded(); }
    bool isPlaying() const { return playerAudio.isPlaying(); }
    PlayerAudio* getPlayerAudio() { return &playerAudio; }
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
    juce::TextButton aButton{ "Set A" };
    juce::TextButton bButton{ "Set B" };
    juce::TextButton abLoopButton{ "A-B Loop OFF" };
    double abLoopStart = 0.0;
    double abLoopEnd = 0.0;
    bool isABLooping = false;
    bool isAPointSet = false;
    bool isBPointSet = false;
    juce::TextButton clearPointsButton{ "Clear Points" };
    bool isLooping = false;
    bool isMuted = false;
    float previousVolume = 1.0f;

    juce::Slider volumeSlider;
    juce::Slider positionSlider;
    juce::Slider speedSlider;
    
    juce::Label currentTimeLabel;
    juce::Label speedLabel;
    juce::Label metadataLable;
    juce::AudioFormatManager formatmanager;
    juce::Label playlistTitle{ "playlistTitle", "Playlist - Available Songs:" };
    std::vector < std::unique_ptr<juce::TextButton>>playButtons;
    std::vector<juce::File>loadedFiles;
    void updatePositionDisplay();
    std::unique_ptr<juce::FileChooser> fileChooser;

    // ===== Waveform Display =====
    juce::AudioThumbnailCache thumbnailCache{ 5 };
    juce::AudioThumbnail thumbnail{ 512, formatmanager, thumbnailCache };
    bool isWaveformLoaded = false;
    double currentPosition = 0.0;
    double totalDuration = 0.0;

    // Event handlers
    void buttonClicked(juce::Button* button) override;
    void sliderValueChanged(juce::Slider* slider) override;
    void timerCallback() override;
    void playfile(int index);
    juce::String formatTime(double seconds);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerGUI)
};
