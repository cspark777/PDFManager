#if !defined(AFX_OUTLOOKBAR_H__E74449DF_CA8F_4053_95FD_96E55BE59E56__INCLUDED_)
#define AFX_OUTLOOKBAR_H__E74449DF_CA8F_4053_95FD_96E55BE59E56__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ShortcutsBar.h : header file
//

//#include "BCGPOutlookBar.h"
#include "CCollectionPanel.h"
#include "common.h"


class COutlookBar : public CBCGPOutlookBar
{
//	friend class CMainFrame;

protected:
	
	DECLARE_DYNAMIC(COutlookBar)

// Attributes
public:

	LIBRARY_VIEW_INFO *m_pViewInfo;

	PARAM_FOR_CDB *m_pParamCDB;
	void Set_ParamForCDB(PARAM_FOR_CDB *pParamCDB,LIBRARY_VIEW_INFO *pViewInfo);
public:
	COutlookBar();           // protected constructor used by dynamic creation
	~COutlookBar();           // protected constructor used by dynamic creation
	int GetActiveShortcut ()
	{
		return GetUnderlinedWindow ()->GetActiveTab ();
	}

	int SetActiveShortcut (int index)
	{
		return GetUnderlinedWindow ()->SetActiveTab (index);
	}
	int GetMaxSize(){ return 200; };


// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COutlookBar)
	public:
	virtual BOOL Create(LPCTSTR lpszCaption, CWnd* pParentWnd, const RECT& rect, UINT nID, DWORD dwStyle, DWORD dwBCGStyle = CBRS_BCGP_RESIZE, CCreateContext* pContext = NULL);
	virtual void OnMoveSlider(CBCGPSlider* inSlider);
	//}}AFX_VIRTUAL

// Implementation
protected:
	

	// Generated message map functions
protected:
	//{{AFX_MSG(COutlookBar)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
		//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
public:
	CCollectionPanel m_wndCollectionPanel;
};

#endif
