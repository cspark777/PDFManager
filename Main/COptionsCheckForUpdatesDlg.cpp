// COptionsCheckForUpdatesDlg.cpp
//*****************************************************************************

#include "stdafx.h"
#include "Benubird.h"
#include "COptionsCheckForUpdatesDlg.h"
#include "COptionsDefs.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



IMPLEMENT_DYNAMIC(COptionsCheckForUpdatesDlg, CDialog)

//*****************************************************************************
// Message map
BEGIN_MESSAGE_MAP(COptionsCheckForUpdatesDlg, CDialog)
	ON_BN_CLICKED(IDC_OPT_CHECK_FOR_UPDATES_CHECK, OnClickedCheckForUpdatesCheck)
END_MESSAGE_MAP()
//*****************************************************************************
COptionsCheckForUpdatesDlg::COptionsCheckForUpdatesDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COptionsCheckForUpdatesDlg::IDD, pParent)
{
}
//*****************************************************************************
COptionsCheckForUpdatesDlg::~COptionsCheckForUpdatesDlg()
{
}
//*****************************************************************************
void COptionsCheckForUpdatesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_OPT_CHECK_FOR_UPDATES_CHECK, m_CheckForUpdatesCheck);
	DDX_Control(pDX, IDC_OPT_CHECK_FOR_UPDATES_COMBO, m_CheckForUpdatesCombo);
}
//*****************************************************************************
BOOL COptionsCheckForUpdatesDlg::ValidateAndProcess()
{
	CWinApp* lApp;
	int lIntValue;
	int lOldIntValue;
	time_t lNow;
	
	lApp = AfxGetApp();

	lOldIntValue = lApp->GetProfileInt(kOptions, kCheckForUpdates, 1);
	
	if (m_CheckForUpdatesCheck.GetCheck() == BST_UNCHECKED)
	{
		lIntValue = -1;
	}
	else
	{
		lIntValue = m_CheckForUpdatesCombo.GetCurSel();
	}
	lApp->WriteProfileInt(kOptions, kCheckForUpdates, lIntValue);

	// Calculate next check for updates
	if (lIntValue != lOldIntValue && lIntValue != -1)
	{
		time(&lNow);
		switch (lIntValue)
		{
			case 0: lNow += 86400; break;
			case 1: lNow += 86400 * 7; break;
		}
		lApp->WriteProfileInt(kOptions, kNextCheckForUpdates, lNow);
	}

	return TRUE;
}
//*****************************************************************************
BOOL COptionsCheckForUpdatesDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CWinApp* lApp;
	int lIntValue;

	lApp = AfxGetApp();

	lIntValue = lApp->GetProfileInt(kOptions, kCheckForUpdates, 0);
	if (lIntValue == -1)
	{
		m_CheckForUpdatesCheck.SetCheck(BST_UNCHECKED);
		m_CheckForUpdatesCombo.SetCurSel(0);
	}
	else
	{
		m_CheckForUpdatesCheck.SetCheck(BST_CHECKED);
		if (m_CheckForUpdatesCombo.GetCount() > lIntValue)
			m_CheckForUpdatesCombo.SetCurSel(lIntValue);
		else
			m_CheckForUpdatesCombo.SetCurSel(0);
	}
	UpdateUI();

	return TRUE;
}
//*****************************************************************************
void COptionsCheckForUpdatesDlg::UpdateUI()
{
	BOOL lEnabled;
	if (m_CheckForUpdatesCheck.GetCheck() == BST_CHECKED)
		lEnabled = TRUE;
	else
		lEnabled = FALSE;
	m_CheckForUpdatesCombo.EnableWindow(lEnabled);
}