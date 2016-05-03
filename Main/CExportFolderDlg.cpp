// CExportFolderDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Benubird.h"
#include "CExportFolderDlg.h"


// CExportFolderDlg dialog

IMPLEMENT_DYNAMIC(CExportFolderDlg, CDialog)

CExportFolderDlg::CExportFolderDlg(int inCollectionID, CWnd* pParent /*=NULL*/)
	: CDialog(CExportFolderDlg::IDD, pParent)
{
	m_nIsForLibrary = 0;
	mCollectionID = inCollectionID;
}

void CExportFolderDlg::SetData_FromParent(PARAM_FOR_CDB *pParamCDB,int nIsForLibrary)
{
	m_pParamCDB = pParamCDB;
	m_nIsForLibrary = nIsForLibrary;
	m_CollectionsCombo.SetData_FromParent(m_pParamCDB,m_nIsForLibrary);
}

CExportFolderDlg::~CExportFolderDlg()
{
}

void CExportFolderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COLLECTIONS_COMBO, m_CollectionsCombo);
	DDX_Control(pDX, IDC_EDIT_OUTPUT, m_BrowseEdit);
}


BEGIN_MESSAGE_MAP(CExportFolderDlg, CDialog)
	ON_CBN_SELCHANGE(IDC_COLLECTIONS_COMBO, &CExportFolderDlg::OnCbnSelchangeCollectionsCombo)
	ON_BN_CLICKED(IDOK, &CExportFolderDlg::OnBnClickedOk)
	ON_EN_UPDATE(IDC_EDIT_OUTPUT, &CExportFolderDlg::OnEnUpdateEditOutput)
END_MESSAGE_MAP()


// CExportFolderDlg message handlers

BOOL CExportFolderDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_CollectionsCombo.Initialize(mCollectionID-FILTERTREE_COLLECTION_MIN+1);

	m_BrowseEdit.EnableFolderBrowseButton();

	UpdateControls();


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CExportFolderDlg::UpdateControls()
{
	BOOL lEnable, lEnable2;

	int i = m_CollectionsCombo.GetCollectionID();
	lEnable = (i != 0);

	lEnable2 = m_BrowseEdit.GetWindowTextLength() > 0;

	GetDlgItem(IDOK)->EnableWindow(lEnable && lEnable2);
}

void CExportFolderDlg::OnCbnSelchangeCollectionsCombo()
{
	UpdateControls();
}

void CExportFolderDlg::OnEnUpdateEditOutput()
{
	UpdateControls();
}


void CExportFolderDlg::OnBnClickedOk()
{
	mCollectionID = m_CollectionsCombo.GetCollectionID()+FILTERTREE_COLLECTION_MIN-1; 
	m_BrowseEdit.GetWindowText(mOutputFolder);

	// TODO: Add your control notification handler code here
	OnOK();
}

