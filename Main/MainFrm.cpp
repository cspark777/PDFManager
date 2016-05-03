// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "Define.h"
#include "Benubird.h"
#include "CRegistry.h"

#include "MainFrm.h"
#include "LibraryDoc.h"
#include "LibraryView.h"
#include "PDFDoc.h"
#include "PDFView.h"
#include "CChangedFilesDlg.h"
#include "COptionsDefs.h"
#include "CBRepository.h"
#include "CBWorkspace.h"
#include "CAboutDlg.h"
#include "CCheckForUpdates.h"
#include "CRegisterBenubirdProDlg.h"
#include "CPDFPropertiesFrame.h"
#include "CPDFPropertiesDlg.h"
#include "COptionsDlg.h"
#include "CContentSearchDlg.h"

#include <Windows.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BOOL gIsTagStr;
BOOL gIsQuickBrowseStr;

extern bool gIsPro;
extern HWND g_AssistantHwnd;
extern CBenubirdApp theApp;

extern CString GetDefaultPath(CString key);

///////////////////////////////////////////////////////////////////////////////////
// CMainFrame
///////////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(CMainFrame, CBCGPMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CBCGPMDIFrameWnd)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_WM_SIZE()
	ON_WM_CLOSE()
	ON_WM_TIMER()

	ON_COMMAND(ID_VIEW_CUSTOMIZE, OnViewCustomize)
	ON_COMMAND(ID_OPTIONS, OnOptions)
	ON_COMMAND_RANGE(ID_VIEW_APPLOOK_2007, ID_VIEW_APPLOOK_2007_3, OnAppLook)
	ON_COMMAND_RANGE(ID_HELP_USER_GUIDE, ID_UPGRADE_BENUBIRD_PRO, OnHelp)
	ON_COMMAND(ID_OPEN_BENUBIRD, OnOpenBenubird)
	ON_COMMAND(ID_SHOWMODIFIEDFILES, OnShowModifiedFiles)
	ON_UPDATE_COMMAND_UI(ID_SHOWMODIFIEDFILES, OnUpdateShowModifiedFiles)
	ON_COMMAND_RANGE(ID_SEARCH_FOCUS, ID_SEARCH_OPTION_TAGS, OnSearch)	
	ON_UPDATE_COMMAND_UI_RANGE(ID_SEARCH_FOCUS, ID_SEARCH_OPTION_TAGS, OnUpdateSearch)
	ON_UPDATE_COMMAND_UI(ID_STATUSBAR_SELECTEDFILES, OnUpdateStatusBar)
	ON_COMMAND(ID_FIND_CONTENT, OnContentSearch)
	ON_UPDATE_COMMAND_UI(ID_FIND_CONTENT, OnUpdateContentSearch)

	ON_MESSAGE(UM_TRAYNOTIFY, OnTrayNotify)

	ON_REGISTERED_MESSAGE(BENUBIRD_ASSISTANT_REFRESH_FILE, OnRefreshFile)
	ON_REGISTERED_MESSAGE(BENUBIRD_ASSISTANT_REFRESH_ALLFILES, OnRefreshAllFiles)
	ON_REGISTERED_MESSAGE(BCGM_ON_CHANGE_RIBBON_CATEGORY, OnActivateCategory)

	//for library

	ON_UPDATE_COMMAND_UI(ID_LIBRARY_NEW, &CMainFrame::OnUpdateAlwaysEnabledButton/*OnUpdateAlwaysDisabledButton*/)
	ON_UPDATE_COMMAND_UI(ID_LIBRARY_OPEN, &CMainFrame::OnUpdateAlwaysEnabledButton/*OnUpdateAlwaysDisabledButton*/)
	ON_UPDATE_COMMAND_UI(ID_LIBRARY_IMPORT, &CMainFrame::OnUpdateAlwaysEnabledButton/*OnUpdateAlwaysDisabledButton*/)
	ON_UPDATE_COMMAND_UI(ID_LIBRARY_EXPORT, &CMainFrame::OnUpdateAlwaysEnabledButton/*OnUpdateAlwaysDisabledButton*/)

	ON_WM_COPYDATA()
	ON_WM_ACTIVATE()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

CMainFrame *gMainFrm = NULL;


/////////////////////////////////////////////////////////////////////////////
// CMyCommandLineInfo 
///////////////////////////////////////////////////////////////////////////////////
CMyCommandLineInfo::CMyCommandLineInfo(void) : CCommandLineInfo(), m_bHelp(FALSE), m_bSuccess(TRUE)
{
}

void CMyCommandLineInfo::ParseParam(LPCTSTR lpszParam, BOOL bSwitch, BOOL /*bLast*/)
{
   if (bSwitch)
   {
      if (0 == lstrcmpi(_T("help"), lpszParam))
      {
         m_bHelp = TRUE;
      }
      else
      {
         // the for loop enables 'compound' switches like "/XYZ"
         BOOL bContinue = TRUE;
         for(int i = 0; (i < lstrlen(lpszParam)) && m_bSuccess && bContinue; i++)
         {
            switch(lpszParam[i])
            {
               case _T('?'):
               case _T('h'):
               case _T('H'):
                  m_bHelp = TRUE;
                  break;
               default:
                  m_bSuccess = bContinue = FALSE;
                  break;
            }
         }
      }
   }
   else
   {
      m_strParameter = lpszParam;
   }
}

///////////////////////////////////////////////////////////////////////////////////
// CMainFrame constructor/desctructor/ application look
///////////////////////////////////////////////////////////////////////////////////
CMainFrame::CMainFrame()
{

	mDefaultStyle = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	gMainFrm = this;
	mSearchMode = kSearchAll;
	mSearchEdit = NULL;
	m_ProgressBar = NULL;
	m_ProgressLabel = NULL;
	m_ProgressText = _T("");

	m_pAutoProxy = NULL;
	m_nChildWindowCount = 0;

	memset(&m_nid, 0 , sizeof(m_nid));


}

CMainFrame::~CMainFrame()
{

}

#define MSGFLT_RESET                            (0)
#define MSGFLT_ALLOW                            (1)
#define MSGFLT_DISALLOW                         (2)
#define MSGFLT_ADD 1
#define MSGFLT_REMOVE 2

typedef struct tagCHANGEFILTERSTRUCT {
    DWORD cbSize;
    DWORD ExtStatus;
} CHANGEFILTERSTRUCT, *PCHANGEFILTERSTRUCT;


DWORD CMainFrame::AllowMsg(UINT msg)
{

	typedef BOOL (WINAPI *FPCHANGEWINDOWMESSAGEFILTEREX)(HWND hWnd,UINT message,DWORD action,PCHANGEFILTERSTRUCT pChangeFilterStruct);
	typedef BOOL (WINAPI *FPCHANGEWINDOWMESSAGEFILTER)(UINT message,DWORD dwFlag);
	

	HMODULE hUser32 = LoadLibrary(_T("user32.dll"));
	if (hUser32)
	{
		// Try ChangeWindowMessageFilterEx first (Win7+)
		FPCHANGEWINDOWMESSAGEFILTEREX ChangeWindowMessageFilterEx = (FPCHANGEWINDOWMESSAGEFILTEREX)GetProcAddress(hUser32, "ChangeWindowMessageFilterEx");
		if (ChangeWindowMessageFilterEx)
		{
			ChangeWindowMessageFilterEx(this->GetSafeHwnd(), msg, MSGFLT_ALLOW, nullptr);
		}
		else
		{
			// Try ChangeWindowMessageFilter (Vista)
			FPCHANGEWINDOWMESSAGEFILTER ChangeWindowMessageFilter = (FPCHANGEWINDOWMESSAGEFILTER)GetProcAddress(hUser32, "ChangeWindowMessageFilter");
			if (ChangeWindowMessageFilter)
			{
				ChangeWindowMessageFilter(msg, MSGFLT_ADD);
			}
		}

		FreeLibrary(hUser32);
	}
	return msg;
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBCGPMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	OnAppLook (theApp.m_nAppLook);

	// Status bar
	if (!CreateStatusBar())
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
	
	// Ribbon bar
	if (!CreateRibbonBar())
	{
		TRACE0("Failed to create ribbon bar\n");
		return -1;      // fail to create
	}

	PrepareSysTrayIcon();
	if(gIsPro)
		SetWindowText(_T("Benubird PDF Pro"));
	else
		SetWindowText(_T("Benubird PDF"));

	CBCGPMDITabParams mdiTabParams;

	mdiTabParams.m_style = CBCGPTabWnd::STYLE_3D_VS2005;
	mdiTabParams.m_bAutoColor = FALSE;
	mdiTabParams.m_bDocumentMenu = TRUE;
	mdiTabParams.m_bActiveTabCloseButton = TRUE;

	EnableMDITabbedGroups (TRUE, mdiTabParams);

	OutputDebugString(_T("------------------------  2"));

	AllowMsg(WM_COPYDATA);

	//init Assistant
	HWND hwnd = this->GetSafeHwnd();
//	SendMessageTimeout(HWND_BROADCAST, BENUBIRD_ASSISTANT_INIT, (WPARAM)hwnd, 0, 0, 100, NULL);
	::PostMessage(HWND_BROADCAST, BENUBIRD_ASSISTANT_INIT, (WPARAM)hwnd, 0);
//	LRESULT result = ::SendMessage(HWND_BROADCAST, BENUBIRD_ASSISTANT_INIT, (WPARAM)hwnd, 0);
//	DWORD dw = GetLastError()
	return 0;
}

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
/*    CLibraryDoc* pDoc = DYNAMIC_DOWNCAST(CLibraryDoc, pContext->m_pCurrentDoc);

    if(pDoc != NULL)
    {
        pDoc->CreateViews(this, pContext);
    }
*/
    return CBCGPMDIFrameWnd::OnCreateClient(lpcs, pContext);
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CBCGPMDIFrameWnd::PreCreateWindow(cs))
		return FALSE;

	//You registered class, now you need to set this class for the window:
	cs.lpszClass = UniqueBenubirdClassName;
	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.style&=~(LONG)FWS_ADDTOTITLE;
	return TRUE;
}

