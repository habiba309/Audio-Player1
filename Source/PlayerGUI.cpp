

#include "PlayerGUI.h"

PlayerGUI::PlayerGUI()
{// Add buttons
    for (auto* btn : { &loadButton, &restartButton , &stopButton ,&loopButton })
    {
        btn->addListener(this);
        addAndMakeVisible(btn);
    }

    // Volume slider
    volumeSlider.setRange(0.0, 1.0, 0.01);
    volumeSlider.setValue(0.5);
    volumeSlider.addListener(this);
    addAndMakeVisible(volumeSlider);
    // Position slider 
    positionSlider.setRange(0.0, 1.0, 0.001);
    positionSlider.addListener(this);
    positionSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    
    addAndMakeVisible(positionSlider);

    // Duration label
    currentTimeLabel.setText("0:00 / 0:00", juce::dontSendNotification);
    currentTimeLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(currentTimeLabel);
    

    startTimer(100);
}
PlayerGUI::~PlayerGUI()
{
    stopTimer(); 
}
void PlayerGUI::resized()
{
    loadButton.setBounds(20, 20, 100, 40);
    restartButton.setBounds(140, 20, 100, 40);
    stopButton.setBounds(260, 20, 100, 40);
    loopButton.setBounds(380, 20, 100, 40);
    currentTimeLabel.setBounds(20, 175, getWidth() - 40, 20);
    volumeSlider.setBounds(20, 140, getWidth() - 40, 30);
    positionSlider.setBounds(20, 200, getWidth() - 40, 30);
    durationLabel.setBounds(20, 215, getWidth() - 40, 20);
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
void PlayerGUI::paint(juce::Graphics& g)
{
	g.fillAll(juce::Colours::darkgrey); //color background
}
void PlayerGUI::updatePositionDisplay()
{
    double length = playerAudio.getLength();
    double position = playerAudio.getPosition();

    if (length > 0.0)
    {
        positionSlider.setValue(position / length, juce::dontSendNotification);

        
        juce::String timeText = formatTime(position) + " / " + formatTime(length);
        currentTimeLabel.setText(timeText, juce::dontSendNotification);
    }

    durationLabel.setText("Duration: " + formatTime(length), juce::dontSendNotification);
}
void PlayerGUI::timerCallback()
{
    updatePositionDisplay(); 
}
juce::String PlayerGUI::formatTime(double seconds)
{
    if (seconds < 0.0) seconds = 0.0;

    int minutes = static_cast<int>(seconds) / 60;
    int secs = static_cast<int>(seconds) % 60;
    return juce::String::formatted("%02d:%02d", minutes, secs); 
}
void PlayerGUI::buttonClicked(juce::Button* button)
{
    if (button == &loadButton)
    {
        juce::FileChooser chooser("Select audio files...",
            juce::File{},
            "*.wav;*.mp3");

        fileChooser = std::make_unique<juce::FileChooser>(
            "Select an audio file...",
            juce::File{},
            "*.wav;*.mp3");

        fileChooser->launchAsync(
            juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles,
            [this](const juce::FileChooser& fc)
            {
                auto file = fc.getResult();
                if (file.existsAsFile())
                {
					playerAudio.loadFile(file);
                    updatePositionDisplay();
                }
            });
    }

    if (button == &restartButton)
    {
        playerAudio.play();
    }

    if (button == &stopButton)
    {
        playerAudio.stop();
        playerAudio.setPosition(0.0); 
        updatePositionDisplay();
    }
    
   
    if (button == &loopButton)
    {
        isLooping = !isLooping;
        playerAudio.setLooping(isLooping);
        if (isLooping)
            loopButton.setButtonText("Loop on");
        else
            loopButton.setButtonText("Loop off");
	}

}
void PlayerGUI::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &volumeSlider)
        playerAudio.setGain((float)slider->getValue());
    else if (slider == &positionSlider)
    {
        double length = playerAudio.getLength();
        if (length > 0.0)
        {
            double newPosition = slider->getValue() * length;
            playerAudio.setPosition(newPosition);
        }
    }
}
