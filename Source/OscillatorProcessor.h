#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class ProcessorBase : public AudioProcessor
{
public:
	//==============================================================================
	ProcessorBase() {}
	~ProcessorBase() {}
	//==============================================================================
	void prepareToPlay(double, int) override {}
	void releaseResources() override {}
	void processBlock(AudioSampleBuffer& buffer, MidiBuffer&) override {}
	//==============================================================================
	AudioProcessorEditor* createEditor() override { return nullptr; }
	bool hasEditor() const override { return false; }
	//==============================================================================
	const String getName() const override { return {}; }
	bool acceptsMidi() const override { return false; }
	bool producesMidi() const override { return false; }
	double getTailLengthSeconds() const override { return 0; }
	//==============================================================================
	int getNumPrograms() override { return 0; }
	int getCurrentProgram() override { return 0; }
	void setCurrentProgram(int) override {}
	const String getProgramName(int) override { return {}; }
	void changeProgramName(int, const String&) override {}
	//==============================================================================
	void getStateInformation(MemoryBlock& destData) override {}
	void setStateInformation(const void* data, int sizeInBytes) override {}
private:
	//==============================================================================
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ProcessorBase)
};

class OscillatorProcessor : public ProcessorBase
{
public:
	OscillatorProcessor()
	{
		oscillator.setFrequency(440.0f);
		oscillator.initialise([](float x) { return std::sin(x); });
	}

	const String getName() const override { return "Oscillator"; }

	void prepareToPlay(double sampleRate, int samplesPerBlock) override
	{
		dsp::ProcessSpec spec{ sampleRate, static_cast<uint32> (samplesPerBlock) };
		oscillator.prepare(spec);
	}
	void processBlock(AudioSampleBuffer& buffer, MidiBuffer& midiMessages) override
	{
		dsp::AudioBlock<float> block(buffer);
		dsp::ProcessContextReplacing<float> context(block);
		oscillator.process(context);
	}
	void reset() override
	{
		oscillator.reset();
	}

private:
	dsp::Oscillator<float> oscillator;
};