void CMainFrame::OnSize(UINT nType, int cx, int cy)
{
	CBCGPMDIFrameWnd::OnSize(nType, cx, cy);

	CWinApp* lApp;
	lApp = AfxGetApp();
	int lMinimalizeToSystemTray = lApp->GetProfileInt(kOptions, _T("MinimizeToSystemTray"), BST_UNCHECKED);
	int lShowIconInSystemTray = lApp->GetProfileInt(kOptions, _T("ShowIconInSystemTray"), BST_UNCHECKED);

	if ( lMinimalizeToSystemTray == BST_CHECKED && lShowIconInSystemTray == BST_CHECKED)
	{
		if (nType == SIZE_MINIMIZED)
		{
			ShowWindow(SW_HIDE);
		}
		if (nType == SIZE_RESTORED)
		{
		}
	}
}

// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CBCGPMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CBCGPMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// otherwise, do default handling
	return CBCGPMDIFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void CMainFrame::OnViewCustomize()
{
	//------------------------------------
	// Create a customize toolbars dialog:
	//------------------------------------
	CBCGPToolbarCustomize* pDlgCust = new CBCGPToolbarCustomize (this,
		TRUE /* Automatic menus scaning */);

	pDlgCust->Create ();
}

void CMainFrame::OnAppLook(UINT id)
{
	CBCGPDockManager::SetDockMode(BCGP_DT_SMART);

	theApp.m_nAppLook = id;

	CBCGPTabbedControlBar::m_StyleTabWnd = CBCGPTabWnd::STYLE_3D;

	switch (theApp.m_nAppLook)
	{
		case ID_VIEW_APPLOOK_2007:
			CBCGPVisualManager2007::SetStyle (CBCGPVisualManager2007::VS2007_LunaBlue);
			break;

		case ID_VIEW_APPLOOK_2007_1:
			CBCGPVisualManager2007::SetStyle (CBCGPVisualManager2007::VS2007_ObsidianBlack);
			break;

		case ID_VIEW_APPLOOK_2007_2:
			CBCGPVisualManager2007::SetStyle (CBCGPVisualManager2007::VS2007_Silver);
			break;
	}

	CBCGPVisualManager::SetDefaultManager (RUNTIME_CLASS (CBCGPVisualManager2007));
	CBCGPDockManager::SetDockMode (BCGP_DT_SMART);
	
	CBCGPDockManager* pDockManager = GetDockManager ();
	if (pDockManager != NULL)
	{
		ASSERT_VALID (pDockManager);
		pDockManager->AdjustBarFrames ();
	}

	CBCGPTabbedControlBar::ResetTabs ();

	RecalcLayout ();
	RedrawWindow (NULL, NULL, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE);

	theApp.WriteInt (_T("ApplicationLook"), theApp.m_nAppLook);
}

