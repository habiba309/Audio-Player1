#include "PlayerGUI.h"

PlayerGUI::PlayerGUI()
{
    for (auto* btn : { &loadButton, &playButton, &restartButton, &stopButton, &loopButton, &muteButton })
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
    int x = 20;
    int y = 20;
    int w = 90;
    int h = 40;
    int space = 10;

    loadButton.setBounds(x, y, w, h); x += w + space;
    playButton.setBounds(x, y, w, h); x += w + space;
    restartButton.setBounds(x, y, w, h); x += w + space;
    stopButton.setBounds(x, y, w, h); x += w + space;
    loopButton.setBounds(x, y, w, h); x += w + space;
    muteButton.setBounds(x, y, w, h);

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
    else if (button == &playButton)
    {
        playerAudio.play();
    }
    else if (button == &restartButton)
    {
        playerAudio.setPosition(0.0);
    }
    else if (button == &stopButton)
    {
        playerAudio.stop();
    }
    else if (button == &loopButton)
    {
        isLooping = !isLooping;
        playerAudio.setLooping(isLooping);
        loopButton.setButtonText(isLooping ? "Unloop" : "Loop");
    }
    else if (button == &muteButton)
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
