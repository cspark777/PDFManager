// CRenameCollectionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Benubird.h"
#include "CRenameCollectionDlg.h"


// CRenameCollectionDlg dialog

IMPLEMENT_DYNAMIC(CRenameCollectionDlg, CDialog)

CRenameCollectionDlg::CRenameCollectionDlg(CString & inCollectionName, CWnd* pParent /*=NULL*/)
	: CDialog(CRenameCollectionDlg::IDD, pParent)
{
	m_CollectionName = inCollectionName;
}

CRenameCollectionDlg::~CRenameCollectionDlg()
{
}

void CRenameCollectionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CRenameCollectionDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CRenameCollectionDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CRenameCollectionDlg message handlers
BOOL CRenameCollectionDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	SetDlgItemText(IDC_NAME_EDIT, m_CollectionName);


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CRenameCollectionDlg::OnBnClickedOk()
{
	GetDlgItemText(IDC_NAME_EDIT, m_CollectionName);
	if (m_CollectionName.IsEmpty())
	{
		AfxMessageBox(_T("The collection name cannot be empty."));
		return;
	}
	OnOK();
}

