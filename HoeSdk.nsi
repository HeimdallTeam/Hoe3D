; HoeSdk.nsi
;
; This script is based on example1.nsi, but it remember the directory, 
; has uninstall support and (optionally) installs start menu shortcuts.
;
; It will install example2.nsi into a directory that the user selects,

;--------------------------------

;--------------------------------
;Include Modern UI

  !include "MUI.nsh"

;--------------------------------
;General

; The name of the installer
Name "HoeSdk Win32"

; The file to write
OutFile "HoeSdk.exe"

; The default installation directory
InstallDir "c:\HoeSdk"

; Registry key to check for directory (so if you install again, it will 
; overwrite the old one automatically)
InstallDirRegKey HKLM "Software\HoeSdk" "Install_Dir"

;--------------------------------
;Interface Configuration

  !define MUI_HEADERIMAGE
  ;!define MUI_HEADERIMAGE_BITMAP "..\resource\nsis.bmp" ; optional
  !define MUI_ABORTWARNING

;--------------------------------
;Pages

  !insertmacro MUI_PAGE_LICENSE "${NSISDIR}\Docs\Modern UI\License.txt"
  !insertmacro MUI_PAGE_COMPONENTS
  !insertmacro MUI_PAGE_DIRECTORY
  !insertmacro MUI_PAGE_INSTFILES
  
  !insertmacro MUI_UNPAGE_CONFIRM
  !insertmacro MUI_UNPAGE_INSTFILES
  
;--------------------------------
;Languages
  ;!insertmacro MUI_LANGUAGE "English"
  !insertmacro MUI_LANGUAGE "Czech"
;--------------------------------
;Language Selection Dialog Settings

  ;Remember the installer language
  !define MUI_LANGDLL_REGISTRY_ROOT "HKCU" 
  !define MUI_LANGDLL_REGISTRY_KEY "Software\HoeSdk" 
  !define MUI_LANGDLL_REGISTRY_VALUENAME "Installer Language"


;--------------------------------

; The stuff to install
Section "Game files"

  SectionIn RO
  
  ; Set output path to the installation directory.
  SetOutPath $INSTDIR
  
 
  SetOutPath "$INSTDIR\Include\HoeCore"
  File "HoeCore\include\HoeCore\*"
  SetOutPath "$INSTDIR\Include\HoeResource"
  File "HoeResource\include\HoeResource\*"
  SetOutPath "$INSTDIR\Include\Hoe3D"
  File "Hoe3D\include\*"
  SetOutPath "$INSTDIR\Include\HoeGame"
  File "HoeGame\include\HoeGame\*"
  SetOutPath "$INSTDIR\Include\HoeUtils"
  File "HoeUtils\include\HoeUtils\*"
  SetOutPath "$INSTDIR\Include\HoeEditor"
  File "HoeEditor\include\HoeEditor\*"
  SetOutPath "$INSTDIR\Lib\x86"
  File "bin\*.lib"
  SetOutPath "$INSTDIR\Lib\arm"
  File "bin_ce\*.lib"
  SetOutPath "$INSTDIR\Bin"
  ; exe files
  ;File "bin\*.dll"
  SetOutPath "$INSTDIR\Utils"
  ;File "bin\utils\*.dle"
  ;File "bin\utils\*.exe"

   ; Write the installation path into the registry
  WriteRegStr HKLM SOFTWARE\HoeSdk "Install_Dir" "$INSTDIR"
  
  ; Write the uninstall keys for Windows
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\HoeSdk" "DisplayName" "HoeSdk Win32"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\HoeSdk" "UninstallString" '"$INSTDIR\uninstall.exe"'
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\HoeSdk" "NoModify" 1
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\HoeSdk" "NoRepair" 1
  WriteUninstaller "uninstall.exe"
  
SectionEnd

; Optional section (can be disabled by the user)
;Section "Start Menu Shortcuts"

  ;CreateDirectory "$SMPROGRAMS\HoeSdk"
  ;CreateShortCut "$SMPROGRAMS\HoeSdk\Uninstall.lnk" "$INSTDIR\uninstall.exe" "" "$INSTDIR\uninstall.exe" 0
  ;CreateShortCut "$SMPROGRAMS\HoeSdk\HoeSdk.lnk" "$INSTDIR\BecherGame_d.exe" "" "$INSTDIR\BecherGame_d.exe" 0
  
;SectionEnd


;--------------------------------
;Installer Functions

;Function .onInit

  ;!insertmacro MUI_LANGDLL_DISPLAY

;FunctionEnd


;--------------------------------

; Uninstaller

Section "Uninstall"
  
  ; Remove registry keys
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\HoeSdk"
  DeleteRegKey HKLM SOFTWARE\HoeSdk

  ; odebrat data files
  Delete "$INSTDIR\data\*.*"
  Delete "$INSTDIR\resource\*.*"
  Delete "$INSTDIR\scripts\*.*"
  Delete "$INSTDIR\sound\*.*"
	
  ; Remove files and uninstaller
  ;Delete $INSTDIR\BecherGame_d.exe

  ; Remove shortcuts, if any
  ;Delete "$SMPROGRAMS\HoeSdk\*.*"

  ; Remove directories used
  RMDir "$INSTDIR"

SectionEnd

;--------------------------------
;Uninstaller Functions

;Function un.onInit

;  !insertmacro MUI_UNGETLANGUAGE
  
;FunctionEnd