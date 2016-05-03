// CLibraryView.cpp 
//*****************************************************************************

#include "stdafx.h"
#include "Benubird.h"
#include "LibraryView.h"

#include "MainFrm.h"
#include "LibraryDoc.h"
#include "CGetTextDlg.h"
#include "CManageTagsDlg.h"
#include "CManageCategoriesDlg.h"
#include "COptionsDlg.h"
#include "CLicenseDlg.h"
#include "CManageWatchedFoldersDlg.h"
#include "CGetInfoDlg.h"
#include "CBrowseFolderDlg.h"
#include "CSmartCollectionsDlg.h"
#include "CEmailUtils.h"
#include "CCopyCollectionDlg.h"
#include "CDeleteFileDlg.h"
#include "CMultipleProfileInformationDlg.h"
#include "CRibbonFindEdit.h"
#include "CVersion.h"
#include "CRenameFileDlg.h"
#include "CDB.h"
#include "CExportZipDlg.h"
#include "CExportFolderDlg.h"
#include "CExportUtils.h"
#include "CImportZipDlg.h"
#include "CImportFolderDlg.h"
#include "CImportUtils.h"
#include "COptionsDefs.h"
#include "CConvertToPDFDlg.h"
#include "CProgressDlg.h"
#include "zip.h"
#include "unzip.h"
#include "CAlertDlg.h"

#include "CBWorkspace.h"
#include "CBRepository.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CString GUIDgen();
extern CDB g_dbDefaultLibrary;
extern BOOL SetCViewPointerOfLib(CString path,CView* pView);
extern BOOL InsertNewLibPath(CString path,CView* pView);

extern CBenubirdApp theApp;

IMPLEMENT_DYNCREATE(CLibraryView, CView)

//*****************************************************************************
// Message map
BEGIN_MESSAGE_MAP(CLibraryView, CView)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CLOSE()

	ON_COMMAND(ID_OPEN, OnOpenFile)
	ON_UPDATE_COMMAND_UI(ID_OPEN, OnUpdateOpenFile)
	ON_COMMAND(ID_CLOSE, OnCloseFile)
	ON_UPDATE_COMMAND_UI(ID_CLOSE, OnUpdateCloseFile)
	
	ON_COMMAND(ID_PRINT, OnPrint)
	ON_UPDATE_COMMAND_UI(ID_PRINT, OnUpdatePrint)
		
	// Ribbon Commands
	

	ON_COMMAND(ID_ADD_FILE, OnAddFile)
	ON_COMMAND(ID_ADD_FOLDER, OnAddFolder)
	ON_COMMAND(ID_CREATE_COLLECTIONS, OnCreateCollections)
	ON_COMMAND(ID_CREATE_SUB_COLLECTIONS, OnCreateSubCollections)
	ON_UPDATE_COMMAND_UI(ID_CREATE_SUB_COLLECTIONS, OnUpdateCreateSubCollections)
	ON_COMMAND(ID_CREATE_SMART_COLLECTIONS, OnCreateSmartCollections)
	ON_COMMAND(ID_SHARE_EMAIL, OnShareEmail)
	ON_UPDATE_COMMAND_UI(ID_SHARE_EMAIL, OnUpdateShareEmail)
	ON_COMMAND(ID_VIEW_FILTER, OnViewFilter)
	ON_UPDATE_COMMAND_UI(ID_VIEW_FILTER, OnUpdateViewFilter)
	ON_COMMAND(ID_VIEW_TAGS, OnViewTags)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TAGS, OnUpdateViewTags)
	ON_COMMAND(ID_VIEW_FILELIST, OnViewFileList)
	ON_UPDATE_COMMAND_UI(ID_VIEW_FILELIST, OnUpdateViewFileList)
	ON_COMMAND(ID_VIEW_ICONS, OnViewIcons)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ICONS, OnUpdateViewIcons)
	ON_COMMAND(ID_VIEW_THUMBNAILS, OnViewThumbnails)
	ON_UPDATE_COMMAND_UI(ID_VIEW_THUMBNAILS, OnUpdateViewThumbnails)
	ON_COMMAND(ID_VIEW_PREVIEW, OnViewPreview)
	ON_UPDATE_COMMAND_UI(ID_VIEW_PREVIEW, OnUpdateViewPreview)
	ON_COMMAND(ID_VAULT_VIEW, OnVaultView)
	ON_COMMAND(ID_METADATA_TAGS, OnMetadataTags)
	ON_COMMAND(ID_LIBRARY_SETTING, OnLibrarySetting)
	

	ON_COMMAND(ID_OPEN_WORKSPACE, OnOpenWorkspace)
	ON_UPDATE_COMMAND_UI(ID_OPEN_WORKSPACE, OnUpdateOpenWorkspace)
	ON_COMMAND(ID_CLOSE_WORKSPACE, OnCloseWorkspace)
	ON_UPDATE_COMMAND_UI(ID_CLOSE_WORKSPACE, OnUpdateCloseWorkspace)

	ON_COMMAND(ID_METADATA_CATEGORIES, OnMetadataCategories)
	ON_COMMAND(ID_METADATA_INFO, OnMetadataInfo)
	ON_UPDATE_COMMAND_UI(ID_METADATA_INFO, OnUpdateMetadataInfo)
	ON_COMMAND(ID_FOLDERS_WATCHED, OnFoldersWatched)
	ON_COMMAND(ID_FREE_UPGRADE, OnFreeUpgrade)
	ON_COMMAND(ID_SWITCH_DIRECT, OnSwitchDirect)
	ON_COMMAND(ID_SWITCH_REPOSITORY, OnSwitchRepository)
	ON_COMMAND(ID_IMPORT_ZIP, OnImportZip)
	ON_COMMAND(ID_IMPORT_FOLDER, OnImportFolder)
	ON_COMMAND(ID_EXPORT_ZIP, OnExportZip)
	ON_COMMAND(ID_EXPORT_FOLDER, OnExportFolder)
	ON_COMMAND(ID_EXPORTTO_FOLDER, OnExportFilesFolder)
	ON_COMMAND(ID_EXPORTTO_ZIPPACKAGE, OnExportFilesZip)
		
	ON_COMMAND_RANGE(ID_HEADER_FILENAME, ID_HEADER_COMMENTS, OnHeaderMenu)
	ON_UPDATE_COMMAND_UI_RANGE(ID_HEADER_FILENAME, ID_HEADER_COMMENTS, OnUpdateHeaderMenu)
	ON_COMMAND(ID_HEADER_RESET_ORDER, OnHeaderMenuReset)
	ON_UPDATE_COMMAND_UI(ID_HEADER_RESET_ORDER, OnUpdateHeaderMenuReset)

	ON_COMMAND(ID_SHOW_IN_WIN_EXPLORER, OnShowInWinExplorer)
	ON_UPDATE_COMMAND_UI(ID_SHOW_IN_WIN_EXPLORER, OnUpdateShowInWinExplorer)
	ON_COMMAND_RANGE(ID_COLLECTIONID_MENUROOT, ID_COLLECTIONID_MENUROOT + ID_COLLECTIONID_MAXNUM, OnAddToCollection)	
	ON_COMMAND_RANGE(ID_REMOVE_FROM_COLLECTION_ID_MENUROOT, ID_REMOVE_FROM_COLLECTION_ID_MENUROOT + ID_COLLECTIONID_MAXNUM, OnRemoveFromCollection)
	ON_COMMAND_RANGE(ID_ADD_TO_TAGS_ID_MENUROOT, ID_ADD_TO_TAGS_ID_MENUROOT + ID_TAGSID_MAXNUM, OnAddToTags)	
	ON_COMMAND_RANGE(ID_REMOVE_FROM_TAGS_ID_MENUROOT, ID_REMOVE_FROM_TAGS_ID_MENUROOT + ID_TAGSID_MAXNUM, OnRemoveFromTags)
	ON_COMMAND(ID_DELETE_FILE, OnDeleteFile)
	ON_COMMAND(ID_RENAME_FILE, OnRenameFile)
	ON_UPDATE_COMMAND_UI_RANGE(ID_RENAME_FILE, ID_RENAME_FILE, OnUpdateRenameFile)
	ON_COMMAND(ID_COPY_FILE, OnCopyFile)
	ON_COMMAND(ID_CREATECOLLECTIONFROMSELECTION, OnCreateCollectionFromSelection)
	
	ON_COMMAND(ID_CONVERTTOPDF_FILE, OnConvertToPDFFile)
	ON_UPDATE_COMMAND_UI(ID_CONVERTTOPDF_FILE, OnUpdateConvertToPDFFile)

	ON_COMMAND(ID_OPEN_DEFAULT_VIEWER, OnOpenDefaultPDFViewer)
	ON_UPDATE_COMMAND_UI(ID_OPEN_DEFAULT_VIEWER, OnUpdateOpenDefaultPDFViewer)

	ON_COMMAND(ID_PREVIEWPDF, OnPreviewPDFFile)
	ON_UPDATE_COMMAND_UI(ID_PREVIEWPDF, OnUpdatePreviewPDFFile)

	ON_COMMAND(ID_EXPORT, OnExport)
	ON_COMMAND(ID_IMPORT, OnImport)

	ON_COMMAND(ID_EXPORTTO_ZIPPACKAGE, OnExportCollectionToZip)
	ON_COMMAND(ID_EXPORTTO_FOLDER, OnExportCollectionToFolder)

	ON_WM_DESTROY()
END_MESSAGE_MAP()

//*****************************************************************************
CLibraryView::CLibraryView()
{
	//mCurrentViewStyle = kDocumentFileListStyle_Details;

	mReportStyle = FILE_VIEW_LIST;
	
	mCurrentHeaderSizes[0]  = 200;	// Filename
	mCurrentHeaderSizes[1]  = 100;	// Title
	mCurrentHeaderSizes[2]  = 100;	// Author
	mCurrentHeaderSizes[3]  = 100;	// Subject
	mCurrentHeaderSizes[4]  = 100;	// Tags
	mCurrentHeaderSizes[5]  = 200;  // Collections
	mCurrentHeaderSizes[6]  = 50;	// Type
	mCurrentHeaderSizes[7]  = 50;	// Size
	mCurrentHeaderSizes[8]  = 0;	// Categories
	mCurrentHeaderSizes[9]  = 200;  // Path
	mCurrentHeaderSizes[10] = 100;	// Date Created
	mCurrentHeaderSizes[11] = 100;	// Date Modified
	mCurrentHeaderSizes[12] = 0;	// Date Added
	mCurrentHeaderSizes[13] = 0;	// Last Opened
	mCurrentHeaderSizes[14] = 0;	// Attributes
	mCurrentHeaderSizes[15] = 200;  // Comments
	

	mDefaultHeaderSizes[0]  = 200;	// Filename
	mDefaultHeaderSizes[1]  = 100;	// Title
	mDefaultHeaderSizes[2]  = 100;	// Author
	mDefaultHeaderSizes[3]  = 100;	// Subject
	mDefaultHeaderSizes[4]  = 100;	// Tags
	mDefaultHeaderSizes[5]  = 200;  // Collections
	mDefaultHeaderSizes[6]  = 50;	// Type
	mDefaultHeaderSizes[7]  = 50;	// Size
	mDefaultHeaderSizes[8]  = 100;	// Categories
	mDefaultHeaderSizes[9]  = 200;	// Path
	mDefaultHeaderSizes[10] = 100;	// Date Created
	mDefaultHeaderSizes[11] = 100;	// Date Modified
	mDefaultHeaderSizes[12] = 100;	// Date Added
	mDefaultHeaderSizes[13] = 100;	// Last Opened
	mDefaultHeaderSizes[14] = 50;	// Attributes
	mDefaultHeaderSizes[15] = 200;  // Comments
	
//	nThumbnailThreadId = 0;
//	nThumbnailThreadHandle = NULL;
	pThumbnailThread = NULL;

	m_wndTagsPanel = NULL;
	m_wndBrowserPanel = NULL;
	m_wndPreviewPanel = NULL;

	LoadHeadersSize();

	m_SettingInfo.nIsUseRepository = 0;	
}

//*****************************************************************************
CLibraryView::~CLibraryView()
{
	if (m_wndTagsPanel != NULL)
	{
		m_wndTagsPanel->DestroyWindow();
		delete m_wndTagsPanel;
	}
	if (m_wndBrowserPanel != NULL)
	{
		m_wndBrowserPanel->DestroyWindow();
		delete m_wndBrowserPanel;
	}

	// Library version only
	if (m_wndPreviewPanel != NULL)
	{
		m_wndPreviewPanel->DestroyWindow();
		delete m_wndPreviewPanel;
	}
}

/////////////////////////////////////////////////////////////////////////////
// CShortcutsView diagnostics

#ifdef _DEBUG
void CLibraryView::AssertValid() const
{
	CView::AssertValid();
}

void CLibraryView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG
CLibraryDoc* CLibraryView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CLibraryDoc)));
	return (CLibraryDoc*)m_pDocument;
}


