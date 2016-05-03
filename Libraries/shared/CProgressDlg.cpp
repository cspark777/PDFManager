
#include <BCGCBProInc.h>			// BCGPro Control Bar
#include "CProgressDlg.h"

#define IDR_ANIMATION_AVI 228 // TODO

CProgressDlg::CProgressDlg() :
	CBCGPProgressDlg()
{
	mParams.m_bDialogLook = TRUE;
	mParams.m_bDisableParentWnd = TRUE;
	mParams.m_bShowCancel = TRUE;
	mParams.m_bShowPercentage = TRUE;
	mParams.m_bShowProgress = TRUE;
	mParams.m_nRangeMax = 100;
	mParams.m_nRangeMin = 0;
	mParams.m_nStep = 1;
	mParams.m_strCaption = _T("");
}

CProgressDlg::~CProgressDlg()
{
}

void CProgressDlg::SetNeverEndingMode()
{
	mParams.m_bShowProgress = FALSE;
	mParams.m_bShowPercentage = FALSE;
	mParams.m_nAnimationResID = IDR_ANIMATION_AVI;
	mParams.m_nAnimationHeight = 60;
}

BOOL CProgressDlg::CreateProgressDlg(CWnd* inParent /*= NULL*/)
{
	m_wndCancel.m_bTransparent = TRUE;
	EnableVisualManagerStyle(FALSE);
	return Create(mParams, inParent);
}

BEGIN_MESSAGE_MAP(CProgressDlg, CBCGPProgressDlg)
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

BOOL CProgressDlg::PumpMessages()
{
	// Must call Create() before using the dialog
	ASSERT (GetSafeHwnd() != NULL);

	HWND hwndThis = GetSafeHwnd();

	MSG msg;

	// Handle dialog messages
	while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
		{
			PostThreadMessage(GetCurrentThreadId(), msg.message, msg.wParam, msg.lParam);
			return FALSE;
		}

		if (!::IsWindow(hwndThis))
		{
			return FALSE;
		}

		if (!IsDialogMessage(&msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);  
		}

		if (!::IsWindow(hwndThis))
		{
			return FALSE;
		}
		//WaitMessage ();
	}
	return TRUE;
}

/*BOOL CProgressDlg::OnEraseBkgnd(CDC* pDC) 
{
	return CDialog::OnEraseBkgnd(pDC);
}*/

/*HBRUSH CProgressDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
}*/