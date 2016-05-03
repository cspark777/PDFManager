// MainFrm.h : interface of the CMainFrame class
//

#pragma once
#include "ChildFrm.h"

class CBenubirdAutoProxy;

#include "CRibbonFindEdit.h"
#include "CDB.h"
#include "BenubirdAutoProxy.h"

#define UM_TRAYNOTIFY	(WM_USER + 1)

//for context menu IDs
#define ID_COLLECTIONID_MENUROOT 35500 //Tono, I hope nobody else uses this ID
#define ID_REMOVE_FROM_COLLECTION_ID_MENUROOT 36500
#define ID_COLLECTIONID_MAXNUM 1000//I hope there will be no more entries in the Collection table
#define ID_ADD_TO_TAGS_ID_MENUROOT 38000
#define ID_REMOVE_FROM_TAGS_ID_MENUROOT 39500
#define ID_TAGSID_MAXNUM 1000

enum kSearchMode
{
	kSearchAll,
	kSearchTitle,
	kSearchAuthor,
	kSearchSubject,
	kSearchTags,
	kSearchContent
};

extern BOOL gIsTagStr;
extern BOOL gIsQuickBrowseStr;

/////////////////////////////////////////////////////////////////////////////
// class CMyCommandLineInfo
/////////////////////////////////////////////////////////////////////////////
class CMyCommandLineInfo : public CCommandLineInfo
{
public:
   BOOL    m_bHelp;
   CString m_strParameter;
   BOOL    m_bSuccess;     // all switches ok

public:
   CMyCommandLineInfo(void);

public:
   virtual void ParseParam( LPCTSTR lpszParam, BOOL bFlag, BOOL bLast );
};

/////////////////////////////////////////////////////////////////////////////
// class CMainFrame
/////////////////////////////////////////////////////////////////////////////
class CMainFrame : public CBCGPMDIFrameWnd
{
	
public:
	CMainFrame();
protected: 
	DECLARE_DYNCREATE(CMainFrame)
	friend class CBenubirdAutoProxy;
	friend class CExportUtils;

// Attributes
public:
	int mSearchMode;
	int m_nChildWindowCount;

public:

	DWORD AllowMsg(UINT msg);


// Operations
public:
	CString GetFindText();
	void InitFindText();
	int GetSearchMode(){ return mSearchMode; }

private:
	void OnTrayContextMenu();

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	friend class CBenubirdApp;
	friend class CCollectionTree;

protected:  // control bar embedded members
	CBCGPRibbonStatusBar	m_wndStatusBar;
	
	
	CBCGPRibbonBar			m_wndRibbonBar;
	CBCGPRibbonProgressBar* m_ProgressBar;
	CBCGPRibbonLabel*       m_ProgressLabel;
	CString					m_ProgressText;

	DWORD					mDefaultStyle;
	CBCGPRibbonMainButton	m_MainButton;

	CRibbonFindEdit*		mSearchEdit;

	NOTIFYICONDATA	        m_nid;
	CBCGPRibbonCategory*	m_pRibbonCategory[3];
public:
	int GetZoom_FromCombo();
	void SetZoom_FromCombo(DWORD_PTR dwZoom);
	int GetCurrentPageNo();
	void SetCurrentPageNo(int no);
	void AddWorkspacePanel();
	void RemoveWorkspacePanel();
	
public:
	BOOL CreateRibbonBar();
protected:
	BOOL CreateStatusBar();

public:
	//OLE automation object
	
	CBenubirdAutoProxy*		m_pAutoProxy;

public:
	void ActivateCategory (int nIndex);

private:
	void PrepareSysTrayIcon();
	void ActivateViewByCategory (CBCGPRibbonCategory* pCategory);
//	void SetActiveView (int nView);

public:
	void ShowSysTrayIcon(HICON inIcon);
	void ShowSysTrayIconMessage(CString inTitle, CString inMessage);
	void HideSysTrayIcon();
	void UpdateSysTrayIcon();

	// Progress bar
	BOOL SetProgressRangeMax(int inMax);
	int  GetProgressRangeMax();
	BOOL SetProgressPos(int inPos);
	BOOL IncProgressPos(int inStep  = 1);
	BOOL IncProgressPosForever(int inStep  = 1);
	int  GetProgressPos();
	void ShowProgress(BOOL bShow = TRUE);
	void SetProgressText(LPCTSTR inStr){ m_ProgressText = inStr; }

// Generated message map functions
protected:
	
	//for Library
	afx_msg void OnUpdateAlwaysEnabledButton(CCmdUI* pCmdUI);

	// General commands for app 
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewCustomize();
	afx_msg void OnAppLook(UINT id);
	afx_msg LRESULT OnTrayNotify(WPARAM wp, LPARAM lp);
	afx_msg void OnOpenBenubird();
	afx_msg void OnShowModifiedFiles();
	afx_msg void OnUpdateShowModifiedFiles(CCmdUI* pCmdUI);

	afx_msg void OnClose();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnHelp(UINT nID);

	afx_msg LRESULT OnRefreshFile( WPARAM, LPARAM );
	afx_msg LRESULT OnRefreshAllFiles( WPARAM, LPARAM );
	afx_msg LRESULT OnActivateCategory(WPARAM wParam, LPARAM lParam);

	afx_msg void OnOptions();
	afx_msg void OnSearch(UINT nID);
	afx_msg void OnUpdateSearch(CCmdUI* pCmdUI);
	afx_msg void OnUpdateStatusBar(CCmdUI* pCmdUI);
	afx_msg void OnContentSearch();
	afx_msg void OnUpdateContentSearch(CCmdUI* pCmdUI);

	DECLARE_MESSAGE_MAP()

	UINT	m_nAppLook;

protected:
	enum {cmdContinueAndShow = -2, cmdContinue = -1, cmdFalse = FALSE, cmdTrue = TRUE};
	int CmdLineProcess();
	int ProcessNovaPDFCmdLine(LPCTSTR inCmdLine);
	int ProcessWatchedFolderData(AssistantData_Entry* lEntry);

public:
	CDB *m_pDB;
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
	void SendDataToAssistant(DWORD msg,LPCTSTR strBuf, int len);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
};