; Script generated by the HM NIS Edit Script Wizard.

; HM NIS Edit Wizard helper defines
!define PRODUCT_NAME "Benubird PDF Pro"
!define PRODUCT_VERSION "1.4.0.9.34"
!define PRODUCT_PUBLISHER "Debenu Pty. Ltd."
!define PRODUCT_WEB_SITE "http://www.debenu.com/"
!define PRODUCT_DIR_REGKEY "Software\Microsoft\Windows\CurrentVersion\App Paths\BenubirdPDFPro.exe"
!define PRODUCT_UNINST_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}"
!define PRODUCT_UNINST_ROOT_KEY "HKLM"

; Replaces Nullsoft Scriptable Install branding text

BrandingText "Benubird PDF Pro"

; Version Information

  VIProductVersion "1.4.0.9.34"
  VIAddVersionKey "ProductName" "Benubird PDF Pro"
  VIAddVersionKey "Comments" "Product Installer"
  VIAddVersionKey "CompanyName" "Debenu Pty. Ltd."
  VIAddVersionKey "LegalTrademarks" "Benubird PDF Pro is a trademark of Debenu Pty. Ltd."
  VIAddVersionKey "LegalCopyright" "(C)2007-2012 Debenu Pty. Ltd."
  VIAddVersionKey "FileDescription" "benubird_pdf_en.exe"
  VIAddVersionKey "FileVersion" "1.4.0.9.34"

; MUI 1.67 compatible ------
!include "MUI.nsh"
!include "Library.nsh"
!include "UpgradeDLL.nsh"

; Define Library COM for registering COM DLLs

 !define LIBRARY_COM

; !define BenubirdShell '"$INSTDIR\BenubirdShell.dll"'

; !define BenubirdShell64 '"$INSTDIR\BenubirdShell64.dll"'

; !define iSED '"$INSTDIR\DebenuPDFLibraryAX0912.dll"'

; Request application privileges for Windows Vista
RequestExecutionLevel admin

; MUI Settings
!define MUI_ABORTWARNING
!define MUI_ICON "res\benubird_installer_icon_64.ico"
!define MUI_UNICON "res\benubird_installer_icon_64.ico"

; MUI Wizard
!define MUI_WELCOMEFINISHPAGE_BITMAP "res\installLogo.bmp"

; Welcome page
!insertmacro MUI_PAGE_WELCOME
; License page
!insertmacro MUI_PAGE_LICENSE "License.txt"
; Directory page
!insertmacro MUI_PAGE_DIRECTORY
; Instfiles page
!insertmacro MUI_PAGE_INSTFILES
; Finish page
!define MUI_FINISHPAGE_RUN "$INSTDIR\BenubirdPDFPro.exe"
!define MUI_FINISHPAGE_SHOWREADME_TEXT "Show Quick Start Guide"
!define MUI_FINISHPAGE_SHOWREADME "$INSTDIR\Benubird PDF Quick Start Guide.pdf"
!define MUI_PAGE_CUSTOMFUNCTION_LEAVE BenubirdCustomActions
!insertmacro MUI_PAGE_FINISH

; Uninstaller pages
!insertmacro MUI_UNPAGE_INSTFILES

; Language files
!insertmacro MUI_LANGUAGE "English"

; MUI end ------

Name "${PRODUCT_NAME} ${PRODUCT_VERSION}"
OutFile "..\benubird_pdf_Pro_en.exe"
InstallDir "$PROGRAMFILES\Benubird PDF Pro"
InstallDirRegKey HKLM "${PRODUCT_DIR_REGKEY}" ""
ShowInstDetails nevershow
ShowUnInstDetails nevershow

Section "MainSection" SEC01
  Call StopBenubirdAssistantProcess
  SetOutPath "$INSTDIR"
  SetOverwrite ifnewer
  File "BenubirdPDFPro.exe"
  CreateDirectory "$SMPROGRAMS\Benubird PDF Pro"
  CreateShortCut "$SMPROGRAMS\Benubird PDF Pro\Benubird PDF Pro.lnk" "$INSTDIR\BenubirdPDFPro.exe"
  CreateShortCut "$DESKTOP\Benubird PDF Pro.lnk" "$INSTDIR\BenubirdPDFPro.exe"
  CreateShortCut "$QUICKLAUNCH.lnk" "$INSTDIR\BenubirdPDFPro.exe"
  CreateShortCut "$STARTMENU\Benubird PDF Pro.lnk" "$INSTDIR\BenubirdPDFPro.exe"
  File "README.txt"
  CreateShortCut "$STARTMENU.lnk" "$INSTDIR\README.txt"
  File "doc\Benubird PDF Quick Start Guide.pdf"
  CreateShortCut "$STARTMENU.lnk" "$INSTDIR\Benubird PDF Quick Start Guide.pdf"
  File "BenubirdShell.dll"
  File "BenubirdShell64.dll"
  File "DebenuPDFLibraryAX0912.dll"
