#include "PlayerGUI.h"

PlayerGUI::PlayerGUI()
{
    for (auto* btn : { &loadButton, &restartButton, &stopButton, &loopButton, &muteButton ,&pauseButton ,&gotostartButton ,&gotoendButton })
    {
        addAndMakeVisible(btn);
        btn->addListener(this);
    }

    
    volumeSlider.setRange(0.0, 1.0,0.01);
    volumeSlider.setValue(0.5);
    volumeSlider.addListener(this);
    addAndMakeVisible(volumeSlider);
    

   
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
    muteButton.setBounds(380, 20, 100, 40);
    pauseButton.setBounds(20, 80, 100, 40);
    gotostartButton.setBounds(140, 80, 100, 40);
    gotoendButton.setBounds(260, 80, 100, 40);
    loopButton.setBounds(380, 80, 100, 40);


    volumeSlider.setBounds(20, 140, getWidth() - 40, 30);
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
    if (slider == &volumeSlider)
        playerAudio.setGain((float)slider->getValue());
}

void PlayerGUI::timerCallback()
{
}
