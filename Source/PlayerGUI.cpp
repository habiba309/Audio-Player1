#include "PlayerGUI.h"

PlayerGUI::PlayerGUI()
{
    // ===== Add Buttons =====
    for (auto* btn : { &loadButton, &restartButton, &stopButton, &loopButton,
                       &muteButton, &pauseButton, &gotostartButton, &gotoendButton })
    {
        addAndMakeVisible(btn);
        btn->addListener(this);
    }

    // ===== Volume Slider =====
    addAndMakeVisible(volumeSlider);
    volumeSlider.setRange(0.0, 1.0, 0.01);
    volumeSlider.setValue(0.5);
    volumeSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    volumeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20); // 👈 الرقم تحت السلايدر
    volumeSlider.setTextBoxIsEditable(true); // 👈 تقدري تكتبي الرقم
    volumeSlider.addListener(this);

    // ===== Speed Slider =====
    addAndMakeVisible(speedSlider);
    speedSlider.setRange(0.5, 2.0, 0.01);   // نصف السرعة إلى ضعفها
    speedSlider.setValue(1.0);              // السرعة العادية
    speedSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    speedSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20); // 👈 الرقم تحت
    speedSlider.setTextBoxIsEditable(true); // 👈 تقدري تكتبي الرقم
    speedSlider.addListener(this);

    // ===== Label فوق سلايدر السرعة =====
    addAndMakeVisible(speedLabel);
    speedLabel.setText("Speed", juce::dontSendNotification);
    speedLabel.setJustificationType(juce::Justification::centred);

    // ===== Timer =====
    startTimerHz(10); // لتحديث القيم لو احتجنا
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

    // ===== السلايدرز =====
    volumeSlider.setBounds(20, 140, getWidth() - 40, 50);
    speedLabel.setBounds(20, 195, getWidth() - 40, 20);
    speedSlider.setBounds(20, 215, getWidth() - 40, 50);
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
        fileChooser->launchAsync(
            juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles,
            [this](const juce::FileChooser& chooser)
            {
                auto file = chooser.getResult();
                if (file.existsAsFile())
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
        playerAudio.setPosition(0.0);

    if (button == &gotoendButton)
        playerAudio.setPosition(playerAudio.getLength());

    // ===== Mute / Unmute =====
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
    else if (slider == &speedSlider)
        playerAudio.setSpeed(speedSlider.getValue());
}

void PlayerGUI::timerCallback()
{
    // ممكن بعدين نضيف تحديث position لو احتجنا
}