BOOL CMainFrame::CreateRibbonBar ()
{
	if (!m_wndRibbonBar.Create(this))
	{
		return FALSE;
	}

	CBCGPRibbonButton* lSubButton;
	CBCGPRibbonCategory* lCategory;
	CBCGPRibbonPanel*    lPanel;
	CBCGPRibbonButtonsGroup* lGroup;
	CRibbonFindEdit* lEdit;
	CBCGPRibbonButton* lButton;
	CBCGPToolBarImages lPanelImages;
	CBCGPToolBarImages lPanelImages1;

	lPanelImages.SetImageSize(CSize(16, 16));
	//****************************************************************************
	// Create 'Home' category
	//****************************************************************************
	lPanelImages.Load(IDB_RB_PANELS_HOME_BMP);
	lCategory = m_wndRibbonBar.AddCategory(_T("Home"), IDB_RB_HOME_SMALL_BMP, IDB_RB_HOME_LARGE_BMP);

	if(gIsPro)
	{
		// Create 'Library Panel 
		lPanel = lCategory->AddPanel(_T("Library"), lPanelImages.ExtractIcon(0));
		lPanel->Add(new CBCGPRibbonButton(ID_LIBRARY_NEW, _T("New Library\n"), 0, 0)); //   Need Alt shortcut
		lPanel->Add(new CBCGPRibbonButton(ID_LIBRARY_OPEN, _T("Open Library\n"), 1, 1)); //   Need Alt shortcut
		lPanel->Add(new CBCGPRibbonButton(ID_LIBRARY_IMPORT, _T("Import Library\n"), 2, 2)); // Need Alt shortcut
		lPanel->Add(new CBCGPRibbonButton(ID_LIBRARY_EXPORT, _T("Export Library\n"), 3, 3)); // Need Alt shortcut
	}

	// Create 'Import' panel
	lPanel = lCategory->AddPanel(_T("Import"), lPanelImages.ExtractIcon(0));
	lPanel->Add(new CBCGPRibbonButton(ID_ADD_FILE, _T("From File\n"), 4, 4)); // Need Alt shortcut
	lPanel->Add(new CBCGPRibbonButton(ID_IMPORT_FOLDER, _T("From Folder\n"), 5, 5));
	//lPanel->Add(new CBCGPRibbonButton(ID_IMPORT_ZIP, _T("From ZIP\n"), 6, 6));
	
	// Create 'Create' panel
	lPanel = lCategory->AddPanel(_T("Create"), lPanelImages.ExtractIcon(1));
	lPanel->Add(new CBCGPRibbonButton(ID_CREATE_COLLECTIONS, _T("Collection\n"), 7, 7)); // Need Alt shortcut
	lPanel->Add(new CBCGPRibbonButton(ID_CREATE_SUB_COLLECTIONS, _T("Sub-Collection\n"), 8, 8)); // Need Alt shortcut

	lPanel->Add(new CBCGPRibbonButton(ID_CREATE_SMART_COLLECTIONS, _T("Smart Collection\n"), 9, 9)); // Need Alt shortcut
	
	// Create 'Export' panel
	//lPanelImages.Load(IDB_RB_PANELS_MANAGE_BMP);
	lPanel = lCategory->AddPanel(_T("Export"), lPanelImages.ExtractIcon(2));
	lPanel->Add(new CBCGPRibbonButton(ID_EXPORT_ZIP, _T("To Zip\n"), 10, 10));
	lPanel->Add(new CBCGPRibbonButton(ID_EXPORT_FOLDER, _T("To Folder\n"), 11, 11));
	lPanel->Add(new CBCGPRibbonButton(ID_SHARE_EMAIL, _T("Send Email\n"), 12, 12));

	// Create 'Manage' panel
	//lPanelImages.Load(IDB_RB_PANELS_MANAGE_BMP);
	lPanel = lCategory->AddPanel(_T("Manage"), lPanelImages.ExtractIcon(3));
	lPanel->Add(new CBCGPRibbonButton(ID_METADATA_TAGS, _T("Library Tags\n"), 13, 13));

	// Create 'Watched folders' panel
	lPanel->Add(new CBCGPRibbonButton(ID_FOLDERS_WATCHED, _T("Watched Folders\n"), 14, 14));
	lPanel->Add(new CBCGPRibbonButton(ID_LIBRARY_SETTING, _T("Library Setting\n"), 15, 15));
	
	// Create 'Show/Hide' panel
	//lPanelImages.Load(IDB_RB_PANELS_HOME_BMP);
	lPanel = lCategory->AddPanel(_T("Show/Hide"), lPanelImages.ExtractIcon(4));
	lPanel->Add(new CBCGPRibbonCheckBox(ID_VIEW_FILTER, _T("Navigator\n")));
	lPanel->Add(new CBCGPRibbonCheckBox(ID_VIEW_TAGS, _T("Tags\n")));
	lPanel->Add(new CBCGPRibbonCheckBox(ID_VIEW_PREVIEW, _T("Preview\n")));
	lPanel->Add(new CBCGPRibbonRadioButton(ID_VIEW_FILELIST, _T("List\n")));
	lPanel->Add(new CBCGPRibbonRadioButton(ID_VIEW_ICONS, _T("Icons\n")));
	lPanel->Add(new CBCGPRibbonRadioButton(ID_VIEW_THUMBNAILS, _T("Thumbnails\n")));

	lPanel->SetCenterColumnVert(TRUE);

	// Create 'Show' panel
	//lPanel = lCategory->AddPanel(_T("Show"), lPanelImages.ExtractIcon(4));
	//lPanel->Add(new CBCGPRibbonButton(ID_VAULT_VIEW, _T("Files to smarten\n"), 5, 5));


	// Create 'Search' panel
	lPanel = lCategory->AddPanel(_T("Search"), lPanelImages.ExtractIcon(5));

	lGroup = new CBCGPRibbonButtonsGroup;
	//lEdit = new CBCGPRibbonEdit(ID_SEARCH_EDIT, 100);
	lEdit = new CRibbonFindEdit(ID_SEARCH_EDIT, 100);
	mSearchEdit = lEdit;
	lEdit->SetEditText(_T("Find..."));
	lButton = new CBCGPRibbonButton(ID_FIND_CONTENT, _T("Find In Content\n"), 22, -1);
	lPanel->Add(lButton);
	lButton = new CBCGPRibbonButton(ID_SEARCH_OPTION, _T("\n"), -1, -1);
	lButton->AddSubItem(new CBCGPRibbonButton(ID_SEARCH_OPTION_ALL, _T("All"), 17));
	lButton->AddSubItem(new CBCGPRibbonSeparator(TRUE));
	lButton->AddSubItem(new CBCGPRibbonButton(ID_SEARCH_OPTION_TITLE, _T("Title"), 18));
	lButton->AddSubItem(new CBCGPRibbonButton(ID_SEARCH_OPTION_AUTHOR, _T("Author"), 19));
	lButton->AddSubItem(new CBCGPRibbonButton(ID_SEARCH_OPTION_SUBJECT, _T("Subject"), 20));
	lButton->AddSubItem(new CBCGPRibbonSeparator(TRUE));
	lButton->AddSubItem(new CBCGPRibbonButton(ID_SEARCH_OPTION_TAGS, _T("Tags"), 21));
	lButton->SetDefaultCommand(FALSE);
	lGroup->AddButton(lEdit);
	lGroup->AddButton(lButton);
	lPanel->Add(lGroup);
	
	lPanel->SetCenterColumnVert(TRUE);

	if(!gIsPro)
	{
	// Load the upgrade panel
		//lPanelImages.Load(IDB_RB_UPGRADE_LARGE);
		lPanel = lCategory->AddPanel(_T("Upgrade..."), lPanelImages.ExtractIcon(6));
		lPanel->Add(new CBCGPRibbonButton(ID_FREE_UPGRADE, _T("Tell me about the Pro version"),16,16));
	}

	//****************************************************************************
	// Create 'Manage' category
	//****************************************************************************
	/*lPanelImages.Load(IDB_RB_PANELS_MANAGE_BMP);
	lCategory = m_wndRibbonBar.AddCategory(_T("Manage"), IDB_RB_MANAGE_SMALL_BMP, IDB_RB_MANAGE_LARGE_BMP);	

	// Create 'Properties' panel
	lPanel = lCategory->AddPanel(_T("Properties"), lPanelImages.ExtractIcon(0));
	lPanel->Add(new CBCGPRibbonButton(ID_METADATA_TAGS, _T("Tags\n"), 0, 0));
	//lPanel->Add(new CBCGPRibbonButton(ID_METADATA_CATEGORIES, _T("Categories\n"), 1, 1));
	lPanel->Add(new CBCGPRibbonButton(ID_METADATA_INFO, _T("Info\n"), 2, 2));

	// Create 'Folders' panel
	lPanel = lCategory->AddPanel(_T("Folders"), lPanelImages.ExtractIcon(1));
	lPanel->Add(new CBCGPRibbonButton(ID_FOLDERS_WATCHED, _T("Watched\n"), 3, 3));*/

	//****************************************************************************
	// Create 'Import and Export' category
	//****************************************************************************
/*
	lPanelImages.Load(IDB_RB_PANELS_IMPORT_AND_EXPORT_BMP);
	lCategory = m_wndRibbonBar.AddCategory(_T("Import and Export"), IDB_RB_IMPORT_AND_EXPORT_SMALL_BMP, IDB_RB_IMPORT_AND_EXPORT_LARGE_BMP);

	// Create 'Import' panel
	lPanel = lCategory->AddPanel(_T("Import"), lPanelImages.ExtractIcon(0));
	lPanel->Add(new CBCGPRibbonButton(ID_IMPORT_ZIP, _T("From Zip\n"), 0, 0));
	lPanel->Add(new CBCGPRibbonButton(ID_IMPORT_FOLDER, _T("From Folder\n"), 1, 1));

	// Create 'Export' panel
	lPanel = lCategory->AddPanel(_T("Export"), lPanelImages.ExtractIcon(1));
	lPanel->Add(new CBCGPRibbonButton(ID_EXPORT_ZIP, _T("To Zip\n"), 2, 2));
	lPanel->Add(new CBCGPRibbonButton(ID_EXPORT_FOLDER, _T("To Folder\n"), 3, 3));
*/

	m_pRibbonCategory[0] = lCategory;	

	//AddWorkspacePanel();
	//****************************************************************************
	//Create 'PDF' category
	//****************************************************************************
//	if(gIsPro)
	{
		lPanelImages.Clear();
		lPanelImages.Load(IDB_RB_PANELS_MANAGE_BMP);
		
		lCategory = m_wndRibbonBar.AddCategory(_T("PDF Preview"), IDB_RB_PDF_SMALL_BMP, IDB_RB_PDF_LARGE_BMP);

		// Create 'File' panel
		lPanel = lCategory->AddPanel(_T("File"), lPanelImages.ExtractIcon(0));
		lPanel->Add(new CBCGPRibbonButton(ID_PDF_PRINT, _T("Print File\n"), 8, 8));
		//lPanel->Add(new CBCGPRibbonButton(ID_PDF_SAVE, _T("Save\n"), 0, 0));
		lPanel->Add(new CBCGPRibbonButton(ID_PDF_SAVE_AS, _T("Save As\n"), 9, 9));
		lPanel->Add(new CBCGPRibbonButton(ID_PDF_SAVE_EXPORT, _T("Export\n"), 12, 12));
		lPanel->Add(new CBCGPRibbonButton(ID_PDF_PROPERTIES, _T("Properties\n"), 1, 1));
		

		// Create 'Zoom' panel
		lPanel = lCategory->AddPanel(_T("Zoom"), lPanelImages.ExtractIcon(1));
		lGroup = new CBCGPRibbonButtonsGroup;
		CBCGPRibbonComboBox *lCombo = new CBCGPRibbonComboBox(ID_PDF_ZOOMCOMBO, TRUE, 50, _T(""));

		lCombo->AddItem(_T("10%"),10);
		lCombo->AddItem(_T("25%"),25);
		lCombo->AddItem(_T("50%"),50);
		lCombo->AddItem(_T("75%"),75);
		lCombo->AddItem(_T("100%"),100);
		lCombo->AddItem(_T("125%"),125);
		lCombo->AddItem(_T("150%"),150);
		lCombo->AddItem(_T("200%"),200);
		lCombo->AddItem(_T("400%"),400);
		lCombo->AddItem(_T("Fit Page"),400);
		lCombo->AddItem(_T("Fit Width"),400);
		lCombo->AddItem(_T("Fit Height"),400);

		lCombo->SelectItem(_T("75%"));

		lPanel->Add(new CBCGPRibbonButton(ID_PDF_ZOOM_OUT, _T("Zoom Out"), 7, 7));
		
		lPanel->Add(lCombo);
		lPanel->Add(new CBCGPRibbonButton(ID_PDF_ZOOM_IN, _T("Zoom In"), 6, 6));
		//lPanel->Add(lGroup);
		lPanel->SetCenterColumnVert(TRUE);

		// Create 'Page' panel
		lPanel = lCategory->AddPanel(_T("Page"), lPanelImages.ExtractIcon(2));
		lPanel->Add(new CBCGPRibbonButton(ID_PDF_PREVPAGE, _T("Prev Page\n"), 4, 4));
		CBCGPRibbonEdit * lEdit = new CBCGPRibbonEdit(ID_PDF_PAGEEDIT, 40, _T("Go Page"));
		lEdit->SetEditText(_T("1"));
		lGroup = new CBCGPRibbonButtonsGroup;
		lGroup->AddButton(lEdit);
		lGroup->AddButton(new CBCGPRibbonButton(ID_PDF_PAGES, _T("/20   \n"), -1, -1));
		lPanel->Add(lGroup);
		lPanel->Add(new CBCGPRibbonButton(ID_PDF_NEXTPAGE, _T("Next Page\n"), 5, 5));
		lPanel->SetCenterColumnVert(TRUE);

		lPanel = lCategory->AddPanel(_T("PDF Viewer"), lPanelImages.ExtractIcon(3));
		lPanel->Add(new CBCGPRibbonButton(ID_PDF_DEFAULT_VIEWER, _T("Open Default\n"), 11, 11));
	}
	m_pRibbonCategory[1] = lCategory;

	//****************************************************************************
	// Create 'Help' category
	//****************************************************************************
	lPanelImages.Load(IDB_RB_PANELS_HELP_BMP);
	lCategory = m_wndRibbonBar.AddCategory(_T("Help"), IDB_RB_HELP_SMALL_BMP, IDB_RB_HELP_LARGE_BMP);

	// Create 'Help' panel
	lPanel = lCategory->AddPanel(_T("Help"),0);// lPanelImages.ExtractIcon(0));
	lPanel->Add(new CBCGPRibbonButton(ID_HELP_USER_GUIDE, _T("User Guide\n"), 0, 0));
	lPanel->Add(new CBCGPRibbonButton(ID_HELP_ONLINE_TUTORIALS, _T("Online Forum\n"), 1, 1));

	// Create 'Product' panel
	lPanel = lCategory->AddPanel(_T("Product"), lPanelImages.ExtractIcon(1));
	lPanel->Add(new CBCGPRibbonButton(ID_PRODUCT_ABOUT, _T("About\n"), 2, 2));
	lPanel->Add(new CBCGPRibbonButton(ID_PRODUCT_LICENSE_INFORMATION, _T("License Information\n"), 3, 3));
	lPanel->Add(new CBCGPRibbonButton(ID_PRODUCT_CHECK_FOR_UPDATES, _T("Check for updates\n"), 4, 4));
	
	// Create 'Contact' panel
	lPanel = lCategory->AddPanel(_T("Contact"), lPanelImages.ExtractIcon(2));
	lPanel->Add(new CBCGPRibbonButton(ID_CONTACT_SUPPORT, _T("Support\n"), 5, 5));
	lPanel->Add(new CBCGPRibbonButton(ID_CONTACT_SALES, _T("Sales\n"), 6, 6));
	lPanel->Add(new CBCGPRibbonButton(ID_CONTACT_BUG_REPORT, _T("Report a Bug"), 7, 7));
	
	//Register
	//lPanel = lCategory->AddPanel(_T("Register"), lPanelImages.ExtractIcon(3));
	//lPanel->Add(new CBCGPRibbonButton(ID_REGISTER_LICENSE, _T("Register"), 8, 8));
	        
	m_pRibbonCategory[2] = lCategory;

	// Create 'Upgrade' panel
	//lPanel = lCategory->AddPanel(_T("Upgrade"), 0); // Nedd panel image
	//lPanel->Add(new CBCGPRibbonButton(ID_UPGRADE_BENUBIRD_PRO, _T("Benubird Pro\n"), 7, 7));

	//****************************************************************************
	//Create main button
	//****************************************************************************
	m_MainButton.SetImage(IDB_RB_MAIN_BMP);

	if(gIsPro)
	{
		m_MainButton.SetToolTipText(_T("Benubird PDF Pro Button"));
	}
	else
	{
		m_MainButton.SetToolTipText(_T("Benubird PDF Button"));
	}

	m_MainButton.SetDescription(_T("Click here to import, export, print and see everything else you can do with your documents."));
	m_wndRibbonBar.SetMainButton(&m_MainButton, CSize(45, 45));
	CBCGPRibbonMainPanel* pMainPanel = m_wndRibbonBar.AddMainCategory(_T("Main"), IDB_RB_MENU_SMALL_BMP, IDB_RB_MENU_LARGE_BMP);

	if(gIsPro)
	{
		// new & open library 
		lButton = new CBCGPRibbonButton(ID_LIBRARY, _T("&Library"), 0, 0);
		pMainPanel->Add(lButton);
		lButton->SetDefaultCommand(FALSE);
	
		lButton->AddSubItem(new CBCGPRibbonLabel(_T("New & Open library...")));
	

		lSubButton = new CBCGPRibbonButton(ID_LIBRARY_NEW, _T("New Library"), 3, 3, TRUE);
		lSubButton->SetDescription(_T("create a new library."));
		lButton->AddSubItem(lSubButton);

		lSubButton = new CBCGPRibbonButton(ID_LIBRARY_OPEN, _T("Open Library"), 4, 4, TRUE);
		lSubButton->SetDescription(_T("open a existing library."));
		lButton->AddSubItem(lSubButton);
	}

	//Import Group
	lButton = new CBCGPRibbonButton(ID_IMPORT, _T("Import"), 1, 1);
	pMainPanel->Add(lButton);
	lButton->SetDefaultCommand(FALSE);

	lButton->AddSubItem(new CBCGPRibbonLabel(_T("Import files from...")));

	if(gIsPro)
	{
		lSubButton = new CBCGPRibbonButton(ID_LIBRARY_IMPORT, _T("Library from Zip"), 5, 5, TRUE);
		lSubButton->SetDescription(_T("Import library from Zip."));
		lButton->AddSubItem(lSubButton);
	}

	lSubButton = new CBCGPRibbonButton(ID_IMPORT_ZIP, _T("ZIP Archive"), 11, 11, TRUE);
	lSubButton->SetDescription(_T("Import files from compressed (zipped) folder."));
	lButton->AddSubItem(lSubButton);

	lSubButton = new CBCGPRibbonButton(ID_IMPORT_FOLDER, _T("Folder"), 10, 10, TRUE);
	lSubButton->SetDescription(_T("Import files from a folder."));
	lButton->AddSubItem(lSubButton);

	lButton = new CBCGPRibbonButton(ID_EXPORT, _T("Export"), 2, 2);
	pMainPanel->Add(lButton);
	lButton->SetDefaultCommand(FALSE);

	lButton->AddSubItem(new CBCGPRibbonLabel(_T("Export a copy of files to")));

	if(gIsPro)
	{
		lSubButton = new CBCGPRibbonButton(ID_LIBRARY_EXPORT, _T("Library to Zip"), 9, 9, TRUE);
		lSubButton->SetDescription(_T("Export library to Zip."));
		lButton->AddSubItem(lSubButton);
	}

	lSubButton = new CBCGPRibbonButton(ID_EXPORT_ZIP, _T("ZIP Archive"), 13, 13, TRUE);
	lSubButton->SetDescription(_T("Copy files to a compressed (zipped) folder."));
	lButton->AddSubItem(lSubButton);

	lSubButton = new CBCGPRibbonButton(ID_EXPORT_FOLDER, _T("Folder"), 12, 12, TRUE);
	lSubButton->SetDescription(_T("Copy files to a folder."));
	lButton->AddSubItem(lSubButton);

	pMainPanel->Add (new CBCGPRibbonSeparator (TRUE));

	lButton = new CBCGPRibbonButton (ID_PRINT, _T("Print"), 6, 6);
	pMainPanel->Add (lButton);

	pMainPanel->AddRecentFilesList(_T("Recently Added Documents"));
	pMainPanel->AddToBottom(new CBCGPRibbonMainPanelButton(ID_OPTIONS, _T("Benubird Options"), 7));
	pMainPanel->AddToBottom(new CBCGPRibbonMainPanelButton(ID_APP_EXIT, _T("Exit Benubird"), 8));

	// Add quick access toolbar commands:
	CBCGPRibbonQATDefaultState qatState;

	qatState.AddCommand (ID_LIBRARY_NEW);
	qatState.AddCommand (ID_LIBRARY_OPEN);
	qatState.AddCommand (ID_LIBRARY_IMPORT);
	qatState.AddCommand (ID_LIBRARY_EXPORT);

	qatState.AddCommand (ID_ADD_FILE);
	qatState.AddCommand (ID_ADD_FOLDER);
	qatState.AddCommand (ID_VIEW_FILTER);
	qatState.AddCommand (ID_VIEW_TAGS);
	qatState.AddCommand (ID_VIEW_PREVIEW);
	qatState.AddCommand(ID_VAULT_VIEW);

	qatState.AddCommand(ID_LIBRARY_SETTING);

	m_wndRibbonBar.SetQuickAccessDefaultState (qatState);

	m_wndRibbonBar.AdjustSizeImmediate(1);

	return TRUE;
}

