/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
NewProjectAudioProcessor::NewProjectAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    vstLoaded_ = loadVSTPlugin();
}

NewProjectAudioProcessor::~NewProjectAudioProcessor()
{
    if(pluginInstance_ != nullptr) {
        delete pluginInstance_;
    }
}

//==============================================================================
const String NewProjectAudioProcessor::getName() const
{
    String pluginName = "[Bridged] ";
    pluginName.preallocateBytes(32);
    if(pluginInstance_ != nullptr) {
        pluginName.append(pluginInstance_->getName(), 21);
    } else {
        pluginName.append("no plugin loaded", 16);
    }
    return pluginName;
}

bool NewProjectAudioProcessor::acceptsMidi() const
{
    if(pluginInstance_ != nullptr) {
        return pluginInstance_->acceptsMidi();
    }
    return false;
}

bool NewProjectAudioProcessor::producesMidi() const
{
    if(pluginInstance_ != nullptr) {
        return pluginInstance_->producesMidi();
    }
    return false;
}

bool NewProjectAudioProcessor::isMidiEffect() const
{
    if(pluginInstance_ != nullptr) {
        return pluginInstance_->isMidiEffect();
    }
    return false;
}

double NewProjectAudioProcessor::getTailLengthSeconds() const
{
    if(pluginInstance_ != nullptr) {
        return pluginInstance_->getTailLengthSeconds();
    }
    return 0.0;
}

int NewProjectAudioProcessor::getNumPrograms()
{
    if(pluginInstance_ != nullptr) {
        return pluginInstance_->getNumPrograms();
    }
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int NewProjectAudioProcessor::getCurrentProgram()
{
    if(pluginInstance_ != nullptr) {
        return pluginInstance_->getCurrentProgram();
    }
    return 0;
}

void NewProjectAudioProcessor::setCurrentProgram (int index)
{
    if(pluginInstance_ != nullptr) {
        pluginInstance_->setCurrentProgram(index);
    }
}

const String NewProjectAudioProcessor::getProgramName (int index)
{
    if(pluginInstance_ != nullptr) {
        return pluginInstance_->getProgramName(index);
    }
    return String{};
}

void NewProjectAudioProcessor::changeProgramName (int index, const String& newName)
{
    if(pluginInstance_ != nullptr) {
        pluginInstance_->changeProgramName(index, newName);
    }
}

//==============================================================================
void NewProjectAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    std::cout << "Prepare to play with sampleRate=" << sampleRate << " and samplesPerBlock=" << samplesPerBlock << std::endl;
    pluginInstance_->prepareToPlay(sampleRate, samplesPerBlock);
}

void NewProjectAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
    std::cout << "Release resources" << std::endl;
    pluginInstance_->releaseResources();
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool NewProjectAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    return false;
}
#endif

void NewProjectAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    /*for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());*/

    if(midiMessages.isEmpty()) {
        return;
    }
    // ..do something to the data...
    for(int i = 0 ; i < midiMessages.data.size() ; i++) {
        std::cout << "MIDI message: 0x" << std::hex << (int)midiMessages.data[i] << std::endl;
    }
    
    if(pluginInstance_ == nullptr) {
        return;
    }
    
    //Write the messages to the VST plugin
    pluginInstance_->processBlock(buffer, midiMessages);
}

//==============================================================================
bool NewProjectAudioProcessor::hasEditor() const
{
    if(pluginInstance_ != nullptr) {
        return pluginInstance_->hasEditor();
    }
    return false;
}

AudioProcessorEditor* NewProjectAudioProcessor::createEditor()
{
    if(pluginInstance_ != nullptr) {
        return pluginInstance_->createEditor();
    }
    return new NewProjectAudioProcessorEditor (*this);
}

//==============================================================================
void NewProjectAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    if(pluginInstance_ != nullptr) {
        pluginInstance_->getStateInformation(destData);
    }
}

void NewProjectAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    if(pluginInstance_ != nullptr) {
        pluginInstance_->setStateInformation(data, sizeInBytes);
    }
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new NewProjectAudioProcessor();
}

bool NewProjectAudioProcessor::loadVSTPlugin()
{
    KnownPluginList plist;
    AudioPluginFormatManager pluginFormatManager;
    pluginFormatManager.addDefaultFormats();
#ifdef _MSC_VER
	String pluginPath = "C:\\Users\\tristan.israel\\Downloads\\PG-8X_2.0.12_Win64\\";
#else
	String pluginPath = "/Library/Audio/Plug-Ins/";
#endif
    
    if(pluginFormatManager.getNumFormats() == 0) {
        return false;
    }
    
    VSTPluginFormat vstFormat;
    std::cout << "Search path: " << vstFormat.getDefaultLocationsToSearch().toString() << std::endl;
    PluginDirectoryScanner scanner(plist, vstFormat, vstFormat.getDefaultLocationsToSearch(), true, File());
    
    String pluginName;
    while(true) {
        String nextname = scanner.getNextPluginFileThatWillBeScanned();
        std::cout << "Plugin: " << nextname << std::endl;
        if (scanner.scanNextFile(true, pluginName)==false)
            break;
    }
    
    if(plist.getNumTypes() == 0) {
        return false;
    }
    PluginDescription *pluginDescription = plist.getType(0);
    String msg = "no error";
    pluginInstance_ = pluginFormatManager.createPluginInstance(*pluginDescription,
                                                               44100,
                                                               1024,
                                                               msg);
    if(pluginInstance_ == nullptr) {
        std::cout << "Error when loading the plugin: " << msg << std::endl;
    }
    /*pluginEditor_ = pluginInstance_->createEditor();
    auto bc = pluginEditor_->getConstrainer();
    pluginEditor_->setBounds(0, 0, bc->getMinimumWidth(), bc->getMinimumHeight());
    addAndMakeVisible (pluginEditor_);*/
    
    return true;
}
