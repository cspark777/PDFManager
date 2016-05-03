// CBBTabPanel.cpp : implementation file
//

#include "stdafx.h"
#include "CBBTabPanel.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBBTabPanel dialog


CBBTabPanel::CBBTabPanel(CWnd* pParent /*=NULL*/)
	: CDialog(CBBTabPanel::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBBTabPanel)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CBBTabPanel::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBBTabPanel)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBBTabPanel, CDialog)
	//{{AFX_MSG_MAP(CBBTabPanel)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


bool CBBTabPanel::ValidateAndProcess()
{
	return false;
}

bool CBBTabPanel::Init()
{
	return false;
}

/////////////////////////////////////////////////////////////////////////////
// CBBTabPanel message handlers
