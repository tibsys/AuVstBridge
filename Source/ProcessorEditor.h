/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "MainProcessor.h"
#include "VSTPluginsHelper.hpp"

//==============================================================================
/**
*/
class ProcessorEditor  : public AudioProcessorEditor
{
public:
	ProcessorEditor(MainProcessor&);
    ~ProcessorEditor();

    //==============================================================================
    //void paint (Graphics&) override;
    void resized() override;

private:    

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    MainProcessor& processor_;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ProcessorEditor)

	void setupUi();	
	void resetPluginInstance(AudioPluginInstance *, bool changed = false);
	void onPluginsComboboxChanged();

	ComboBox pluginsCombobox_;
	Label pluginsComboBoxLabel_;
	int lastPluginIndex_ = 0;

	VSTPluginsHelper vstPluginsHelper_;
	std::shared_ptr<KnownPluginList> knownPlugins_;
	AudioProcessorEditor *pluginEditor_ = nullptr;	

};
