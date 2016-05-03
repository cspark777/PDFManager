//*******************************************************************************
// COPYRIGHT NOTES
// ---------------
// This is a sample for BCGControlBar Library Professional Edition
// Copyright (C) 1998-2012 BCGSoft Ltd.
// All rights reserved.
//
// This source code can be used, distributed or modified
// only under terms and conditions 
// of the accompanying license agreement.
//*******************************************************************************
//
// ShortcutsView.cpp : implementation file
//

#include "stdafx.h"
#include "Benubird.h"
#include "LibraryDoc.h"
#include "mainfrm.h"

#include "ShortcutsView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define IDC_START_IMAGE	1

/////////////////////////////////////////////////////////////////////////////
// CShortcutsView

IMPLEMENT_DYNCREATE(CShortcutsView, CView)

BEGIN_MESSAGE_MAP(CShortcutsView, CView)
	//{{AFX_MSG_MAP(CShortcutsView)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND ()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_START_IMAGE, OnClickHotSpot)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CShortcutsView construction/destruction

CShortcutsView::CShortcutsView()
{
}

CShortcutsView::~CShortcutsView()
{
}

BOOL CShortcutsView::PreCreateWindow(CREATESTRUCT& cs)
{
	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CShortcutsView drawing

BOOL CShortcutsView::OnEraseBkgnd(CDC*)
{
	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// CShortcutsView diagnostics

#ifdef _DEBUG
void CShortcutsView::AssertValid() const
{
	CView::AssertValid();
}

void CShortcutsView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CLibraryDoc* CShortcutsView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CLibraryDoc)));
	return (CLibraryDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CShortcutsView message handlers

void CShortcutsView::OnDraw(CDC* /*pDC*/)
{

}

int CShortcutsView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
	{
		return -1;
	}
	
	CRect rectClient;
	GetClientRect (rectClient);

	m_wndHotSpot.Create (rectClient, this, IDC_START_IMAGE);
	SetBitmap ();
	
	return 0;
}

void CShortcutsView::SetBitmap ()
{
	if (m_wndHotSpot.GetSafeHwnd () == NULL)
	{
		return;
	}
	/*
	m_wndHotSpot.SetImage (IDB_HOME);
	m_wndHotSpot.SetBackgroundColor (RGB (227, 232, 242));
	m_wndHotSpot.SetHighlightHotArea (82, 45);

	m_wndHotSpot.AddHotSpot (ID_SHORTCUTS_HOME, CRect (CPoint (33, 302), CSize (125, 50)));
	m_wndHotSpot.AddHotSpot (ID_ACTIVATE_VIEW_1, CRect (CPoint (54, 139), CSize (246, 11)));
	m_wndHotSpot.AddHotSpot (ID_ACTIVATE_VIEW_2, CRect (CPoint (54, 169), CSize (246, 11)));
	m_wndHotSpot.AddHotSpot (ID_ACTIVATE_VIEW_3, CRect (CPoint (54, 199), CSize (246, 11)));
	m_wndHotSpot.AddHotSpot (ID_ACTIVATE_VIEW_4, CRect (CPoint (54, 229), CSize (246, 11)));
	m_wndHotSpot.AddHotSpot (ID_ACTIVATE_VIEW_5, CRect (CPoint (54, 259), CSize (246, 11)));*/
}

void CShortcutsView::OnClickHotSpot()
{
	AfxGetMainWnd ()->PostMessage(WM_COMMAND, m_wndHotSpot.GetHotSpot (), 0);
}

void CShortcutsView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);

	if (m_wndHotSpot.GetSafeHwnd () != NULL)
	{
		m_wndHotSpot.SetWindowPos (NULL, -1, -1, cx, cy, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
	}
}

void CShortcutsView::UpdateCaptionText ()
{
	CMainFrame* pFrame = DYNAMIC_DOWNCAST(CMainFrame, AfxGetMainWnd ());

	if (pFrame != NULL)
	{
		pFrame->SetTitle (_T(" "));
	}
}

void CShortcutsView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	CView::OnActivateView(bActivate, pActivateView, pDeactiveView);

	if (bActivate)
	{
		UpdateCaptionText ();
	}
}
