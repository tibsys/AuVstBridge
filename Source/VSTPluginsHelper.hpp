#pragma once

#include "DebugTools.h"

class VSTPluginsHelper
{

private:
	void addVST2PluginsToPluginsList(std::shared_ptr<KnownPluginList>& list) {
		VSTPluginFormat vstFormat;
		DebugTools::log("Scan VST plugins...");
		DebugTools::log(std::stringstream() << "VST search path: " << vstFormat.getDefaultLocationsToSearch().toString());
		PluginDirectoryScanner scanner(*list, vstFormat, vstFormat.getDefaultLocationsToSearch(), true, File());

		String pluginName;
		while (true) {
			String nextname = scanner.getNextPluginFileThatWillBeScanned();
            if (nextname.equalsIgnoreCase(JucePlugin_Name)) {
                scanner.skipNextFile();
            }
			DebugTools::log(std::stringstream() << "Found plugin: " << nextname);
            if (scanner.scanNextFile(true, pluginName) == false) {
				break;
            }
		}

	};

	void addVST3PluginsToPluginsList(std::shared_ptr<KnownPluginList>& list) {
		VST3PluginFormat vst3Format;
		DebugTools::log("Scan VST3 plugins...");
		DebugTools::log(std::stringstream() << "VST3 search path: " << vst3Format.getDefaultLocationsToSearch().toString());
		PluginDirectoryScanner scanner(*list, vst3Format, vst3Format.getDefaultLocationsToSearch(), true, File());

		String pluginName;
		while (true) {
			String nextname = scanner.getNextPluginFileThatWillBeScanned();
            if (nextname.equalsIgnoreCase(JucePlugin_Name)) {
                scanner.skipNextFile();
            }
			DebugTools::log(std::stringstream() << "Found plugin: " << nextname);
			if (scanner.scanNextFile(true, pluginName) == false)
				break;
		}

	};

public:
	VSTPluginsHelper() {};

	std::shared_ptr<KnownPluginList> pluginsList() {
		auto plist = std::make_shared<KnownPluginList>();
		AudioPluginFormatManager pluginFormatManager;
		pluginFormatManager.addDefaultFormats();		

		DebugTools::log("Getting VST plugins list");
		addVST2PluginsToPluginsList(plist);
		addVST3PluginsToPluginsList(plist);

		if (pluginFormatManager.getNumFormats() == 0) {
			DebugTools::log("No plugins format available... aborting.");
			return plist;
		}		

		for (int i = 0; i < pluginFormatManager.getNumFormats(); i++) {
			if (pluginFormatManager.getFormat(i)->getName() == "VST") {
				addVST2PluginsToPluginsList(plist);
			} else if(pluginFormatManager.getFormat(i)->getName() == "VST3") {
				addVST3PluginsToPluginsList(plist);
			}
		}

		return plist;
	}

	AudioPluginInstance* getPluginWithId(int pluginId, std::shared_ptr<KnownPluginList> &pluginsList)
	{
		DebugTools::log(std::stringstream() << "Trying to load plugin with ID " << pluginId);
		PluginDescription** plugin_it;

		for (plugin_it = pluginsList->begin(); plugin_it < pluginsList->end(); plugin_it++) {
			if (plugin_it != nullptr) {
				PluginDescription *pluginDescription = *plugin_it;
				if (pluginDescription == nullptr) continue;
				if (pluginDescription->uid == pluginId) {					
					AudioPluginFormatManager pluginFormatManager;
					pluginFormatManager.addDefaultFormats();
					String msg = "no error";
					AudioPluginInstance *pluginInstance = pluginFormatManager.createPluginInstance(*pluginDescription,
						44100,
						512,
						msg);

					if (pluginInstance == nullptr) {
						DebugTools::log(std::stringstream() << "Error when loading the plugin: " << msg);
					} 
					
					DebugTools::log("Plugin loaded");
					DebugTools::log(std::stringstream() << "Plugin category: " << pluginDescription->category);
					DebugTools::log(std::stringstream() << "Plugin is instrument: " << pluginDescription->isInstrument);
					return pluginInstance;
				}
			}
		}

		DebugTools::log(std::stringstream() << "Could not find plugin with ID " << pluginId);
		return nullptr;
	}

	AudioPluginInstance* getPluginWithIdentifierString(String identifierString)
	{
		DebugTools::log(std::stringstream() << "Trying to load plugin with identifier string " << identifierString);
		PluginDescription** plugin_it;

		std::shared_ptr<KnownPluginList> pluginsList = this->pluginsList();

		for (plugin_it = pluginsList->begin(); plugin_it < pluginsList->end(); plugin_it++) {
			if (plugin_it != nullptr) {
				PluginDescription *pluginDescription = *plugin_it;
				if (pluginDescription == nullptr) continue;
				if (pluginDescription->matchesIdentifierString(identifierString)) {
					AudioPluginFormatManager pluginFormatManager;
					pluginFormatManager.addDefaultFormats();
					String msg = "no error";
					AudioPluginInstance *pluginInstance = pluginFormatManager.createPluginInstance(*pluginDescription,
						44100,
						512,
						msg);

					if (pluginInstance == nullptr) {
						DebugTools::log(std::stringstream() << "Error when loading the plugin: " << msg);
					}

					DebugTools::log("Plugin loaded");
					DebugTools::log(std::stringstream() << "Plugin category: " << pluginDescription->category);
					DebugTools::log(std::stringstream() << "Plugin is instrument: " << pluginDescription->isInstrument);
					return pluginInstance;
				}
			}
		}

		DebugTools::log(std::stringstream() << "Could not find plugin with identifier string " << identifierString);
		return nullptr;
	}
};
