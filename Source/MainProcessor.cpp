#include "MainProcessor.h"
#include "ProcessorEditor.h"

//==============================================================================
MainProcessor::MainProcessor()
     : AudioProcessor(
		 BusesProperties()
			.withInput("Input", AudioChannelSet::stereo(), true)
			.withOutput("Output", AudioChannelSet::stereo(), true)
	   ),
	   audioProcessorGraph_(new AudioProcessorGraph())
{    	
}

MainProcessor::~MainProcessor()
{
	/*if (pluginInstance_ != nullptr) {
		delete pluginInstance_;
	}*/
}

//==============================================================================
const String MainProcessor::getName() const
{
    String pluginName = "[Bridged] xxxx";    
	DebugTools::log(std::stringstream() << "getName() called");
    return pluginName;
}

bool MainProcessor::acceptsMidi() const
{
	return true;
}

bool MainProcessor::producesMidi() const
{
	return true;
}

double MainProcessor::getTailLengthSeconds() const
{
	DebugTools::log(std::stringstream() << "getTailLengthInSeconds() called");  
	return audioProcessorGraph_->getTailLengthSeconds();    
}

int MainProcessor::getNumPrograms()
{
	DebugTools::log(std::stringstream() << "getNumPrograms() called");    
	return audioProcessorGraph_->getNumPrograms();
}

int MainProcessor::getCurrentProgram()
{
	DebugTools::log(std::stringstream() << "getCurrentProgram() called");    
	return audioProcessorGraph_->getCurrentProgram();
}

void MainProcessor::setCurrentProgram (int index)
{
	DebugTools::log(std::stringstream() << "setCurrentProgram(" << index << ")");    
	return audioProcessorGraph_->setCurrentProgram(index);
}

const String MainProcessor::getProgramName (int index)
{
	DebugTools::log(std::stringstream() << "getProgramName( " << index << ")");    
	return audioProcessorGraph_->getProgramName(index);
}

void MainProcessor::changeProgramName (int index, const String& newName)
{
	DebugTools::log(std::stringstream() << "changeProgramName(" << index << ", " << newName << ")");    
	return audioProcessorGraph_->changeProgramName(index, newName);
}

//==============================================================================
void MainProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    DebugTools::log(std::stringstream() << "Prepare to play with sampleRate=" << sampleRate << " and samplesPerBlock=" << samplesPerBlock);

	audioProcessorGraph_->setPlayConfigDetails(getMainBusNumInputChannels(), getMainBusNumOutputChannels(), sampleRate, samplesPerBlock);
	audioProcessorGraph_->prepareToPlay(sampleRate, samplesPerBlock);
	initializeGraph();
}

void MainProcessor::processBlock(AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
	//DebugTools::log(std::stringstream() << "processBlock() called");

	for (int i = getTotalNumInputChannels(); i < getTotalNumOutputChannels(); ++i) {
		buffer.clear(i, 0, buffer.getNumSamples());
	}

	if(pluginInitialized_)
		audioProcessorGraph_->processBlock(buffer, midiMessages);
	/*if(pluginInitialized_)
		pluginInstance_->processBlock(buffer, midiMessages);*/
}

void MainProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
	DebugTools::log("releaseResources() called");
	audioProcessorGraph_->releaseResources();
}

bool MainProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
	DebugTools::log(std::stringstream() << "isBusesLayoutSupported() called");
	
	if (layouts.getMainInputChannelSet() == AudioChannelSet::disabled()
		|| layouts.getMainOutputChannelSet() == AudioChannelSet::disabled())
		return false;
	if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
		&& layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
		return false;
	return layouts.getMainInputChannelSet() == layouts.getMainOutputChannelSet();	
}



//==============================================================================
bool MainProcessor::hasEditor() const
{
	return true;
}

AudioProcessorEditor* MainProcessor::createEditor()
{	
	//This editor will be used to select and configure plugin to be hosted	
	return new ProcessorEditor(*this);	
}

//==============================================================================
void MainProcessor::getStateInformation (MemoryBlock& destData)
{
	DebugTools::log(std::stringstream() << "getStateInformation() called");
	audioProcessorGraph_->getStateInformation(destData);
}

