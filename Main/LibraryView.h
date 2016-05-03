// CLibraryView.h
//*****************************************************************************	

#ifndef _CLibraryView_H_
#define _CLibraryView_H_

#if _MSC_VER > 1000
#pragma once
#endif

#include "resource.h"
#include "CDocumentFileList.h"
#include "CRibbonFindEdit.h"
#include "CPreviewPanel.h"
#include "OutlookBar.h"
#include "CBrowserPanel.h"
#include "CTagsPanel.h"
#include "LibraryDoc.h"
#include "common.h"

#include "LibrarySettingDlg.h" 


// Number of columns
#define NUM_HEADER 16

enum ViewType 
{  
	FILE_VIEW_LIST = 0,
	FILE_VIEW_ICONS,
	FILE_VIEW_THUMBNAILS
};

//*****************************************************************************
// CLibraryView class
class CLibraryView : public CView
{
// Public methods
protected:
	CLibraryView();
	DECLARE_DYNCREATE(CLibraryView)

// Attributes
public:
	
	CLibraryDoc* GetDocument();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CShortcutsView)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnDraw(CDC* pDC);
	virtual void OnInitialUpdate();
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
public:

	void OnOpenFileWrapper();

	LIBRARY_SETTING_INFO	m_SettingInfo;
	LIBRARY_VIEW_INFO		*m_pViewInfo;

	DebenuPDFLibraryAX0912 m_qpdf;

	CBWorkspace m_workspace;
	CBRepository m_Repository;
	
	PARAM_FOR_CDB *m_pParamCDB;
	CDB *m_pDB;
	CDBSQL* m_pFilesSQL;
public:
	void SendLibrarySettingToAssistant(DWORD state);
	void SetCDB(CDB *pDB);
	void RecursiveDelete(CString szPath);
	void OnExport_Library_toZip(TCHAR* szRootFolderPath);
	bool ExportFilesToZip_ForLibrary(int inCollectionID, TCHAR* szZipPath,TCHAR* szRootFolderPath, CString inPassword);

	virtual ~CLibraryView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	
// Member variables
private:
	int mCurrentHeaderSizes[NUM_HEADER];
	int mDefaultHeaderSizes[NUM_HEADER];
	ViewType mReportStyle;

	CIntArray mSelIDs;
	CIntArray mNextID;
	int		  mFirstSelIndex;
	int       mLastSelIndex;

	CWinThread *pThumbnailThread;

	CLibraryDoc *m_pDoc;
	
public:
	CBrowserPanel*			m_wndBrowserPanel;
	CTagsPanel*				m_wndTagsPanel;
	CPreviewPanel*			m_wndPreviewPanel;
	COutlookBar				m_wndOutlookBar;
	CDocumentFileList		mDocumentFileList;

public:
	void ShowBars(BOOL bShow);

	int GetCurrentHeaderSize(int inPos){ return mCurrentHeaderSizes[inPos]; }
	void SetViewStyle(ViewType inType);
	ViewType GetViewStyle() {return mReportStyle; };

	void SaveHeadersSize();
	void LoadHeadersSize();
	void SaveHeadersColumnOrder();
	void LoadHeadersColumnOrder();

	POSITION GetListFirstSelectedItemPosition() const;
	int GetListNextSelectedItem(POSITION& pos) const;
	UINT GetListSelectedCount() const;
	int GetListItemCount() const;
	int GetListMinSize() const;
	void UpdateItems(int nStartItem, int nEndItem);

	CTagsPanel* GetTagsPanel(){ return m_wndTagsPanel; }
	CBrowserPanel* GetBrowsePanel(){ return m_wndBrowserPanel; }
	CPreviewPanel* GetPreviewPanel(){ return m_wndPreviewPanel; }
	BOOL IsTagsPanelEmpty(){ return m_wndTagsPanel->IsEmpty(); }

	long GetSelectedCollID() {return m_wndOutlookBar.m_wndCollectionPanel.GetSelectedCollID();}
	int GetSelectedCollectionGuiID();
		
	BOOL CreateCollectionBar();
	BOOL CreateOutlookBar();

	void SearchByContent(int inCollectionID, CString lKeyword);

	void Refresh();

	void RefreshFileList();
	void RefreshTagList();
	void RefreshQuickBrowser();
	void RefreshPreview();

// Import/Export
private:
	int ImportFilesFromZip(CString inZipPath, CString inPassword, CFileProperties_Entry * inFileProperties);
	int ImportFilesFromFolder(CString & inFolder, bool inIncludeSubfolders, CFileProperties_Entry* inFileProperties);
	int ImportFromFileList(CStringList & inFileList, CFileProperties_Entry* inFileProperties);
	bool ExportFilesToFolder(int inCollectionID, CString inFolder , bool inIncludeDB = false);
	bool ExportFilesToZip(int inCollectionID, CString inZipPath, CString inPassword, bool inIncludeDB = false);
	bool PrepareFileList(int inCollectionID, CTFiles* outFilesTable);
	bool CheckKeywordInFile(CString lPath, CString lKeyword);

