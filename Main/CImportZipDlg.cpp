// CImportZipDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Benubird.h"
#include "CImportZipDlg.h"

#include "CIncludeFileTypesDlg.h"
#include "CEditFilePropertiesDlg.h"
#include "COptionsDlg.h"
#include "MainFrm.h"
#include "LibraryView.h"

extern CMainFrame* gMainFrm;


// CImportZipDlg dialog


IMPLEMENT_DYNAMIC(CImportZipDlg, CDialog)

CImportZipDlg::CImportZipDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CImportZipDlg::IDD, pParent)
{
	m_nIsForLibrary = 0;
}

CImportZipDlg::~CImportZipDlg()
{
}

void CImportZipDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_INPUT, m_BrowseEdit);
	DDX_Control(pDX, IDC_CHECK_PASSWORD, m_PasswordCheck);
	DDX_Control(pDX, IDC_EDIT_PASSWORD, m_PasswordEdit);
	DDX_Control(pDX, IDC_COLLECTION_COMBO, m_CollectionCombo);
}


BEGIN_MESSAGE_MAP(CImportZipDlg, CDialog)
	ON_BN_CLICKED(IDC_CHECK_PASSWORD, &CImportZipDlg::OnBnClickedCheckPassword)
	ON_EN_CHANGE(IDC_EDIT_INPUT, &CImportZipDlg::OnEnChangeEditInput)
	ON_BN_CLICKED(IDOK, &CImportZipDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_RADIO_ALL_FILES, &CImportZipDlg::OnBnClickedRadioAllFiles)
	ON_BN_CLICKED(IDC_RADIO_SOME_FILES, &CImportZipDlg::OnBnClickedRadioSomeFiles)
	ON_BN_CLICKED(IDC_BUTTON_FILE_TYPES, &CImportZipDlg::OnBnClickedButtonFileTypes)
	ON_BN_CLICKED(IDC_BUTTON_OPTIONS, &CImportZipDlg::OnBnClickedButtonOptions)
END_MESSAGE_MAP()


// CImportZipDlg message handlers

BOOL CImportZipDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CString lStrFilter;
	lStrFilter.LoadString(IDS_ZIP_FILES_FILTER);

	m_BrowseEdit.EnableFileBrowseButton(_T(".zip"), lStrFilter);

	((CButton*)GetDlgItem(IDC_RADIO_ALL_FILES))->SetCheck(mZipImportProperties.mFileTypes.GetCount() == 0);
	((CButton*)GetDlgItem(IDC_RADIO_SOME_FILES))->SetCheck(mZipImportProperties.mFileTypes.GetCount() != 0);

	if(m_nIsForLibrary == 1)
	{
		return TRUE;
	}
	else
	{
		CMDIChildWnd * pChild = ((CMDIFrameWnd*)(AfxGetApp()->m_pMainWnd))->MDIGetActive();
		CLibraryView* pView = DYNAMIC_DOWNCAST(CLibraryView, pChild->GetActiveView ());
		m_CollectionCombo.AddString(_T("-"));
		m_CollectionCombo.Initialize(pView->GetSelectedCollID(), CCollectionsComboBox::includeCollections);
		UpdateControls();
	}
	

	return TRUE;
}


void CImportZipDlg::UpdateControls()
{
	CString lSourcePath;
	m_BrowseEdit.GetWindowText(lSourcePath);

	BOOL lEnable1 = !lSourcePath.IsEmpty();
	BOOL lEnable2 = m_PasswordCheck.GetCheck();

	m_PasswordEdit.EnableWindow(lEnable2);
	GetDlgItem(IDOK)->EnableWindow(lEnable1);

	BOOL lEnable = IsDlgButtonChecked(IDC_RADIO_SOME_FILES);
	GetDlgItem(IDC_BUTTON_FILE_TYPES)->EnableWindow(lEnable);

	CString lStr;
	if (lEnable)
		mZipImportProperties.mFileTypes.GetStringsFormatted(lStr);
	else
		lStr = _T("All");

	if (lStr.IsEmpty())
		lStr = _T("none");

	SetDlgItemText(IDC_STATIC_FILE_TYPES, lStr);

}
void CImportZipDlg::OnBnClickedCheckPassword()
{
	UpdateControls();
}

void CImportZipDlg::OnEnChangeEditInput()
{
	UpdateControls();
}

void CImportZipDlg::OnBnClickedOk()
{
	//check the source
	m_BrowseEdit.GetWindowText(m_SourcePath);

	CFileFind lFinder;
	if (m_SourcePath.IsEmpty() || lFinder.FindFile(m_SourcePath) == FALSE)
	{
		MessageBox(_T("Invalid source path"));
		m_BrowseEdit.SetFocus();
		return;
	}

	m_Password.Empty();

	if (m_PasswordCheck.GetCheck() != 0)
		m_PasswordEdit.GetWindowText(m_Password);

	mZipImportProperties.mCollectionID = m_CollectionCombo.GetCollectionID();
	mZipImportProperties.mCollectionIDGroup = FILTERTREE_COLLECTION_MIN;

	OnOK();
}

void CImportZipDlg::OnBnClickedRadioAllFiles()
{
	UpdateControls();
}

void CImportZipDlg::OnBnClickedRadioSomeFiles()
{
	UpdateControls();
}

void CImportZipDlg::OnBnClickedButtonOptions()
{
	CEditFilePropertiesDlg lDlg(&mZipImportProperties, &mIDDataTags);
	lDlg.DoModal();
}

void CImportZipDlg::OnBnClickedButtonFileTypes()
{
	CIDData lFileIDs;
	CIncludeFileTypesDlg lDlg(&mZipImportProperties.mFileTypes);
	lDlg.DoModal();

	UpdateControls();
}
void CImportZipDlg::SetData_FromParent(PARAM_FOR_CDB *pParamCDB,int nIsForLibrary)
{
	m_nIsForLibrary = nIsForLibrary;
	m_pParamCDB = pParamCDB;
	m_CollectionCombo.SetData_FromParent(m_pParamCDB,nIsForLibrary);
}
