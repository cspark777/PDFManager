// CAboutTabDlg.cpp

#include "stdafx.h"
#include "CAboutTabDlg.h"
#include "CBBTabPanel.h"
#include "CAboutDlg.h"
#include "DebenuPDFLibraryAX0912.h"
#include "sqlite3.h"
#include "Define.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CAboutTabDlg::CAboutTabDlg(CWnd* pParent)
: CBBTabPanel(pParent)
{
}

void CAboutTabDlg::DoDataExchange(CDataExchange* pDX)
{
	CBBTabPanel::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DEBENU_URL_BTN, m_DebenuURLBtn);
}

bool CAboutTabDlg::ValidateAndProcess()
{
	return true;
}

BOOL CAboutTabDlg::OnInitDialog()
{
	CBBTabPanel::OnInitDialog();

	CVersion lVersion;
	CString lStr;
	GetDlgItemText(IDC_ABOUT_VERSION_STATIC, lStr);
	lVersion.GetVersionString(lStr);

#ifdef _FREE
	//lStr = "Benubird PDF Version";
#endif

	// ----------------------
	// Add the version number for QPL
	// KDA: 07/07/2009
	// ----------------------
	DebenuPDFLibraryAX0912 QP; // Update to version 7.19 || June 05, 2010
	QP.UnlockKey(_T(QUICKPDF_LICENSE_KEY));
	lStr = CString("Quick PDF Library Version: ") + QP.LibraryVersion().c_str();
	SetDlgItemText(IDC_QPL_VERSION_STATIC, lStr);
	// ----------------------

	// Add the version number for sqlite
	// KDA: 09/06/2010
	SetDlgItemText(IDC_SQL_VERSION_STATIC, CString("SQLite Version: ") + CString(sqlite3_libversion()));

	m_DebenuURLBtn.SizeToContent();

	return TRUE;
}

void CAboutTabDlg::OnCancel() 
{
	((CAboutDlg*)GetParent())->OnCancel();
}

BOOL CAboutTabDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_ESCAPE)
		{
			OnCancel();
			return TRUE;
		}
		else if (GetKeyState(VK_LCONTROL) & 0x8000 || GetKeyState(VK_RCONTROL) & 0x8000)
		{
			if (pMsg->wParam == VK_TAB)
			{
				((CAboutDlg*)GetParent())->OnChangeTab();
				return TRUE;
			}
		}
	}
	return CBBTabPanel::PreTranslateMessage(pMsg);
}

BEGIN_MESSAGE_MAP(CAboutTabDlg, CBBTabPanel)
	ON_STN_CLICKED(IDC_ABOUT_VERSION_STATIC, &CAboutTabDlg::OnStnClickedAboutVersionStatic)
END_MESSAGE_MAP()

void CAboutTabDlg::OnStnClickedAboutVersionStatic()
{
	// TODO: Add your control notification handler code here
}