// Private methods
private:
	void InitializeDocumentFileListHeader();
	void InitializeDocumentFileList();

// Messages
protected:
	//{{AFX_MSG(CLibraryView)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
		
	// Add files/folders
	afx_msg void OnAddFile();
	afx_msg void OnAddFolder();

	// Collections
	CString CreateCollection(CString inName, bool inSubCollection);
	afx_msg void OnCreateCollections();
	afx_msg void OnCreateSubCollections();
	afx_msg void OnUpdateCreateSubCollections(CCmdUI* pCmdUI);
	afx_msg void OnCreateSmartCollections();
	
	// Views
	afx_msg void OnViewFilter();
	afx_msg void OnUpdateViewFilter(CCmdUI* pCmdUI);
	afx_msg void OnViewTags();
	afx_msg void OnUpdateViewTags(CCmdUI* pCmdUI);
	afx_msg void OnViewFileList();
	afx_msg void OnUpdateViewFileList(CCmdUI* pCmdUI);
	afx_msg void OnViewIcons();
	afx_msg void OnUpdateViewIcons(CCmdUI* pCmdUI);
	afx_msg void OnViewThumbnails();
	afx_msg void OnUpdateViewThumbnails(CCmdUI* pCmdUI);
	afx_msg void OnViewPreview();
	afx_msg void OnUpdateViewPreview(CCmdUI* pCmdUI);
	afx_msg void OnVaultView();
	
	// Metadata
	
	afx_msg void OnLibrarySetting();
	afx_msg void OnMetadataTags();
	afx_msg void OnMetadataCategories();
	afx_msg void OnMetadataInfo();
	afx_msg void OnUpdateMetadataInfo(CCmdUI* pCmdUI);
	
	// Import/Export
	afx_msg void OnFoldersWatched();
	afx_msg void OnImportZip();
	afx_msg void OnImportFolder();
	afx_msg void OnExportZip();
	afx_msg void OnExportFolder();
	afx_msg void OnExportFilesZip();
	afx_msg void OnExportFilesFolder();
	afx_msg void OnExport(){};
	afx_msg void OnImport(){};

	afx_msg void OnFreeUpgrade();
	afx_msg void OnSwitchDirect();
	afx_msg void OnSwitchRepository();

	// Repository commands
	afx_msg void OnOpenWorkspace();
	afx_msg void OnUpdateOpenWorkspace(CCmdUI* pCmdUI);
	afx_msg void OnCloseWorkspace();
	afx_msg void OnUpdateCloseWorkspace(CCmdUI* pCmdUI);

	// File commands
	afx_msg void OnOpenFile();
	afx_msg void OnUpdateOpenFile(CCmdUI* pCmdUI);
	afx_msg void OnCloseFile();
	afx_msg void OnUpdateCloseFile(CCmdUI* pCmdUI);

	afx_msg void OnPrint();
	afx_msg void OnUpdatePrint(CCmdUI* pCmdUI);
	afx_msg void OnShareEmail();
	afx_msg void OnDeleteFile();
	afx_msg void OnRenameFile();
	afx_msg void OnUpdateRenameFile(CCmdUI* pCmdUI);
	afx_msg void OnCopyFile();

	afx_msg void OnPreviewPDFFile();
	afx_msg void OnUpdatePreviewPDFFile(CCmdUI* pCmdUI);
	afx_msg void OnConvertToPDFFile();
	afx_msg void OnUpdateConvertToPDFFile(CCmdUI* pCmdUI);
	afx_msg void OnOpenDefaultPDFViewer();
	afx_msg void OnUpdateOpenDefaultPDFViewer(CCmdUI* pCmdUI);
	
	afx_msg void OnAddToCollection(UINT nID);
	afx_msg void OnRemoveFromCollection(UINT nID);
	afx_msg void OnAddToTags(UINT nID);
	afx_msg void OnRemoveFromTags(UINT nID);
	afx_msg void OnUpdateShareEmail(CCmdUI* pCmdUI);
	afx_msg void OnShowInWinExplorer();
	afx_msg void OnUpdateShowInWinExplorer(CCmdUI* pCmdUI);
	afx_msg void OnDeleteCollection();
	afx_msg void OnRenameCollection();
	afx_msg void OnCreateCollectionFromSelection();

	afx_msg void OnHeaderMenu(UINT nID);
	afx_msg void OnUpdateHeaderMenu(CCmdUI* pCmdUI);
	afx_msg void OnHeaderMenuReset();
	afx_msg void OnUpdateHeaderMenuReset(CCmdUI* pCmdUI);

	afx_msg void OnExportCollectionToZip();
	afx_msg void OnExportCollectionToFolder();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();
};

#endif