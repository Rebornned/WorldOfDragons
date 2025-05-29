[Setup]
AppName=Targaryen Dragons
AppVersion=1.0
AppPublisher=Nosremehd
DefaultDirName={userappdata}\T_Dragons
DefaultGroupName=Targaryen Dragons
OutputDir=output
OutputBaseFilename=T_Dragons_setup
Compression=lzma
SolidCompression=yes
SetupIconFile=C:\Users\Dhemerson\Desktop\Programacao\GKT_C\WorldOfDragons\winDistribution\assets\img_files\T_dragons_ico.ico
UninstallDisplayIcon={app}\assets\img_files\T_dragons_ico.ico
PrivilegesRequired=admin

[Files]
Source: "C:\Users\Dhemerson\Desktop\Programacao\GKT_C\WorldOfDragons\winDistribution\assets\*"; DestDir: "{app}\assets"; Flags: recursesubdirs createallsubdirs
Source: "C:\Users\Dhemerson\Desktop\Programacao\GKT_C\WorldOfDragons\winDistribution\bin\*"; DestDir: "{app}\bin"; Flags: recursesubdirs createallsubdirs
Source: "C:\Users\Dhemerson\Desktop\Programacao\GKT_C\WorldOfDragons\winDistribution\files\*"; DestDir: "{app}\files"; Flags: recursesubdirs createallsubdirs
Source: "C:\Users\Dhemerson\Desktop\Programacao\GKT_C\WorldOfDragons\winDistribution\lib\*"; DestDir: "{app}\lib"; Flags: recursesubdirs createallsubdirs
Source: "C:\Users\Dhemerson\Desktop\Programacao\GKT_C\WorldOfDragons\winDistribution\share\*"; DestDir: "{app}\share"; Flags: recursesubdirs createallsubdirs
Source: "C:\Users\Dhemerson\Desktop\Programacao\GKT_C\WorldOfDragons\winDistribution\bin\TargaryenDragons.exe"; DestDir: "{app}\bin"; Flags: ignoreversion
Source: "C:\Users\Dhemerson\Desktop\Programacao\GKT_C\WorldOfDragons\winDistribution\assets\fonts\Pixellari.ttf"; DestDir: "{tmp}"; Flags: deleteafterinstall

[Dirs]
Name: "{app}\accounts"

[Icons]
Name: "{group}\T_Dragons"; Filename: "{app}\bin\TargaryenDragons.exe"; IconFilename: "{app}\assets\img_files\T_dragons_ico.ico"
Name: "{commondesktop}\Targaryen Dragons"; Filename: "{app}\bin\TargaryenDragons.exe"; IconFilename: "{app}\assets\img_files\T_dragons_ico.ico"; Tasks: desktopicon

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Run]
Filename: "{app}\assets\fonts\install_font.bat"; Parameters: """{tmp}\Pixellari.ttf"" Pixellari.ttf"; Flags: runhidden runascurrentuser
Filename: "{app}\bin\TargaryenDragons.exe"; Parameters: ""; Flags: nowait postinstall skipifsilent runascurrentuser;

[UninstallDelete]
Type: filesandordirs; Name: "{app}\assets"
Type: filesandordirs; Name: "{app}\bin"
Type: filesandordirs; Name: "{app}\files"
Type: filesandordirs; Name: "{app}\accounts"
Type: filesandordirs; Name: "{app}\lib"
Type: filesandordirs; Name: "{app}\share"
Type: filesandordirs; Name: "{app}" ; 

[UninstallRun]
Filename: "{app}\assets\fonts\uninstall_font.bat"; Parameters: "Pixellari.ttf"; Flags: runhidden runascurrentuser; RunOnceId: uninstall_font
