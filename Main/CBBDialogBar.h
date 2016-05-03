#if !defined(AFX_MyDialogBAR_H__3108F6A3_320E_4244_AA91_6E4035798E63__INCLUDED_)
#define AFX_MyDialogBAR_H__3108F6A3_320E_4244_AA91_6E4035798E63__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyDialogBar.h : header file
//

#include "RebarDC.h"

#define HORIZ_MARGIN 15
#define VERT_MARGIN  15

/////////////////////////////////////////////////////////////////////////////
// CBBDialogBar dialog

class CBBDialogBarButton: public CBCGPButton
{
	DECLARE_DYNCREATE(CBBDialogBarButton)

public:

	CBBDialogBarButton();
	virtual ~CBBDialogBarButton();

protected:
	virtual void OnDrawBorder (CDC* pDC, CRect& rectClient, UINT uiState);
	//virtual void OnFillBackground (CDC* pDC, const CRect& rectClient);
	//virtual void OnDrawParentBackground (CDC* pDC, CRect rectClient);
	virtual void OnDraw (CDC* pDC, const CRect& rect, UINT uiState);

	DECLARE_MESSAGE_MAP()
};

class CBBDialogBarMenuButton: public CBCGPMenuButton
{
	DECLARE_DYNCREATE(CBBDialogBarMenuButton)

public:

	CBBDialogBarMenuButton();
	virtual ~CBBDialogBarMenuButton();

	
protected:
	virtual void OnDrawBorder (CDC* pDC, CRect& rectClient, UINT uiState);
	//virtual void OnFillBackground (CDC* pDC, const CRect& rectClient);
	//virtual void OnDrawParentBackground (CDC* pDC, CRect rectClient);
	virtual void OnDraw (CDC* pDC, const CRect& rect, UINT uiState);
	virtual void OnShowMenu ();

	afx_msg UINT OnGetDlgCode();

	DECLARE_MESSAGE_MAP()
};

class CBBDialogBar : public CBCGPDialogBar
{
	friend class CBBDialogBarButton;
	friend class CBBDialogBarMenuButton;
	
	DECLARE_SERIAL(CBBDialogBar)

// Construction
public:
	CBBDialogBar();
	virtual ~CBBDialogBar();

	virtual int GetHeight (int height) const
	{
		return height;
	}
	virtual	void Resize (BOOL /*bFull*/)
	{

	}

	//virtual	void DrawBackground (CWnd& wnd, CDC* pDC, CRect rectClient);

// Implementation
protected:

	BOOL IsInitialized () const
	{
		return m_bInitialized;
	}

	virtual	BOOL LoadImages()
	{
		return FALSE;
	}
	virtual	void ChangeStyle();
	virtual	void DrawButton(CBCGPButton& btn, CDC* pDC, const CRect& rect, UINT uiState);
	virtual	BOOL PrepareButtonMenu(CBCGPMenuButton& /*btn*/)
	{
		return TRUE;
	}

	virtual CSize CalcFixedLayout(BOOL bStretch, BOOL bHorz);
	virtual BOOL OnShowControlBarMenu (CPoint /*point*/)
	{
		return FALSE;
	}

	virtual BOOL OnInitDialog ();
	//virtual void PaintMemDC ();

	// Generated message map functions
	//{{AFX_MSG(CBBDialogBar)
	afx_msg LRESULT HandleInitDialog(WPARAM wParam, LPARAM lParam);
	afx_msg void OnPaint();
	//afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSysColorChange();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	BOOL	m_bInitialized;
	Debenu::CMemDC	m_memDC;
	BOOL	m_bIsTransparent;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MyDialogBAR_H__3108F6A3_320E_4244_AA91_6E4035798E63__INCLUDED_)