;  File "sqlite3.dll"
  File "BenubirdAssistant.exe"
  File "novapv.exe"
  Call RegisterBenubirdShell

!define LIBRARY_X64
  Call RegisterBenubirdShell64
!undef LIBRARY_X64

  Call RegisteriSED
  Call InstallNovaPDF
  Call StartBenubirdAssistantProcess
SectionEnd

Section -AdditionalIcons
  WriteIniStr "$INSTDIR\${PRODUCT_NAME}.url" "InternetShortcut" "URL" "${PRODUCT_WEB_SITE}"
  CreateShortCut "$SMPROGRAMS\Benubird PDF Pro\Website.lnk" "$INSTDIR\${PRODUCT_NAME}.url"
  CreateShortCut "$SMPROGRAMS\Benubird PDF Pro\Uninstall.lnk" "$INSTDIR\uninst.exe"
  CreateShortCut "$SMPROGRAMS\Benubird PDF Pro\README.lnk" "$INSTDIR\README.txt"
  CreateShortCut "$SMPROGRAMS\Benubird PDF Pro\Benubird Quick Start Guide.lnk" "$INSTDIR\Benubird PDF Quick Start Guide.pdf"
SectionEnd

Section -Post
  WriteUninstaller "$INSTDIR\uninst.exe"
  WriteRegStr HKLM "${PRODUCT_DIR_REGKEY}" "" "$INSTDIR\BenubirdPDF.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayName" "$(^Name)"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "UninstallString" "$INSTDIR\uninst.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayIcon" "$INSTDIR\BenubirdPDF.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayVersion" "${PRODUCT_VERSION}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "URLInfoAbout" "${PRODUCT_WEB_SITE}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "Publisher" "${PRODUCT_PUBLISHER}"
SectionEnd

Function RegisterBenubirdShell

;	  ${If} ${FileExists} "$INSTDIR\BenubirdShell.dll"
;			!insertmacro UpgradeDLL "BenubirdShell.dll" "$INSTDIR\BenubirdShell.dll" "$TEMP"
;		${Else}
;	  	File "BenubirdShell.dll"
;			RegDLL $INSTDIR\BenubirdShell.dll
;	  ${EndIf}

; !insertmacro InstallLib REGDLL NOTSHARED REBOOT_NOTPROTECTED "BenubirdShell.dll" $INSTDIR\BenubirdShell.dll $TEMP

; !insertmacro InstallLib REGDLL NOTSHARED REBOOT_NOTPROTECTED "BenubirdShell.dll" $INSTDIR\BenubirdShell.dll $INSTDIR

 ExecWait 'regsvr32.exe /s "$INSTDIR\BenubirdShell.dll"'

 ExecWait '"$INSTDIR\BenubirdPDF.exe" /s /register'
		
FunctionEnd


Function RegisterBenubirdShell64
!define LIBRARY_X64


;	  ${If} ${FileExists} "$INSTDIR\BenubirdShell64.dll"
;			!insertmacro UpgradeDLL "BenubirdShell64.dll" "$INSTDIR\BenubirdShell64.dll" "$TEMP"
;		${Else}
;	  	File "BenubirdShell64.dll"
;			RegDLL $INSTDIR\BenubirdShell64.dll
;	  ${EndIf}

; !insertmacro InstallLib REGDLL NOTSHARED REBOOT_NOTPROTECTED "BenubirdShell64.dll" $INSTDIR\BenubirdShell64.dll $TEMP

; !insertmacro InstallLib REGDLL NOTSHARED REBOOT_NOTPROTECTED "BenubirdShell64.dll" $INSTDIR\BenubirdShell64.dll $INSTDIR

 ExecWait 'regsvr32.exe /s "$INSTDIR\BenubirdShell64.dll"'


!undef LIBRARY_X64

FunctionEnd


Function RegisteriSED

; !insertmacro InstallLib REGDLL NOTSHARED REBOOT_NOTPROTECTED "DebenuPDFLibraryAX0912.dll" $INSTDIR\DebenuPDFLibraryAX0912.dll $INSTDIR

 ExecWait 'regsvr32.exe /s "$INSTDIR\DebenuPDFLibraryAX0912.dll"'
	
	; If statement to see if QuickPDFAX0719.dll is already installed,
	; if it is already installed then app tries to upgrade it.
	
	;	  ${If} ${FileExists} "$INSTDIR\DebenuPDFLibraryAX0912.dll"
	;		!insertmacro UpgradeDLL "DebenuPDFLibraryAX0912.dll" "$INSTDIR\DebenuPDFLibraryAX0912.dll" "$TEMP"
	;	${Else}
	;  	File "DebenuPDFLibraryAX0912.dll"
	;		!insertmacro InstallLib REGDLL NOTSHARED REBOOT_NOTPROTECTED "DebenuPDFLibraryAX0912.dll" $INSTDIR\DebenuPDFLibraryAX0912.dll $TEMP
 	; ${EndIf}
	