CBCGPRibbonPanel *g_pCBCGPRibbonPanel = NULL;
void CMainFrame::AddWorkspacePanel()
{
	CBCGPRibbonButton* lSubButton;
	CBCGPRibbonCategory* lCategory = this->m_pRibbonCategory[0];
	CBCGPRibbonPanel*    lPanel;
	CBCGPRibbonButtonsGroup* lGroup;
	CRibbonFindEdit* lEdit;
	CBCGPRibbonButton* lButton;
	CBCGPToolBarImages lPanelImages;
	
// Create 'Repository' panel
	if(g_pCBCGPRibbonPanel != NULL) return;
	lPanelImages.Load(IDB_RB_PANELS_HOME_BMP);
	g_pCBCGPRibbonPanel = lCategory->AddPanel(_T("Workspace"), lPanelImages.ExtractIcon(7));
	g_pCBCGPRibbonPanel->Add(new CBCGPRibbonButton(ID_OPEN_WORKSPACE, _T("Open to Workspace\n"), 22, 17));
	g_pCBCGPRibbonPanel->Add(new CBCGPRibbonButton(ID_CLOSE_WORKSPACE, _T("Restore to Repository\n"), 23, 18));

	
	m_wndRibbonBar.RecalcLayout();
}
void CMainFrame::RemoveWorkspacePanel()
{
	if(g_pCBCGPRibbonPanel == NULL) return;
	int index = this->m_pRibbonCategory[0]->GetPanelIndex(g_pCBCGPRibbonPanel);
	this->m_pRibbonCategory[0]->RemovePanel(index);
	g_pCBCGPRibbonPanel = NULL;
	m_wndRibbonBar.RecalcLayout();
}

void CMainFrame::OnOpenBenubird()
{
	OutputDebugString(_T("------------------------  3"));
	ShowWindow (SW_SHOW);
	ActivateFrame();
}

void CMainFrame::OnShowModifiedFiles()
{
	//CChangedFilesDlg lDlg;//pyj
	//lDlg.DoModal();		//pyj
}

void CMainFrame::OnUpdateShowModifiedFiles(CCmdUI* pCmdUI)
{
	// -----------------------------------
/*	if(gUseRepository)
	{
	//	pCmdUI->Enable(gWorkspace.GetNumChangedFiles() > 0); pyj
	}
	else
	{
		AfxMessageBox(_T("OnUpdateShowModifiedFiles"));
	}*/
	// -----------------------------------
}

void CMainFrame::OnClose()
{
	HideSysTrayIcon();

	CBCGPMDIFrameWnd::OnClose();
}

void CMainFrame::OnTimer(UINT_PTR nIDEvent)
{
	//gWFolderManager.MarkWFoldersToProcess(nIDEvent);
	CBCGPMDIFrameWnd::OnTimer(nIDEvent);
}

void CMainFrame::OnHelp(UINT nID)
{
	CString lStr;
	switch (nID)
	{
		// Szabi: case 240
		case ID_HELP_USER_GUIDE:
		{
			int i;
			TCHAR path[MAX_PATH];
			GetModuleFileName(AfxGetInstanceHandle(), path, sizeof(path));
			CString lPath;
			lPath = path;
			i = lPath.ReverseFind('\\');
			if (i != -1) 
				lPath.Delete(i, lPath.GetLength() - i);
			lStr = lPath + _T("\\Benubird PDF Quick Start Guide.pdf");
			break;
		}
		case ID_HELP_ONLINE_TUTORIALS:
		{
			//lStr = _T("http://www.debenu.com/benubird/tutorials.html");
			lStr = _T("http://forums.debenu.com/benubird_forum1.html");
			break;
		}
		case ID_PRODUCT_ABOUT:
		{
			CAboutDlg lAboutDlg(0);
			lAboutDlg.DoModal();
			break;
		}
		case ID_PRODUCT_LICENSE_INFORMATION:
		{
			CAboutDlg lAboutDlg(1);
			lAboutDlg.DoModal();
			break;
		}
		case ID_PRODUCT_CHECK_FOR_UPDATES:
		{			
			CCheckForUpdates::CheckForUpdates(false);
			return;
/*
			//check for updates
			lStr = _T("http://www.debenu.com/benubird/updates.html");
			break;
*/
		}
		case ID_CONTACT_SUPPORT:
		{
			lStr = _T("http://www.debenu.com/support/index.html");
			break;
		}
		case ID_CONTACT_SALES:
		{			
			lStr = _T("http://www.debenu.com/store/index.html");
			break;
		}
		case ID_CONTACT_BUG_REPORT:
		{
			lStr = _T("http://www.debenu.com/support/report_bug.html");
			break;
		}
		case ID_UPGRADE_BENUBIRD_PRO:
		{
			CRegisterBenubirdProDlg lDlg;
			lDlg.DoModal();
			break;
		}
		default:
			break;
	}
	::ShellExecute(NULL, NULL, lStr, NULL, NULL, NULL);
}

