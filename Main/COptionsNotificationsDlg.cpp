// COptionsNotificationsDlg.cpp 
//*****************************************************************************

#include "stdafx.h"
#include "Benubird.h"
#include "COptionsNotificationsDlg.h"
#include "COptionsDefs.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNAMIC(COptionsNotificationsDlg, CDialog)

//*****************************************************************************
// Message map
BEGIN_MESSAGE_MAP(COptionsNotificationsDlg, CDialog)
END_MESSAGE_MAP()
//*****************************************************************************
COptionsNotificationsDlg::COptionsNotificationsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COptionsNotificationsDlg::IDD, pParent)
{
}
//*****************************************************************************
COptionsNotificationsDlg::~COptionsNotificationsDlg()
{
}
//*****************************************************************************
void COptionsNotificationsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_OPT_NOTIFY_WHEN_ADD_SILENTLY_CHECK, m_AddSilentlyCheck);
	DDX_Control(pDX, IDC_OPT_NOTIFY_WHEN_ADD_DIRECTLY_CHECK, m_AddDirectlyCheck);
	DDX_Control(pDX, IDC_OPT_NOTIFY_WHEN_UPDATE_AVAILABLE_CHECK, m_UpdateAvailableCheck);
}
//*****************************************************************************
BOOL COptionsNotificationsDlg::ValidateAndProcess()
{
	int lIntValue;
	CWinApp* lApp;

	lApp = AfxGetApp();

	lIntValue = m_AddSilentlyCheck.GetCheck();
	lApp->WriteProfileInt(kOptions, kNotifyWhenAddSilently, lIntValue);

	lIntValue = m_AddDirectlyCheck.GetCheck();
	lApp->WriteProfileInt(kOptions, kNotifyWhenAddDirectly, lIntValue);

	lIntValue = m_UpdateAvailableCheck.GetCheck();
	lApp->WriteProfileInt(kOptions, kNotifyWhenUpdateAvailable, lIntValue);

	return TRUE;
}
//*****************************************************************************
BOOL COptionsNotificationsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	int lIntValue;
	CWinApp* lApp;

	lApp = AfxGetApp();

	lIntValue = lApp->GetProfileInt(kOptions, kNotifyWhenAddSilently, BST_UNCHECKED);
	m_AddSilentlyCheck.SetCheck(lIntValue);
	
	lIntValue = lApp->WriteProfileInt(kOptions, kNotifyWhenAddDirectly, BST_UNCHECKED);
	m_AddDirectlyCheck.SetCheck(lIntValue);
	
	lIntValue = lApp->WriteProfileInt(kOptions, kNotifyWhenUpdateAvailable, BST_UNCHECKED);
	m_UpdateAvailableCheck.SetCheck(lIntValue);
	
	return TRUE;
}