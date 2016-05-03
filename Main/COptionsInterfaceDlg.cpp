// COptionsInterfaceDlg.cpp 
//*****************************************************************************

#include "stdafx.h"
#include "Benubird.h"
#include "COptionsInterfaceDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(COptionsInterfaceDlg, CDialog)

//*****************************************************************************
// Message map
BEGIN_MESSAGE_MAP(COptionsInterfaceDlg, CDialog)
END_MESSAGE_MAP()
//*****************************************************************************
COptionsInterfaceDlg::COptionsInterfaceDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COptionsInterfaceDlg::IDD, pParent)
{
}
//*****************************************************************************
COptionsInterfaceDlg::~COptionsInterfaceDlg()
{
}
//*****************************************************************************
void COptionsInterfaceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_OPT_INTERFACE_COLOR_SCHEME_COMBO, m_ColorSchemeCombo);
}
//*****************************************************************************
BOOL COptionsInterfaceDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	int lPos = 0;
	switch (theApp.m_nAppLook)
	{
		case ID_VIEW_APPLOOK_2007:
			lPos = 0;
			break;
		
		case ID_VIEW_APPLOOK_2007_1:
			lPos = 1;
			break;
		
		case ID_VIEW_APPLOOK_2007_2:
			lPos = 2;
			break;
	}
	m_ColorSchemeCombo.SetCurSel(lPos);

	return TRUE;  
}
//*****************************************************************************
BOOL COptionsInterfaceDlg::ValidateAndProcess()
{
	int lIntValue;
	lIntValue = m_ColorSchemeCombo.GetCurSel();
	switch (lIntValue)
	{
		case 0: 
			theApp.m_nAppLook = ID_VIEW_APPLOOK_2007;
			CBCGPVisualManager2007::SetStyle (CBCGPVisualManager2007::VS2007_LunaBlue);
			break;

		case 1:
			 theApp.m_nAppLook = ID_VIEW_APPLOOK_2007_1;
			 CBCGPVisualManager2007::SetStyle (CBCGPVisualManager2007::VS2007_ObsidianBlack);
			break;

		case 2:
			theApp.m_nAppLook = ID_VIEW_APPLOOK_2007_2;
			CBCGPVisualManager2007::SetStyle (CBCGPVisualManager2007::VS2007_Silver);
			break;
	}

	CBCGPVisualManager::SetDefaultManager (RUNTIME_CLASS (CBCGPVisualManager2007));
	CBCGPDockManager::SetDockMode (BCGP_DT_SMART);
	
	
	RedrawWindow (NULL, NULL, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE);

	return TRUE;
}
