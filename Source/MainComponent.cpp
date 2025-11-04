#include "MainComponent.h"

MainComponent::MainComponent()
{



    addAndMakeVisible(player1);
    addAndMakeVisible(player2);
    setSize(1200, 600);
    setAudioChannels(0, 2);
}

MainComponent::~MainComponent()
{
    shutdownAudio();
}

void MainComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    player1.prepareToPlay(samplesPerBlockExpected, sampleRate);
    player2.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    bufferToFill.buffer->clear();

    int activePlayers = 0;


    if (player1.hasFileLoaded() && player1.isPlaying())
    {
        player1.getNextAudioBlock(bufferToFill);
        activePlayers++;
    }


    if (player2.hasFileLoaded() && player2.isPlaying())
    {

        if (activePlayers > 0)
        {
            juce::AudioBuffer<float> tempBuffer(bufferToFill.buffer->getNumChannels(), bufferToFill.numSamples);
            juce::AudioSourceChannelInfo tempInfo(&tempBuffer, 0, bufferToFill.numSamples);

            player2.getNextAudioBlock(tempInfo);

            for (int channel = 0; channel < bufferToFill.buffer->getNumChannels(); ++channel)
            {
                bufferToFill.buffer->addFrom(channel, bufferToFill.startSample,
                    tempBuffer, channel, 0, bufferToFill.numSamples, 1.0f);
            }
        }
        else
        {

            player2.getNextAudioBlock(bufferToFill);
        }

        activePlayers++;
    }


    if (activePlayers == 2)
    {
        bufferToFill.buffer->applyGain(0.7f);
    }
}

void MainComponent::releaseResources()
{
    player1.releaseResources();
    player2.releaseResources();
}

void MainComponent::resized()
{
    auto area = getLocalBounds();
    player1.setBounds(area.removeFromLeft(getWidth() / 2));
    player2.setBounds(area);
}
