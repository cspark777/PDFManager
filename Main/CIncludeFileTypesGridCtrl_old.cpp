//*******************************************************************************
// COPYRIGHT NOTES
// ---------------
// This is a sample for BCGControlBar Library Professional Edition
// Copyright (C) 1998-2007 BCGSoft Ltd.
// All rights reserved.
//
// This source code can be used, distributed or modified
// only under terms and conditions 
// of the accompanying license agreement.
//*******************************************************************************
//
// CellTypesGridCtrl.cpp : implementation file
//

#include "stdafx.h"
//#include "BCGPGridExample.h"
#include "CIncludeFileTypesGridCtrl.h"
//#include "CustomCells.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define idProgressAnim	100

inline int Rand (int nMax)
{
	if (nMax <= 0)
	{
		return 0;
	}

	int nRes = rand () % nMax;
	return min (nRes, nMax - 1);
}

#define COLORS_NUM	7

LPCTSTR colors [COLORS_NUM] =
{
	_T("Red"),
	_T("Orange"),
	_T("Yellow"),
	_T("Green"),
	_T("Blue"),
	_T("Indigo"),
	_T("Violet"),
};

/////////////////////////////////////////////////////////////////////////////
// CIncludeFileTypesGridCtrl

CIncludeFileTypesGridCtrl::CIncludeFileTypesGridCtrl()
{
	m_pProgressItem = NULL;
	m_bProgressIsOn = FALSE;
}

CIncludeFileTypesGridCtrl::~CIncludeFileTypesGridCtrl()
{
}


BEGIN_MESSAGE_MAP(CIncludeFileTypesGridCtrl, CBCGPGridCtrl)
	//{{AFX_MSG_MAP(CIncludeFileTypesGridCtrl)
	ON_WM_CREATE()
	ON_WM_DESTROY()
//	ON_WM_TIMER()
//	ON_WM_HSCROLL()
	//}}AFX_MSG_MAP
//	ON_COMMAND(IDC_ANIMATE, OnAnimateProgress)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CIncludeFileTypesGridCtrl message handlers

int CIncludeFileTypesGridCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CBCGPGridCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	srand ((unsigned) time (NULL));

	SetWholeRowSel (TRUE);
	EnableMarkSortedColumn (FALSE);
	EnableHeader (TRUE, 0);
	
	InsertColumn (0, _T("File Type"), 30);
	InsertColumn (1, _T("Description"), 300);
	
	AddEmptyRow();

	CBCGPGridRow* pRow = CreateNewRow ();
	ASSERT_VALID (pRow);

	
	//pRow->GetItem(1)->SetValue(_T(".doc"));
	pRow->ReplaceItem (1, new CBCGPGridCheckItem(TRUE));

	AddRow (pRow, FALSE);

	AddEmptyRow();
	pRow = CreateNewRow ();

	
	/*
	AddRow (pRow, FALSE);
	AddEmptyRow ();
	
	AdjustLayout ();
	
	LoadState (_T("CellTypesGrid"));
	AdjustLayout ();*/

	return 0;
}

void CIncludeFileTypesGridCtrl::OnDestroy() 
{
	SaveState (_T("CellTypesGrid"));
	CBCGPGridCtrl::OnDestroy();
}

CBCGPGridRow* CIncludeFileTypesGridCtrl::CreateNewRow ()
{
	CBCGPGridRow* pRow = CreateRow (GetColumnCount ());

	for (int i = 0; i < GetColumnCount ();i++)
	{
		pRow->GetItem (i)->AllowEdit (FALSE);
	}

	return pRow;
}

void CIncludeFileTypesGridCtrl::AddEmptyRow ()
{
	AddRow (CreateNewRow (), FALSE);
}

/*void CIncludeFileTypesGridCtrl::OnAnimateProgress()
{
	if (m_bProgressIsOn)
	{
		KillTimer (idProgressAnim);
		m_bProgressIsOn = FALSE;
	}
	else
	{
		SetTimer (idProgressAnim, 50, NULL);
		m_bProgressIsOn = TRUE;
	}
}*/

/*void CIncludeFileTypesGridCtrl::OnTimer(UINT_PTR nIDEvent) 
{
	CBCGPGridCtrl::OnTimer(nIDEvent);

	if (nIDEvent == idProgressAnim)
	{
		ASSERT_VALID (m_pProgressItem);

		long nVal = (long) m_pProgressItem->GetValue ();

		if (nVal >= m_pProgressItem->GetTotal ())
		{
			KillTimer (idProgressAnim);
			m_pProgressItem->SetValue (15l);
			m_bProgressIsOn = FALSE;
		}
		else
		{
			m_pProgressItem->SetValue (nVal + 5l);
		}

		m_pProgressItem->Redraw ();
	}
}*/

/*void CIncludeFileTypesGridCtrl::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CItemSliderCtrl* pItemSliderCtrl = DYNAMIC_DOWNCAST
		(CItemSliderCtrl, pScrollBar);

	if (pItemSliderCtrl != NULL)
	{
		CWnd::OnHScroll (nSBCode, nPos, pScrollBar);
		pItemSliderCtrl->Redraw ();
	}
	else
	{
		CBCGPGridCtrl::OnHScroll(nSBCode, nPos, pScrollBar);
	}
}*/