FunctionEnd

Function InstallNovaPDF
;MessageBox MB_ICONINFORMATION|MB_OK '"$INSTDIR\novapv.exe" /VERYSILENT /SUPPRESSMSGBOXES /NOICONS /NOCANCEL /NORESTART /PrinterName="Add to Benubird PDF as PDF" /OEMCompany="Debenu Pty Ltd" /OEMApplication="Benubird PDF Pro" /OEMSaveFolder="%AppData%\Benubird\Conversion\" /OEMActionApp="$PROGRAMFILES\Benubird PDF Pro\BenubirdPDFPro.exe" /RegisterName="Debenu Pty Ltd" /RegisterKey="91BA-LQ0T-XEYM-Z63R-AMKF-V7TM-L8WB-LFB4"'

	ExecWait '"$INSTDIR\novapv.exe" /VERYSILENT /SUPPRESSMSGBOXES /NOICONS /NOCANCEL /NORESTART /PrinterName="Add to Benubird PDF as PDF" /OEMCompany="Debenu Pty Ltd" /OEMApplication="Benubird PDF Pro" /OEMSaveFolder="%AppData%\Benubird\Conversion\" /OEMActionApp="$PROGRAMFILES\Benubird PDF Pro\BenubirdPDFPro.exe" /RegisterName="Debenu Pty Ltd" /RegisterKey="91BA-LQ0T-XEYM-Z63R-AMKF-V7TM-L8WB-LFB4"' $0
	DetailPrint "Some program return $0"
FunctionEnd

Function StartBenubirdAssistantProcess
	
		; This function starts the BenubirdAssistant.exe process after
		; Benubird PDF has been installed. It also writes a new entry
		; in the registry that starts the BenubirdAssistant.exe process
		; whenever the machine startsup.
		
		; This code starts BenubirdAssistant.exe after installation.
			
		ExecShell "open" "$INSTDIR\BenubirdAssistant.exe" SW_HIDE

		; This code ensures the BenubirdAssistant.exe prcess loads on startup.
		
		WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Run" "Benubird PDF Pro" "$INSTDIR\BenubirdAssistant.exe"

FunctionEnd

Function BenubirdCustomActions
  ExecShell "open" "http://www.debenu.com/benubird/thanks_for_installing_benubird.html"
  ReadEnvStr $0 COMSPEC
FunctionEnd

Function StopBenubirdAssistantProcess
	
		; This function finds BenubirdAssistant.exe in the processes
		; and kills its. BenubirdAssistant.exe must be killed before
		; it can be replaced with a newer version. It must also be
		; stopped if you are upgrading BenubirdShell.dll.
	
  Processes::FindProcess "BenubirdAssistant"
    Pop $R0

    StrCmp $R0 "1" make_my_day noooooo

    make_my_day:
      DetailPrint "-> Killed $0 processes, failed to kill $1 processes"

    noooooo:
      DetailPrint "In the process of installing Benubird PDF"

  Processes::KillProcess "BenubirdAssistant"
    Pop $R0

    StrCmp $R0 "1" dead_meat why_wont_you_die

  dead_meat:
     DetailPrint "-> Killed $0 processes, faild to kill $1 processes"

  why_wont_you_die:
    DetailPrint "In the process of installing Benubird PDF"
FunctionEnd

; ===================================================================================
; = FUNCTION AREA: Uninstall																												=
; = This area deals with functions that are used during the uninstallation process. =
; ===================================================================================

Function un.RegisterBenubirdShell
	ExecWait 'regsvr32.exe /u /s "$INSTDIR\BenubirdShell.dll"'
	; DetailPrint "Unregistering BenubirdShell.dll"
	
	; !insertmacro UninstallLib REGDLL NOTSHARED REBOOT_NOTPROTECTED "$INSTDIR\BenubirdShell.dll"
	
FunctionEnd

Function un.RegisteriSED
	ExecWait 'regsvr32.exe /s "$INSTDIR\DebenuPDFLibraryAX0912.dll"'
;	DetailPrint "Unregistering BenubirdShell.dll"

; !insertmacro UninstallLib REGDLL NOTSHARED REBOOT_NOTPROTECTED "$INSTDIR\DebenuPDFLibraryAX0912.dll"
FunctionEnd


Function un.UninstallNovaPDF
	