void CMainFrame::PrepareSysTrayIcon()
{
	memset(&m_nid, 0 , sizeof(m_nid));
	m_nid.cbSize = sizeof(m_nid);
	m_nid.uFlags = NIF_ICON | NIF_TIP | NIF_MESSAGE;

	// Set tray notification window:
	m_nid.hWnd = GetSafeHwnd ();
	m_nid.uCallbackMessage = UM_TRAYNOTIFY;

	// Set tray icon and tooltip:
//	m_nid.hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	UpdateSysTrayIcon();

	//CString strToolTip = _T("BCGPTrayDemo");
	//_tcsncpy(m_nid.szTip, strToolTip, strToolTip.GetLength());
}


/*void CMainFrame::ShowIconInSysTray()
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	memset(&m_nid, 0 , sizeof(m_nid));
	m_nid.cbSize = sizeof(m_nid);
	m_nid.uFlags = NIF_ICON | NIF_TIP | NIF_MESSAGE;

	// Set tray notification window:
	m_nid.hWnd = GetSafeHwnd ();
	m_nid.uCallbackMessage = UM_TRAYNOTIFY;

	// Set tray icon and tooltip:
	m_nid.hIcon = m_hIcon;

	CString strToolTip = _T("BCGPTrayDemo");
	_tcsncpy (m_nid.szTip, strToolTip, strToolTip.GetLength ());

	Shell_NotifyIcon (NIM_ADD, &m_nid);

	//CBCGPToolBar::AddToolBarForImageCollection (IDR_MENUIMAGES);

	// ONLY FOR TEST
	/*CBCGPPopupWindow* pPopup = new CBCGPPopupWindow;

	pPopup->SetAnimationType(CBCGPPopupMenu::ANIMATION_TYPE::FADE);
	pPopup->SetAnimationSpeed(30);
	pPopup->SetTransparency(200);
	pPopup->SetSmallCaption(FALSE);
	pPopup->SetAutoCloseTime(5000);
	CBCGPPopupWndParams params;
	params.m_strText = _T("There is a new updated version of Benubird available. Version n can now be downloaded.");
	params.m_strURL = _T("Download update");
	params.m_nURLCmdID = 101;

	pPopup->Create (this, params, NULL, CPoint(-1, -1));
	pPopup->SetWindowText (_T("New version of Benubird available"));
	
}*/


LRESULT CMainFrame::OnTrayNotify(WPARAM /*wp*/, LPARAM lp)
{
	UINT uiMsg = (UINT) lp;

	switch (uiMsg)
	{
		case WM_RBUTTONUP:
			OnTrayContextMenu ();
			return 1;

		case WM_LBUTTONDBLCLK:
			ShowWindow (SW_SHOW);
			ActivateFrame();
			SetForegroundWindow();
			return 1;
	}
	return 0;
}

void CMainFrame::OnTrayContextMenu ()
{
	CPoint point;
	::GetCursorPos (&point);

	CMenu menu;
	menu.LoadMenu (IDR_TRAY_MENU);

	CBCGPPopupMenu::SetForceShadow (TRUE);

	HMENU hMenu = menu.GetSubMenu(0)->Detach ();
	CBCGPPopupMenu* pMenu = GetWorkspace ()->GetContextMenuManager()->ShowPopupMenu (
		hMenu, point.x, point.y, this, TRUE);

	pMenu->SetForegroundWindow ();
}


CString CMainFrame::GetFindText()
{
	CString lStr;
	lStr.Empty();
	if (mSearchEdit == NULL)
		return lStr;
	
	lStr = mSearchEdit->GetEditText(); 

	switch (mSearchMode)
	{
		case kSearchAll:
			if (lStr == _T("Find...")) lStr.Empty();
			break;
		case kSearchTitle:
			if (lStr == _T("Find Title...")) lStr.Empty();
			break;
		case kSearchAuthor:
			if (lStr == _T("Find Author...")) lStr.Empty();
			break;
		case kSearchSubject:
			if (lStr == _T("Find Subject...")) lStr.Empty();
			break;
		case kSearchTags:
			if (lStr == _T("Find Tags...")) lStr.Empty();
			break;		
	}
	return lStr;
}

void CMainFrame::InitFindText()
{
	if (!mSearchEdit) return;

	switch (mSearchMode)
	{
		case kSearchAll:
			mSearchEdit->SetEditText(_T("Find..."));	
			break;
		case kSearchTitle:
			mSearchEdit->SetEditText(_T("Find Title..."));		
			break;
		case kSearchAuthor:
			mSearchEdit->SetEditText(_T("Find Author..."));		
			break;
		case kSearchSubject:
			mSearchEdit->SetEditText(_T("Find Subject..."));	
			break;
		case kSearchTags:
			mSearchEdit->SetEditText(_T("Find Tags..."));		
			break;		
	}
}

