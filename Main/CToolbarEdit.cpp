// AddressEdit.cpp : implementation file
//

#include "stdafx.h"
#include "CToolbarEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CToolbarEdit

CToolbarEdit::CToolbarEdit()
{
}

CToolbarEdit::~CToolbarEdit()
{
}


BEGIN_MESSAGE_MAP(CToolbarEdit, CRichEditCtrl)
	//{{AFX_MSG_MAP(CToolbarEdit)
	ON_WM_PAINT()
	ON_WM_NCPAINT()
	ON_WM_KILLFOCUS()
	ON_CONTROL_REFLECT(EN_HSCROLL, OnHscroll)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CToolbarEdit message handlers


int DrawText (CDC* pDC, const CString& strText, CRect rectText, UINT uiDTFlags, COLORREF clrText)
{
	ASSERT_VALID (pDC);

	if (globalData.DwmIsCompositionEnabled ())
	{
		CBCGPVisualManager::GetInstance ()->DrawTextOnGlass (pDC, strText, rectText, uiDTFlags, 0, clrText);

		return pDC->GetTextExtent (strText).cy;
	}

	COLORREF clrTextOld = (COLORREF)-1;
	if (clrText != (COLORREF)-1)
	{
		clrTextOld = pDC->SetTextColor (clrText);
	}

	int nRes = pDC->DrawText (strText, rectText, uiDTFlags);

	if (clrTextOld != (COLORREF)-1)
	{
		pDC->SetTextColor (clrTextOld);
	}

	return nRes;
}

void CToolbarEdit::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CBCGPMemDC memDC (dc, this);
	CDC* pDC = &memDC.GetDC ();

	CString str;
	GetWindowText (str);

	CRect rect;
	GetClientRect (rect);

	COLORREF clrText = globalData.clrBarText;
	COLORREF clrSelBackground = globalData.clrHilite;
	COLORREF clrSelText = globalData.clrTextHilite;

	BOOL bIsDrawOnGlass = globalData.DwmIsCompositionEnabled ();

	if (bIsDrawOnGlass)
	{
		CBCGPDrawManager dm (*pDC);
		dm.DrawRect (rect, globalData.clrWindow, (COLORREF)-1);
	}
	else
	{
		pDC->FillRect (rect, &globalData.brWindow);
	}

	if (str.IsEmpty ())
	{
		return;
	}

	CRgn rgn;
	rgn.CreateRectRgnIndirect (rect);
	pDC->SelectClipRgn (&rgn);

	pDC->SetBkMode (TRANSPARENT);

	CFont* pOldFont = pDC->SelectObject (&globalData.fontRegular);
	ASSERT_VALID (pOldFont);

	rect.left = GetCharPos (0).x;

	UINT uiDTFlags = DT_LEFT | DT_SINGLELINE | DT_NOPREFIX;

	if (bIsDrawOnGlass)
	{
		CBCGPVisualManager::GetInstance ()->DrawTextOnGlass (pDC, str, rect, uiDTFlags, 0, 
			clrText);
	}
	else
	{
		DrawText (pDC, str, rect, uiDTFlags, clrText);
	}

	long nStart, nEnd;
	GetSel (nStart, nEnd);

	if (nEnd >= nStart)
	{
		CString strSel = GetSelText ();

		CRect rectSel = rect;
		rectSel.left = GetCharPos (nStart).x;

		if (bIsDrawOnGlass)
		{
			CRect rectSelFill = rectSel;
			rectSelFill.right = GetCharPos (nEnd).x;

			if (rectSelFill.Width () > 1)
			{
				int cySel = pDC->GetTextExtent (strSel).cy;

				rectSelFill.top = rectSelFill.CenterPoint ().y - cySel / 2 - 1;
				rectSelFill.bottom = rectSelFill.top + cySel + 2;

				CBCGPDrawManager dm (*pDC);
				dm.DrawRect (rectSelFill, clrSelBackground, (COLORREF)-1);
			}
		}
		else
		{
			pDC->SetBkMode (OPAQUE);
			pDC->SetBkColor (clrSelBackground);
		}

		if (bIsDrawOnGlass)
		{
			CBCGPVisualManager::GetInstance ()->DrawTextOnGlass (pDC, strSel, rectSel, uiDTFlags, 0, 
				clrSelText);
		}
		else
		{
			DrawText (pDC, strSel, rectSel, uiDTFlags, clrSelText);
		}
	}

	pDC->SelectObject (pOldFont);
	pDC->SelectClipRgn (NULL);
}

void CToolbarEdit::Init ()
{
	SetFont (&globalData.fontRegular);
	SendMessage (EM_SETTEXTMODE, TM_PLAINTEXT);
	SetEventMask (GetEventMask () | ENM_SCROLL);
}

BOOL CToolbarEdit::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_ERASEBKGND)
	{
		return TRUE;
	}
	
	return CRichEditCtrl::PreTranslateMessage(pMsg);
}

void CToolbarEdit::OnNcPaint() 
{
	if ((GetStyle () & WS_BORDER) == WS_BORDER ||
		(GetExStyle () & WS_EX_CLIENTEDGE) == WS_EX_CLIENTEDGE)
	{
		CBCGPVisualManager::GetInstance ()->OnDrawControlBorder (this);
	}
}

void CToolbarEdit::OnKillFocus(CWnd* pNewWnd) 
{
	CRichEditCtrl::OnKillFocus(pNewWnd);

	SetSel (0, 0);
	RedrawWindow ();
}

void CToolbarEdit::OnHscroll() 
{
	RedrawWindow ();
}
