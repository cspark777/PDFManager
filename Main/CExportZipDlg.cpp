// CExportZipDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Benubird.h"
#include "CExportZipDlg.h"
#include "CConfirmPasswordDlg.h"


// CExportZipDlg dialog

IMPLEMENT_DYNAMIC(CExportZipDlg, CDialog)

CExportZipDlg::CExportZipDlg(int inCollectionID, CWnd* pParent /*=NULL*/)
	: CDialog(CExportZipDlg::IDD, pParent)
{
	m_nIsForLibrary = 0;
	mCollectionID = inCollectionID;
}
void CExportZipDlg::SetData_FromParent(PARAM_FOR_CDB *pParamCDB,int nIsForLibrary)
{
	m_pParamCDB = pParamCDB;
	m_nIsForLibrary = nIsForLibrary;
	m_CollectionsCombo.SetData_FromParent(m_pParamCDB,m_nIsForLibrary);
}
CExportZipDlg::~CExportZipDlg()
{
}

void CExportZipDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COLLECTIONS_COMBO, m_CollectionsCombo);
	DDX_Control(pDX, IDC_EDIT_OUTPUT, m_BrowseEdit);
	DDX_Control(pDX, IDC_EDIT_PASSWORD, mPasswordEdit);
	DDX_Control(pDX, IDC_EDIT_PASSWORD, mPasswordEdit);
}


BEGIN_MESSAGE_MAP(CExportZipDlg, CDialog)
	ON_CBN_SELCHANGE(IDC_COLLECTIONS_COMBO, &CExportZipDlg::OnCbnSelchangeCollectionsCombo)
	ON_BN_CLICKED(IDC_CHECK_ENCRYPT, &CExportZipDlg::OnBnClickedCheckEncrypt)
	ON_BN_CLICKED(IDOK, &CExportZipDlg::OnBnClickedOk)
	ON_EN_UPDATE(IDC_EDIT_OUTPUT, &CExportZipDlg::OnEnUpdateEditOutput)
END_MESSAGE_MAP()


// CExportZipDlg message handlers

BOOL CExportZipDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_CollectionsCombo.Initialize(mCollectionID-FILTERTREE_COLLECTION_MIN+1);

	CString lStrFilter;
	lStrFilter.LoadString(IDS_ZIP_FILES_FILTER);

	m_BrowseEdit.EnableFileBrowseButton(_T(".zip"), lStrFilter);

	CString lStrTitle;
	lStrTitle.LoadString(IDS_SAVE_OUTPUT_AS);
//	m_BrowseEdit.SetBrowseDialogTitle(lStrTitle);

	UpdateControls();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CExportZipDlg::UpdateControls()
{
	BOOL lEnable, lEnable2;

	if(m_nIsForLibrary != 0)
		m_CollectionsCombo.EnableWindow(0);

	long i = m_CollectionsCombo.GetCollectionID();
	lEnable = m_nIsForLibrary ? 1 :(i != 0);

	lEnable2 = m_BrowseEdit.GetWindowTextLength() > 0;

	GetDlgItem(IDOK)->EnableWindow(lEnable && lEnable2);

	lEnable = IsDlgButtonChecked(IDC_CHECK_ENCRYPT);
	GetDlgItem(IDC_EDIT_PASSWORD)->EnableWindow(lEnable);
}

void CExportZipDlg::OnCbnSelchangeCollectionsCombo()
{
	UpdateControls();
}

void CExportZipDlg::OnBnClickedCheckEncrypt()
{
	UpdateControls();
	mPasswordEdit.SetFocus();
	mPasswordEdit.SetSel(0, -1);
}

void CExportZipDlg::OnEnUpdateEditOutput()
{
	UpdateControls();
}


void CExportZipDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	mCollectionID = m_CollectionsCombo.GetCollectionID()+FILTERTREE_COLLECTION_MIN-1; 
	m_BrowseEdit.GetWindowText(mOutputZipFile);

	mPassword.Empty();
	if (IsDlgButtonChecked(IDC_CHECK_ENCRYPT))
	{
		mPasswordEdit.GetWindowText(mPassword);
		
		//check the password if not blank
		if (mPassword.IsEmpty())
		{
			AfxMessageBox(_T("Password cannot be empty."));
			mPasswordEdit.SetFocus();
			return;
		}

		//re-renter the pasword dialog
		CConfirmPasswordDlg lDlg;
		if (lDlg.DoModal() != IDOK)
		{
			mPasswordEdit.SetFocus();
			return;
		}

		if (mPassword != lDlg.mPassword)
		{
			AfxMessageBox(_T("Re-entered password does not match."));
			mPasswordEdit.SetFocus();
			return;
		}
	}

	OnOK();
}