BOOL CMainFrame::CreateStatusBar()
{
	if (!m_wndStatusBar.Create(this))
	{
		return FALSE;
	}
	m_wndStatusBar.AddDynamicElement (new CBCGPRibbonStatusBarPane (
		ID_STATUSBAR_SELECTEDFILES, _T(""), TRUE));
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////////
// Progress Bar
///////////////////////////////////////////////////////////////////////////////////
BOOL CMainFrame::SetProgressRangeMax(int inMax)
{
	if (m_ProgressBar == NULL) return FALSE;
	m_ProgressBar->SetRange(0, inMax);
	return TRUE;
}

BOOL CMainFrame::SetProgressPos(int inPos)
{
	BOOL lResult;
	if (m_ProgressBar == NULL) 
		lResult = FALSE;
	int lMin = m_ProgressBar->GetRangeMin();
	int lMax = m_ProgressBar->GetRangeMax();
	if (inPos > lMax || inPos < lMin)
	{
		lResult = FALSE;
	}
	else
	{
		m_ProgressBar->SetPos(inPos);
		lResult = TRUE;
	}
	m_wndStatusBar.RecalcLayout();
	m_wndStatusBar.RedrawWindow();
	return lResult;
}

BOOL CMainFrame::IncProgressPos(int inStep /* = 1*/)
{
	return SetProgressPos(GetProgressPos() + inStep);
}

BOOL CMainFrame::IncProgressPosForever(int inStep /* = 1*/)
{
	BOOL lRetVal = IncProgressPos(inStep);
	if(lRetVal == FALSE)
		lRetVal = SetProgressPos(m_ProgressBar->GetRangeMin()); //start from begining
	return lRetVal;
}

int CMainFrame::GetProgressPos()
{
	if (m_ProgressBar == NULL) return -1;
	return m_ProgressBar->GetPos();
}

int CMainFrame::GetProgressRangeMax()
{
	if (m_ProgressBar == NULL) return -1;
	return m_ProgressBar->GetRangeMax();
}

void CMainFrame::ShowProgress(BOOL bShow /* = TRUE)*/)
{
	if (bShow)
	{
		int cxFree = m_wndStatusBar.GetSpace ();
		if (cxFree < 20)
		{
			// Not enough space for progress bar
			return;
		}
		
		int cxProgress = min (cxFree, 150);
		if (m_ProgressBar == NULL)
		{
			m_ProgressBar = new CBCGPRibbonProgressBar(ID_STATUSBAR_PROGRESS, cxProgress);
			m_ProgressBar->SetInfiniteMode(FALSE);
			m_wndStatusBar.AddDynamicElement(m_ProgressBar);
			m_ProgressBar->SetRange(0, 100);
			m_ProgressLabel = new CBCGPRibbonLabel(m_ProgressText);
			m_wndStatusBar.AddDynamicElement(m_ProgressLabel);
		}
	}
	else
	{
		m_wndStatusBar.RemoveElement(ID_STATUSBAR_PROGRESS);
		if (m_ProgressLabel != NULL) m_wndStatusBar.RemoveElement(m_ProgressLabel->GetID());
		m_ProgressLabel = NULL;
		m_ProgressBar = NULL;
	}
	m_wndStatusBar.RecalcLayout();
	m_wndStatusBar.RedrawWindow();
	
	CBCGPPopupMenu::UpdateAllShadows ();
}
/*
LRESULT CMainFrame::OnCopyData(WPARAM wParam, LPARAM lParam)
{
	//pyj
	
	if (CmdLineProcess() == FALSE)
	{
		return 0;
	}

	// Process incoming data from another copy of Benubird when executed by novaPDF
	// or by Benubird Assistant when processing watched folders
	PCOPYDATASTRUCT lpPCOPYDATASTRUCT = (PCOPYDATASTRUCT) lParam;
	if (lpPCOPYDATASTRUCT->cbData > 0)
	{
		//BenubirdData_Transfer* pBenubirdData_Transfer = new BenubirdData_Transfer;
		//memcpy(pBenubirdData_Transfer, lpPCOPYDATASTRUCT->lpData, sizeof(lpPCOPYDATASTRUCT->cbData)-1);
		BenubirdData_Transfer* pBenubirdData_Transfer = new BenubirdData_Transfer;
		memcpy(pBenubirdData_Transfer,(BenubirdData_Transfer*)(lpPCOPYDATASTRUCT->lpData),sizeof(BenubirdData_Transfer));
		
		UINT uiSource = pBenubirdData_Transfer->mMessageType;
		switch(uiSource)
		{
		case Command_Line_Message:
			ProcessNovaPDFCmdLine(pBenubirdData_Transfer->mCmdLineData.mPath); // However, we are still having to deal 
			// with watched folder updates, which are not correctly processed at present
			break;
		case Watched_Folder_Message:
			ProcessWatchedFolderData(&pBenubirdData_Transfer->mAssistantData);
			break;
		default:
			
			break;

		}
		free(pBenubirdData_Transfer);
	}

	return NULL;
}
*/
int CMainFrame::CmdLineProcess()
{
	const GUID CDECL BASED_CODE _tlid =
			{ 0x728777C4, 0xF5AC, 0x4023, { 0xAD, 0x55, 0xAC, 0xDA, 0xD7, 0xA7, 0x1E, 0xC5 } };
	const WORD _wVerMajor = 1;
	const WORD _wVerMinor = 0;

	int lRetValue = cmdContinue;

	// Parse command line for automation or reg/unreg switches.
	CCommandLineInfo cmdInfo;
	theApp.ParseCommandLine(cmdInfo);

	// App was launched with /Embedding or /Automation switch.
	// Run app as automation server.
	if (cmdInfo.m_bRunEmbedded || cmdInfo.m_bRunAutomated)
	{
		// Register class factories via CoRegisterClassObject().
		COleTemplateServer::RegisterAll();
		return TRUE;
	}
	// App was launched with /Unregserver or /Unregister switch.  Remove
	// entries from the registry.
	else if (cmdInfo.m_nShellCommand == CCommandLineInfo::AppUnregister)
	{
		COleObjectFactory::UpdateRegistryAll(FALSE);
		AfxOleUnregisterTypeLib(_tlid, _wVerMajor, _wVerMinor);
		return FALSE;
	}
	// App was launched standalone or with other switches (e.g. /Register
	// or /Regserver).  Update registry entries, including typelibrary.
	else
	{
		COleTemplateServer::RegisterAll();

		COleObjectFactory::UpdateRegistryAll();
		AfxOleRegisterTypeLib(AfxGetInstanceHandle(), _tlid);
		if (cmdInfo.m_nShellCommand == CCommandLineInfo::AppRegister)
			return FALSE;

		lRetValue = cmdContinueAndShow;
	}

	// process parameters from NovaPDF OEM
	//ProcessNovaPDFCmdLine();

	return lRetValue;
}

int CMainFrame::ProcessNovaPDFCmdLine(LPCTSTR inCmdLine)
{
	// Parse command line
	int nArgs;
	CString lParam;
	LPWSTR *szArglist = CommandLineToArgvW(CT2CW(inCmdLine), &nArgs);
	if (nArgs < 2)
	{
		LocalFree(szArglist); // forgetabout it, no filename
		return FALSE;
	}
	try
	{
		// Extract the registry settings and populate the newly formed PDF	
		CRegistry lDefReg(TRUE);
		CFileProperties_Entry lEntry;
		lEntry.mAuthor, lEntry.mCategories, lEntry.mCollectionID, lEntry.mCollectionIDGroup, lEntry.mComments, lEntry.mFileTypes,
			lEntry.mFileTypes, lEntry.mSubject, lEntry.mTags, lEntry.mTitle, lEntry.mUseAuthor, lEntry.mUseCategories, lEntry.mUseComments,
				lEntry.mUseSubject, lEntry.mUseTags, lEntry.mUseTitle = NULL;

		CFileProperties_Entry* plEntry = &lEntry;
		int lCollection = -1;
		int iInternal = 0;
		if (lDefReg.Open(kPDFDefaultProfileKey))
		{
		// PDF was created internally -- extract all the metadata from the 
			// registry and move the file afterwards
			lDefReg.Read(_T("CollectionID"), lCollection);
			lDefReg.Read(_T("Document Author"), plEntry->mAuthor);
			lDefReg.Read(_T("Document Keywords"), plEntry->mTags);
			lDefReg.Read(_T("Document Subject"), plEntry->mSubject);
			lDefReg.Read(_T("Document Title"), plEntry->mTitle);
			lDefReg.Read(_T("Path"), plEntry->mPath);
			lDefReg.Read(_T("Internal"), iInternal);
			lDefReg.Close(); 
		}

		// Extract the filename
		CPath lPath;
		CStringList lListOfFiles;
		if (nArgs >= 2) // Grab the first file
		{
			lParam.Format(_T("%ws"), szArglist[1]);
			lPath.SetPath(lParam);
			if (lPath.ExistFile() == FALSE)
			{
				throw(0);
			}
			lParam.Replace(_T("\\\\"), _T("\\"));
		}

		// Prompt for metadata if this method is not called within Benubird
		if (iInternal!=1)
		{
			PDF_Entry lPDF_Entry;
			CPDFPropertiesFrame::LoadPDFMetadata(lParam, &lPDF_Entry);
			CPDFPropertiesDlg lDlg(&lPDF_Entry);
			if (lDlg.DoModal() == IDOK)
			{
				CPDFPropertiesFrame::SavePDFMetadata(lParam, &lPDF_Entry);
			}
		}

		// Double check that mPath exists
		if(plEntry->mPath == "")
		{
			// Replace with PDF Output Folder
			CString lStrPathRep = AfxGetApp()->GetProfileString(kOptions, kPDFOutputFolder, _T(""));
			if (lStrPathRep.IsEmpty())
			{
				// Use the Repository location if none is specified			
				lStrPathRep = AfxGetApp()->GetProfileString(kOptions, kRepositoryFolder, _T(""));
/*
				// Use standard desktop folder if none is specified
				TCHAR szPath[MAX_PATH];
				if(!(SHGetSpecialFolderPath(NULL, szPath, CSIDL_DESKTOP, FALSE)))  // Desktop
				{
					throw(0);
				}
				lStrPathRep = szPath;*/
			}
			
			// Remove the backslash, if it exists...to be sure that when we add a slash, that is 
			// is to a slash-less path
			// --------------------------------------------------
			CPath tempPath;
			lStrPathRep = tempPath.RemoveBackSlash(lStrPathRep);
			// --------------------------------------------------
			plEntry->mPath = lStrPathRep + _T("\\") + lPath.GetFileTitle() + _T(".pdf");
		}
/*
		if(!gUseRepository) 
		{
			CopyFile(lParam, plEntry->mPath, FALSE); // Copy the file to the new location
			lListOfFiles.AddTail(plEntry->mPath);
		}*/
		else 
		{
			lListOfFiles.AddTail(lParam);
		}

		// ----------------------------------------------
		int lCount = 0;
		if(lCollection>-1)
		{
			plEntry->mCollectionID = lCollection;
		}

	/*	if(gUseRepository) // Add the file to the Repository 
		{
		//	lCount = gRepository.AddFiles(lListOfFiles, plEntry, TRUE); pyj
		}
		else // For direct linking, skip the repository
		{
			//pyj
			//CTFiles lTFiles;
			//lCount = lTFiles.AddFiles(lListOfFiles, plEntry);
		}*/
		// ----------------------------------------------
	}
	catch(...)
	{
	}
	
	// Cleanup [don't check for errors]
	DeleteFile(lParam);
	LocalFree(szArglist);

	// Clear the registry settings
	CRegistry lDefReg2(FALSE);
	lDefReg2.DeleteKey(kPDFDefaultProfileKey);

//?	Refresh();
	return TRUE;
}

LRESULT CMainFrame::OnRefreshFile( WPARAM wParam, LPARAM lParam )
{
	CMDIChildWnd * pChild = ((CMDIFrameWnd*)(AfxGetApp()->m_pMainWnd))->MDIGetActive();
	CLibraryDoc* pDoc = DYNAMIC_DOWNCAST(CLibraryDoc, pChild->GetActiveDocument ());

	BOOL lRefreshOnlyLine = (BOOL) wParam;
    int lID = (int) lParam;
	pDoc->RefreshFile(lID, !lRefreshOnlyLine);

	return 0;
}

LRESULT CMainFrame::OnRefreshAllFiles( WPARAM, LPARAM )
{
	//Refresh();
	return 0;
}

///////////////////////////////////////////////////////////////////////////////////
// 
///////////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnUpdateSearch(CCmdUI* pCmdUI)
{
	UINT lID = pCmdUI->m_nID;
	switch (lID)
	{
		case ID_SEARCH_FOCUS:
			break;

		case ID_SEARCH_OPTION_ALL:
			if (mSearchMode == kSearchAll)
				pCmdUI->SetCheck(1);
			else
				pCmdUI->SetCheck(0);
			break;

		case ID_SEARCH_OPTION_TITLE:
			if (mSearchMode == kSearchTitle)
				pCmdUI->SetCheck(1);
			else
				pCmdUI->SetCheck(0);
			break;

		case ID_SEARCH_OPTION_AUTHOR:
			if (mSearchMode == kSearchAuthor)
				pCmdUI->SetCheck(1);
			else
				pCmdUI->SetCheck(0);
			break;

		case ID_SEARCH_OPTION_SUBJECT:
			if (mSearchMode == kSearchSubject)
				pCmdUI->SetCheck(1);
			else
				pCmdUI->SetCheck(0);
			break;

		case ID_SEARCH_OPTION_TAGS:
			if (mSearchMode == kSearchTags)
				pCmdUI->SetCheck(1);
			else
				pCmdUI->SetCheck(0);
			break;
	}
}

void CMainFrame::OnUpdateContentSearch(CCmdUI* pCmdUI)
{
}

///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMainFrame::ShowSysTrayIcon(HICON inIcon)
{ 
	HICON lOldIcon = m_nid.hIcon;
	m_nid.hIcon = inIcon;
	m_nid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
//	m_nid.uCallbackMessage = ID_SHOWMODIFIEDFILES;

	if (lOldIcon == NULL)
	{
		Shell_NotifyIcon (NIM_ADD, &m_nid);
	}
	else 
	{
		Shell_NotifyIcon (NIM_MODIFY, &m_nid);
	}
}

void CMainFrame::ShowSysTrayIconMessage(CString inTitle, CString inMessage)
{
	if (m_nid.hWnd == NULL)
		return;

	m_nid.uFlags = NIF_INFO;
	wcsncpy_s(m_nid.szInfo, inMessage, 256);
	wcsncpy_s(m_nid.szInfoTitle, inTitle, 64);

	m_nid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP | NIF_INFO;
	m_nid.uTimeout = 10 * 1000;   // convert time to ms

	BOOL bSuccess = Shell_NotifyIcon (NIM_MODIFY, &m_nid);

	m_nid.szInfo[0] = _T('\0');	
}


void CMainFrame::HideSysTrayIcon()
{ 
	Shell_NotifyIcon (NIM_DELETE, &m_nid); 
	m_nid.hIcon = NULL;
}

void CMainFrame::UpdateSysTrayIcon()
{
	CWinApp* lApp = AfxGetApp();
	int lShow = lApp->GetProfileInt(kOptions, kShowIconInSystemTray, 0);

	// -------------------------------------
/*	if(gUseRepository) // Required for the Repository only
	{
		HICON lIcon = NULL;
		/* pyj
		if (gWorkspace.GetNumChangedFiles() > 0)
		{
			lIcon = AfxGetApp()->LoadIcon(IDR_WORKSPACE_NOTIFY);
			lShow = 1;
		}
		else
		{
			lIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
		}

		if (lShow) 
			ShowSysTrayIcon(lIcon);
		else
			HideSysTrayIcon();
	}*/
	// -------------------------------------
}

void CMainFrame::ActivateCategory (int nIndex)
{
	m_wndRibbonBar.SetActiveCategory(m_pRibbonCategory[nIndex]);
}

void CMainFrame::ActivateViewByCategory (CBCGPRibbonCategory* pCategory)
{
	ASSERT_VALID (pCategory);

	// library view
	if (pCategory == m_pRibbonCategory[0])
	{
		//CLibraryView* pView = NULL;
		//CMDIChildWnd * pChild = NULL;
		//while(pView == NULL) {
		//	MDINext();
		//	pChild = MDIGetActive();
		//	pView = DYNAMIC_DOWNCAST(CLibraryView, pChild->GetActiveView ());
		//}
	} // pdf view
	else if (pCategory == m_pRibbonCategory[1])
	{
		//CPDFView* pView = NULL;
		//CMDIChildWnd * pChild = NULL;
		//while(pView == NULL) {
		//	MDINext();
		//	pChild = MDIGetActive();
		//	pView = DYNAMIC_DOWNCAST(CPDFView, pChild->GetActiveView ());
		//}
	}
	else
		return;

	CBCGPPopupMenu::UpdateAllShadows ();
}

LRESULT CMainFrame::OnActivateCategory(WPARAM /*wParam*/, LPARAM lParam)
{
	if ((LPVOID)lParam == (LPVOID)&m_wndRibbonBar)
	{
		CBCGPRibbonCategory* pCategory = m_wndRibbonBar.GetActiveCategory ();
		ASSERT_VALID (pCategory);

		ActivateViewByCategory (pCategory);
	}

	return 0L;
}

int CMainFrame::ProcessWatchedFolderData(AssistantData_Entry* lEntry)
{
// skusim pridat subor
	CTWatchedFolder lTWFolder;
	lTWFolder.GetWatchedFolderByID(lEntry->mWFolderID);

	// zatial necham velmi skarede, treba dat dokopy s benubird assistantom a pouzit
	// iba jednu fciu - aj JOZO!
	if (!lTWFolder.mDataIsValid) return 1;

	CTFiles lTFiles; 
	lTFiles.mPath = lEntry->mPath;
	CString lEncPath;
	EncodeString(lTFiles.mPath, lEncPath);

	//AfxMessageBox(lTFiles.mPath);

	// fill the private members in CTFiles class
	//lTFiles.mFlags; //0x1 - missing,
	lTFiles.mExtID = -1;
	lTFiles.m_ProgressDlg = NULL;
	lTFiles.mName = _T("");

	// load summary fom file (from disc!)
	lTFiles.mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_TITLE] = !lTWFolder.mUseTitle;
	lTFiles.mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_SUBJECT] = !lTWFolder.mUseSubject;
	lTFiles.mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_AUTHOR] = !lTWFolder.mUseAuthor;
	lTFiles.mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_CATEGORY] = !lTWFolder.mUseCategories;	
	lTFiles.mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_KEYWORDS] = !lTWFolder.mUseTags;	
	lTFiles.mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_COMMENTS] = !lTWFolder.mUseComments;	
	lTFiles.LoadDataFromFile(); // loads also dates

	// update summary info from watched folder settings
	if(lTWFolder.mUseTitle != 0)
	{
		lTFiles.mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_TITLE] = true;
		lTFiles.mTitle = lTWFolder.mTitle;
	}
	else
	{
		lTFiles.mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_TITLE] = false;
	}

	if(lTWFolder.mUseAuthor != 0)
	{
		lTFiles.mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_AUTHOR] = true;
		lTFiles.mAuthor = lTWFolder.mAuthor;
	}
	else
	{
		lTFiles.mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_AUTHOR] = false;
	}

	if(lTWFolder.mUseSubject != 0)
	{
		lTFiles.mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_SUBJECT] = true;
		lTFiles.mSubject = lTWFolder.mSubject;
	}
	else
	{
		lTFiles.mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_SUBJECT] = false;
	}

	if(lTWFolder.mUseCategories != 0)
	{
		lTFiles.mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_CATEGORY] = true;
		lTFiles.mCategory = lTWFolder.mCategories;
	}
	else
	{
		lTFiles.mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_CATEGORY] = false;
	}

	if(lTWFolder.mUseComments)
	{
		lTFiles.mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_COMMENTS] = true;
		lTFiles.mComments = lTWFolder.mComments;
	}
	else
	{
		lTFiles.mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_COMMENTS] = false;
	}

	if(lTWFolder.mUseTags)
	{/*//pyj
		lTFiles.mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_KEYWORDS] = true;
		// read tags from the watched folder to lTFiles.mKeywords
		CIDData lTagsIDData;
		CTTags lTTags;
		if(lTTags.GetCIDDataByWFolderID(lTagsIDData, lTWFolder.mID))
		{
			lTagsIDData.GetStringsFormatted(lTFiles.mKeywords, _T("; "));
		}
		*/
	}
	else
	{
		lTFiles.mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_KEYWORDS] = false;
	}

	CString lEncName, lEncTitle, lEncSubject, lEncAuthor, lEncCat, lEncComments;
	EncodeString(lTFiles.mFindData.cFileName, lEncName);
	EncodeString(lTFiles.mTitle, lEncTitle);
	EncodeString(lTFiles.mSubject, lEncSubject);
	EncodeString(lTFiles.mAuthor, lEncAuthor);
	EncodeString(lTFiles.mCategory, lEncCat);
	EncodeString(lTFiles.mComments, lEncComments);

	m_pDB->GetCurrentFileTime(lTFiles.mAddedDate);
	CString lCreatedTmStmp, lModifiedTmStmp, lAccessedTmStmp, lAddedTmStmp;
	Filetime2Timestamp(lTFiles.mFindData.ftCreationTime, lCreatedTmStmp);
	Filetime2Timestamp(lTFiles.mFindData.ftLastWriteTime, lModifiedTmStmp);
	Filetime2Timestamp(lTFiles.mFindData.ftLastAccessTime, lAccessedTmStmp);
	Filetime2Timestamp(lTFiles.mAddedDate, lAddedTmStmp);

	// I need to update the summary info of the file on the disc at first - due to
	// avoid conflicts with the UpdateFiles function from Benubird! 
	lTFiles.UpdateFileOnDiskFromMembers();

	CString lSQL_AddFile;
	lSQL_AddFile.Format(_T("insert into Files values (NULL, '%s', '%s', %u, %u, %u, %u, %u, %u, %u, %u, %u, '%s', '%s', '%s', '%s', '%s', %u, %u, %u, %u, '%s', '%s', '%s', '%s')"), 
		lEncPath,
		lEncName,
		lTFiles.mFindData.nFileSizeHigh,
		lTFiles.mFindData.nFileSizeLow,
		lTFiles.mFindData.ftCreationTime.dwHighDateTime, //cr time
		lTFiles.mFindData.ftCreationTime.dwLowDateTime,
		lTFiles.mFindData.ftLastWriteTime.dwHighDateTime, //wr time
		lTFiles.mFindData.ftLastWriteTime.dwLowDateTime,
		lTFiles.mFindData.ftLastAccessTime.dwHighDateTime,
		lTFiles.mFindData.ftLastAccessTime.dwLowDateTime,
		lTFiles.mFindData.dwFileAttributes,
		lEncTitle,
		lEncSubject,
		lEncAuthor,
		lEncCat,
		lEncComments,
		lTFiles.mAddedDate.dwHighDateTime,
		lTFiles.mAddedDate.dwLowDateTime,
		lTFiles.mFlags,
		lTFiles.mExtID,
		lCreatedTmStmp,
		lModifiedTmStmp,
		lAccessedTmStmp,
		lAddedTmStmp
		);

	m_pDB->mDB.execDML(lSQL_AddFile);

	// get the identifier of the currently inserted file
	int lID = lTFiles.GetFileIDByPath(lTFiles.mPath);

	///////////////////////////////////////////////////////////////////////////////
	// additional tables can be updated only after the insert into the CTFile table 
	// update keywords table (tags)
	lTFiles.AddTagsFromKeywords(lID, lTFiles.mKeywords);

	// update collections
	if(lTWFolder.mCollectionID > 0)
	{
		m_pDB->AddFileToCollection(lTWFolder.mCollectionID, lID);
	}

	//if (lEntry )delete lEntry;
	//if (lCDS) delete lCDS;
	return TRUE;	
}

