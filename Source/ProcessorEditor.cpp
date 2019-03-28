/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "ProcessorEditor.h"
#include "VSTPluginsHelper.hpp"

#define COMBOBOX_WIDTH	300
#define COMBOBOX_HEIGHT 30
#define SPACE_BETWEEN_COMPONENTS 20

//==============================================================================
ProcessorEditor::ProcessorEditor (MainProcessor& p)
    : AudioProcessorEditor (&p), processor_ (p)
{
	setupUi();

	pluginsCombobox_.setBounds(100, 0, COMBOBOX_WIDTH, COMBOBOX_HEIGHT);
	pluginsCombobox_.setTextWhenNoChoicesAvailable("No plugin available");
	pluginsComboBoxLabel_.setText("Select Plugin:", dontSendNotification);
	pluginsComboBoxLabel_.attachToComponent(&pluginsCombobox_, true);
	addAndMakeVisible(pluginsComboBoxLabel_);	
	addAndMakeVisible(pluginsCombobox_);	

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
		if (ID != currentPluginId_ && pluginEditor_ != nullptr) {
			DebugTools::log("Destroy previous plugin's editor");						
			delete pluginEditor_;
		}
		if (ID > 0) {
			DebugTools::log(std::stringstream() << "Selected plugin ID: " << ID);
			AudioPluginInstance *pluginInstance = vstPluginsHelper_.getPluginWithId(ID, knownPlugins_);
			if (pluginInstance != nullptr) {
				processor_.setPluginInstance(pluginInstance);
				currentPluginId_ = ID;
				pluginEditor_ = pluginInstance->createEditor();				
				auto bc = pluginEditor_->getConstrainer();								
				addAndMakeVisible(pluginEditor_);				
				pluginEditor_->setBounds(0, COMBOBOX_HEIGHT + SPACE_BETWEEN_COMPONENTS, bc->getMinimumWidth(), bc->getMinimumHeight());
				setSize(bc->getMinimumWidth(), COMBOBOX_HEIGHT + SPACE_BETWEEN_COMPONENTS + bc->getMinimumHeight());
			}
		}
		else {
			DebugTools::log(std::stringstream() << "ERROR: plugin selected has an invalid ID: " << ID);
		}
	};

	//Reset the currently selected plugin editor if any
	if (processor_.getPluginInstance() != nullptr) {
		pluginsCombobox_.setSelectedId(processor_.getPluginInstance()->getPluginDescription().uid);		
	}
	else {
		setSize(500, 300);
	}

	//TODO: Check the parameters to restore previously selected plugin

}

ProcessorEditor::~ProcessorEditor()
{    
	DebugTools::log("Destroy processor editor");
	if (pluginEditor_ != nullptr) {
		delete pluginEditor_;
	}
}

//==============================================================================
/*void ProcessorEditor::paint (Graphics& g)
{
   
}*/

void ProcessorEditor::resized()
{
	
}

void ProcessorEditor::setupUi()
{
	
}