//*****************************************************************************
BOOL CLibraryView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS, ::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1), NULL);
	return TRUE;
}

//*****************************************************************************
void CLibraryView::SendLibrarySettingToAssistant(DWORD state) // state = 1:open  0:close
{
	CLibraryDoc *pDoc = this->GetDocument();

	wchar_t tmpBuf[0x100];
	memset(tmpBuf,0,0x100*sizeof(wchar_t));
	memcpy(tmpBuf,&m_SettingInfo.nIsUseRepository,2);//this length 2 means 4 byte.
	memcpy(&tmpBuf[2],&state,2);
	wcscpy((wchar_t*)(tmpBuf+4),pDoc->m_csDBPathName.GetBuffer());
	int length = ((pDoc->m_csDBPathName).GetLength() + 1) * 2 ;
	((CMainFrame*)AfxGetApp()->m_pMainWnd)->SendDataToAssistant(BENUBIRD_ASSISTANT_LIBRARY_SETTING,tmpBuf,length+8);
}
int CLibraryView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_qpdf.UnlockKey(_T(QUICKPDF_LICENSE_KEY));
	if(m_qpdf.Unlocked() != 1)
	{
		return -1;
	}

	CMainFrame* pMainFrame = DYNAMIC_DOWNCAST(CMainFrame, ((CMainFrame*)AfxGetMainWnd()));

	CLibraryDoc *pDoc = this->GetDocument();
	m_pParamCDB = pDoc->m_pParamCDB;
	m_pDB = m_pParamCDB->pDB;
	m_pFilesSQL = m_pParamCDB->pFilesSQL;

	//init LIBRARY_SETTING_INFO structure m_SettingInfo;
	TCHAR szTmp[0x100];
	memset(szTmp,0,0x100*sizeof(TCHAR));
	if(m_pParamCDB->pDB->GetInfoValue(_T("isRepository"),szTmp)==0)
	{
		AfxMessageBox(_T("Get IsRepository Failed"));
		ExitProcess(0);
	}
	if(wcscmp(szTmp,_T("YES"))==0)
	{
		m_pParamCDB->nIsUseRepository = 1; // read repository status from db.
		m_SettingInfo.nIsUseRepository = 1;
		pMainFrame->AddWorkspacePanel();
	}
	else
	{
		m_pParamCDB->nIsUseRepository = 0; // read repository status from db.
		m_SettingInfo.nIsUseRepository = 0;
		//pMainFrame->RemoveWorkspacePanel();
	}

	if(m_pParamCDB->pDB->GetInfoValue(_T("Repository"),szTmp)==0)
	{
		AfxMessageBox(_T("Get Repository Failed"));
		ExitProcess(0);
	}
	m_SettingInfo.strRepositoryPath = szTmp;

	if(m_pParamCDB->pDB->GetInfoValue(_T("Workspace"),szTmp)==0)
	{
		AfxMessageBox(_T("Get Workspace Failed"));
		ExitProcess(0);
	}
	m_SettingInfo.strWorkspacePath = szTmp;

	if(m_pParamCDB->pDB->GetInfoValue(_T("WorkspaceType"),szTmp)==0)
	{
		AfxMessageBox(_T("Get WorkspaceType Failed"));
		ExitProcess(0);
	}
	if(wcscmp(szTmp,_T("0"))==0)
	{
		m_SettingInfo.nWorkspaceType = 0;
		m_pParamCDB->nWorkspaceType = 0;
	}
	else if(wcscmp(szTmp,_T("1"))==0)
	{
		m_SettingInfo.nWorkspaceType = 1;
		m_pParamCDB->nWorkspaceType = 1;
	}
	else if(wcscmp(szTmp,_T("2"))==0)
	{
		m_SettingInfo.nWorkspaceType = 2;
		m_pParamCDB->nWorkspaceType = 2;
	}

	//let Assistant to know.
	SendLibrarySettingToAssistant(1);

	m_pViewInfo = &pDoc->m_ViewInfo;

	m_Repository.Set_ParamForCDB(m_pParamCDB);
	m_Repository.InitializePath(m_SettingInfo.strRepositoryPath.GetBuffer());
	m_pViewInfo->pRepository = &m_Repository;

	m_workspace.Set_ParamFromParent(m_pParamCDB);
	m_workspace.Initialize(m_SettingInfo.strWorkspacePath);
	m_pViewInfo->pWorkspace = &m_workspace;

	m_wndOutlookBar.Set_ParamForCDB(m_pParamCDB,m_pViewInfo);
	mDocumentFileList.Set_ParamForCDB(m_pParamCDB,m_pViewInfo);

	RECT rcClient;
	GetClientRect(&rcClient);

	// File list
	mDocumentFileList.Create(WS_CHILD | LVS_REPORT | LVS_SHOWSELALWAYS | LVS_OWNERDATA , rcClient, this, ID_DOCUMENT_THUMB_LIST);
	mDocumentFileList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | WS_EX_ACCEPTFILES);
	mDocumentFileList.DragAcceptFiles(TRUE);
	mDocumentFileList.ShowWindow(SW_SHOW);

	// Browser
	m_wndBrowserPanel = new CBrowserPanel;
	m_wndBrowserPanel->Set_ParamForCDB(m_pParamCDB);
	if (!m_wndBrowserPanel->Create(_T("Browser"), this, FALSE, MAKEINTRESOURCE(IDD_BROWSER_DLG), 
		CBRS_SIZE_DYNAMIC | CBRS_ALIGN_TOP | CBRS_TOOLTIPS | CBRS_FLYBY, ID_BROWSER_PANEL))
	{
		TRACE0("Failed to create Browser bar\n");
		return FALSE;
	}

	
	
	// Tags Layout
	m_wndTagsPanel = new CTagsPanel;
	m_wndTagsPanel->Set_ParamForCDB(m_pParamCDB);
	if (!m_wndTagsPanel->Create (_T("Tags"), this, FALSE, MAKEINTRESOURCE (IDD_TAGS_DLG), 
		CBRS_ALIGN_RIGHT | CBRS_TOOLTIPS | CBRS_FLOAT_MULTI, ID_TAGS_PANEL))
	{
		TRACE0("Failed to create Tags Layout bar\n");
		return FALSE;
	}

	// Preview
	m_wndPreviewPanel = new CPreviewPanel;
	if (!m_wndPreviewPanel->Create(_T("Preview"), this, FALSE, MAKEINTRESOURCE(IDD_PREVIEW_DLG), 
		CBRS_ALIGN_LEFT | CBRS_TOOLTIPS | CBRS_FLOAT_MULTI, ID_PREVIEW_PANEL))
	{
		TRACE0("Failed to create Preview bar\n");
		return FALSE;
	}

	// Collection panel
	if (!CreateOutlookBar ())
	{
		TRACE0("Failed to create shortcuts bar\n");
		return -1;      // fail to create
	}


	SetCViewPointerOfLib(pDoc->m_csDBPathName,this);
	
	
	return 0;
}

void CLibraryView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	CMDIChildWnd * pChild = ((CMDIFrameWnd*)(AfxGetApp()->m_pMainWnd))->MDIGetActive();
	if(pChild == NULL)
		return;
	m_pDoc = DYNAMIC_DOWNCAST(CLibraryDoc, pChild->GetActiveDocument ());
	mDocumentFileList.m_pDoc = m_pDoc;

	InitializeDocumentFileListHeader();
	InitializeDocumentFileList();	

	((CBCGPMDIChildWnd*)pChild)->DockControlBar(&m_wndOutlookBar);
	((CBCGPMDIChildWnd*)pChild)->DockControlBar(m_wndTagsPanel);
	((CBCGPMDIChildWnd*)pChild)->DockControlBar(m_wndBrowserPanel);

	m_wndPreviewPanel->DockToWindow(&m_wndOutlookBar, CBRS_ALIGN_BOTTOM);
	m_wndPreviewPanel->ShowControlBar(FALSE, FALSE, FALSE);

	((CMainFrame*)AfxGetApp()->m_pMainWnd)->ActivateCategory(0);
}

void CLibraryView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
{
	CView::OnActivateView(bActivate, pActivateView, pDeactiveView);

	if (bActivate)
	{
		((CMainFrame*)AfxGetApp()->m_pMainWnd)->ActivateCategory(0);
//		Refresh();
	}
}

void CLibraryView::ShowBars(BOOL bShow)
{
	m_wndPreviewPanel->ShowControlBar(bShow, FALSE, FALSE);
	m_wndTagsPanel->ShowControlBar(bShow, FALSE, FALSE);
	m_wndBrowserPanel->ShowControlBar(bShow, FALSE, FALSE);
	m_wndOutlookBar.ShowControlBar(bShow, FALSE, FALSE);
}

//*****************************************************************************
void CLibraryView::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	mDocumentFileList.SetWindowPos(NULL, -1, -1, cx, cy, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
}

void CLibraryView::OnDraw(CDC* /*pDC*/)
{
	// TODO: add draw code for native data here
}

BOOL CLibraryView::CreateOutlookBar ()
{
	const CString strCaption = _T("Shortcuts");
	const int nInitialWidth = 200;

	m_wndOutlookBar.SetMode2003();
	return m_wndOutlookBar.Create (strCaption, this, 
		CRect (0, 0, nInitialWidth, nInitialWidth), 
		2222, WS_CHILD | WS_VISIBLE | CBRS_LEFT);
}

//*****************************************************************************
void CLibraryView::InitializeDocumentFileListHeader()
{
	mDocumentFileList.InsertColumn(0,  _T("Filename"),		LVCFMT_LEFT, mCurrentHeaderSizes[0]	== -1 ? mDefaultHeaderSizes[0]  : mCurrentHeaderSizes[0]);
	mDocumentFileList.InsertColumn(1,  _T("Title"),		LVCFMT_LEFT, mCurrentHeaderSizes[1]	== -1 ? mDefaultHeaderSizes[1]  : mCurrentHeaderSizes[1]);
	mDocumentFileList.InsertColumn(2,  _T("Author"),		LVCFMT_LEFT, mCurrentHeaderSizes[2]	== -1 ? mDefaultHeaderSizes[2]  : mCurrentHeaderSizes[2]);
	mDocumentFileList.InsertColumn(3,  _T("Subject"),		LVCFMT_LEFT, mCurrentHeaderSizes[3]	== -1 ? mDefaultHeaderSizes[3]  : mCurrentHeaderSizes[3]);
	mDocumentFileList.InsertColumn(4,  _T("Tags"),			LVCFMT_LEFT, mCurrentHeaderSizes[4]	== -1 ? mDefaultHeaderSizes[4]  : mCurrentHeaderSizes[4]);
	mDocumentFileList.InsertColumn(5,  _T("Collections"),  LVCFMT_LEFT, mCurrentHeaderSizes[5]  == -1 ? mDefaultHeaderSizes[5]  : mCurrentHeaderSizes[5]);
	mDocumentFileList.InsertColumn(6,  _T("Type"),			LVCFMT_LEFT, mCurrentHeaderSizes[6]	== -1 ? mDefaultHeaderSizes[6]  : mCurrentHeaderSizes[6]);
	mDocumentFileList.InsertColumn(7,  _T("Size"),			LVCFMT_LEFT, mCurrentHeaderSizes[7]	== -1 ? mDefaultHeaderSizes[7]  : mCurrentHeaderSizes[7]);
	mDocumentFileList.InsertColumn(8,  _T("Category"),		LVCFMT_LEFT, mCurrentHeaderSizes[8]	== -1 ? mDefaultHeaderSizes[8]  : mCurrentHeaderSizes[8]);
	mDocumentFileList.InsertColumn(9,  _T("Path"),			LVCFMT_LEFT, mCurrentHeaderSizes[9]	== -1 ? mDefaultHeaderSizes[9]  : mCurrentHeaderSizes[9]);
	mDocumentFileList.InsertColumn(10, _T("Date Created"),	LVCFMT_LEFT, mCurrentHeaderSizes[10]	== -1 ? mDefaultHeaderSizes[10] : mCurrentHeaderSizes[10]);
	mDocumentFileList.InsertColumn(11, _T("Date Modified"),LVCFMT_LEFT, mCurrentHeaderSizes[11] == -1 ? mDefaultHeaderSizes[11] : mCurrentHeaderSizes[11]);
	mDocumentFileList.InsertColumn(12, _T("Date Added"),	LVCFMT_LEFT, mCurrentHeaderSizes[12] == -1 ? mDefaultHeaderSizes[12] : mCurrentHeaderSizes[12]);
	mDocumentFileList.InsertColumn(13, _T("Last Opened"),	LVCFMT_LEFT, mCurrentHeaderSizes[13] == -1 ? mDefaultHeaderSizes[13] : mCurrentHeaderSizes[13]);
	mDocumentFileList.InsertColumn(14, _T("Attributes"),	LVCFMT_LEFT, mCurrentHeaderSizes[14] == -1 ? mDefaultHeaderSizes[14] : mCurrentHeaderSizes[14]);
	mDocumentFileList.InsertColumn(15, _T("Comments"),     LVCFMT_LEFT, mCurrentHeaderSizes[15] == -1 ? mDefaultHeaderSizes[15] : mCurrentHeaderSizes[15]);
	LoadHeadersColumnOrder();
}

