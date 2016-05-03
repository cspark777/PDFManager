// MyDialogBar.cpp : implementation file
//

#include "stdafx.h"
//#include "BCGPIE7Demo.h"
#include "CBBDialogBar.h"

#include "MainFrm.h"
#include "BCGPTagManager.h"

//#include "BCGPIE7DemoDoc.h"
//#include "BCGPIE7DemoView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CBBDialogBarButton, CBCGPButton)

CBBDialogBarButton::CBBDialogBarButton()
{
	m_bTransparent = TRUE;
	m_nFlatStyle   = BUTTONSTYLE_NOBORDERS;
	m_bDrawFocus   = FALSE;
}

CBBDialogBarButton::~CBBDialogBarButton()
{

}

BEGIN_MESSAGE_MAP(CBBDialogBarButton, CBCGPButton)
END_MESSAGE_MAP()

void CBBDialogBarButton::OnDrawBorder (CDC* pDC, CRect& rectClient, UINT /*uiState*/)
{
	if (!CBCGPWinXPVisualManager::IsWinXPThemeAvailible ())
	{
		if (m_nFlatStyle == BUTTONSTYLE_FLAT)
		{
			if (m_bPushed)
			{
				pDC->Draw3dRect (&rectClient, globalData.clrBarShadow, globalData.clrBarHilite);
			}
			else if (m_bHighlighted)
			{
				pDC->Draw3dRect (&rectClient, globalData.clrBarHilite, globalData.clrBarShadow);
			}

			rectClient.DeflateRect (1, 1);
		}
	}
}

void CBBDialogBarButton::OnDraw (CDC* pDC, const CRect& rect, UINT uiState)
{
	CBBDialogBar* pParent = DYNAMIC_DOWNCAST(CBBDialogBar, GetParent ());
	if (pParent != NULL)
	{
		pParent->DrawButton (*this, pDC, rect, uiState);
	}
}

/*void CBBDialogBarButton::OnFillBackground (CDC* pDC, const CRect& rectClient)
{
	OnDrawParentBackground (pDC, rectClient);
}*/

/*void CBBDialogBarButton::OnDrawParentBackground (CDC* pDC, CRect rectClient)
{
	CBBDialogBar* pParent = DYNAMIC_DOWNCAST(CBBDialogBar, GetParent ());
	if (pParent != NULL)
	{
		pParent->DrawBackground (*this, pDC, rectClient);
	}
}*/



IMPLEMENT_DYNCREATE(CBBDialogBarMenuButton, CBCGPMenuButton)

CBBDialogBarMenuButton::CBBDialogBarMenuButton()
{
	m_bTransparent = TRUE;
	m_nFlatStyle   = BUTTONSTYLE_NOBORDERS;
	m_bDrawFocus   = FALSE;
	m_bStayPressed = TRUE;
	m_bOSMenu      = FALSE;
}

CBBDialogBarMenuButton::~CBBDialogBarMenuButton()
{

}

BEGIN_MESSAGE_MAP(CBBDialogBarMenuButton, CBCGPMenuButton)
	ON_WM_GETDLGCODE()
END_MESSAGE_MAP()

void CBBDialogBarMenuButton::OnDrawBorder (CDC* pDC, CRect& rectClient, UINT /*uiState*/)
{
	if (!CBCGPWinXPVisualManager::IsWinXPThemeAvailible ())
	{
		if (m_nFlatStyle == BUTTONSTYLE_FLAT)
		{
			if (m_bPushed)
			{
				pDC->Draw3dRect (&rectClient, globalData.clrBarShadow, globalData.clrBarHilite);
			}
			else if (m_bHighlighted)
			{
				pDC->Draw3dRect (&rectClient, globalData.clrBarHilite, globalData.clrBarShadow);
			}

			rectClient.DeflateRect (1, 1);
		}
	}
}

void CBBDialogBarMenuButton::OnDraw (CDC* pDC, const CRect& rect, UINT uiState)
{
	CBBDialogBar* pParent = DYNAMIC_DOWNCAST(CBBDialogBar, GetParent ());
	if (pParent != NULL)
	{
		pParent->DrawButton (*this, pDC, rect, uiState);
	}
}

/*void CBBDialogBarMenuButton::OnFillBackground (CDC* pDC, const CRect& rectClient)
{
	OnDrawParentBackground (pDC, rectClient);
}*/

/*void CBBDialogBarMenuButton::OnDrawParentBackground (CDC* pDC, CRect rectClient)
{
	CBBDialogBar* pParent = DYNAMIC_DOWNCAST(CBBDialogBar, GetParent ());
	if (pParent != NULL)
	{
		pParent->DrawBackground (*this, pDC, rectClient);
	}
}*/

void CBBDialogBarMenuButton::OnShowMenu ()
{
	BOOL bRes = TRUE;

	CBBDialogBar* pParent = DYNAMIC_DOWNCAST(CBBDialogBar, GetParent ());
	if (pParent != NULL)
	{
		bRes = pParent->PrepareButtonMenu (*this);
	}

	if (bRes)
	{
		CBCGPMenuButton::OnShowMenu ();

		if (m_pToolTip->GetSafeHwnd () != NULL)
		{
			m_pToolTip->Pop ();

			CString str;
			m_pToolTip->GetText (str, this);
			m_pToolTip->UpdateTipText (str, this);
		}
	}
}

