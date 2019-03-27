/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginEditor.h"
#include "VSTPluginsHelper.hpp"

//==============================================================================
NewProjectAudioProcessorEditor::NewProjectAudioProcessorEditor (MainProcessor& p)
    : AudioProcessorEditor (&p), processor_ (p)
{
	setupUi();

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (500, 300);	
}

NewProjectAudioProcessorEditor::~NewProjectAudioProcessorEditor()
{    
	if (pluginEditor_ != nullptr) {
		delete pluginEditor_;
	}
}

//==============================================================================
/*void NewProjectAudioProcessorEditor::paint (Graphics& g)
{
   
}*/

void NewProjectAudioProcessorEditor::resized()
{
	pluginsCombobox_.setBounds(150, 10, getWidth() - 160, 30);
}

void NewProjectAudioProcessorEditor::setupUi()
{
	addAndMakeVisible(pluginsComboBoxLabel_);
	pluginsComboBoxLabel_.setText("Select VST Plugin:", dontSendNotification);
	pluginsComboBoxLabel_.attachToComponent(&pluginsCombobox_, true);
	addAndMakeVisible(pluginsCombobox_);	
	pluginsCombobox_.setTextWhenNoChoicesAvailable("No plugin available");
	
	knownPlugins_ = vstPluginsHelper_.pluginsList();
	PluginDescription** plugin_it;

	for (plugin_it = knownPlugins_->begin(); plugin_it < knownPlugins_->end(); plugin_it++) {
		if (plugin_it != nullptr) {
			PluginDescription *plugin = *plugin_it;
			if (plugin == nullptr) continue;
			pluginsCombobox_.addItem(plugin->name, plugin->uid);
		}
	}
	
	pluginsCombobox_.onChange = [this] { 
		int ID = pluginsCombobox_.getSelectedId(); 
		if (ID > 0) {
			DebugTools::log(std::stringstream() << "Selected plugin ID: " << ID);
			AudioPluginInstance *pluginInstance = vstPluginsHelper_.getPluginWithId(ID, knownPlugins_);
			if (pluginInstance != nullptr) {					
				pluginEditor_ = pluginInstance->createEditor();				
				auto bc = pluginEditor_->getConstrainer();				
				pluginEditor_->setBounds(0, 0, bc->getMinimumWidth(), bc->getMinimumHeight());				
				addAndMakeVisible(pluginEditor_);		
				processor_.setPluginInstance(pluginInstance); 
				setSize(1200, 1000);
			}
		}
		else {
			DebugTools::log(std::stringstream() << "ERROR: plugin selected has an invalid ID: " << ID);
		}
	};
	
	//TODO: Check the parameters to restore previously selected plugin
}

