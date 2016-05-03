// CContentSearchDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Benubird.h"
#include "CContentSearchDlg.h"


// CContentSearchDlg dialog

IMPLEMENT_DYNAMIC(CContentSearchDlg, CDialog)

CContentSearchDlg::CContentSearchDlg(int inCollectionID, CWnd* pParent /*=NULL*/)
	: CDialog(CContentSearchDlg::IDD, pParent)
{
	m_nIsForLibrary = 0;
	mCollectionID = inCollectionID;
}

void CContentSearchDlg::SetData_FromParent(PARAM_FOR_CDB *pParamCDB,int nIsForLibrary)
{
	m_pParamCDB = pParamCDB;
	m_nIsForLibrary = nIsForLibrary;
	m_CollectionsCombo.SetData_FromParent(m_pParamCDB,m_nIsForLibrary);
}

CContentSearchDlg::~CContentSearchDlg()
{
}

void CContentSearchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COLLECTIONS_COMBO, m_CollectionsCombo);
	DDX_Control(pDX, IDC_EDIT_FOLDER, m_BrowseEdit);
	DDX_Control(pDX, IDC_EDIT_WORD, m_Keyword);
}


BEGIN_MESSAGE_MAP(CContentSearchDlg, CDialog)
	ON_CBN_SELCHANGE(IDC_COLLECTIONS_COMBO, &CContentSearchDlg::OnCbnSelchangeCollectionsCombo)
	ON_BN_CLICKED(IDOK, &CContentSearchDlg::OnBnClickedOk)
	ON_EN_UPDATE(IDC_EDIT_OUTPUT, &CContentSearchDlg::OnEnUpdateEditOutput)
	ON_EN_UPDATE(IDC_EDIT_WORD, &CContentSearchDlg::OnEnUpdateEditWord)
END_MESSAGE_MAP()


// CContentSearchDlg message handlers

BOOL CContentSearchDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_CollectionsCombo.Initialize(mCollectionID-FILTERTREE_COLLECTION_MIN+1);

	m_BrowseEdit.EnableFolderBrowseButton();

	UpdateControls();


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CContentSearchDlg::UpdateControls()
{
	BOOL lEnable, lEnable2;

	int i = m_CollectionsCombo.GetCollectionID();
	lEnable = (i != 0);

	lEnable2 = m_Keyword.GetWindowTextLength() > 0;

	GetDlgItem(IDOK)->EnableWindow(lEnable && lEnable2);
}

void CContentSearchDlg::OnCbnSelchangeCollectionsCombo()
{
	UpdateControls();
}

void CContentSearchDlg::OnEnUpdateEditOutput()
{
	UpdateControls();
}

void CContentSearchDlg::OnEnUpdateEditWord()
{
	UpdateControls();
}

void CContentSearchDlg::OnBnClickedOk()
{
	mCollectionID = m_CollectionsCombo.GetCollectionID()+FILTERTREE_COLLECTION_MIN-1; 
	m_BrowseEdit.GetWindowText(mOutFolder);
	m_Keyword.GetWindowText(mKeyword);

	// TODO: Add your control notification handler code here
	OnOK();
}

