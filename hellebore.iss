[Setup]
AppName=Hellebore
AppVersion=1.0.1
DefaultDirName={commoncf64}
DefaultGroupName=Hellebore
OutputBaseFilename=Hellebore-windows
 
[Files]
//Source: "TestPlugin.dll"; DestDir: "{app}\Steinberg\VST2"
Source: "build\src\Hellebore_artefacts\Release\VST3\*"; DestDir: "{commoncf64}\VST3"; Flags: ignoreversion recursesubdirs
//Source: "C:\Users\33788\Desktop\Sinensis.vst3.zip"; DestDir: "{commoncf64}\VST3"
//Source: "TestPlugin.dpm"; DestDir: "{app}\Digidesign\DAE\Plug-Ins"
//Source: "TestPlugin.aaxplugin\*"; DestDir: "{app}\Avid\Audio\Plug-Ins\TestPlugin.aaxplugin"; Flags: recursesubdirs