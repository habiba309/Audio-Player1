

#include "PlayerGUI.h"

PlayerGUI::PlayerGUI()
{
    for (auto* btn : { &loadButton, &restartButton, &stopButton, &loopButton, &muteButton })
    {
        addAndMakeVisible(btn);
        btn->addListener(this);
    }

    addAndMakeVisible(volumeSlider);
    volumeSlider.setRange(0.0, 1.0);
    volumeSlider.setValue(1.0);
    volumeSlider.addListener(this);

    addAndMakeVisible(positionSlider);
    positionSlider.setRange(0.0, 1.0);
    positionSlider.addListener(this);

    addAndMakeVisible(durationLabel);
    addAndMakeVisible(currentTimeLabel);

    startTimerHz(10); // update every 100ms
}

PlayerGUI::~PlayerGUI() {}

void PlayerGUI::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::darkgrey);
}

void PlayerGUI::resized()
{
    loadButton.setBounds(20, 20, 100, 40);
    restartButton.setBounds(140, 20, 100, 40);
    stopButton.setBounds(260, 20, 100, 40);
    loopButton.setBounds(380, 20, 100, 40);
    muteButton.setBounds(500, 20, 100, 40); //  زرار Mute

    volumeSlider.setBounds(20, 80, 200, 20);
    positionSlider.setBounds(20, 120, 400, 20);
    currentTimeLabel.setBounds(430, 120, 60, 20);
    durationLabel.setBounds(500, 120, 60, 20);
}

void PlayerGUI::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    playerAudio.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void PlayerGUI::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    playerAudio.getNextAudioBlock(bufferToFill);
}

void PlayerGUI::releaseResources()
{
    playerAudio.releaseResources();
}

void PlayerGUI::updatePositionDisplay()
{
    auto current = playerAudio.getCurrentPosition();
    auto total = playerAudio.getLengthInSeconds();

    positionSlider.setValue(current / total, juce::dontSendNotification);
    currentTimeLabel.setText(formatTime(current), juce::dontSendNotification);
    durationLabel.setText(formatTime(total), juce::dontSendNotification);
}

juce::String PlayerGUI::formatTime(double seconds)
{
    int totalSeconds = static_cast<int>(seconds);
    int minutes = totalSeconds / 60;
    int secs = totalSeconds % 60;
    return juce::String::formatted("%02d:%02d", minutes, secs);
}

void PlayerGUI::buttonClicked(juce::Button* button)
{
    if (button == &loadButton)
    {
        fileChooser = std::make_unique<juce::FileChooser>("Select an audio file...");
        fileChooser->launchAsync(juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles,
            [this](const juce::FileChooser& chooser)
            {
                auto file = chooser.getResult();
                playerAudio.loadFile(file);
            });
    }

    if (button == &restartButton)
        playerAudio.setPosition(0.0);

    if (button == &stopButton)
        playerAudio.stop();

    if (button == &loopButton)
    {
        isLooping = !isLooping;
        playerAudio.setLooping(isLooping);
        loopButton.setButtonText(isLooping ? "Unloop" : "Loop");
    }

    //  Mute / Unmute
    if (button == &muteButton)
    {
        if (!isMuted)
        {
            previousVolume = (float)volumeSlider.getValue(); // حفظ الصوت القديم
            playerAudio.setGain(0.0f);
            muteButton.setButtonText("Unmute");
            isMuted = true;
        }
        else
        {
            playerAudio.setGain(previousVolume); // رجّع الصوت القديم
            muteButton.setButtonText("Mute");
            isMuted = false;
        }
    }
}

void PlayerGUI::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &volumeSlider && !isMuted)
        playerAudio.setGain((float)volumeSlider.getValue());
    else if (slider == &positionSlider)
        playerAudio.setPositionRelative((float)positionSlider.getValue());
}

void PlayerGUI::timerCallback()
{
    updatePositionDisplay();
}