UINT CBBDialogBarMenuButton::OnGetDlgCode() 
{
	return CBCGPButton::OnGetDlgCode ();
}


/////////////////////////////////////////////////////////////////////////////
// CBBDialogBar

IMPLEMENT_SERIAL (CBBDialogBar, CBCGPDialogBar, VERSIONABLE_SCHEMA | 1)

CBBDialogBar::CBBDialogBar()
{
	m_bInitialized = FALSE;
	m_bIsTransparent = FALSE;
}

CBBDialogBar::~CBBDialogBar()
{
	
}

/*void CBBDialogBar::DrawBackground (CWnd& wnd, CDC* pDC, CRect rectClient)
{
	CRect rect = rectClient;
	wnd.ClientToScreen (rect);
	ScreenToClient (rect);

	pDC->BitBlt (rectClient.left, rectClient.top, 
	rectClient.Width (), rectClient.Height (), &m_memDC.GetDC (), rect.left, rect.top, SRCCOPY);
}*/

void CBBDialogBar::DrawButton(CBCGPButton& btn, CDC* pDC, const CRect& rect, UINT uiState)
{
	CBCGPToolbarButton btnTB;
	CBCGPVisualManager::BCGBUTTON_STATE state = CBCGPVisualManager::ButtonsIsRegular;

	if ((uiState & ODS_DISABLED) == ODS_DISABLED)
	{	
		btnTB.m_nStyle = TBBS_DISABLED;
	}
	else
	{
		if (btn.IsPushed ())
		{
			btnTB.m_nStyle = TBBS_PRESSED;
		}
		
		if (btn.IsHighlighted ())
		{
			state = CBCGPVisualManager::ButtonsIsHighlighted;
		}
	}

	CBCGPVisualManager::GetInstance ()->OnFillButtonInterior (pDC, &btnTB, rect, state);
}

void CBBDialogBar::ChangeStyle()
{
	LoadImages ();
}

BEGIN_MESSAGE_MAP(CBBDialogBar, CBCGPDialogBar)
	//{{AFX_MSG_MAP(CBBDialogBar)
	ON_WM_PAINT()
//	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_SYSCOLORCHANGE()
	ON_MESSAGE(WM_INITDIALOG, HandleInitDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBBDialogBar message handlers

BOOL CBBDialogBar::OnInitDialog ()
{
	CRect rectClient;
	GetClientRect (rectClient);

	m_sizeDefault = rectClient.Size ();

	return TRUE;
}

LRESULT CBBDialogBar::HandleInitDialog(WPARAM wParam, LPARAM lParam)
{
	if (!CBCGPDialogBar::HandleInitDialog (wParam, lParam))
	{
		return FALSE;
	}

	m_bInitialized = OnInitDialog ();

	if (!IsInitialized ())
	{
		return FALSE;
	}

	ChangeStyle ();

	Resize (TRUE);

	return TRUE;
}

void CBBDialogBar::OnPaint ()
{
	CRect rectClient;
	GetClientRect (rectClient);
	CPaintDC dc (this);

	/*dc.BitBlt (rectClient.left, rectClient.top, 
		rectClient.Width (), rectClient.Height (), &m_memDC.GetDC (), 
		rectClient.left, rectClient.top, SRCCOPY);*/
}

/*void CBBDialogBar::PaintMemDC ()
{
	CRect rectClient;
	GetClientRect (rectClient);

	CRect rect = rectClient;
	ClientToScreen (rect);
	CFrameWnd* pFrame = BCGPGetParentFrame (this);
	pFrame->ScreenToClient (rect);

	if (m_bIsTransparent)
	{
		m_memDC.GetDC().FillSolidRect (rectClient, RGB (0, 0, 0));
	}
	else
	{
		m_memDC.GetDC().BitBlt (rectClient.left, rectClient.top, 
			rectClient.Width (), rectClient.Height (), &CRebarDC::GetInstance ().GetDC (), 
			rect.left, rect.top, SRCCOPY);
	}
}*/

/*BOOL CBBDialogBar::OnEraseBkgnd(CDC* pDC) 
{

	//CRect rectClient;
	//GetClientRect (rectClient);

	//CBCGPVisualManager::GetInstance()->FillRebarPane(pDC, this, rectClient);

	return TRUE;
}*/

void CBBDialogBar::OnSize(UINT nType, int cx, int cy)
{
	CBCGPDockingControlBar::OnSize (nType, cx, cy);

	if (cx != 0 && IsInitialized ())
	{
		m_memDC.SetSize (CSize (cx, cy));

//		PaintMemDC ();
		if (IsWindowVisible ())
		{
			Invalidate ();
		}

		Resize (FALSE);
	}
}

CSize CBBDialogBar::CalcFixedLayout(BOOL bStretch, BOOL bHorz)
{
	CSize size (CBCGPDockingControlBar::CalcFixedLayout (bStretch, bHorz));

	size.cy = GetHeight (size.cy);

	return size;
}

void CBBDialogBar::OnSysColorChange()
{
	CBCGPDockingControlBar::OnSysColorChange ();

	if (IsInitialized ())
	{
		Resize (TRUE);
//		PaintMemDC ();
		Invalidate ();
	}
}