;	MessageBox MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2 "Do you wish to uninstall the Benubird PDF Driver?" IDNO NoDelete


	ExecWait '"$PROGRAMFILES\Softland\novaPDF OEM 7\unins000.exe" /SILENT /VERYSILENT /SUPPRESSMSGBOXES /NORESTART'
	DetailPrint "Some program return $0"

!define LIBRARY_X64	
	ExecWait '"$PROGRAMFILES64\Softland\novaPDF OEM 7\unins000.exe" /SILENT /VERYSILENT /SUPPRESSMSGBOXES /NORESTART'
	DetailPrint "Some program return $0"
!undef LIBRARY_X64


;  NoDelete:	

FunctionEnd

Function un.RemoveLibrary
	
		; This function displays a message box that asks the user 
		; if they wish to remove the database, repository and 
		; workspace for Benubird PDF from their machine when uninstalling.
	
	  MessageBox MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2 "Do you wish to remove the Benubird PDF library and all of the files you have added to the file repository?" IDNO NoDelete
    RMDir /r "$APPDATA\Benubird" ; skipped if no
  NoDelete:
FunctionEnd

Function un.onUninstSuccess
  HideWindow
  MessageBox MB_ICONINFORMATION|MB_OK "$(^Name) was successfully removed from your computer."
FunctionEnd

Function un.onInit
  MessageBox MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2 "Are you sure you want to completely remove $(^Name) and all of its components?" IDYES +2
FunctionEnd

Function un.StopBenubirdAssistantProcess
	
		; This function finds BenubirdAssistant.exe in the processes
		; and kills its. BenubirdAssistant.exe must be killed before
		; BenubirdShell.dll can be unregistered and deleted.
	
  Processes::FindProcess "BenubirdAssistant"
    Pop $R0

    StrCmp $R0 "1" make_my_day noooooo

    make_my_day:
      DetailPrint "-> Killed $0 processes, failed to kill $1 processes"

    noooooo:
      DetailPrint "In the process of uninstalling Benubird PDF"

  Processes::KillProcess "BenubirdAssistant"
    Pop $R0

    StrCmp $R0 "1" dead_meat why_wont_you_die

  dead_meat:
     DetailPrint "-> Killed $0 processes, faild to kill $1 processes"

  why_wont_you_die:
    DetailPrint "In the process of uninstalling Benubird PDF"

FunctionEnd

Section Uninstall
	ReadEnvStr $0 COMSPEC
  Call un.StopBenubirdAssistantProcess
	Call un.RegisterBenubirdShell
	Call un.RegisteriSED
	Call un.UninstallNovaPDF
  Delete "$INSTDIR\${PRODUCT_NAME}.url"
  Delete "$INSTDIR\uninst.exe"
;  Delete "$INSTDIR\sqlite3.dll"
  Delete "$INSTDIR\Benubird PDF Quick Start Guide.pdf"
  Delete "$INSTDIR\README.txt"
  Delete "$INSTDIR\BenubirdPDF.exe"
  Delete "$INSTDIR\DebenuPDFLibraryAX0912.dll"
  Delete "$INSTDIR\BenubirdShell.dll"
  Delete "$INSTDIR\BenubirdShell64.dll"
  Delete "$INSTDIR\BenubirdAssistant.exe"
	Delete "$INSTDIR\BenubirdAssistant.log"
  Delete "$INSTDIR\BenubirdPro.log"
  Delete "$INSTDIR\novapv.exe"

  Delete "$SMPROGRAMS\Benubird PDF Pro\Benubird PDF Pro.lnk"
  Delete "$SMPROGRAMS\Benubird PDF Pro\README.lnk"
  Delete "$SMPROGRAMS\Benubird PDF Pro\Benubird Quick Start Guide.lnk"
  Delete "$SMPROGRAMS\Benubird PDF Pro\Website.lnk"
  Delete "$SMPROGRAMS\Benubird PDF Pro\Uninstall.lnk"

  Delete "$STARTMENU\Benubird PDF Pro.lnk"
  Delete "$QUICKLAUNCH\Benubird PDF Pro.lnk"
  Delete "$DESKTOP\Benubird PDF Pro.lnk"
	Call un.RemoveLibrary

  RMDir /REBOOTOK "$SMPROGRAMS\Benubird PDF Pro"

  RMDir /r /REBOOTOK "$INSTDIR"
  RMDir ""

  DeleteRegKey ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}"
  DeleteRegKey HKLM "${PRODUCT_DIR_REGKEY}"
  SetAutoClose true
	IfRebootFlag 0 noreboot

	MessageBox MB_YESNO "A reboot is required to finish the installation. Do you wish to reboot now?" IDNO noreboot
    Reboot
	noreboot:
SectionEnd