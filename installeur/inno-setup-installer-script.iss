#define MyAppName "Bomberman"
#define MyAppVersion "1.0"
#define MyAppPublisher "HE-Arc"
#define MyAppURL "https://forge.ing.he-arc.ch/gitlab/inf/1819/p2-qt/bomberman"
#define MyAppExeName "Bomberman.exe"
#define MyAppIcoName "icon.ico"
#define MyAppGroupName "Bomberman"
#define MyAppUninstaller "unins000.exe"

[Setup]
; NOTE: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{0DD2EFE8-2F63-44FD-A395-E8F9EC827FCA}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
;AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName={pf}\{#MyAppName}
DisableProgramGroupPage=yes
DefaultGroupName={#MyAppGroupName}
OutputDir=D:\users\teo\Documents\hes\2e\p2\bomberman\installeur
OutputBaseFilename=bomberman-setup
Compression=lzma
SolidCompression=yes

[Languages]
Name: "french"; MessagesFile: "compiler:Languages\French.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}";
Name: "startmenuicon"; Description: "Créer un raccourci dans le menu démarrer"; GroupDescription: "{cm:AdditionalIcons}"; 
Name: "quicklaunchicon"; Description: "{cm:CreateQuickLaunchIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked; OnlyBelowVersion: 0,6.1

[Files]
Source: "D:\users\teo\Documents\hes\2e\p2\bomberman\dev\build-Bomberman-Desktop_Qt_5_11_1_MinGW_32bit-Release\release\Bomberman.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\users\teo\Documents\hes\2e\p2\bomberman\installeur\{#MyAppIcoName}"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\users\teo\Documents\hes\2e\p2\bomberman\dev\build-Bomberman-Desktop_Qt_5_11_1_MinGW_32bit-Release\release\maps\*"; DestDir: "{app}\maps"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "D:\users\teo\Documents\hes\2e\p2\bomberman\dev\build-Bomberman-Desktop_Qt_5_11_1_MinGW_32bit-Release\release\iconengines\*"; DestDir: "{app}\iconengines"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "D:\users\teo\Documents\hes\2e\p2\bomberman\dev\build-Bomberman-Desktop_Qt_5_11_1_MinGW_32bit-Release\release\imageformats\*"; DestDir: "{app}\imageformats"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "D:\users\teo\Documents\hes\2e\p2\bomberman\dev\build-Bomberman-Desktop_Qt_5_11_1_MinGW_32bit-Release\release\platforms\*"; DestDir: "{app}\platforms"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "D:\users\teo\Documents\hes\2e\p2\bomberman\dev\build-Bomberman-Desktop_Qt_5_11_1_MinGW_32bit-Release\release\styles\*"; DestDir: "{app}\styles"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "D:\users\teo\Documents\hes\2e\p2\bomberman\dev\build-Bomberman-Desktop_Qt_5_11_1_MinGW_32bit-Release\release\translations\*"; DestDir: "{app}\translations"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "D:\users\teo\Documents\hes\2e\p2\bomberman\dev\build-Bomberman-Desktop_Qt_5_11_1_MinGW_32bit-Release\release\D3Dcompiler_47.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\users\teo\Documents\hes\2e\p2\bomberman\dev\build-Bomberman-Desktop_Qt_5_11_1_MinGW_32bit-Release\release\libEGL.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\users\teo\Documents\hes\2e\p2\bomberman\dev\build-Bomberman-Desktop_Qt_5_11_1_MinGW_32bit-Release\release\libGLESV2.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\users\teo\Documents\hes\2e\p2\bomberman\dev\build-Bomberman-Desktop_Qt_5_11_1_MinGW_32bit-Release\release\opengl32sw.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\users\teo\Documents\hes\2e\p2\bomberman\dev\build-Bomberman-Desktop_Qt_5_11_1_MinGW_32bit-Release\release\Qt5Core.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\users\teo\Documents\hes\2e\p2\bomberman\dev\build-Bomberman-Desktop_Qt_5_11_1_MinGW_32bit-Release\release\Qt5Gui.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\users\teo\Documents\hes\2e\p2\bomberman\dev\build-Bomberman-Desktop_Qt_5_11_1_MinGW_32bit-Release\release\Qt5Svg.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\users\teo\Documents\hes\2e\p2\bomberman\dev\build-Bomberman-Desktop_Qt_5_11_1_MinGW_32bit-Release\release\Qt5Widgets.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\users\teo\Documents\hes\2e\p2\bomberman\dev\build-Bomberman-Desktop_Qt_5_11_1_MinGW_32bit-Release\release\libwinpthread-1.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\users\teo\Documents\hes\2e\p2\bomberman\dev\build-Bomberman-Desktop_Qt_5_11_1_MinGW_32bit-Release\release\libstdc++-6.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\users\teo\Documents\hes\2e\p2\bomberman\dev\build-Bomberman-Desktop_Qt_5_11_1_MinGW_32bit-Release\release\libgcc_s_dw2-1.dll"; DestDir: "{app}"; Flags: ignoreversion
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Icons]
Name: "{userdesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; IconFilename: "{app}\{#MyAppIcoName}"; Tasks: desktopicon
Name: "{userappdata}\Microsoft\Internet Explorer\Quick Launch\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: quicklaunchicon
Name: "{group}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; IconFilename: "{app}\{#MyAppIcoName}"; Tasks: startmenuicon
Name: "{group}\{cm:ProgramOnTheWeb,{#MyAppName}}"; Filename: "{#MyAppURL}"; Tasks: startmenuicon
Name: "{group}\{cm:UninstallProgram,{#MyAppName}}"; Filename: "{app}\{#MyAppUninstaller}"; Tasks: startmenuicon

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent

