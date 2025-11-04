#pragma once // PlayerAudio.h
#include <JuceHeader.h>

class PlayerAudio
{
public:
	PlayerAudio();
	~PlayerAudio();
	void prepareToPlay(int samplesPerBlockExpected, double sampleRate);
	void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill);
	void releaseResources();
	bool loadFile(const juce::File& file);
	void play();
	void stop();
	bool isPlaying() const;
	void setLooping(bool shouldLoop);
	void setGain(float gain);
	void setPosition(double pos);
	double getPosition() const;
	double getLength() const;
	void setSpeed(double newSpeed);
	double getCurrentPosition() const;
	double getLengths() const;
	bool hasFileLoaded() const { return readerSource != nullptr; }
private:
	juce::AudioFormatManager formatManager;
	std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
	juce::AudioTransportSource transportSource;
	std::unique_ptr<juce::ResamplingAudioSource> resamplingSource;
	
	// JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerAudio)
};
