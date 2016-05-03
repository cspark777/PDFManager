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
// ChildFrm.cpp : implementation of the CChildFrame class
//

#include "stdafx.h"
#include "Benubird.h"

#include "ChildFrm.h"

#include "LibraryDoc.h"
#include "LibraryView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////

extern BOOL RemoveLibPath(CString path);
extern bool gIsPro;

/////////////////////////////////////////////////////////////////////////////
// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CBCGPMDIChildWnd)

BEGIN_MESSAGE_MAP(CChildFrame, CBCGPMDIChildWnd)
	//{{AFX_MSG_MAP(CChildFrame)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
	ON_WM_CLOSE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChildFrame construction/destruction

CChildFrame::CChildFrame()
{
	// TODO: add member initialization code here
	
}

CChildFrame::~CChildFrame()
{
}

int CChildFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBCGPMDIChildWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	EnableDocking(CBRS_ALIGN_ANY);
//	EnableAutoHideBars(CBRS_ALIGN_ANY);

	return 0;
}

BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CBCGPMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;

    cs.style = WS_CHILD | WS_VISIBLE | WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU
        | FWS_ADDTOTITLE | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_MAXIMIZE;

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CChildFrame diagnostics

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CBCGPMDIChildWnd::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CBCGPMDIChildWnd::Dump(dc);
}

#endif //_DEBUG



void CChildFrame::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	

	TCHAR szChildWindowName[0x100];
	this->GetWindowText(szChildWindowName,0x100);
	CDocument *pDoc = this->GetActiveDocument();
	CView *pView = this->GetActiveView();

	CLibraryDoc* pLibDoc = DYNAMIC_DOWNCAST(CLibraryDoc,pDoc );
	CLibraryView *pLibView = DYNAMIC_DOWNCAST(CLibraryView,pView );

	if((!gIsPro)&&(pLibDoc != NULL))
		return;

	if(pLibDoc != NULL)
	{
		pLibView->SendLibrarySettingToAssistant(0);
		RemoveLibPath(pLibDoc->m_csDBPathName);

	}

	CBCGPMDIChildWnd::OnClose();
}
