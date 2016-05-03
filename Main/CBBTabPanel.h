#if !defined(AFX_CAPTABPANEL_H__230EF9B1_E007_4251_ABEF_FA539A9D33CD__INCLUDED_)
#define AFX_CAPTABPANEL_H__230EF9B1_E007_4251_ABEF_FA539A9D33CD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CBBTabPanel.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBBTabPanel dialog


//! CBBTabPanel - each panel on tab dialog must be derived from this class
class CBBTabPanel : public CDialog
{
// Construction
public:
	CBBTabPanel(CWnd* pParent = NULL);   // standard constructor
	//! you must overvrite this method, otherwise the on OK message will be canceled.
	//! do here validation and processing your panels job
	virtual bool ValidateAndProcess();

	//! you can use it for your own initialization, but you can do this in OnInitDialog method too
	virtual bool Init();
	virtual void EnableChanges(BOOL Enable){};

// Dialog Data
	//{{AFX_DATA(CBBTabPanel)
	enum { IDD = -1 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBBTabPanel)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBBTabPanel)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CAPTABPANEL_H__230EF9B1_E007_4251_ABEF_FA539A9D33CD__INCLUDED_)