void CMainFrame::OnOptions()
{
	
	COptionsDlg lDlg;
	lDlg.DoModal();
	//OnAppLook(theApp.m_nAppLook);
	
}

void CMainFrame::OnSearch(UINT nID)
{
	CChildFrame* pChild = DYNAMIC_DOWNCAST(CChildFrame, this->GetActiveFrame());
	if(pChild == NULL)
		return;
	CLibraryView* pView = DYNAMIC_DOWNCAST(CLibraryView, pChild->GetActiveView());
	if(pView == 0) 
		return;

	switch (nID)
	{
		case ID_SEARCH_FOCUS:
			mSearchEdit->OnSetFocus(TRUE);
			break;

		case ID_SEARCH_EDIT:
		{
			CString lFindStr;
			CString lStr;
			lFindStr = mSearchEdit->GetEditText();
			
			switch (mSearchMode)
			{
				case kSearchAll:
					pView->Refresh();
					break;
				case kSearchTitle:
					pView->Refresh();
					break;
				case kSearchAuthor:
					pView->Refresh();
					break;
				case kSearchSubject:
					pView->Refresh();
					break;
				case kSearchTags:
					pView->Refresh();
					break;
			}
			
			break;
		}
		case ID_SEARCH_OPTION_ALL:
			mSearchMode = kSearchAll;
			InitFindText();
			break;
		case ID_SEARCH_OPTION_TITLE:
			mSearchMode = kSearchTitle;
			InitFindText();
			break;
		case ID_SEARCH_OPTION_AUTHOR:
			mSearchMode = kSearchAuthor;
			InitFindText();
			break;
		case ID_SEARCH_OPTION_SUBJECT:
			mSearchMode = kSearchSubject;
			InitFindText();
			break;
		case ID_SEARCH_OPTION_TAGS:
			mSearchMode = kSearchTags;
			InitFindText();
			break;
	}
}

