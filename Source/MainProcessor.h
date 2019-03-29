/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "DebugTools.h"
//For testing only
//#include "OscillatorProcessor.h"

using AudioGraphIOProcessor = AudioProcessorGraph::AudioGraphIOProcessor;
using Node = AudioProcessorGraph::Node;

class ProcessorEditor;

//==============================================================================
/**
*/
class MainProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    MainProcessor();
    virtual ~MainProcessor();

    //==============================================================================
	const String getName() const override;
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
	//==============================================================================
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;
	//==============================================================================
	bool acceptsMidi() const override;
	bool producesMidi() const override;	
	double getTailLengthSeconds() const override;
	//==============================================================================
	AudioProcessorEditor* createEditor() override;
	bool hasEditor() const override;
	//==============================================================================
	int getNumPrograms() override;
	int getCurrentProgram() override;
	void setCurrentProgram(int index) override;
	const String getProgramName(int index) override;
	void changeProgramName(int index, const String& newName) override;
	//==============================================================================
	void getStateInformation(MemoryBlock& destData) override;
	void setStateInformation(const void* data, int sizeInBytes) override;
	//==============================================================================
	void setPluginInstance(AudioPluginInstance* instance) {
		DebugTools::log(std::stringstream() << "setPluginInstance() called");
		pluginInstance_ = instance;
		updateGraph();		
		processorLayoutsChanged();
	}
	AudioPluginInstance* getPluginInstance() const { return pluginInstance_; }
	//==============================================================================
	void setCurrentEditorDimension(std::pair<int,int> dimension) {
		if (pluginInstance_ != nullptr) {
			editorsDimension_[pluginInstance_->getPluginDescription().uid] = std::pair<int, int>(dimension.first, dimension.second);
		}
	}
	std::pair<int, int> getCurrentEditorDimension() {
		if (pluginInstance_ != nullptr) {
			return editorsDimension_[pluginInstance_->getPluginDescription().uid];
		} 
		return std::pair<int, int>();
	}
	//==============================================================================

private:
    //==============================================================================
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainProcessor)

	void initializeGraph();
	void connectAudioNodes();
	void connectMidiNodes();
	void updateGraph();

	AudioPluginInstance* pluginInstance_ = nullptr;
	std::unique_ptr<AudioProcessorGraph> audioProcessorGraph_;
	Node::Ptr audioInputNode_;
	Node::Ptr audioOutputNode_;
	Node::Ptr midiInputNode_;
	Node::Ptr midiOutputNode_;
	Node::Ptr pluginNode_;
	
	bool pluginInitialized_ = false;
	bool graphReady_ = false;

	//Workaround for plugin editor size recording (to be improved)
	std::map<int, std::pair<int, int>> editorsDimension_;
};
