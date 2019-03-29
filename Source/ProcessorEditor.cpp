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

	//Set the currently selected plugin back if any
	if (processor_.getPluginInstance() != nullptr) {
		pluginsCombobox_.setSelectedId(processor_.getPluginInstance()->getPluginDescription().uid);		
		resetPluginInstance(processor_.getPluginInstance());
	}
	else {
		setSize(500, 300);
	}

	pluginsCombobox_.onChange = [this] { onPluginsComboboxChanged(); };
}

void ProcessorEditor::resetPluginInstance(AudioPluginInstance *pluginInstance, bool changed) {
	if (pluginInstance != nullptr) {
		if (changed) {
			processor_.setPluginInstance(pluginInstance);
		}
		pluginEditor_ = pluginInstance->createEditorIfNeeded();
		auto bc = pluginEditor_->getConstrainer();
		addAndMakeVisible(pluginEditor_);
		std::pair<int, int> dimensions = processor_.getCurrentEditorDimension();
		if (dimensions.first == 0 && dimensions.second == 0) {
			dimensions.first = bc->getMinimumWidth();
			dimensions.second = bc->getMinimumHeight();
			processor_.setCurrentEditorDimension(dimensions);
		} 
		pluginEditor_->setBounds(0, COMBOBOX_HEIGHT + SPACE_BETWEEN_COMPONENTS, dimensions.first, dimensions.second);
		setSize(dimensions.first, COMBOBOX_HEIGHT + SPACE_BETWEEN_COMPONENTS + dimensions.second);
	}
}

void ProcessorEditor::onPluginsComboboxChanged()
{
	int ID = pluginsCombobox_.getSelectedId();
	bool changed = ((processor_.getPluginInstance() == nullptr) || (ID != processor_.getPluginInstance()->getPluginDescription().uid));
	if (changed) {
		if (pluginEditor_ != nullptr) {
			DebugTools::log("Destroy previous plugin's editor");
			delete pluginEditor_;
		}
		if (ID > 0) {  
			DebugTools::log(std::stringstream() << "Selected plugin ID: " << ID);
			AudioPluginInstance *pluginInstance = vstPluginsHelper_.getPluginWithId(ID, knownPlugins_);
			resetPluginInstance(pluginInstance, changed);
		}
		else {
			DebugTools::log(std::stringstream() << "ERROR: plugin selected has an invalid ID: " << ID);
		}
	}
	
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