void MainProcessor::setStateInformation (const void* data, int sizeInBytes)
{
	DebugTools::log(std::stringstream() << "setStateInformation called. sizeInBytes= " << sizeInBytes);
	audioProcessorGraph_->setStateInformation(data, sizeInBytes);
}

void MainProcessor::initializeGraph()
{
	DebugTools::log("initializeGraph() called");

	audioProcessorGraph_->clear();
	audioInputNode_ = audioProcessorGraph_->addNode(new AudioGraphIOProcessor(AudioGraphIOProcessor::audioInputNode));
	audioOutputNode_ = audioProcessorGraph_->addNode(new AudioGraphIOProcessor(AudioGraphIOProcessor::audioOutputNode));
	midiInputNode_ = audioProcessorGraph_->addNode(new AudioGraphIOProcessor(AudioGraphIOProcessor::midiInputNode));
	midiOutputNode_ = audioProcessorGraph_->addNode(new AudioGraphIOProcessor(AudioGraphIOProcessor::midiOutputNode));
	connectAudioNodes();
	connectMidiNodes();
}

void MainProcessor::connectAudioNodes()
{
	DebugTools::log("connectAudioNodes() called");
	for (int channel = 0; channel < 2; ++channel) {
		audioProcessorGraph_->addConnection(
			{ 
				{ audioInputNode_->nodeID,  channel },
				{ audioOutputNode_->nodeID, channel } 
			}
		);
	}
}

void MainProcessor::connectMidiNodes()
{
	DebugTools::log("connectMidiNodes() called");
	if (!graphReady_) {
		DebugTools::log("Connecting MIDI in to MIDI out directly");
		audioProcessorGraph_->addConnection(
			{
				{ midiInputNode_->nodeID,  AudioProcessorGraph::midiChannelIndex },
				{ midiOutputNode_->nodeID, AudioProcessorGraph::midiChannelIndex }
			}
		);
	}
	else {
		DebugTools::log("Connecting MIDI in to plugin MIDI in");
		audioProcessorGraph_->addConnection(
			{
				{ midiInputNode_->nodeID,  AudioProcessorGraph::midiChannelIndex },
				{ pluginNode_->nodeID, AudioProcessorGraph::midiChannelIndex }
			}
		);
		audioProcessorGraph_->addConnection(
			{
				{ pluginNode_->nodeID,  AudioProcessorGraph::midiChannelIndex },
				{ midiOutputNode_->nodeID, AudioProcessorGraph::midiChannelIndex }
			}
		);
	}
}

void MainProcessor::updateGraph()
{
	DebugTools::log("updateGraph() called");
	if (pluginInstance_ == nullptr) {
		return;
	}
	
	//Remove all existing nodes
	for (auto node : audioProcessorGraph_->getNodes()) {
		audioProcessorGraph_->removeNode(node);
	}

	pluginNode_ = audioProcessorGraph_->addNode(pluginInstance_);
	
	//Remove all existing connections
	for (auto connection : audioProcessorGraph_->getConnections()) {
		audioProcessorGraph_->removeConnection(connection);
	}

	//Reset plugin config details and prepare to play
	pluginInstance_->setPlayConfigDetails(getMainBusNumInputChannels(), getMainBusNumOutputChannels(), getSampleRate(), getBlockSize());	
	pluginInstance_->prepareToPlay(getSampleRate(), getBlockSize());
	
	//Connect the inputs and outputs to the plugin inputs and outputs	
	for (int channel = 0; channel < 2; ++channel) {
		audioProcessorGraph_->addConnection( //Audio input to plugin input
			{ 
				{ audioInputNode_->nodeID, channel },
				{ pluginNode_->nodeID, channel }
			}
		);	
		audioProcessorGraph_->addConnection( //Plugin output to audio output
			{ 
				{ pluginNode_->nodeID, channel },
				{ audioOutputNode_->nodeID, channel } 
			}
		);
	}
	
	graphReady_ = true;

	//Reconnect MIDI nodes
	connectMidiNodes();

	//Enable all buses in the plugin
	pluginInstance_->enableAllBuses();	

	pluginInitialized_ = true;
	DebugTools::log("Graph initialized");
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MainProcessor();
}
