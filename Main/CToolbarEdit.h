#if !defined(AFX_ADDRESSEDIT_H__8850A964_B0F6_45AF_8691_4337AEDD3C75__INCLUDED_)
#define AFX_ADDRESSEDIT_H__8850A964_B0F6_45AF_8691_4337AEDD3C75__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AddressEdit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CToolbarEdit window

class CToolbarEdit : public CRichEditCtrl
{
// Construction
public:
	CToolbarEdit();

// Attributes
public:

// Operations
public:
	void Init ();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CToolbarEdit)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CToolbarEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(CToolbarEdit)
	afx_msg void OnPaint();
	afx_msg void OnNcPaint();
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnHscroll();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

int DrawText (CDC* pDC, const CString& strText, CRect rectText, UINT uiDTFlags, COLORREF clrText);

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADDRESSEDIT_H__8850A964_B0F6_45AF_8691_4337AEDD3C75__INCLUDED_)
