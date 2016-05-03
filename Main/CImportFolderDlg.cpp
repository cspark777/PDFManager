// CImportFolderDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Benubird.h"
#include "CImportFolderDlg.h"

#include "CIncludeFileTypesDlg.h"
#include "CEditFilePropertiesDlg.h"
#include "COptionsDefs.h"
#include "MainFrm.h"
#include "LibraryView.h"

extern CMainFrame* gMainFrm;

// CImportFolderDlg dialog

IMPLEMENT_DYNAMIC(CImportFolderDlg, CDialog)

CImportFolderDlg::CImportFolderDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CImportFolderDlg::IDD, pParent)
{
	mIncludeSubfolders = false;
	m_nIsForLibrary = 1;
}

CImportFolderDlg::~CImportFolderDlg()
{
}

void CImportFolderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_INPUT, m_BrowseEdit);
	DDX_Control(pDX, IDC_COLLECTION_COMBO, m_CollectionCombo);
}


BEGIN_MESSAGE_MAP(CImportFolderDlg, CDialog)
	ON_EN_CHANGE(IDC_EDIT_INPUT, &CImportFolderDlg::OnEnChangeEditInput)
	ON_BN_CLICKED(IDOK, &CImportFolderDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_RADIO_ALL_FILES, &CImportFolderDlg::OnBnClickedRadioAllFiles)
	ON_BN_CLICKED(IDC_RADIO_SOME_FILES, &CImportFolderDlg::OnBnClickedRadioSomeFiles)
	ON_BN_CLICKED(IDC_BUTTON_OPTIONS, &CImportFolderDlg::OnBnClickedButtonOptions)
	ON_BN_CLICKED(IDC_BUTTON_FILE_TYPES, &CImportFolderDlg::OnBnClickedButtonFileTypes)
END_MESSAGE_MAP()


// CImportFolderDlg message handlers

BOOL CImportFolderDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_BrowseEdit.EnableFolderBrowseButton();

	m_BrowseEdit.SetWindowText(mPath);

	((CButton*)GetDlgItem(IDC_RADIO_ALL_FILES))->SetCheck(mFolderImportProperties.mFileTypes.GetCount() == 0);
	((CButton*)GetDlgItem(IDC_RADIO_SOME_FILES))->SetCheck(mFolderImportProperties.mFileTypes.GetCount() != 0);
	((CButton*)GetDlgItem(IDC_CHECK_INCLUDE_SUBFOLDERS))->SetCheck(mIncludeSubfolders);

//	((CComboBox*)GetDlgItem(IDC_COMBO_FILE_OPERATION))->SetCurSel(mFolderImportProperties.mFileOperation);

	m_CollectionCombo.AddString(_T("-"));
	CMDIChildWnd * pChild = ((CMDIFrameWnd*)(AfxGetApp()->m_pMainWnd))->MDIGetActive();
	m_CollectionCombo.Initialize(((CLibraryView*)pChild->GetActiveView ())->GetSelectedCollID(), CCollectionsComboBox::includeCollections);

	UpdateControls();

	return TRUE;
}

void CImportFolderDlg::UpdateControls()
{
	CString lSourcePath;
	m_BrowseEdit.GetWindowText(lSourcePath);

	BOOL lEnable = !lSourcePath.IsEmpty();
	GetDlgItem(IDOK)->EnableWindow(lEnable);

	lEnable = IsDlgButtonChecked(IDC_RADIO_SOME_FILES);
	GetDlgItem(IDC_BUTTON_FILE_TYPES)->EnableWindow(lEnable);

	CString lStr;

	if (lEnable)
		mFolderImportProperties.mFileTypes.GetStringsFormatted(lStr, _T("; "));
	else
		lStr = _T("All");

	if (lStr.IsEmpty())
		lStr = _T("none");

	SetDlgItemText(IDC_STATIC_FILE_TYPES, lStr);

}

void CImportFolderDlg::OnEnChangeEditInput()
{
	UpdateControls();
}

void CImportFolderDlg::OnBnClickedOk()
{
	//check the source
	CString lPath;
	m_BrowseEdit.GetWindowText(lPath);

	CFileFind lFinder;
	if (lPath.IsEmpty() || lFinder.FindFile(lPath) == FALSE)
	{
		MessageBox(_T("Invalid source path"));
		m_BrowseEdit.SetFocus();
		return;
	}
	
	mPath = lPath;
	mIncludeSubfolders = (IsDlgButtonChecked(IDC_CHECK_INCLUDE_SUBFOLDERS) == TRUE);
	mFolderImportProperties.mCollectionID = m_CollectionCombo.GetCollectionID();
	mFolderImportProperties.mCollectionIDGroup = FILTERTREE_COLLECTION_MIN;

	OnOK();
}

void CImportFolderDlg::OnBnClickedRadioAllFiles()
{
	UpdateControls();
}

void CImportFolderDlg::OnBnClickedRadioSomeFiles()
{
	UpdateControls();
}

void CImportFolderDlg::OnBnClickedButtonOptions()
{
	CEditFilePropertiesDlg lDlg(&mFolderImportProperties, &mIDDataTags);
	lDlg.DoModal();
}

void CImportFolderDlg::OnBnClickedButtonFileTypes()
{
	CIDData lFileIDs;
	CIncludeFileTypesDlg lDlg(&mFolderImportProperties.mFileTypes);
	lDlg.DoModal();

	UpdateControls();

}

void CImportFolderDlg::SetData_FromParent(PARAM_FOR_CDB *pParamCDB,int nIsForLibrary)
{
	m_pParamCDB = pParamCDB;
	m_nIsForLibrary = nIsForLibrary;
	m_CollectionCombo.SetData_FromParent(m_pParamCDB,m_nIsForLibrary);
}