//*****************************************************************************
void CLibraryView::InitializeDocumentFileList()
{
	//CMainFrame* lMainFrame;
	//lMainFrame = (CMainFrame*)AfxGetMainWnd();
	//?lMainFrame->InitFindText();
	RefreshFileList();
}

//*****************************************************************************
void CLibraryView::RefreshFileList()
{
	int lTreeFilterID = GetSelectedCollectionGuiID();

	::AfxGetApp()->DoWaitCursor(TRUE);
	mDocumentFileList.StopPrepareThumbnail();
	mDocumentFileList.SetRedraw(FALSE);

	try
	{
		// clear all
		m_pDoc->sFilesArray.RemoveAll();

		// Prepare SQL
		m_pFilesSQL->Clear();
		m_pFilesSQL->mSpecCondition.RemoveAll();

		m_pFilesSQL->AddField(_T("Files.idFile"));

		// field number 25
		m_pFilesSQL->AddField(TagField);
		// field number 26
		m_pFilesSQL->AddField(CollectionField);

		m_pFilesSQL->AddTable(_T("Files"));

		CString strTagsCondition, strBrowserCondition;
		GetTagsPanel()->GetStringForSelect(strTagsCondition);
		GetBrowsePanel()->GetStringForSelect(strBrowserCondition);

		CString lFindStr = ((CMainFrame*)AfxGetMainWnd())->GetFindText();
		int lSearchMode = ((CMainFrame*)AfxGetMainWnd())->GetSearchMode();

		m_pDoc->UpdateSQLFromCollection(lTreeFilterID,m_pFilesSQL);
		m_pDoc->UpdateSQLFromBrowser(strBrowserCondition, m_pFilesSQL);
		m_pDoc->UpdateSQLFromTag(strTagsCondition, m_pFilesSQL);
		m_pDoc->UpdateSQLFromFind(IsTagsPanelEmpty(), lSearchMode, lFindStr, m_pFilesSQL);
		m_pDoc->UpdateSQLSort(mDocumentFileList.m_iColumn, mDocumentFileList.m_bAscending, m_pFilesSQL);
		
		CString lSQLStr = m_pFilesSQL->GetSelect();

		/*
		CTFiles lTFiles;
		if (!lSQLStr.IsEmpty()) 
			lTFiles.ExecSelect(lSQLStr);

		while (lTFiles.mDataIsValid)
		{
			CTFiles_Entry lEntry;
			lEntry.mWasRead = false;
			lEntry.mPreviewID = -1;

			lEntry.mID = lTFiles.mID;


			lEntry.mPath = lTFiles.mPath;
			lEntry.mName = lTFiles.mName;

			lEntry.mExt = _T("");
			if (lTFiles.mName.Find('.') != -1)
				lEntry.mExt = lTFiles.mName.Right(lTFiles.mName.GetLength() - lTFiles.mName.ReverseFind('.') - 1);

			//memcpy(&m_pDoc->sFilesArray[i].mFindData, &lTFiles.mFindData, sizeof(WIN32_FIND_DATA));
			lEntry.mFindData.nFileSizeHigh = lTFiles.mFindData.nFileSizeHigh;
			lEntry.mFindData.nFileSizeLow = lTFiles.mFindData.nFileSizeLow;
			lEntry.mFindData.ftCreationTime.dwHighDateTime = lTFiles.mFindData.ftCreationTime.dwHighDateTime;
			lEntry.mFindData.ftCreationTime.dwLowDateTime = lTFiles.mFindData.ftCreationTime.dwLowDateTime;
			lEntry.mFindData.ftLastWriteTime.dwHighDateTime = lTFiles.mFindData.ftLastWriteTime.dwHighDateTime;
			lEntry.mFindData.ftLastWriteTime.dwLowDateTime = lTFiles.mFindData.ftLastWriteTime.dwLowDateTime;
			lEntry.mFindData.ftLastAccessTime.dwHighDateTime = lTFiles.mFindData.ftLastAccessTime.dwHighDateTime;
			lEntry.mFindData.ftLastAccessTime.dwLowDateTime = lTFiles.mFindData.ftLastAccessTime.dwLowDateTime;
			lEntry.mFindData.dwFileAttributes;
			lEntry.mTitle = lTFiles.mTitle;
			lEntry.mSubject = lTFiles.mSubject;
			lEntry.mAuthor = lTFiles.mAuthor;
			lEntry.mCategory = lTFiles.mCategory;
			lEntry.mComments = lTFiles.mComments;
			lEntry.mAddedDate.dwHighDateTime = lTFiles.mAddedDate.dwHighDateTime;
			lEntry.mAddedDate.dwLowDateTime = lTFiles.mAddedDate.dwLowDateTime;
			lEntry.mFlags = lTFiles.mFlags;
			lEntry.mExtID = lTFiles.mExtID;

			lEntry.mTags = lTFiles.mQ.getStringField(25); // virtual field TagsStr
				
			lEntry.mSize = _T("");
			m_pDoc->UFileSize2Str(lEntry.mSize, 
				lTFiles.mFindData.nFileSizeHigh, lTFiles.mFindData.nFileSizeLow);
								
			// prepare date/times
			CTime lTimeCr(lTFiles.mFindData.ftCreationTime);
			lEntry.mCreatedTmStmp = lTimeCr.Format(_T("%d %B %Y")); 

			CTime lTimeMod(lTFiles.mFindData.ftLastWriteTime);
			lEntry.mModifiedTmStmp = lTimeMod.Format(_T("%d %B %Y")); 

			CTime lTimeAdded(lTFiles.mAddedDate);
			lEntry.mAccessedTmStmp = lTimeAdded.Format(_T("%d %B %Y")); 

			CTime lTimeOpened(lTFiles.mFindData.ftLastAccessTime);
			lEntry.mAddedTmStmp = lTimeOpened.Format(_T("%d %B %Y")); 

			// Atributes
			lEntry.mAttr = _T("");
			if(lTFiles.mFlags == FILE_FLAG_FNF)
				lEntry.mAttr = _T("MISSING FROM DISC");
			else
			{
				if ((lTFiles.mFindData.dwFileAttributes  & FILE_ATTRIBUTE_SYSTEM) == FILE_ATTRIBUTE_SYSTEM)
					lEntry.mAttr +=	_T("S");
				if ((lTFiles.mFindData.dwFileAttributes  & FILE_ATTRIBUTE_READONLY) == FILE_ATTRIBUTE_READONLY)
					lEntry.mAttr +=	_T("R");	
				if ((lTFiles.mFindData.dwFileAttributes  & FILE_ATTRIBUTE_HIDDEN) == FILE_ATTRIBUTE_HIDDEN)
					lEntry.mAttr +=	_T("H");	
				if ((lTFiles.mFindData.dwFileAttributes  & FILE_ATTRIBUTE_ARCHIVE) == FILE_ATTRIBUTE_ARCHIVE)
					lEntry.mAttr += _T("A");
				if ((lTFiles.mFindData.dwFileAttributes  & FILE_ATTRIBUTE_COMPRESSED) == FILE_ATTRIBUTE_COMPRESSED)
					lEntry.mAttr +=	_T("C");	
			}

			lEntry.mInWorkspace = gWorkspace.FileIsInWorkspace(lTFiles.mID);
			lEntry.mCollections = lTFiles.mQ.getStringField(26); // virtual field CollectionsStr
			

			lEntry.mSubject = lTFiles.mQ.getStringField(2);
			lEntry.mAuthor = lTFiles.mQ.getStringField(1);
			m_pDoc->sFilesArray.Add(lEntry);

			lTFiles.LoadNextRow();

		}			
*/		
		CTFiles lTable(m_pParamCDB);
		lTable.mQ = m_pDB->mDB.execQuery(lSQLStr);
		while(!lTable.mQ.eof())
		{
			CTFiles_Entry lEntry;
			lEntry.mWasRead = false;
			lEntry.mPreviewID = -1;
			lEntry.mID = lTable.mQ.getIntField(0);
			lTable.mQ.nextRow();
			//m_pDoc->sFilesArray.SetAtGrow(i, lEntry);
			
			lEntry.mSubject = lTable.mQ.getStringField(2);
			lEntry.mAuthor = lTable.mQ.getStringField(1);

			m_pDoc->sFilesArray.Add(lEntry);
			
		}

		m_pFilesSQL->mField.RemoveAll();
		// lucia Preco je tu toto zmazane!!?
		//m_pFilesSQL->mSort.RemoveAll();

		m_pFilesSQL->AddField(_T("Files.*"));
		m_pFilesSQL->AddField(TagField);
		m_pFilesSQL->AddField(CollectionField);

		// prepare list control
		mDocumentFileList.SetupListCtrl(mReportStyle);
	}
	catch(...)
	{
	}

	mDocumentFileList.SetRedraw(TRUE);

	::AfxGetApp()->DoWaitCursor(FALSE);
	return;
}

bool CLibraryView::CheckKeywordInFile(CString lPath, CString lKeyword)
{
	int	pdfId = 0;
	int pageRef = 0;
	int pageCount = 0;
	int idxPage = 1;
	int isFound = 0;

	pdfId = m_qpdf.DAOpenFile(lPath.GetBuffer(MAX_PATH), L"");
	if(pdfId == 0)
		return FALSE;

	pageCount = m_qpdf.DAGetPageCount(pdfId);

	while(idxPage <= pageCount)
	{
		pageRef = m_qpdf.DAFindPage(pdfId, idxPage);
		wstring pageText = m_qpdf.DAExtractPageText(pdfId, pageRef, 7);
		if(pageText.find(lKeyword.GetBuffer(255)) != wstring::npos) {
			isFound = 1;
			break;
		}
		idxPage++;

	}

	m_qpdf.DACloseFile(pdfId);

	if(isFound)
		return TRUE;
	else
		return FALSE;
}

//*****************************************************************************
void CLibraryView::SearchByContent(int inCollectionID, CString lKeyword)
{
	mDocumentFileList.StopPrepareThumbnail();
	mDocumentFileList.SetRedraw(FALSE);

	// clear all
	m_pDoc->sFilesArray.RemoveAll();


	CTFiles lFilesTable(m_pParamCDB);
	if (inCollectionID != FILTERTREE_CURRENT_SELECTION)
	{
		if (!PrepareFileList(inCollectionID, &lFilesTable))
			return;
	} else {
		int lTreeFilterID = GetSelectedCollectionGuiID();
		if (!PrepareFileList(lTreeFilterID, &lFilesTable))
			return;
	}

	CProgressDlg* lProgressDlg = new CProgressDlg;
	lProgressDlg->SetMax(lFilesTable.GetCount());
	lProgressDlg->SetCaption(_T("Searching by content of pdf files..."));
	lProgressDlg->CreateProgressDlg();

	while(!lFilesTable.mQ.eof())
	{
		lFilesTable.LoadCurrentRow();

		// check keyword in the file
		if(CheckKeywordInFile(lFilesTable.mPath, lKeyword))
		{
			CTFiles_Entry lEntry;
			lEntry.mWasRead = false;
			lEntry.mPreviewID = -1;
			lEntry.mID = lFilesTable.mQ.getIntField(0);
			lEntry.mSubject = lFilesTable.mQ.getStringField(2);
			lEntry.mAuthor = lFilesTable.mQ.getStringField(1);

			m_pDoc->sFilesArray.Add(lEntry);
		}

		lFilesTable.mQ.nextRow();

		lProgressDlg->SetMessage(lFilesTable.mName);
		lProgressDlg->StepIt();
		if (lProgressDlg->IsCancelled()) break;

		//lFilesTable.mName
	}
	lProgressDlg->StepIt();

	delete lProgressDlg;
	lProgressDlg = NULL;

	// prepare list control
	mDocumentFileList.SetupListCtrl(mReportStyle);
	mDocumentFileList.SetRedraw(TRUE);

	return;
}

//*****************************************************************************
void CLibraryView::OnHeaderMenu(UINT nID)
{

	CBCGPHeaderCtrl *pHead = mDocumentFileList.GetHeaderCtrl_MY();
	int nColCount = pHead->GetItemCount();
    int* pOrderArray = new int[nColCount];
	mDocumentFileList.GetColumnOrderArray(pOrderArray, nColCount);

	LVCOLUMN col;
	col.mask = LVCF_WIDTH;
	nID = nID - ID_HEADER_FILENAME;

	int nWidth = mDocumentFileList.GetColumnWidth(nID);
	if(nWidth == 0)
	{
		nWidth = mDefaultHeaderSizes[nID];
		mCurrentHeaderSizes[nID] = mDefaultHeaderSizes[nID];
	}
	else
	{
		nWidth = 0;
		mCurrentHeaderSizes[nID] = 0;
	}
	mDocumentFileList.SetColumnWidth(nID,nWidth);
	mDocumentFileList.SetRedraw();
	mDocumentFileList.InvalidateRect(false);

}
//*****************************************************************************
void CLibraryView::OnUpdateHeaderMenu(CCmdUI* pCmdUI)
{
	int lColumn;
	BOOL lChecked;

	lColumn = pCmdUI->m_nID - ID_HEADER_FILENAME;
	if (mDocumentFileList.GetColumnWidth(lColumn) <= 0)
		lChecked = FALSE;
	else
		lChecked = TRUE;
	pCmdUI->SetCheck(lChecked);
}

