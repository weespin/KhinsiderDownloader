; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!
#define MyAppName "Khinsider QT"
#define MyAppVersion "3.0"
#define MyAppPublisher "Weespin"
#define MyAppURL "https://weesp.in"
#define MyAppExeName "appKhinsiderQT.exe"
#define MyAppAssocName MyAppName + " File"
#define MyAppAssocExt ".myp"
#define MyAppAssocKey StringChange(MyAppAssocName, " ", "") + MyAppAssocExt

[Setup]
; NOTE: The value of AppId uniquely identifies this application. Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{ADCA76C2-2411-4D28-BE0E-84C6DA672504}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
;AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName={autopf}\{#MyAppName}
UninstallDisplayIcon={app}\{#MyAppExeName}
; "ArchitecturesAllowed=x64compatible" specifies that Setup cannot run
; on anything but x64 and Windows 11 on Arm.
ArchitecturesAllowed=x64compatible
; "ArchitecturesInstallIn64BitMode=x64compatible" requests that the
; install be done in "64-bit mode" on x64 or Windows 11 on Arm,
; meaning it should use the native 64-bit Program Files directory and
; the 64-bit view of the registry.
ArchitecturesInstallIn64BitMode=x64compatible
ChangesAssociations=yes
DisableProgramGroupPage=yes
LicenseFile=..\..\LICENSE
; Remove the following line to run in administrative install mode (install for all users).
PrivilegesRequired=lowest
PrivilegesRequiredOverridesAllowed=dialog
OutputBaseFilename=KhinsiderInstaller
SolidCompression=yes
WizardStyle=modern
OutputDir=..\..\build\Installer
RestartIfNeededByRun=no
[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked
Name: "vcredist"; Description: "Install Visual C++ Redistributable (required if not already installed)"; GroupDescription: "Additional tasks:"; Flags: checkedonce

[Files]
Source: "..\..\build\Release\*"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs

[Registry]
Root: HKA; Subkey: "Software\Classes\{#MyAppAssocExt}\OpenWithProgids"; ValueType: string; ValueName: "{#MyAppAssocKey}"; ValueData: ""; Flags: uninsdeletevalue
Root: HKA; Subkey: "Software\Classes\{#MyAppAssocKey}"; ValueType: string; ValueName: ""; ValueData: "{#MyAppAssocName}"; Flags: uninsdeletekey
Root: HKA; Subkey: "Software\Classes\{#MyAppAssocKey}\DefaultIcon"; ValueType: string; ValueName: ""; ValueData: "{app}\{#MyAppExeName},0"
Root: HKA; Subkey: "Software\Classes\{#MyAppAssocKey}\shell\open\command"; ValueType: string; ValueName: ""; ValueData: """{app}\{#MyAppExeName}"" ""%1"""

[Icons]
Name: "{autoprograms}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{autodesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent
Filename: "{tmp}\vcredist.exe"; Parameters: "/q /norestart"; StatusMsg: "Installing Visual C++ Redistributable..."; Tasks: vcredist; BeforeInstall: DownloadVCRedist

[Code]
#ifdef UNICODE
  #define AW "W"
#else
  #define AW "A"
#endif

function URLDownloadToFile(pCaller: LongWord; szURL, szFileName: String; dwReserved, lpfnCB: LongWord): Integer;
  external 'URLDownloadToFile{#AW}@urlmon.dll stdcall';

procedure DownloadVCRedist;
begin
  if not FileExists(ExpandConstant('{tmp}\vcredist.exe')) then
  begin
    Log('Downloading VC++ Redistributable...');
    URLDownloadToFile(0, 'https://aka.ms/vs/17/release/vc_redist.x64.exe', ExpandConstant('{tmp}\vcredist.exe'), 0, 0);
    Log('Download completed.');
  end;
end;

function IsVCRedistInstalled: Boolean;
begin
  Result := FileExists(ExpandConstant('{sys}\msvcp140.dll'));
  if Result then
    Log('Visual C++ Redistributable appears to be already installed')
  else
    Log('Visual C++ Redistributable does not appear to be installed');
end;

procedure CurPageChanged(CurPageID: Integer);
var
  TaskIndex: Integer;
begin
  if CurPageID = wpSelectTasks then
  begin
    if IsVCRedistInstalled then
    begin
      TaskIndex := WizardForm.TasksList.Items.IndexOf('Install Visual C++ Redistributable (required if not already installed, internet connection required)');
      if TaskIndex >= 0 then
        WizardForm.TasksList.Checked[TaskIndex] := False;
    end;
  end;
end;