void CMainFrame::OnContentSearch()
{
	CChildFrame* pChild = DYNAMIC_DOWNCAST(CChildFrame, this->GetActiveFrame());
	if(pChild == NULL)
		return;
	CLibraryView* pView = DYNAMIC_DOWNCAST(CLibraryView, pChild->GetActiveView());
	if(pView == 0) 
		return;

	CContentSearchDlg dlg(FILTERTREE_CURRENT_SELECTION);
	dlg.SetData_FromParent(pView->m_pParamCDB,0);
	if (dlg.DoModal() != IDOK)
		return;

//	ExportFilesToFolder(dlg.mCollectionID, dlg.mOutputFolder);
	pView->SearchByContent(dlg.mCollectionID, dlg.mKeyword);
}

void CMainFrame::OnUpdateStatusBar(CCmdUI* pCmdUI)
{
	CMDIChildWnd * pChild = ((CMDIFrameWnd*)(AfxGetApp()->m_pMainWnd))->MDIGetActive();
	if(pChild == NULL)
		return;

	CLibraryDoc* pLibraryDoc = DYNAMIC_DOWNCAST(CLibraryDoc, pChild->GetActiveDocument ());
	CLibraryView* pLibraryView = DYNAMIC_DOWNCAST(CLibraryView, pChild->GetActiveView ());
	
	CPDFDoc* pPDFDoc = DYNAMIC_DOWNCAST(CPDFDoc, pChild->GetActiveDocument ());
	CPDFView* pPDFView = DYNAMIC_DOWNCAST(CPDFView, pChild->GetActiveView ());

	if (pLibraryDoc != NULL && pLibraryView != NULL)
	{
		int lInLibrary  = 0;
		int lInList     = 0;
		int lInSelected = 0;
	
		CDBSQL lDBSQL;
		lDBSQL.AddField(_T("count (*)"));
		lDBSQL.AddTable(_T("Files"));
		CString lSQLStr = lDBSQL.GetSelect();
		if (lSQLStr.IsEmpty())
			return;
		m_pDB = pLibraryDoc->m_pParamCDB->pDB;
		lInLibrary = m_pDB->mDB.execScalar(lSQLStr);
		lInList = pLibraryView->GetListItemCount();
		lInSelected = pLibraryView->GetListSelectedCount();
	
		CString lStr1 = _T("%d files in library | %d files found | %d files selected");
		CString lStr2;
		lStr2.Format(lStr1, lInLibrary, lInList, lInSelected);

		pCmdUI->SetText(lStr2);
		pCmdUI->Enable();

		m_wndStatusBar.RecalcLayout();
		m_wndStatusBar.RedrawWindow();
	}
	if (pPDFDoc != NULL && pPDFView != NULL)
	{
		int nPage = pPDFDoc->m_nPdfCurrentPageNo;
		CString lStr1 = _T("Page: %d");
		CString lStr2;
		lStr2.Format(lStr1, nPage);

		pCmdUI->SetText(lStr2);
		pCmdUI->Enable();

		m_wndStatusBar.RecalcLayout();
		m_wndStatusBar.RedrawWindow();
	}
}

int CMainFrame::GetZoom_FromCombo()
{ 
  CBCGPRibbonCategory* lCategory = this->m_pRibbonCategory[1];
  CBCGPRibbonPanel*    lPanel = lCategory->GetPanel(1);
  CBCGPRibbonComboBox* zoomCombo = (CBCGPRibbonComboBox*)lPanel->GetElement(1);
  int nZoomNo = zoomCombo->GetCurSel();
  return nZoomNo;
}

void CMainFrame::SetZoom_FromCombo(DWORD_PTR dwZoom)
{
  CBCGPRibbonCategory* lCategory = this->m_pRibbonCategory[1];
  CBCGPRibbonPanel*    lPanel = lCategory->GetPanel(1);
  CBCGPRibbonComboBox* zoomCombo = (CBCGPRibbonComboBox*)lPanel->GetElement(1);
  zoomCombo->SelectItem(dwZoom);
}

int CMainFrame::GetCurrentPageNo()
{
	CBCGPRibbonCategory* lCategory = this->m_pRibbonCategory[1];
	CBCGPRibbonPanel*    lPanel = lCategory->GetPanel(2);
	CBCGPRibbonButtonsGroup* lGroup = (CBCGPRibbonButtonsGroup*)lPanel->GetElement(1);
	CBCGPRibbonEdit *lpEdit = (CBCGPRibbonEdit*)lGroup->GetButton(0);
	CString szCurrentPageNo = lpEdit->GetEditText();
	int nCurrentPageNo = _wtoi(szCurrentPageNo.GetBuffer());
	return nCurrentPageNo;
}

void CMainFrame::SetCurrentPageNo(int no)
{
	CBCGPRibbonCategory* lCategory = this->m_pRibbonCategory[1];
	CBCGPRibbonPanel*    lPanel = lCategory->GetPanel(2);
	CBCGPRibbonButtonsGroup* lGroup = (CBCGPRibbonButtonsGroup*)lPanel->GetElement(1);
	CBCGPRibbonEdit *lpEdit = (CBCGPRibbonEdit*)lGroup->GetButton(0);
	TCHAR szCurrentPageNo[0x20];
	wsprintf(szCurrentPageNo,_T("%d"),no);
	lpEdit->SetEditText(szCurrentPageNo);
}

void CMainFrame::OnUpdateAlwaysEnabledButton(CCmdUI* pCmdUI)
{
  TCHAR szStr[0x20];
  CFrameWnd *pChildWnd = NULL;
  if(pCmdUI->m_nID == ID_LIBRARY_EXPORT)
  {
	  pChildWnd = this->GetActiveFrame();
	  if(pChildWnd == this)
	  {
		  pCmdUI->Enable(FALSE);
		  return;
	  }

  }
  pCmdUI->Enable(TRUE);
}


BOOL CMainFrame::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct)
{
	CString strPrevLibNames[0x100];
	CString strLibName_Assistant,strLibName_ActiveLib;
	int msg = pCopyDataStruct->dwData;
	switch (msg)
	{
	case BENUBIRD_ASSISTANT_HWND:
		{
			memcpy(&g_AssistantHwnd,pCopyDataStruct->lpData,4);

			//restore previous setting
			TCHAR tmp[4];
			SendDataToAssistant(BENUBIRD_ASSISTANT_GET_PREVOPTION_ASK,tmp,4);

			break;
		}
	case BENUBIRD_ASSISTANT_REFRESH:
		{
			strLibName_Assistant =(LPCTSTR)pCopyDataStruct->lpData;

			CMDIChildWnd * pChild = ((CMDIFrameWnd*)(AfxGetApp()->m_pMainWnd))->MDIGetActive();
			if(pChild == NULL)
				return FALSE;
			CLibraryDoc* pLibraryDoc = DYNAMIC_DOWNCAST(CLibraryDoc, pChild->GetActiveDocument ());

			if(pLibraryDoc == 0)
				break;

			strLibName_ActiveLib = pLibraryDoc->m_csDBPathName;

			if(wcscmp(strLibName_Assistant.GetBuffer(),strLibName_ActiveLib.GetBuffer()) == 0)
			{
				CLibraryView* pLibraryView = DYNAMIC_DOWNCAST(CLibraryView, pChild->GetActiveView ());
				pLibraryView->Refresh();
			}

			break;
		}
	case BENUBIRD_ASSISTANT_GET_PREVOPTION_RESPONSE:
		{
			if(!gIsPro)
			{	
				theApp.OnOpenDefaultLibrary();
			}
			else
			{
				DWORD count = 0;
				DWORD len = 0;
				TCHAR *pBuf = (TCHAR*)pCopyDataStruct->lpData;
				while(len<pCopyDataStruct->cbData)
				{			
					strPrevLibNames[count] = pBuf;
					pBuf = pBuf + strPrevLibNames[count].GetLength()+1;
					len = len + (strPrevLibNames[count].GetLength()+1)*2;
					count++;
				}
			

				for(int i=0;i<count;i++)
				{
					OutputDebugString(_T("------------------------  10"));
					theApp.CreateLibraryView(strPrevLibNames[i].GetBuffer());
				}
			}
			break;
		}
	}
	
	return TRUE;
}

void CMainFrame::SendDataToAssistant(DWORD msg,LPCTSTR strBuf, int len)
{
//	if(!gIsPro)
//		return;
	COPYDATASTRUCT cds;
	cds.dwData = msg;
	cds.cbData = len;
	cds.lpData = (PVOID)strBuf;
	int nWaitSeconds = 5;
//	while(g_AssistantHwnd == 0 && nWaitSeconds-- > 0)
//		Sleep(1000);
	::SendMessage((HWND)g_AssistantHwnd,WM_COPYDATA,0,(LPARAM)&cds);

}


void CMainFrame::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CBCGPMDIFrameWnd::OnActivate(nState, pWndOther, bMinimized);
}
