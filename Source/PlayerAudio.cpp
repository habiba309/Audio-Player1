#include "PlayerAudio.h"

PlayerAudio::PlayerAudio()
{
    formatManager.registerBasicFormats();

   
    resamplingSource = std::make_unique<juce::ResamplingAudioSource>(&transportSource, false, 2);
}

PlayerAudio::~PlayerAudio() {}

void PlayerAudio::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    
    resamplingSource->prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void PlayerAudio::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    resamplingSource->getNextAudioBlock(bufferToFill);
}

void PlayerAudio::releaseResources()
{
    resamplingSource->releaseResources();
}

bool PlayerAudio::loadFile(const juce::File& file)
{
    if (file.existsAsFile())
    {
        if (auto* reader = formatManager.createReaderFor(file))
        {
            transportSource.stop();
            transportSource.setSource(nullptr);
            readerSource.reset();

            readerSource = std::make_unique<juce::AudioFormatReaderSource>(reader, true);

            transportSource.setSource(readerSource.get(), 0, nullptr, reader->sampleRate);
            transportSource.start();
        }
    }
    return true;
}

bool PlayerAudio::isPlaying() const
{
    return transportSource.isPlaying();
}

void PlayerAudio::setLooping(bool shouldLoop)
{
    if (readerSource)
        readerSource->setLooping(shouldLoop);
}

void PlayerAudio::play()
{
    transportSource.start();
}