//*****************************************************************************
void CLibraryView::OnHeaderMenuReset()
{

	CBCGPHeaderCtrl *pHead = mDocumentFileList.GetHeaderCtrl_MY();
	int nColCount = pHead->GetItemCount();
    int* pOrderArray = new int[nColCount];

	mDocumentFileList.GetColumnOrderArray(pOrderArray,nColCount);
	int nWidth = 0;
	for( int i=0;i<NUM_HEADER;i++)
	{
		pOrderArray[i] = i;
		mCurrentHeaderSizes[i] = mDefaultHeaderSizes[i];
		nWidth = mDocumentFileList.GetColumnWidth(i);
		mDocumentFileList.SetColumnWidth(i,mCurrentHeaderSizes[i]);
	}
	mDocumentFileList.SetColumnOrderArray(nColCount,pOrderArray);

	SaveHeadersSize();
	SaveHeadersColumnOrder();

	delete pOrderArray;

	mDocumentFileList.SetRedraw();
	mDocumentFileList.InvalidateRect(false);

}
//*****************************************************************************
void CLibraryView::OnUpdateHeaderMenuReset(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CLibraryView::SaveHeadersSize()
{
	int lWidth;
	CString lStr;
	CWinApp* lApp;
	lApp = AfxGetApp();

	for (int i = 0; i < NUM_HEADER; i++)
	{
		lStr.Format(_T("Header%d"), i + 1);
		lWidth = mDocumentFileList.GetColumnWidth(i);
		lApp->WriteProfileInt(_T("Headers"), lStr, lWidth);
	}
}

void CLibraryView::LoadHeadersSize()
{
	CString lStr;
	CWinApp* lApp;
	int lWidth;

	lApp = AfxGetApp();

	for (int i = 0; i < NUM_HEADER; i++)
	{
		lStr.Format(_T("Header%d"), i + 1);
		lWidth = lApp->GetProfileInt(_T("Headers"), lStr, mCurrentHeaderSizes[i]);
		mCurrentHeaderSizes[i] = lWidth;
	}
}

void CLibraryView::SaveHeadersColumnOrder()
{
	int  nColumnCount = mDocumentFileList.GetHeaderCtrl().GetItemCount();
	CWinApp* lApp;
	lApp = AfxGetApp();

	LPINT pnOrder = (LPINT) malloc(nColumnCount*sizeof(int));
	ASSERT(pnOrder != NULL);

	mDocumentFileList.GetColumnOrderArray(pnOrder, nColumnCount);
	lApp->WriteProfileBinary(_T("Headers"), _T("ColumnsOrder"), (LPBYTE)pnOrder, nColumnCount*sizeof(INT));
	free(pnOrder);
}

void CLibraryView::LoadHeadersColumnOrder()
{
	int  nColumnCount = mDocumentFileList.GetHeaderCtrl().GetItemCount();
	UINT nSize = 0;
	CWinApp* lApp;
	lApp = AfxGetApp();

	LPINT pnOrder = NULL;
	lApp->GetProfileBinary(_T("Headers"), _T("ColumnsOrder"), (LPBYTE*)&pnOrder, &nSize);
	if(pnOrder) {
		mDocumentFileList.SetColumnOrderArray(nColumnCount, pnOrder);
		free(pnOrder);
	}
}

void CLibraryView::SetViewStyle(ViewType inType)
{
	if (mReportStyle == inType)
		return;

	mReportStyle = inType;

	// store the selection
	CIntArray lSelRowsArray;
	if(GetListSelectedCount() > 0)
	{
		POSITION lPos = GetListFirstSelectedItemPosition();
		while(lPos)
		{
			int lRowID = ((int)lPos) - 1;
			lSelRowsArray.Add(lRowID);
			GetListNextSelectedItem(lPos);
		}
	}

	mDocumentFileList.SetupListCtrl(mReportStyle);

//	RefreshFileList();
}

void CLibraryView::UpdateItems(int nStartItem, int nEndItem)
{
	mDocumentFileList.ClearCache(nStartItem, nEndItem);
	mDocumentFileList.Invalidate();
//	mDocumentFileList.RedrawItems(nStartItem, nEndItem);
}

POSITION CLibraryView::GetListFirstSelectedItemPosition() const 
{ 
	return mDocumentFileList.GetFirstSelectedItemPosition();
}


int CLibraryView::GetListNextSelectedItem(POSITION& pos) const 
{ 
	return mDocumentFileList.GetNextSelectedItem(pos);
}

UINT CLibraryView::GetListSelectedCount() const 
{ 
	return mDocumentFileList.GetSelectedCount();
}

int CLibraryView::GetListItemCount() const
{
	return mDocumentFileList.GetItemCount();
}

int CLibraryView::GetListMinSize() const
{
	int lWidth = 0;

	if (mReportStyle == FILE_VIEW_ICONS)
	{
		lWidth = 270;
	}
	else if (mReportStyle == FILE_VIEW_THUMBNAILS)
	{
		lWidth = 500;
	}
	else
	{
		int lArray[NUM_HEADER];
		mDocumentFileList.GetColumnOrderArray(lArray, NUM_HEADER);
		for (int i = 0; i < NUM_HEADER; i++)
		{
			lWidth = mDocumentFileList.GetColumnWidth(lArray[i]);
			if (lWidth != 0) 
			{
				return lWidth;
			}
		}
		return lWidth;
	}
	return 0;
}

int CLibraryView::GetSelectedCollectionGuiID()
{
	return m_wndOutlookBar.m_wndCollectionPanel.GetSelectedCollID();
}

void CLibraryView::RefreshTagList()
{
	if (m_wndTagsPanel && m_wndTagsPanel->IsVisible())
		m_wndTagsPanel->Refresh();
}

void CLibraryView::RefreshQuickBrowser()
{
	if (m_wndBrowserPanel && m_wndBrowserPanel->IsVisible())
		m_wndBrowserPanel->Refresh();
}

void CLibraryView::RefreshPreview()
{
	if (m_wndPreviewPanel && m_wndPreviewPanel->IsVisible())
		m_wndPreviewPanel->Refresh();
}

void CLibraryView::Refresh()
{
	RefreshQuickBrowser();
	RefreshTagList();
	RefreshFileList();
	RefreshPreview();
}

///////////////////////////////////////////////////////////////////////////////////
// File commands
///////////////////////////////////////////////////////////////////////////////////
void CLibraryView::OnShareEmail()
{
	std::vector<CString> lAttachment;
	CString lSubject;
	if (AfxMessageBox(_T("Do you wish to attach the selected files to your default email program?"), MB_YESNO) == IDYES)
	{	
		int lCount = GetListSelectedCount();
		if (lCount > 0)
		{
			POSITION lPos = GetListFirstSelectedItemPosition();
			while(lPos)
			{
				int lSelItemIdx = GetListNextSelectedItem(lPos);
				CString lStr;
				lStr.Format(_T("%s"), m_pDoc->sFilesArray[lSelItemIdx].mPath);
				lAttachment.push_back(lStr);
			}
			if (GetListSelectedCount() == 1)
			{
				CString lTmpStr;
				lTmpStr = lAttachment[0];
				lSubject = lTmpStr.Right(lTmpStr.GetLength() - lTmpStr.ReverseFind('\\') - 1);
			}
			else
				lSubject = _T("Files from Benubird");
		}
		
		CEmailUtils lEmailUtils;
		if (lAttachment.size() != 0)
			lEmailUtils.SendMail(AfxGetMainWnd()->GetSafeHwnd(), lAttachment, CT2CA(lSubject));
	}
}

void CLibraryView::OnOpenFileWrapper()
{
	OnOpenFile();
}

BOOL FileIsExisting(CString fileName)
{
	HANDLE file1 = CreateFile(fileName,GENERIC_READ,0,0,OPEN_EXISTING,0,0);
	if(file1 == INVALID_HANDLE_VALUE)
	{
		DWORD dwError1 = GetLastError();
		if((dwError1 == ERROR_FILE_NOT_FOUND)||(dwError1 == ERROR_INVALID_NAME))
		{
			return false;
		}
		else
		{
			return true;
		}
	}
	else
	{
		CloseHandle(file1);
		return true;
	}
}
void CLibraryView::OnOpenFile()
{
	int lCount = GetListSelectedCount();
	if (lCount > 0)
	{
		POSITION lPos = GetListFirstSelectedItemPosition();
		while(lPos)
		{
			int lSelItemIdx = GetListNextSelectedItem(lPos);
			CString strFileName = m_pDoc->sFilesArray[lSelItemIdx].mPath;

			if(!FileIsExisting(strFileName))
			{
				continue;
			}
			

			// -------------- Open file from Repository
			if(m_SettingInfo.nIsUseRepository == 1)
			{
				m_workspace.AddFile(m_pDoc->sFilesArray[lSelItemIdx].mID); //pyj
				m_pDoc->sFilesArray[lSelItemIdx].mInWorkspace = true;
			}
			else // Open file directly
			{		
				theApp.OpenDocumentFile_ForPDFView(strFileName,m_pParamCDB->szLibPathName);
			}
			
			// ----------------------------------------------
		}
		Invalidate();
//		m_wndView.RefreshFileList();
	}
}

void CLibraryView::OnClose()
{
	//SaveHeadersSize();
	//SaveHeadersColumnOrder();
}

void CLibraryView::OnUpdateOpenFile(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(FALSE);

	int lCount = GetListSelectedCount();
	if (lCount > 0)
	{
		POSITION lPos = GetListFirstSelectedItemPosition();
		while(lPos)
		{
			int lSelItemIdx = GetListNextSelectedItem(lPos);
			if(!m_pDoc->sFilesArray[lSelItemIdx].mInWorkspace)
			{
				pCmdUI->Enable(TRUE);
				return;
			}
		}
	}
}

void CLibraryView::OnCloseFile()
{
	// Not required for direct linking
	if(m_SettingInfo.nIsUseRepository == 1)
	{
		int lCount = GetListSelectedCount();
		if (lCount > 0)
		{
			POSITION lPos = GetListFirstSelectedItemPosition();
			while(lPos)
			{
				int lSelItemIdx = GetListNextSelectedItem(lPos);

				m_workspace.AskForSaveToRepository(m_pDoc->sFilesArray[lSelItemIdx].mID); //pyj

				m_pDoc->sFilesArray[lSelItemIdx].mInWorkspace = false;
			}
			Refresh();
			//m_wndView.RefreshFileList();
		}
	}
}

void CLibraryView::OnUpdateCloseFile(CCmdUI* pCmdUI)
{
	if(m_SettingInfo.nIsUseRepository == 1) // Required for Repository only
	{
		int lCount = GetListSelectedCount();
		if (lCount > 0)
		{
			POSITION lPos = GetListFirstSelectedItemPosition();
			while(lPos)
			{
				int lSelItemIdx = GetListNextSelectedItem(lPos);
				if(m_pDoc->sFilesArray[lSelItemIdx].mInWorkspace)
				{
					pCmdUI->Enable(TRUE);
					return;
				}
			}
		}
	}
	pCmdUI->Enable(FALSE);
}


void CLibraryView::OnPrint()
{
	int lCount = GetListSelectedCount();
	if (lCount > 0)
	{
		POSITION lPos = GetListFirstSelectedItemPosition();
		while(lPos)
		{
			int lSelItemIdx = GetListNextSelectedItem(lPos);
			CString lStr;

			// Required for Repository  only
			// -----------------------------------------
			if(m_SettingInfo.nIsUseRepository == 1)
			{
				int lFileID = m_pDoc->sFilesArray[lSelItemIdx].mID;
				m_workspace.AskForSaveToRepository(lFileID);
			}
			// -----------------------------------------

			lStr.Format(_T("%s"), m_pDoc->sFilesArray[lSelItemIdx].mPath);
			if(((int)ShellExecute(NULL, _T("print"), lStr, NULL, NULL, 0)) <= 32) 
			{
				// Success?
			}
		}
	}
}

void CLibraryView::OnUpdatePrint(CCmdUI* pCmdUI)
{
	BOOL lEnable = FALSE;
	if (GetListSelectedCount() > 0)
		lEnable = TRUE;
	pCmdUI->Enable(lEnable);
}

void CLibraryView::OnUpdateShareEmail(CCmdUI* pCmdUI)
{
	BOOL lEnabled;
	if (GetListSelectedCount() > 0)
		lEnabled = TRUE;
	else
		lEnabled = FALSE;
	pCmdUI->Enable(lEnabled);
}

void CLibraryView::OnShowInWinExplorer()
{
	int lCount = GetListSelectedCount();
	if (lCount > 0)
	{
		POSITION lPos = GetListFirstSelectedItemPosition();
		while(lPos)
		{
			int lSelItemIdx = GetListNextSelectedItem(lPos);
			CString lStr = m_pDoc->sFilesArray[lSelItemIdx].mPath;	

			// Update the file location from the Repository
			// ----------------------------------------------
			if(m_SettingInfo.nIsUseRepository == 1)
			{
				int lFileID = m_pDoc->sFilesArray[lSelItemIdx].mID;

				m_workspace.AddFile(lFileID, false); 

				CString lFolder, lFileName;
				
				if (!m_workspace.GetWorkspaceFileFolder(lFileID, lFolder, lFileName))
					continue;
				
				CString lStr;
				lStr.Format(_T("%s"), lFolder + lFileName);
			}
			// ----------------------------------------------
		
			lStr = _T("/select, ") + lStr;
			if(((int)ShellExecute(NULL, NULL ,_T("explorer"), lStr, NULL, SW_MAXIMIZE)) <= 32) 
			{
				// Succes?
			}
		}
	}
}

void CLibraryView::OnUpdateShowInWinExplorer(CCmdUI* pCmdUI)
{
	int lCount = GetListSelectedCount();		
	pCmdUI->Enable(lCount == 1);
}

void CLibraryView::OnUpdateMetadataInfo(CCmdUI* pCmdUI)
{
	BOOL lEnabled;
	if (GetListSelectedCount() > 0)
		lEnabled = TRUE;
	else
		lEnabled = FALSE;
	pCmdUI->Enable(lEnabled);
}

void CLibraryView::OnDeleteFile()
{
	// Szabi: case 232
	int lCount = GetListSelectedCount();
		
	if (lCount < 0)
		return;
	
	CString lStr;
	CString lTitle;

	int lColID = GetSelectedCollID();

	if (lColID >= FILTERTREE_COLLECTION_MIN && lColID < FILTERTREE_COLLECTION_MAX)
	{
		//just removing from the collection
		if (lCount == 1)
		{
			CString lFileName = _T("");
			CString lStr1;
			
			lTitle = _T("Remove files from collection");
			lStr1 = _T("Are you sure you want to remove '%s' from selected collection?");
			POSITION lPos = GetListFirstSelectedItemPosition();
			if (lPos)
			{
				int lSelItemIdx = GetListNextSelectedItem(lPos);
				lFileName = m_pDoc->sFilesArray[lSelItemIdx].mName;
			}
			lStr.Format(lStr1, lFileName);
		}
		else
		{
			lTitle = _T("Remove files from collection");
			lStr = _T("Are you sure you want to remove the selected files from selected collection?");
		}

		CWnd lWnd;
		if (lWnd.MessageBox(lStr, lTitle, MB_YESNO | MB_ICONQUESTION) == IDYES)
		{

			lColID -= FILTERTREE_COLLECTION_MIN;

			int lNumRemovedNow = 0;

			POSITION lPos = GetListFirstSelectedItemPosition();
			while(lPos)
			{
				int lSelItemIdx = GetListNextSelectedItem(lPos);
				int lFileID = m_pDoc->sFilesArray[lSelItemIdx].mID;
				
				if (m_pDB->RemoveFileFromCollection(lColID, lFileID) == 1)
					lNumRemovedNow++;
			}
			Refresh();
/*
			if (lNumRemovedNow > 0)
			{
				//If we have selected the unsorted Collection we have to refresh the file view
				InitFindText();
				RefreshQuickBrowser();
				RefreshFileList();
			}*/
		}
	}
	else
	{
		
		if (lCount == 1)
		{
			CString lFileName = _T("");
			CString lStr1;
			
			lTitle = _T("Remove file");
			
			if(m_SettingInfo.nIsUseRepository == 1)
			{
				lStr1 = _T("Are you sure you want to remove '%s' \nfrom Benubird and move it to the Recycle Bin?");
			}
			else
			{
				lStr1  = _T("Are you sure you want to remove\n'%s' from Benubird");
			}

			POSITION lPos = GetListFirstSelectedItemPosition();
			if (lPos)
			{
				int lSelItemIdx = GetListNextSelectedItem(lPos);
				lFileName = m_pDoc->sFilesArray[lSelItemIdx].mName;
			}
			lStr.Format(lStr1, lFileName);
		}
		else
		{
			lTitle = _T("Remove files");
			if(m_SettingInfo.nIsUseRepository == 1)
			{
				lStr = _T("Are you sure you want to remove the selected files \nfrom Benubird and move them to the Recycle Bin?");
			}
			else
			{
				lStr = _T("Are you sure you want to remove\nthe selected files from Benubird?");
			}
		}

		CWnd lWnd;
		if (lWnd.MessageBox(lStr, lTitle, MB_YESNO | MB_ICONQUESTION) == IDYES)
		{
			int lNumDelFiles = 0;
			POSITION lPos = GetListFirstSelectedItemPosition();

			CTFiles lTFiles(m_pParamCDB);

			CProgressDlg* lProgressDlg = NULL;
			lProgressDlg = new CProgressDlg;
			lProgressDlg->SetMax(lCount);
			lProgressDlg->SetCaption(_T("Deleting files..."));
			lProgressDlg->CreateProgressDlg();

			while(lPos)
			{
				int lSelItemIdx = GetListNextSelectedItem(lPos);
				
				int lFileID = m_pDoc->sFilesArray[lSelItemIdx].mID;
				lTFiles.GetFileByID(lFileID);
									
				TCHAR lPath[MAX_PATH];
				//_tcscpy(lPath, lTFiles.mPath);
				wcscpy_s(lPath, lTFiles.mPath);
				lPath[_tcslen(lPath) + 1] = '\0';

				CString lStr;
				lStr.Format(_T("Deleting file %s"), lPath);
				if (lProgressDlg != NULL)
				{
					lProgressDlg->SetMessage(lStr);
					lProgressDlg->StepIt();
					if (lProgressDlg->IsCancelled())
						break;
				}
				
				// Szabi: It's necessary, when we delete file from Repository?
				CFileFind lFinder;

				// Required for Repository only
				// ----------------------------------------------

				BOOL bProcess = true;
				if(m_SettingInfo.nIsUseRepository == 1)
				{
					bProcess = m_workspace.AskForSaveToRepository(lFileID);  
				}
				if(bProcess)
				{
					if(lTFiles.DeleteByID(lFileID))
					{
						lNumDelFiles++;
						
					// Move the files to the Recycle Bin
					// Do not display an error if it occurs
						if(m_SettingInfo.nIsUseRepository == 1)
						{
							CUtils::SendFileToTheRecycleBin(lPath);
						}
					}
				}
				// ----------------------------------------------
			}

			if (lProgressDlg) 
			{
				delete lProgressDlg;
				lProgressDlg = NULL;
			}

			Refresh();
			//m_wndView.RefreshFileList();
/*
			if(lNumDelFiles > 0)
			{
				Refresh();
			}*/
		}
	}
}

void CLibraryView::OnUpdateRenameFile(CCmdUI* pCmdUI)
{
	int lCount = GetListSelectedCount();		
	pCmdUI->Enable(lCount == 1);
}

void CLibraryView::OnRenameFile()
{
	int lCount = GetListSelectedCount();		
	if (lCount < 0)
		return;

	POSITION lPos = GetListFirstSelectedItemPosition();
	if (lPos == NULL) return;

	int lSelItemIdx = GetListNextSelectedItem(lPos);

	int lFileID = m_pDoc->sFilesArray[lSelItemIdx].mID;

	// ------------------------------------------------
	BOOL bProcess = true;
	if(m_SettingInfo.nIsUseRepository == 1) // Attempt to add the files to the repository if the Repository is used
	{
		bProcess = m_workspace.AskForSaveToRepository(lFileID); 
	}
	if(bProcess)
	{

	//		int lSelIndex = m_wndView.GetListNextSelectedItem(lPos);
		CRenameFileDlg lRenameDlg(m_pDoc->sFilesArray[lSelItemIdx].mName, m_pDoc->sFilesArray[lSelItemIdx].mID);
		lRenameDlg.DoModal();
	}
	// ------------------------------------------------
//	m_wndView.RefreshFileList();
	Refresh();
}

void CLibraryView::OnCopyFile()
{
	int lCount = GetListSelectedCount();
	if (lCount < 0)
		return;
	mDocumentFileList.PrepareOleData(1);
}

void CLibraryView::OnUpdatePreviewPDFFile(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(FALSE);

	int lCount = GetListSelectedCount();
	if (lCount > 0)
	{
		POSITION lPos = GetListFirstSelectedItemPosition();
		while(lPos)
		{
			int lSelItemIdx = GetListNextSelectedItem(lPos);
			CString strFileName = m_pDoc->sFilesArray[lSelItemIdx].mPath;
			if(CUtils::IsPDF(strFileName))
			{
				pCmdUI->Enable(TRUE);
			}
		}
	}
}

void CLibraryView::OnPreviewPDFFile()
{
	int lCount = GetListSelectedCount();
	if (lCount == 1)
	{
		/*
		POSITION lPos = GetListFirstSelectedItemPosition();
		while(lPos)
		{
			int lSelItemIdx = GetListNextSelectedItem(lPos);
			
			CPDFPreviewDlg PDFPreviewDlg;
			PDFPreviewDlg.m_strFile = m_pDoc->sFilesArray[lSelItemIdx].mPath;;
			PDFPreviewDlg.DoModal();
			
		}
		*/
		OnOpenFile();
	}
}

void CLibraryView::OnUpdateOpenDefaultPDFViewer(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(FALSE);

	int lCount = GetListSelectedCount();
	if (lCount > 0)
	{
		POSITION lPos = GetListFirstSelectedItemPosition();
		while(lPos)
		{
			int lSelItemIdx = GetListNextSelectedItem(lPos);
			CString strFileName = m_pDoc->sFilesArray[lSelItemIdx].mPath;
			if(CUtils::IsPDF(strFileName))
			{
				pCmdUI->Enable(TRUE);
			}
		}
	}
}

void CLibraryView::OnOpenDefaultPDFViewer()
{
	int lCount = GetListSelectedCount();
	if (lCount > 0)
	{
		POSITION lPos = GetListFirstSelectedItemPosition();
		while(lPos)
		{
			int lSelItemIdx = GetListNextSelectedItem(lPos);
			CString strFileName = m_pDoc->sFilesArray[lSelItemIdx].mPath;

			if(!FileIsExisting(strFileName))
			{
				continue;
			}
			ShellExecute(NULL, _T("open"), strFileName, NULL, NULL, SW_SHOWNORMAL);
		}
	}
}

void CLibraryView::OnUpdateConvertToPDFFile(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(FALSE);

	int lCount = GetListSelectedCount();
	if (lCount > 0)
	{
		POSITION lPos = GetListFirstSelectedItemPosition();
		while(lPos)
		{
			int lSelItemIdx = GetListNextSelectedItem(lPos);
			if (m_pDoc->CanConvertToPDF(m_pDoc->sFilesArray[lSelItemIdx].mPath))
			{
				pCmdUI->Enable(TRUE);
				return;
			}
		}
	}
}

void CLibraryView::OnConvertToPDFFile()
{
	int lCount = GetListSelectedCount();
	if (lCount < 0)
		return;

	CConvertToPDFDlg lConvertToPDFDlg;
	lConvertToPDFDlg.Set_ParamForCDB(m_pParamCDB,m_pViewInfo);
	if(lConvertToPDFDlg.DoModal()==IDOK)
		Refresh();
}

void CLibraryView::OnAddToTags(UINT nID)
{
	CTFiles lTFiles(m_pParamCDB);
	int lCount = GetListSelectedCount();
	if (lCount > 0)
	{
		POSITION lPos = GetListFirstSelectedItemPosition();
		while(lPos)
		{
			int lSelItemIdx = GetListNextSelectedItem(lPos);

			int lFileID = m_pDoc->sFilesArray[lSelItemIdx].mID;

			// ------------------------------------------------
			BOOL bProcess = true;
			if(m_SettingInfo.nIsUseRepository == 1) // Required for Repository
			{
				bProcess = m_workspace.AskForSaveToRepository(lFileID);  
			}
			if(bProcess)
			{
				m_pDB->AddTagToFile(nID - ID_ADD_TO_TAGS_ID_MENUROOT, m_pDoc->sFilesArray[lSelItemIdx].mID);
				lTFiles.GetFileByID(m_pDoc->sFilesArray[lSelItemIdx].mID);
				lTFiles.UpdateFileOnDiskFromMembers();
				lTFiles.UpdateFileFromDisk(false); // don't need to refresh files due to final Refresh()
			}
			// ------------------------------------------------
		}
		//m_wndView.RefreshFileList();

		Refresh();
	}
}

void CLibraryView::OnRemoveFromTags(UINT nID)
{
	CTFiles lTFiles(m_pParamCDB);
	int lCount = GetListSelectedCount();
	if (lCount > 0)
	{
		POSITION lPos = GetListFirstSelectedItemPosition();
		while(lPos)
		{
			int lSelItemIdx = GetListNextSelectedItem(lPos);
			int lFileID = m_pDoc->sFilesArray[lSelItemIdx].mID;

		// ------------------------------------------------------
			BOOL bProcess = true;
			if(m_SettingInfo.nIsUseRepository == 1) // Add to the Repository
			{
				bProcess = m_workspace.AskForSaveToRepository(lFileID);  
			}
			if(bProcess)
			{		
				m_pDB->RemoveTagFromFile(nID - ID_REMOVE_FROM_TAGS_ID_MENUROOT, m_pDoc->sFilesArray[lSelItemIdx].mID);
				lTFiles.GetFileByID(m_pDoc->sFilesArray[lSelItemIdx].mID);
				lTFiles.UpdateFileOnDiskFromMembers();
				lTFiles.UpdateFileFromDisk(false); // don't need to refresh files due to final Refresh()
			}
			// ------------------------------------------------------
		}
//		m_wndView.RefreshFileList();
//		RefreshTagList();
		Refresh();
	}
}

void CLibraryView::OnRemoveFromCollection(UINT nID)
{
	int lNumRemovedNow = 0;
	int lCount = GetListSelectedCount();
	if(lCount > 0)
	{
		POSITION lPos = GetListFirstSelectedItemPosition();
		while(lPos)
		{
			int lSelItemIdx = GetListNextSelectedItem(lPos);
			int lFileID = m_pDoc->sFilesArray[lSelItemIdx].mID;
			
			if (m_pDB->RemoveFileFromCollection(nID - ID_REMOVE_FROM_COLLECTION_ID_MENUROOT, lFileID) == 1)
				lNumRemovedNow++;
		}
//?		InitFindText();
		Refresh();
	}	
}

void CLibraryView::OnAddToCollection(UINT nID)
{
	int lNumAddedNow = 0;
	int lCount = GetListSelectedCount();
	if(lCount > 0)
	{
		POSITION lPos = GetListFirstSelectedItemPosition();
		while(lPos)
		{
			int lSelItemIdx = GetListNextSelectedItem(lPos);
			int lFileID = m_pDoc->sFilesArray[lSelItemIdx].mID;
			if (m_pDB->AddFileToCollection(nID - ID_COLLECTIONID_MENUROOT, lFileID) == 1)
				lNumAddedNow++;
		}
//?		InitFindText();
		Refresh();
	}
}

void CLibraryView::OnCreateCollectionFromSelection()
{
	// Get collection name ... should be called the name of the first file, minus its extension
	int lCount = GetListSelectedCount();
	if(lCount == 0) return;

	POSITION lPos = GetListFirstSelectedItemPosition();
	if(lPos == NULL) return;

	int lSelItemIdx = GetListNextSelectedItem(lPos);
	CString lName = m_pDoc->sFilesArray[lSelItemIdx].mName;
	if (lName.IsEmpty())
		lName = _T("Untitled collection");
	
	// Remove the file extension
	int pos = lName.ReverseFind('.');
	if(pos>=0)
	{
		lName = lName.Mid(0, pos);
	}

	// Add a new collection to the database
	CTCollections lCol(m_pParamCDB->pDB);
	CString lStr;
	CString lNumStr;;
	int lNum = 1;
	lStr = lName;
	while (lCol.AddNewCollection(lName, -1) != TRUE) // Should theoretically add a number to the name if it is already in the database
	{
		lNumStr.Format(_T(" %d"), ++lNum);
		lName = lStr + lNumStr;
	}

	// Get the collection ID from the database
	CTCollections lTColl(m_pParamCDB->pDB);
	lTColl.GetCollectionsByName(lName);
	if (!lTColl.mDataIsValid) return;

	// Add the selected files to the database
	OnAddToCollection(lTColl.mID+ID_COLLECTIONID_MENUROOT);
	m_wndOutlookBar.m_wndCollectionPanel.AddNewCollection(lName, -1);
	m_wndOutlookBar.m_wndCollectionPanel.m_wndTree2.SortCollections(); // Sort collections after adding new selection
}

///////////////////////////////////////////////////////////////////////////////////
// Add files / folders
///////////////////////////////////////////////////////////////////////////////////
void CLibraryView::OnAddFile()
{
//why???	m_pDB->UpdateFiles();

	//Open file dialog and load the selected files
	CStringList lFileNames;
	CFileDialog lFileDlg(true, _T("*"),NULL, OFN_ALLOWMULTISELECT, _T("All files ( *.*)|*.*||"));

	int lcMaxFiles = 2048;
	int lcbBuffSize = 2048;//(lcMaxFiles * (MAX_PATH + 1)) + 1;

	lFileDlg.m_ofn.nMaxFile = lcMaxFiles;
	CString lFileName;
	
	lFileDlg.m_ofn.lpstrFile = lFileName.GetBuffer(4096);
	lFileDlg.m_ofn.lpstrFile[0] = NULL;
	if(lFileDlg.DoModal()==IDOK)
	{
		POSITION lPos = lFileDlg.GetStartPosition();
		while(lPos)
		{
			CString lPathName = lFileDlg.GetNextPathName(lPos);
			lFileNames.AddTail(lPathName);
		}
	}
	lFileName.ReleaseBuffer();

	if (lFileNames.GetCount() == 0)
		return;

	CTFiles lTFiles(m_pParamCDB);
	CIntArray lAddedFilesIDs;

// ----------------------- // Do not add if the repository is being used
	int lAddedFiles = 0;
	if(m_SettingInfo.nIsUseRepository == 1) 
	{
		lAddedFiles = m_pViewInfo->pRepository->AddFiles(lFileNames); 
	}
	else
	{
		lAddedFiles = lTFiles.AddFiles(lFileNames, NULL, &lAddedFilesIDs);

		if(lAddedFiles > 0)
		{
			CString lStr, lStrTemplate;
			//CString lFileStr;
			//lStrTemplate.LoadString(IDS_N_FILES_ADDED_SUCCESS);
			//lAddedFiles > 1 ? lFileStr = _T("files") : lFileStr = _T("file");
			//lStr.Format(lStrTemplate, lAddedFiles, lFileStr);

			if (lAddedFiles > 1)
				lStrTemplate = _T("%d files have been added.");
			else
				lStrTemplate = _T("%d file has been added.");
			lStr.Format(lStrTemplate, lAddedFiles);

//			CWnd::MessageBox(lStr, _T("Import success"), MB_OK | MB_ICONINFORMATION);
			if(AfxMessageBox(lStr, MB_OK|MB_ICONINFORMATION) == IDOK)
			{
				if(lAddedFilesIDs.GetCount() == 1)
				{
					CGetInfoDlg lGetInfoDlg(&lAddedFilesIDs);
					lGetInfoDlg.Set_ParamForCDB(m_pParamCDB);
					lGetInfoDlg.DoModal();
				}
				else
				{
					CMultipleProfileInformationDlg lDlg(&lAddedFilesIDs);
					lDlg.Set_ParamForCDB(m_pParamCDB);
					lDlg.DoModal();
				}
			}
		}

	// Auto-popup editing of metadata - KDA 1 FEB, 2009
		//if(lAddedFiles>1)
		//	OnMetadataInfo();

		
/*		InitFindText();
		RefreshQuickBrowser();
		RefreshFileList();*/
	}
	Refresh();
}

void CLibraryView::OnAddFolder()
{
	CBrowseFolderDlg lDlg(TRUE);
	lDlg.DoModal();
}
///////////////////////////////////////////////////////////////////////////////////
// Collections
///////////////////////////////////////////////////////////////////////////////////
CString CLibraryView::CreateCollection(CString inName, bool inSubCollection)
{
	CTCollections lCol(m_pParamCDB->pDB);
	CString lStr;
	CString lCollectionStr;
	CString lNumStr;;
	int lNum = 1;
	lStr = inName;
	lCollectionStr = lStr;

	int lParentID = -1;
	int lColParentID = -1;
	if (inSubCollection)
	{
		lParentID = GetSelectedCollectionGuiID();
		if (lParentID < FILTERTREE_COLLECTION_MIN || lParentID > FILTERTREE_COLLECTION_MAX)
			lParentID = -1;
		else
			lColParentID = lParentID - FILTERTREE_COLLECTION_MIN;
	}

	while (lCol.AddNewCollection(lCollectionStr, lColParentID) != TRUE)
	{
		lNumStr.Format(_T(" %d"), ++lNum);
		lCollectionStr = lStr + lNumStr;
	}
	m_wndOutlookBar.m_wndCollectionPanel.AddNewCollection(lCollectionStr, lParentID);

	return lCollectionStr;
}

void CLibraryView::OnCreateCollections()
{
	CreateCollection(_T("Untitled collection"), false);
}

void CLibraryView::OnCreateSubCollections()
{
	CreateCollection(_T("Untitled collection"), true);
}

void CLibraryView::OnUpdateCreateSubCollections(CCmdUI* pCmdUI)
{
	int lID = m_wndOutlookBar.m_wndCollectionPanel.GetSelectedCollID();
	if (TREEITEMID_IS_COLLECTION(lID))
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

void CLibraryView::OnCreateSmartCollections()
{
	CSmartCollectionsDlg lDlg;
	if(IDOK == lDlg.DoModal())
	{
		CString lStr;
		CString lCollectionStr;
		CString lNumStr;;
		int lNum = 1;
		lStr = _T("Untitled Smart");
		lCollectionStr = lStr;
		while (lDlg.mTSmartColl->AddNewSmartColl(lCollectionStr) != TRUE)
		{
			lNumStr.Format(_T(" %d"), ++lNum);
			lCollectionStr = lStr + lNumStr;
		}
		m_wndOutlookBar.m_wndCollectionPanel.AddNewSmartCollection(lCollectionStr);
	}
}

///////////////////////////////////////////////////////////////////////////////////
// Views
///////////////////////////////////////////////////////////////////////////////////
void CLibraryView::OnViewFilter()
{
	BOOL lVisible;
	lVisible = m_wndBrowserPanel->IsVisible();
	if (lVisible) 
	{
		m_wndBrowserPanel->ResetSelection();
//?		InitFindText();
	}
	m_wndBrowserPanel->ShowControlBar(!lVisible, FALSE, FALSE);
	Refresh();
}

void CLibraryView::OnUpdateViewFilter(CCmdUI* pCmdUI)
{
	BOOL lVisible;
	lVisible = m_wndBrowserPanel->IsVisible();
	pCmdUI->SetCheck(lVisible);
}

void CLibraryView::OnViewTags()                             
{
	BOOL lVisible;
	lVisible = m_wndTagsPanel->IsVisible();
	if (lVisible)
	{
		m_wndTagsPanel->ResetSelection();
//?		InitFindText();
	}
	m_wndTagsPanel->ShowControlBar(!lVisible, FALSE, FALSE);
	Refresh();
}

void CLibraryView::OnUpdateViewTags(CCmdUI* pCmdUI)
{
	BOOL lVisible;
	lVisible = m_wndTagsPanel->IsVisible();
	pCmdUI->SetCheck(lVisible);
}

void CLibraryView::OnViewFileList()
{
	SetViewStyle(FILE_VIEW_LIST);

	Refresh();
}

void CLibraryView::OnUpdateViewFileList(CCmdUI* pCmdUI)
{
	int lCheck = GetViewStyle()==FILE_VIEW_LIST ? BST_CHECKED : BST_UNCHECKED;
	pCmdUI->SetCheck(lCheck);
}

void CLibraryView::OnViewIcons()
{
	SetViewStyle(FILE_VIEW_ICONS);

	Refresh();
}

void CLibraryView::OnUpdateViewIcons(CCmdUI* pCmdUI)
{
	int lCheck = GetViewStyle()==FILE_VIEW_ICONS ? BST_CHECKED : BST_UNCHECKED;
	pCmdUI->SetCheck(lCheck);
}

void CLibraryView::OnViewThumbnails()
{
	SetViewStyle(FILE_VIEW_THUMBNAILS);

	Refresh();
}

void CLibraryView::OnUpdateViewThumbnails(CCmdUI* pCmdUI)
{
	int lCheck = GetViewStyle()==FILE_VIEW_THUMBNAILS ? BST_CHECKED : BST_UNCHECKED;
	pCmdUI->SetCheck(lCheck);
}

void CLibraryView::OnViewPreview()
{
	BOOL lVisible;
	lVisible = m_wndPreviewPanel->IsVisible();
	if (lVisible) 
	{

	}
	m_wndPreviewPanel->ShowControlBar(!lVisible, FALSE, FALSE);
//	CalculateOutlookPercent();
}

void CLibraryView::OnUpdateViewPreview(CCmdUI* pCmdUI)
{
	BOOL lVisible;
	lVisible = m_wndPreviewPanel->IsVisible();
	pCmdUI->SetCheck(lVisible);
}

void CLibraryView::OnVaultView()
{
	// OUTLOOK
	//m_wndCollectionPanel.ShowUnsorted();
}

///////////////////////////////////////////////////////////////////////////////////
// Metadata
///////////////////////////////////////////////////////////////////////////////////
void CLibraryView::OnMetadataTags()
{
	CManageTagsDlg lDlg;
	if(lDlg.DoModal() == IDOK)
	{
		Refresh();
	}
}
void CLibraryView::OnLibrarySetting()
{
	CPoint point = CPoint(57,28);
/*	
	::SendMessageW(mDocumentFileList.GetSafeHwnd(),WM_LBUTTONDOWN,MK_LBUTTON,28<<16+57);
	Sleep(10);
	::SendMessageW(mDocumentFileList.GetSafeHwnd(),WM_LBUTTONUP,MK_LBUTTON,28<<16+57);
	Sleep(10);
	::SendMessageW(mDocumentFileList.GetSafeHwnd(),WM_LBUTTONDBLCLK,MK_LBUTTON,28<<16+57);
	//mDocumentFileList.Wrapper_OnLButtonDown(1,point);
	//mDocumentFileList.Wrapper_OnLButtonDblClk(1,point);
	return;
*/

	CMainFrame* pMainFrame = DYNAMIC_DOWNCAST(CMainFrame, ((CMainFrame*)AfxGetMainWnd()));
	CLibraryDoc *pDoc = this->GetDocument();
	CLibrarySettingDlg dlg;


	//for default
	TCHAR szDefaultLib[0x100];
	memset(szDefaultLib,0,0x100*sizeof(TCHAR));
	g_dbDefaultLibrary.GetDefaultLibrary_For_Default(szDefaultLib);
	CString strCurrentPath = pDoc->m_csDBPathName;
	if(wcscmp(strCurrentPath.GetBuffer(),szDefaultLib)==0)
	{
		m_SettingInfo.nIsDefaultLibrary = 1;
	}
	else
	{
		m_SettingInfo.nIsDefaultLibrary = 0;
	}


	
	dlg.SetInitVarible(m_SettingInfo);
	
	
	if(dlg.DoModal() == IDOK)
	{
		::PostMessage(HWND_BROADCAST, BENUBIRD_ASSISTANT_STOP, 0, 0);

		m_SettingInfo.nIsUseRepository = dlg.m_nIsUseRepository;
		m_SettingInfo.strRepositoryPath = dlg.m_StrRepositoryPath;
		m_SettingInfo.strWorkspacePath = dlg.m_StrWorkspacePath;
		m_SettingInfo.nWorkspaceType = dlg.m_nWorkspaceType;
		m_pParamCDB->nIsUseRepository = m_SettingInfo.nIsUseRepository;

		//for default
		if(dlg.m_nIsDefaultLibrary != m_SettingInfo.nIsDefaultLibrary)
		{
			if(dlg.m_nIsDefaultLibrary == 1)
				g_dbDefaultLibrary.SetDefaultLibrary_For_Default(pDoc->m_csDBPathName.GetBuffer());
			else
				g_dbDefaultLibrary.SetDefaultLibrary_For_Default(_T(""));
		}
		
		if(m_SettingInfo.nIsUseRepository == 1)
		{
			m_pParamCDB->pDB->SetInfoValue(_T("isRepository"),_T("YES"));
			pMainFrame->AddWorkspacePanel();
		}
		else
		{
			m_pParamCDB->pDB->SetInfoValue(_T("isRepository"),_T("NO"));
			pMainFrame->RemoveWorkspacePanel();

			//remove existing files in workspace



			int lCount = GetListItemCount();
			
			if (lCount > 0)
			{
				for(int i=0;i<lCount;i++)
				{
					m_workspace.AskForSaveToRepository(m_pDoc->sFilesArray[i].mID); //pyj
				}

				
				//m_wndView.RefreshFileList();
			}
			
		}
		Refresh();

		m_pParamCDB->pDB->SetInfoValue(_T("Repository"),m_SettingInfo.strRepositoryPath.GetBuffer());
		m_pParamCDB->pDB->SetInfoValue(_T("Workspace"),m_SettingInfo.strWorkspacePath.GetBuffer());

		wchar_t w[36] = {0};
		_itow(m_SettingInfo.nWorkspaceType, w, 10);
		m_pParamCDB->pDB->SetInfoValue(_T("WorkspaceType"),w);

		m_pParamCDB->nIsUseRepository = m_SettingInfo.nIsUseRepository;
		m_Repository.SetRepositoryPath(m_SettingInfo.strRepositoryPath.GetBuffer());
		m_workspace.SetWorkspacePath(m_SettingInfo.strWorkspacePath.GetBuffer());

		SendLibrarySettingToAssistant(1);

		::PostMessage(HWND_BROADCAST, BENUBIRD_ASSISTANT_START, 0, 0);
	}

	return;
}


void CLibraryView::OnOpenWorkspace()
{
	OnOpenFile();
}


void CLibraryView::OnCloseWorkspace()
{
	OnCloseFile();
}

void CLibraryView::OnUpdateOpenWorkspace(CCmdUI* pCmdUI)
{
	int lCount = GetListSelectedCount();
	if (lCount > 0)
	{
		POSITION lPos = GetListFirstSelectedItemPosition();
		while(lPos)
		{
			int lSelItemIdx = GetListNextSelectedItem(lPos);

			CTFiles lFiles(m_pParamCDB);
			int lFileID = m_pDoc->sFilesArray[lSelItemIdx].mID;//lFiles.GetFileIDByPath(m_pDoc->sFilesArray[lSelItemIdx].mPath);

			if(m_SettingInfo.nIsUseRepository == 1) {
				if(!m_workspace.FileIsInWorkspace(lFileID))
				{
					pCmdUI->Enable(TRUE);
					
					return;
				}
			}
		}
	}
	pCmdUI->Enable(FALSE);
}

void CLibraryView::OnUpdateCloseWorkspace(CCmdUI* pCmdUI)
{
	OnUpdateCloseFile(pCmdUI);
}

void CLibraryView::OnMetadataCategories()
{
	CManageCategoriesDlg lDlg;
	lDlg.DoModal();
}

void CLibraryView::OnMetadataInfo()
{
	int lCount = GetListSelectedCount();
	if(lCount > 0)
	{
		POSITION lPos = GetListFirstSelectedItemPosition();
		CIntArray lSelFileIDsArray;
		while(lPos)
		{
			int lSelItemIdx = GetListNextSelectedItem(lPos);
			int lFileID = m_pDoc->sFilesArray[lSelItemIdx].mID;
			lSelFileIDsArray.Add(lFileID);
		}

		if(lSelFileIDsArray.GetCount() == 1)
		{
			CGetInfoDlg lDlg(&lSelFileIDsArray);
			lDlg.Set_ParamForCDB(m_pParamCDB);
			lDlg.DoModal();
		}
		else
		{
			CMultipleProfileInformationDlg lDlg(&lSelFileIDsArray);
			lDlg.Set_ParamForCDB(m_pParamCDB);
			lDlg.DoModal();
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////
// Import/Export
///////////////////////////////////////////////////////////////////////////////////
void CLibraryView::OnFoldersWatched()
{
	//
	::PostMessage(HWND_BROADCAST, BENUBIRD_ASSISTANT_STOP, 0, 0);
	//::SendMessage(HWND_BROADCAST, BENUBIRD_ASSISTANT_STOP, 0, 0);

	/*BroadcastSystemMessage( 
		BSF_IGNORECURRENTTASK,		// do not send message to this process
		BSM_ALLCOMPONENTS,						// broadcast only to applications
		BENUBIRD_ASSISTANT_STOP,	// registered private message
		0,							// message-specific value
		0);							// message-specific value&*/



	CManageWatchedFoldersDlg lDlg;
	lDlg.Set_ParamForCDB(m_pParamCDB);
	lDlg.DoModal();
	// nastartuj Assistanta
	::PostMessage(HWND_BROADCAST, BENUBIRD_ASSISTANT_START, 0, 0);
	//::SendMessage(HWND_BROADCAST, BENUBIRD_ASSISTANT_START, 0, 0);
}

void CLibraryView::OnFreeUpgrade()
{
	
	CString lStr = _T("http://www.debenu.com/benubird/getpro.php");
	::ShellExecute(NULL, NULL, lStr, NULL, NULL, NULL);	
	

	//AfxMessageBox(_T("This is free version."));
}

void CLibraryView::OnSwitchDirect()
{
	if(AfxMessageBox(_T("If you switch to Direct, you'll need to reorganising your files, are you sure? (also will have to close Benubird and restart)"), MB_OKCANCEL)==IDOK)
	{
		m_SettingInfo.nIsUseRepository =0;
		theApp.WriteProfileInt(kOptions, kUseRepository, BST_UNCHECKED);
		AfxPostQuitMessage(WM_QUIT);
	}
}

void CLibraryView::OnSwitchRepository()
{
	if(AfxMessageBox(_T("The Repository might sound good, but it'll create a duplicate of your files, and you'll lose any organizing that you did in creating folders... Sure? (also will have to close Benubird and restart)"), MB_OKCANCEL)==IDOK)
	{
		m_SettingInfo.nIsUseRepository = 1;
		theApp.WriteProfileInt(kOptions, kUseRepository, BST_CHECKED);
		AfxPostQuitMessage(WM_QUIT);
	}
}

void CLibraryView::OnImportZip()
{

//	AfxMessageBox(_T("ZIP"));

	CImportZipDlg lDlg;
	lDlg.SetData_FromParent(m_pParamCDB,0);
	if (lDlg.DoModal() == IDOK)
	{
		int lRet = ImportFilesFromZip(lDlg.m_SourcePath, lDlg.m_Password, &lDlg.mZipImportProperties);
		Refresh();
	}
}



void CLibraryView::OnImportFolder()
{
	CImportFolderDlg lDlg;
	lDlg.SetData_FromParent(m_pParamCDB,0);
	if (lDlg.DoModal() == IDOK)
	{
		int lRet = ImportFilesFromFolder(lDlg.mPath, lDlg.mIncludeSubfolders, &lDlg.mFolderImportProperties);
		Refresh();
	}
}

void CLibraryView::OnExportZip()
{
	CExportZipDlg dlg(GetSelectedCollectionGuiID());
	dlg.SetData_FromParent(m_pParamCDB,0);
	if (dlg.DoModal() != IDOK)
		return;

	ExportFilesToZip(dlg.mCollectionID, dlg.mOutputZipFile, dlg.mPassword);

	
}

void CLibraryView::RecursiveDelete(CString szPath)
{
	CFileFind ff;
	CString path = szPath;
	
	if(path.Right(1) != "\\")
		path += "\\";

	path += "*.*";

	BOOL res = ff.FindFile(path);

	while(res)
	{
		res = ff.FindNextFile();
		if (!ff.IsDots() && !ff.IsDirectory())
			DeleteFile(ff.GetFilePath());
		else if (ff.IsDirectory())
		{
			path = ff.GetFilePath();
			RecursiveDelete(path);
			RemoveDirectory(path);
		}
	}
}

bool CLibraryView::ExportFilesToZip_ForLibrary(int inCollectionID, TCHAR* szZipPath,TCHAR* szRootFolderPath, CString inPassword)
{
	CLibraryDoc *pDoc = this->GetDocument();

	CString str_TmpGUID;
	CString str_TmpSrcFilePath;
	TCHAR str_TmpRoot[MAX_PATH];
	
	

	CTFiles lFilesTable(m_pParamCDB);
	if (!PrepareFileList(inCollectionID, &lFilesTable))
		return false;

	CProgressDlg* lProgressDlg = new CProgressDlg;
	lProgressDlg->SetMax(lFilesTable.GetCount());
	lProgressDlg->SetCaption(_T("Exporting files to zip archive..."));
	lProgressDlg->CreateProgressDlg();

	bool lCanceled = false;

	//copy and open tmp db
	CString csTmpDBName = pDoc->m_csDBName;
	CString csTmpDBPath = pDoc->m_csDBPathName + _T("_tmp");
	CopyFile(pDoc->m_csDBPathName,csTmpDBPath,true);
	CDB tmpDB;
	tmpDB.Initialize(csTmpDBPath.GetBuffer());
	PARAM_FOR_CDB tmpParamCDB;
	memset(&tmpParamCDB,0,sizeof(PARAM_FOR_CDB));
	tmpParamCDB.pDB = &tmpDB;
	


	HZIP hz = CreateZip(szZipPath, CT2CA(inPassword));

	while(!lFilesTable.mQ.eof())
	{
		lFilesTable.LoadCurrentRow();

		lProgressDlg->SetMessage(lFilesTable.mName);
			
		memset(str_TmpRoot,0,MAX_PATH*sizeof(TCHAR));
		wcscpy(str_TmpRoot,szRootFolderPath);

		str_TmpGUID = GUIDgen();
		if (::PathAppend(str_TmpRoot, str_TmpGUID.GetBuffer()) == FALSE) 
		{
			return false;
		}
		CreateDirectory(str_TmpRoot, NULL);
		wcscat(str_TmpRoot,_T("\\"));

		str_TmpSrcFilePath = str_TmpRoot + lFilesTable.mName;
		
		CopyFile(lFilesTable.mPath,str_TmpSrcFilePath,true);

		CString str_TmpDstFilePath = _T("\\"); 
		str_TmpDstFilePath =str_TmpGUID +  str_TmpDstFilePath + lFilesTable.mName;

		ZipAdd(hz, str_TmpDstFilePath,str_TmpSrcFilePath );

		CTFiles tFiles(&tmpParamCDB);
		tFiles.UpdateFilePath(lFilesTable.mPath,str_TmpDstFilePath);

		if (lProgressDlg->IsCancelled())
		{
			lCanceled = true;
			break;
		}

		lFilesTable.mQ.nextRow();
	}

	//zip db file
	tmpParamCDB.pDB->Close();
	ZipAdd(hz, csTmpDBName,csTmpDBPath );
	DeleteFile(csTmpDBPath);


	delete lProgressDlg;
	lProgressDlg = NULL;

	CloseZip(hz);

	if (lCanceled)
	{	
		DeleteFile(szZipPath);
	}

	return true;
}
void CLibraryView::OnExport_Library_toZip(TCHAR* szRootFolderPath)
{
	TCHAR szTmp[0x100];
	memset(szTmp,0,0x100*sizeof(TCHAR));
	wcscpy(szTmp,szRootFolderPath);

	CString csFileName;
	CExportZipDlg dlg(GetSelectedCollectionGuiID());
	dlg.SetData_FromParent(m_pParamCDB,1);
	if (dlg.DoModal() != IDOK)
		return;

	csFileName = dlg.mOutputZipFile;
	BOOL bCanCreateZip = FALSE;
	HANDLE file = CreateFile(csFileName,GENERIC_READ,0,0,OPEN_EXISTING ,0,0);
	DWORD dwError = 0;
	if(file == INVALID_HANDLE_VALUE)
	{
		dwError = GetLastError();
		if(dwError == 0x02) //FILE_NOT_FOUND
		{
			bCanCreateZip = TRUE;
		}
	}

	if(bCanCreateZip==FALSE)
	{
		AfxMessageBox(_T("Cannot create zip file"));
		return;
	}

	CString str_uuid = GUIDgen();
	if (::PathAppend(szTmp, str_uuid.GetBuffer()) == FALSE) 
	{
		return ;
	}
	CreateDirectory(szTmp, NULL);

	ExportFilesToZip_ForLibrary(dlg.mCollectionID, csFileName.GetBuffer(),szTmp, dlg.mPassword);

	
}

void CLibraryView::OnExportFolder()
{
	CExportFolderDlg dlg(GetSelectedCollectionGuiID());
	dlg.SetData_FromParent(m_pParamCDB,0);
	if (dlg.DoModal() != IDOK)
		return;

	ExportFilesToFolder(dlg.mCollectionID, dlg.mOutputFolder);
}

void CLibraryView::OnExportFilesZip()
{
	CExportZipDlg dlg(FILTERTREE_CURRENT_SELECTION);
	dlg.SetData_FromParent(m_pParamCDB,0);
	if (dlg.DoModal() != IDOK)
		return;

	ExportFilesToZip(dlg.mCollectionID, dlg.mOutputZipFile, dlg.mPassword);
}

void CLibraryView::OnExportFilesFolder()
{
	CExportFolderDlg dlg(FILTERTREE_CURRENT_SELECTION);
	dlg.SetData_FromParent(m_pParamCDB,0);
	if (dlg.DoModal() != IDOK)
		return;

	ExportFilesToFolder(dlg.mCollectionID, dlg.mOutputFolder);
}

bool CLibraryView::ExportFilesToFolder(int inCollectionID, CString inFolder, bool inIncludeDB)
{
	CTFiles lFilesTable(m_pParamCDB);
	if (!PrepareFileList(inCollectionID, &lFilesTable))
		return false;

	CString lFolder = inFolder;
	if (lFolder.Right(1) == _T("\\"))
		lFolder.Delete(lFolder.GetLength()-1);

	//check if folder exists
	CreateDirectory(inFolder, NULL);

	CProgressDlg* lProgressDlg = new CProgressDlg;
	lProgressDlg->SetMax(lFilesTable.GetCount());
	lProgressDlg->SetCaption(_T("Exporting files to folder..."));
	lProgressDlg->CreateProgressDlg();

	if (inCollectionID != FILTERTREE_CURRENT_SELECTION)
	{
		while(!lFilesTable.mQ.eof())
		{
			lFilesTable.LoadCurrentRow();
			lFilesTable.UpdateFileOnDiskFromMembers();

			lProgressDlg->SetMessage(lFilesTable.mName);
			lProgressDlg->StepIt();
			if (lProgressDlg->IsCancelled()) break;

			CString lNewPath = lFolder + _T("\\") + lFilesTable.mName; 
			CopyFile(lFilesTable.mPath, lNewPath, FALSE);

			lFilesTable.mQ.nextRow();
		}
		lProgressDlg->StepIt();
	}
	else
	{
		//get file ids from current selection
		int lCount = GetListSelectedCount();
		if(lCount > 0)
		{
			POSITION lPos = GetListFirstSelectedItemPosition();
			while(lPos)
			{
				int lSelItemIdx = GetListNextSelectedItem(lPos);
				int lFileID = m_pDoc->sFilesArray[lSelItemIdx].mID;

				lFilesTable.GetFileByID(lFileID);
				lFilesTable.UpdateFileOnDiskFromMembers();
				lFilesTable.LoadCurrentRow();

				lProgressDlg->SetMessage(lFilesTable.mName);
				lProgressDlg->StepIt();
				if (lProgressDlg->IsCancelled()) break;

				CString lNewPath = lFolder + _T("\\") + lFilesTable.mName; 
				CopyFile(lFilesTable.mPath, lNewPath, FALSE);
			}
		}
	}

	delete lProgressDlg;
	lProgressDlg = NULL;

	return true;
}

bool CLibraryView::ExportFilesToZip(int inCollectionID, CString inZipPath, CString inPassword, bool inIncludeDB)
{
	CTFiles lFilesTable(m_pParamCDB);
	if (!PrepareFileList(inCollectionID, &lFilesTable))
		return false;

	CProgressDlg* lProgressDlg = new CProgressDlg;
	lProgressDlg->SetMax(lFilesTable.GetCount());
	lProgressDlg->SetCaption(_T("Exporting files to zip archive..."));
	lProgressDlg->CreateProgressDlg();

	bool lCanceled = false;

	HZIP hz = CreateZip(inZipPath, CT2CA(inPassword));

	if (inCollectionID != FILTERTREE_CURRENT_SELECTION)
	{
		while(!lFilesTable.mQ.eof())
		{
			lFilesTable.LoadCurrentRow();

			lProgressDlg->SetMessage(lFilesTable.mName);
			
			ZipAdd(hz, lFilesTable.mName, lFilesTable.mPath);

			if (lProgressDlg->IsCancelled())
			{
				lCanceled = true;
				break;
			}

			lFilesTable.mQ.nextRow();
		}
	}
	else
	{
		//get file ids from current selection
		int lCount = GetListSelectedCount();
		if(lCount > 0)
		{
			POSITION lPos = GetListFirstSelectedItemPosition();
			while(lPos)
			{
				int lSelItemIdx = GetListNextSelectedItem(lPos);
				int lFileID = m_pDoc->sFilesArray[lSelItemIdx].mID;

				lFilesTable.GetFileByID(lFileID);
				lFilesTable.LoadCurrentRow();

				lProgressDlg->SetMessage(lFilesTable.mName);
				lProgressDlg->StepIt();
				if (lProgressDlg->IsCancelled()) break;

				ZipAdd(hz, lFilesTable.mName, lFilesTable.mPath);
			}
		}
	}


	delete lProgressDlg;
	lProgressDlg = NULL;

	CloseZip(hz);

	if (lCanceled)
	{	
		DeleteFile(inZipPath);
	}

	return true;
}

bool CLibraryView::PrepareFileList(int inCollectionID, CTFiles* outFilesTable)
{
	CDBSQL lFilesSQL;
	lFilesSQL.Clear();

	CString lCondition;

	lFilesSQL.AddTable(_T("Files"));

	m_pDoc->UpdateSQLFromCollection(inCollectionID, &lFilesSQL);

	CString lSQLStr = lFilesSQL.GetSelect();
	if (!lSQLStr.IsEmpty()) 
	{
		outFilesTable->mQ = m_pDB->mDB.execQuery(lSQLStr);
		return true;
	}

	return false;
}

int CLibraryView::ImportFilesFromZip(CString inZipPath, CString inPassword, CFileProperties_Entry* inFileProperties)
{
	int lRet = 0;

	CProgressDlg* lProgressDlg = NULL;

	try
	{
		LPSTR szPassword = CT2CA(inPassword);
		HZIP hz = OpenZip (inZipPath, inPassword.IsEmpty() ? NULL : szPassword );
		ZIPENTRY ze; 
		GetZipItem(hz,-1,&ze); 
		int numitems=ze.index;

		lProgressDlg = new CProgressDlg;
		lProgressDlg->SetMax(numitems);
		lProgressDlg->SetCaption(_T("Extracting Zip archive..."));
		lProgressDlg->CreateProgressDlg();

		CStringList lFileList;

		TCHAR lTempDir[_MAX_PATH+1];
		DWORD lLen = GetTempPath(_MAX_PATH, lTempDir);

		if (lTempDir[lLen-1] == '\\')
			lTempDir[lLen-1] = '\0';



		CString lDestFolder;
		int i = 1;
		while (true)
		{
			lDestFolder.Format(_T("%s\\BZ%d"),lTempDir, i);

			CFileFind lFinder;
			if (lFinder.FindFile(lDestFolder) == FALSE)
				break;
			i++;
		}

		::SHCreateDirectoryEx(NULL, lDestFolder, NULL);

		TCHAR lDrv[_MAX_DRIVE];
		TCHAR lFld[_MAX_DIR];
		TCHAR lNam[_MAX_FNAME];
		TCHAR lExt[_MAX_EXT];

		_tsplitpath_s(lDestFolder + _T("\\dummy.txt"), lDrv, lFld, lNam, lExt);


		TCHAR lDestPath[_MAX_PATH];
		ZRESULT res = 0;

		for (int i=0; i < numitems; i++)
		{ 
			GetZipItem(hz,i,&ze);

			CString lStrProgress;
			lStrProgress.Format(_T("Organizing file %s"), ze.name);

			lProgressDlg->SetMessage(lStrProgress);
			lProgressDlg->StepIt();
			if (lProgressDlg->IsCancelled())
				break;

			//create the dest path
			_tmakepath_s(lDestPath, lDrv, lFld, ze.name, NULL);

			res = UnzipItem(hz, i, lDestPath);

			lFileList.AddTail(lDestPath);
		}
		CloseZip(hz);

		CFileProperties_Entry lFolderEntry;
		if (inFileProperties)
			lFolderEntry = *inFileProperties;

		lRet = ImportFilesFromFolder(lDestFolder, true, &lFolderEntry);

		//cleanup
		POSITION lPos = lFileList.GetHeadPosition();
		while (lPos)
		{
			CString lFilePath = lFileList.GetNext(lPos);
			DeleteFile(lFilePath);
		}
		RemoveDirectory(lDestFolder);
	}
	catch (...)
	{}

	if (lProgressDlg)
	{
		delete lProgressDlg;
		lProgressDlg = NULL;
	}

	return lRet;
}

int CLibraryView::ImportFilesFromFolder(CString & inFolder, bool inIncludeSubfolders, CFileProperties_Entry* inFileProperties)
{
	CStringList lFileList;
	if (!LoadFilesFromDisk(inFolder, inIncludeSubfolders, &lFileList))
		return 0;

	return ImportFromFileList(lFileList, inFileProperties);
}

int CLibraryView::ImportFromFileList(CStringList & inFileList, CFileProperties_Entry* inFileProperties)
{
	return (m_pViewInfo->pRepository->AddFiles(inFileList, inFileProperties) > 0);
}

void CLibraryView::OnExportCollectionToZip()
{
	OnExportZip();
}

void CLibraryView::OnExportCollectionToFolder()
{
	OnExportFolder();
}
void CLibraryView::SetCDB(CDB *pDB)
{
	m_pDB = pDB;
}





void CLibraryView::OnDestroy()
{
	CView::OnDestroy();

	if(mDocumentFileList.GetSafeHwnd() == NULL)
		return;

	SaveHeadersSize();
	SaveHeadersColumnOrder();
}
