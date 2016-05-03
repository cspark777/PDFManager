#if !defined(AFX_CBBTabCtrl_H__E43E0EE1_F86F_4B77_8993_8B19B0893AF8__INCLUDED_)
#define AFX_CBBTabCtrl_H__E43E0EE1_F86F_4B77_8993_8B19B0893AF8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CBBTabCtrl.h : header file
//

#include "afxcmn.h"

#define gMaxNumPanels	15

#include "CBBTabPanel.h"

/////////////////////////////////////////////////////////////////////////////
// CBBTabCtrl window

class CBBTabCtrl : public CTabCtrl
{
public:
	CBBTabCtrl();
	virtual ~CBBTabCtrl();

	//! Shows up the panel with a given index
	void SelTab(int inIndex);

	//! Adds the panel into the tab, do not forget to set properties "child & NoFrame"
	int AddPanel(CBBTabPanel *inPanel, CString inName);

	//! Initializes each panel from the tab
	void InitPanels();

	//! Calls validation and processing method of each panel
	bool ValidateAndProcess();

	//! adjust the panels place into the tab control
	void SetRectangle();

	//! remove all panels
	void RemoveAll();

	//! get current item
	CBBTabPanel* GetCurrentPanel();

	void SetEnable(BOOL Enabled);

	int GetNumOfPanels(){ return mNofPanels; }


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBBTabCtrl)
	//}}AFX_VIRTUAL

// Implementation

	// Generated message map functions
protected:
	//{{AFX_MSG(CBBTabCtrl)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:

	CBBTabPanel	*mTabPanels[gMaxNumPanels];	//! member for storing a pointer to panels (dialog) 
	int			mNofPanels;					//! counter for a panels

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CBBTabCtrl_H__E43E0EE1_F86F_4B77_8993_8B19B0893AF8__INCLUDED_)
