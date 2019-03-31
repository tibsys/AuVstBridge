# AU-VST-Bridge

**Current version**: 1.0.4

Do you have VST plugins you really want to use in MainStage or GarageBand but you can't because they won't load VST plugins? AU-VST-Bridge is an Audio Unit plugin which can host VST (2 and 3) plugin. 

Currently it can only host one plugin, but maybe in the future it will be able to host and chain multiple plugins... but as long as you can have multiple AU-VST-Bridge in your DAW you can host multiple VST plugins and chain them in the DAW directly.

## Specifications
- This is an Audio Unit plugin for macOS 64 bit
- It is developped using the free version of [JUCE](https://juce.com/)
- It uses mainly C++11 specification and is declared using C++14 specification because of the DSP linkage

## Remarks
- As I'm a beginner with JUCE I may have made mistakes in the conception... feel free to comment and contribute
- If you want a binary version please contact me
- The plugin has been developped and tested for some specific VST 2 plugins and may not be compilant with all VST plugins. Please share your own experience for bug fixing

## Dependencies
- Steinberg VST3 SDK @ https://github.com/steinbergmedia/vst3sdk.git
- Steinberg VST2 SDK addons (unlicensed but currently necessary for VST2 plugins)
- JUCE framework

## Setup instructions
1. Copy the AU-VT-Brudge.component bundle in the audio plugins directory on your Mac (~/Library/Audio/Plug-Ins/Components, or /Library/Audio/Plug-Ins/Components)
2. If the plugin does not appear in your DAW, consider restarting the computer

## Usage
A documentation in PDF is coming soon...
1. Start you DAW
2. Add an instrument plugin -> Select AU-VST-Plugin
3. Open the plugin editor (depends on your DAW)
4. The list in the top of the screen contains all available VST plugins
5. Select the VST plugin you want to host in your DAW
6. The VST plugins behaves exactly as if it were directly hosted in the DAW as an Audio Unit plugin (it should...)

## Bug tracker
Workflow: CONFIRMED (means reproduced) -> FIXED (means action taken) -> RESOLVED (means verified)

- [AU] filter plugins list to exclude AU-VST-Bridge ** RESOLVED v1.0.3 **
- [AU] replace transparent background with an opaque one ** RESOLVED v1.0.3 **
- [All] save settings (plugin loaded and plugin settings) ** RESOLVED v1.0.3 **
- [AU] computer crashes when saving settings (without any plugin selected?) ** RESOLVED v1.0.3 **
- [All] settings are not kept when editor is closed and reopened ** RESOLVED v1.0.3 **
- [All] dimensions of the editor are strange when opened the second time ** CONFIRMED v1.0.3 **
- [AU] when reloaded in MainStage, plugin settings are OK but there is not editor ** RESOLVED v1.0.3 **
- [AU] crash when changing plugin hosted in Mainstage ** FIXED v1.0.4 **
- [AU] Spicy Guitar plugin editor is outside our plugin editor's area ** CONFIRMED v1.0.4 **
- [All] Write documentation
- [All] Finalise README.md (details about project and files) 