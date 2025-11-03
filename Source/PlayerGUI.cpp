#include "PlayerGUI.h"

PlayerGUI::PlayerGUI()
{
    for (auto* btn : { &loadButton, &restartButton, &stopButton, &loopButton, &muteButton ,&pauseButton ,&gotostartButton ,&gotoendButton })
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
    auto area = getLocalBounds().reduced(10);

    // تقسيم الواجهة لأجزاء
    auto buttonRow = area.removeFromTop(40);
    auto volumeRow = area.removeFromTop(40);
    auto positionRow = area.removeFromTop(40);
    auto timeRow = area.removeFromTop(30);

    // ترتيب الأزرار في صف واحد
    int buttonWidth = buttonRow.getWidth() / 7;
    loadButton.setBounds(buttonRow.removeFromLeft(buttonWidth).reduced(2));
    restartButton.setBounds(buttonRow.removeFromLeft(buttonWidth).reduced(2));
    stopButton.setBounds(buttonRow.removeFromLeft(buttonWidth).reduced(2));
    pauseButton.setBounds(buttonRow.removeFromLeft(buttonWidth).reduced(2));
    gotostartButton.setBounds(buttonRow.removeFromLeft(buttonWidth).reduced(2));
    gotoendButton.setBounds(buttonRow.removeFromLeft(buttonWidth).reduced(2));
    muteButton.setBounds(buttonRow.removeFromLeft(buttonWidth).reduced(2));

    // سطر السلايدر بتاع الصوت
    volumeSlider.setBounds(volumeRow.reduced(10, 5));

    // سطر السلايدر بتاع البوزيشن
    positionSlider.setBounds(positionRow.reduced(10, 5));

    // سطر الوقت (يمين ويسار)
    currentTimeLabel.setBounds(timeRow.removeFromLeft(60));
    durationLabel.setBounds(timeRow.removeFromRight(60));
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
    if (button == &pauseButton)
    {
        if (playerAudio.isPlaying())
        {
            playerAudio.stop();
            pauseButton.setButtonText("Play");
        }
        else
        {
            playerAudio.play();
            pauseButton.setButtonText("Pause");
        }

    }
    if (button == &gotostartButton)
    {
        playerAudio.setPosition(0.0);
    }
    if (button == &gotoendButton)
    {
        playerAudio.setPosition(playerAudio.getLength());
    }

    // Mute / Unmute
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

}

void PlayerGUI::timerCallback()
{
}
