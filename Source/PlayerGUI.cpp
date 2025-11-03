#include "PlayerGUI.h"

PlayerGUI::PlayerGUI()
{
    // ===== Add Buttons =====
    formatmanager.registerBasicFormats();
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
    metadataLable.setJustificationType(juce::Justification::left);
    metadataLable.setInterceptsMouseClicks(false, false);
    metadataLable.setColour(juce::Label::textColourId, juce::Colours::whitesmoke);
    addAndMakeVisible(metadataLable);
    playlistTitle.setJustificationType(juce::Justification::centredLeft);
    playlistTitle.setInterceptsMouseClicks(false, false);
    playlistTitle.setColour(juce::Label::textColourId, juce::Colours::whitesmoke);
    playlistTitle.setFont(juce::Font(14.0f, juce::Font::bold));
    addAndMakeVisible(playlistTitle);

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
    metadataLable.setBounds(20, 270, getWidth() - 40, 40);
    int startx = 20;
    int starty = metadataLable.getBottom() + 10;
    playlistTitle.setBounds(startx, starty, getWidth() - 40, 25);
    int ButtonHeight = 25;
    int Buttonstarty = playlistTitle.getBottom() + 5;
    int space = 3;
    int maxButtonWidth = 250;
    for (size_t i = 0; i < playButtons.size(); ++i)
    {
        if (playButtons[i] != nullptr)
        {
            juce::String buttonText = playButtons[i]->getButtonText();
            int requirWidth = buttonText.length() * 9 + 40;
            if (requirWidth > maxButtonWidth)
            {
                maxButtonWidth = requirWidth;
            }
            int y = Buttonstarty + (i * (ButtonHeight + space));
            playButtons[i]->setBounds(startx, y, maxButtonWidth, ButtonHeight);
        }
    }
    maxButtonWidth = juce::jmin(maxButtonWidth, getWidth() - 40);
    for (size_t i = 0; i < playButtons.size(); ++i)
    {
        if (playButtons[i] != nullptr)
        {
            int y = Buttonstarty + (i * (ButtonHeight + space));
            playButtons[i]->setBounds(startx, y, maxButtonWidth, ButtonHeight);
        }

    }
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

void PlayerGUI::playfile(int index)
{
    if (index >= 0 && index < loadedFiles.size())
    {
        playerAudio.loadFile(loadedFiles[index]);
        std::unique_ptr<juce::AudioFormatReader> reader(formatmanager.createReaderFor(loadedFiles[index]));
        if (reader != nullptr)
        {
            double duration = reader->sampleRate > 0 ? reader->lengthInSamples / reader->sampleRate : 0.0;
            juce::StringPairArray metadata = reader->metadataValues;
            juce::String artist = metadata["artist"].trim();
            juce::String title = metadata["title"].trim();
            juce::String album = metadata["album"].trim();
            juce::String comment = metadata["comment"].trim();

            juce::StringArray details;
            if (artist.isNotEmpty()) details.add("Artist->" + artist);
            if (title.isNotEmpty()) details.add("Title->" + title);
            if (album.isNotEmpty()) details.add("Album->" + album);
            if (comment.isNotEmpty()) details.add("Comment->" + comment);
            if (details.isEmpty())
            {
                details.add("File name->" + loadedFiles[index].getFileNameWithoutExtension());
            }

            int min, sec;
            min = static_cast<int>(duration) / 60;
            sec = static_cast<int>(duration) % 60;
            juce::String durationText = "Duration->" + juce::String(min) + ":" + juce::String(sec).paddedLeft('0', 2);
            details.add(durationText);

            metadataLable.setText(details.joinIntoString("\n"), juce::dontSendNotification);
        }
    }
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
        juce::FileChooser chooser("Select audio files...",
            juce::File{},
            "*.wav;*.mp3");

        fileChooser = std::make_unique<juce::FileChooser>(
            "Select an audio file...",
            juce::File{},
            "*.wav;*.mp3");

        fileChooser->launchAsync(
            juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectMultipleItems,
            [this](const juce::FileChooser& fc)
            {
                auto res = fc.getResults();
                for (auto& file : res)
                {
                    if (file.existsAsFile())
                    {
                        loadedFiles.push_back(file);
                        auto playbutton = std::make_unique<juce::TextButton>("Play-> " + file.getFileName());
                        playbutton->addListener(this);
                        addAndMakeVisible(playbutton.get());
                        playButtons.push_back(std::move(playbutton));
                    }
                }
                resized();

            });
    }
    for (size_t i = 0; i < playButtons.size(); ++i)
    {
        if (button == playButtons[i].get())
        {
            playfile(static_cast<int>(i));
            return;
        }
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
