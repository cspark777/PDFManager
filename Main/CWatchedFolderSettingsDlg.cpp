// CWatchedFolderSettingsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Benubird.h"
#include "CWatchedFolderSettingsDlg.h"
#include "CTagsSelectionDlg.h"
#include "CCategoriesSelectionDlg.h"
#include "CIncludeFileTypesDlg.h"
#include "CBrowseFolderDlg.h"


// CWatchedFolderSettingsDlg dialog

IMPLEMENT_DYNAMIC(CWatchedFolderSettingsDlg, CDialog)

CWatchedFolderSettingsDlg::CWatchedFolderSettingsDlg(DWORD_PTR inWFolderID, CWnd* pParent /*=NULL*/)
	: CDialog(CWatchedFolderSettingsDlg::IDD, pParent)
{
	mWFolderID = inWFolderID;
}

CWatchedFolderSettingsDlg::~CWatchedFolderSettingsDlg()
{
	mBoldFont.DeleteObject();
}

void CWatchedFolderSettingsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_WF_CHOOSE_FILE_TYPES_BTN, m_ChooseFileTypeBtn);
	DDX_Control(pDX, IDC_WATCHED_SETTINGS_COLL_COMBO, mCollectionCombo);
	DDX_Control(pDX, IDC_WATCHF_INCLUDESUBFOLDERS_CHECK, m_InclSubfoldersCheckBtn);
	DDX_Control(pDX, IDC_WFOLDSET_FREQUENCY_COMBO, m_FrequencyCombo);
}


BEGIN_MESSAGE_MAP(CWatchedFolderSettingsDlg, CDialog)
	ON_BN_CLICKED(IDC_WF_EDIT_TAGS_BTN, &CWatchedFolderSettingsDlg::OnBnClickedWfEditTagsBtn)
	ON_BN_CLICKED(IDC_WF_EDIT_CATEGORIES_BTN, &CWatchedFolderSettingsDlg::OnBnClickedWfEditCategoriesBtn)
	ON_BN_CLICKED(IDC_WF_CHOOSE_FILE_TYPES_BTN, &CWatchedFolderSettingsDlg::OnBnClickedWfChooseFileTypesBtn)
	ON_BN_CLICKED(IDC_WFS_BROWSE_BTN, &CWatchedFolderSettingsDlg::OnBnClickedWfsBrowseBtn)
	ON_BN_CLICKED(IDC_WF_ALL_FILE_TYPES_RADIO, &CWatchedFolderSettingsDlg::OnBnClickedWfAllFileTypesRadio)
	ON_BN_CLICKED(IDC_WF_ONLY_THESE_FILE_TYPES_RADIO, &CWatchedFolderSettingsDlg::OnBnClickedWfOnlyTheseFileTypesRadio)
	ON_BN_CLICKED(IDOK, &CWatchedFolderSettingsDlg::OnBnClickedOk)
	ON_EN_CHANGE(IDC_EDIT7, &CWatchedFolderSettingsDlg::OnEnChangeEdit7)
	ON_BN_CLICKED(IDC_WF_TITLE_CHECK, &CWatchedFolderSettingsDlg::OnBnClickedWfTitleCheck)
	ON_BN_CLICKED(IDC_WF_AUTHOR_CHECK, &CWatchedFolderSettingsDlg::OnBnClickedWfAuthorCheck)
	ON_BN_CLICKED(IDC_WF_SUBJECT_CHECK, &CWatchedFolderSettingsDlg::OnBnClickedWfSubjectCheck)
	ON_BN_CLICKED(IDC_WF_CAT_CHECK, &CWatchedFolderSettingsDlg::OnBnClickedWfCatCheck)
	ON_BN_CLICKED(IDC_WF_TAGS_CHECK, &CWatchedFolderSettingsDlg::OnBnClickedWfTagsCheck)
	ON_BN_CLICKED(IDC_WF_COMMENTS_CHECK, &CWatchedFolderSettingsDlg::OnBnClickedWfCommentsCheck)
END_MESSAGE_MAP()


// CWatchedFolderSettingsDlg message handlers

void CWatchedFolderSettingsDlg::OnBnClickedWfEditTagsBtn()
{
	if(mWFolderID >= 0)
	{
		CTagsSelectionDlg lDlg(&mSelectedTags);
		if(IDOK == lDlg.DoModal())
		{
			CString lStr;
			mSelectedTags.GetStringsFormatted(lStr);
			SetDlgItemText(IDC_WF_TAGS_EDIT, lStr);
		}
	}
}

void CWatchedFolderSettingsDlg::OnBnClickedWfEditCategoriesBtn()
{
	CCategoriesSelectionDlg lDlg;
	lDlg.DoModal();
}

void CWatchedFolderSettingsDlg::OnBnClickedWfChooseFileTypesBtn()
{
	CString lStr;
	CIncludeFileTypesDlg lDlg(&mSelectedExtensions);
	if (lDlg.DoModal() == IDOK)
	{
		mSelectedExtensions.GetStringsFormatted(lStr);
//		lStr = lDlg.GetFileTypes();
		GetDlgItem(IDC_WFS_INCLUDE)->SetWindowText(lStr);
	}
}
void CWatchedFolderSettingsDlg::Set_ParamForCDB(PARAM_FOR_CDB *pParamCDB)
{
	m_pParamCDB = pParamCDB;
}
BOOL CWatchedFolderSettingsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	LOGFONT lf;
	GetFont()->GetLogFont(&lf);
	lf.lfWeight = FW_BOLD;
	mBoldFont.CreateFontIndirect(&lf);
	GetDlgItem(IDC_WF_LOCATION_LABEL)->SetFont(&mBoldFont);
	GetDlgItem(IDC_WF_INCLUDE_LABEL)->SetFont(&mBoldFont);
//	GetDlgItem(IDC_WF_TAGS_CHECK)->SetFont(&mBoldFont);
//	GetDlgItem(IDC_WF_CAT_CHECK)->SetFont(&mBoldFont);
//	GetDlgItem(IDC_WF_COMMENTS_CHECK)->SetFont(&mBoldFont);
	GetDlgItem(IDC_WF_COLLECTION_LABEL)->SetFont(&mBoldFont);
	GetDlgItem(IDC_WF_FREQUENCY_LABEL)->SetFont(&mBoldFont);
	
	// Set the path with the standard desktop folder by default
	// -------------------------------
	TCHAR szPath[MAX_PATH];
	if(!(SHGetSpecialFolderPath(NULL, szPath, CSIDL_DESKTOP, FALSE)))  // Desktop
	{
		throw(0);
	}
	SetDlgItemText(IDC_WFS_LOCATION, szPath);
	// -------------------------------

	CheckRadioButton(IDC_WF_ALL_FILE_TYPES_RADIO, IDC_WF_ONLY_THESE_FILE_TYPES_RADIO, IDC_WF_ALL_FILE_TYPES_RADIO);
	m_FrequencyCombo.SetCurSel(0);

	CTCollections lCol(m_pParamCDB->pDB);

	int lSelCollID = 0;
	if(mWFolderID > 0)
	{ //load the data from the DB and init the dialog
		CTWatchedFolder lTWFolder(m_pParamCDB->pDB);
		if(lTWFolder.GetWatchedFolderByID(mWFolderID))
		{
			if(lTWFolder.mDataIsValid)
			{
				SetDlgItemText(IDC_WFS_LOCATION, lTWFolder.mPath);
				lSelCollID = lTWFolder.mCollectionID;
				if(lTWFolder.mAllFileType)
				{
					CheckRadioButton(IDC_WF_ALL_FILE_TYPES_RADIO, IDC_WF_ONLY_THESE_FILE_TYPES_RADIO, IDC_WF_ALL_FILE_TYPES_RADIO);
			//		GetDlgItem(IDC_WFS_INCLUDE)->EnableWindow(FALSE);
				}
				else
				{
					CheckRadioButton(IDC_WF_ALL_FILE_TYPES_RADIO, IDC_WF_ONLY_THESE_FILE_TYPES_RADIO, IDC_WF_ONLY_THESE_FILE_TYPES_RADIO);
					GetDlgItem(IDC_WF_CHOOSE_FILE_TYPES_BTN)->EnableWindow();
			//		GetDlgItem(IDC_WFS_INCLUDE)->EnableWindow();
				}
				if(lTWFolder.mIncludeSubfolders)
					m_InclSubfoldersCheckBtn.SetCheck(BST_CHECKED);
				else
					m_InclSubfoldersCheckBtn.SetCheck(BST_UNCHECKED);
				
				m_FrequencyCombo.SetCurSel(lTWFolder.mCheckFrequency);
			
				CheckDlgButton(IDC_WF_TITLE_CHECK, lTWFolder.mUseTitle);
				CheckDlgButton(IDC_WF_AUTHOR_CHECK, lTWFolder.mUseAuthor);
				CheckDlgButton(IDC_WF_SUBJECT_CHECK, lTWFolder.mUseSubject);
				CheckDlgButton(IDC_WF_CAT_CHECK, lTWFolder.mUseCategories);
				CheckDlgButton(IDC_WF_COMMENTS_CHECK, lTWFolder.mUseComments);
				CheckDlgButton(IDC_WF_TAGS_CHECK, lTWFolder.mUseTags);

				SetDlgItemText(IDC_WF_TITLE_EDIT, lTWFolder.mTitle);
				SetDlgItemText(IDC_WF_AUTHOR_EDIT, lTWFolder.mAuthor);
				SetDlgItemText(IDC_WF_SUBJECT_EDIT, lTWFolder.mSubject);
				SetDlgItemText(IDC_WF_CAT_EDIT, lTWFolder.mCategories);
				SetDlgItemText(IDC_WF_COMMENTS_EDIT, lTWFolder.mComments);
			}
		}
		

		//listing extensions
		CTFExtensions lFExt(m_pParamCDB->pDB);
		lFExt.GetCIDDataByWFolderID(mSelectedExtensions, (int)mWFolderID);
		CString lExtensionsString = _T("");
		if(lTWFolder.mAllFileType)
			lExtensionsString.Format(IDS_WFS_ALL);
		else
			mSelectedExtensions.GetStringsFormatted(lExtensionsString);

		SetDlgItemText(IDC_WFS_INCLUDE, lExtensionsString);

		//tags listing
		CString lStr;
		CTTags lTTags(m_pParamCDB->pDB);
		lTTags.GetCIDDataByWFolderID(mSelectedTags, (int)mWFolderID);
		mSelectedTags.GetStringsFormatted(lStr);
		SetDlgItemText(IDC_WF_TAGS_EDIT, lStr);
	}

	OnBnClickedWfTitleCheck();
	OnBnClickedWfAuthorCheck();
	OnBnClickedWfSubjectCheck();
	OnBnClickedWfCatCheck();
	OnBnClickedWfTagsCheck();
	OnBnClickedWfCommentsCheck();

	int lIdx;
	lIdx = mCollectionCombo.AddString(_T(" - none - "));
	mCollectionCombo.SetItemData(lIdx, 0);
	mCollectionCombo.SetCurSel(lIdx);

	if(	lCol.GetCollectionsByNames(-2))
	{
		while(lCol.mDataIsValid)
		{
			lIdx = mCollectionCombo.AddString(lCol.mName);
			mCollectionCombo.SetItemData(lIdx, lCol.mID);
			if(lSelCollID == lCol.mID)
				mCollectionCombo.SetCurSel(lIdx);
			lCol.LoadNextRow();
		}
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CWatchedFolderSettingsDlg::OnBnClickedWfsBrowseBtn()
{
	/*CBrowseFolderDlg lDlg(FALSE);
	if (lDlg.DoModal() == IDOK)
	{
		mPath = lDlg.GetPath();
		m_LocationStaticCtrl.SetWindowText(mPath);
	}*/
	if (theApp.GetShellManager ()->BrowseForFolder(mPath, this, mPath, NULL, BIF_USENEWUI | BIF_RETURNONLYFSDIRS))
	{
		SetDlgItemText(IDC_WFS_LOCATION, mPath);
	}
}

void CWatchedFolderSettingsDlg::OnBnClickedWfAllFileTypesRadio()
{
	m_ChooseFileTypeBtn.EnableWindow(FALSE);
//	GetDlgItem(IDC_WFS_INCLUDE)->EnableWindow(FALSE);
	CString lStr;
	if(lStr.GetLength() == 0)
		lStr.Format(IDS_WFS_ALL);
	GetDlgItem(IDC_WFS_INCLUDE)->SetWindowText(lStr);
}

void CWatchedFolderSettingsDlg::OnBnClickedWfOnlyTheseFileTypesRadio()
{

	m_ChooseFileTypeBtn.EnableWindow(TRUE);
//	GetDlgItem(IDC_WFS_INCLUDE)->EnableWindow();
	CString lStr;
	mSelectedExtensions.GetStringsFormatted(lStr);
	GetDlgItem(IDC_WFS_INCLUDE)->SetWindowText(lStr);
}

void CWatchedFolderSettingsDlg::OnBnClickedOk()
{
	UpdateData();
	GetDlgItemText(IDC_WFS_LOCATION, mPath);
	if(mPath.GetLength() < 1)
		return;

	CTWatchedFolder lTWFolder(m_pParamCDB->pDB);
	lTWFolder.mID = (int)mWFolderID;
	lTWFolder.mPath = mPath;
	if(IsDlgButtonChecked(IDC_WF_ALL_FILE_TYPES_RADIO))
		lTWFolder.mAllFileType = 1;
	else
		lTWFolder.mAllFileType = 0;
	int lCurSel = mCollectionCombo.GetCurSel();
	if(lCurSel >= 0)
		lTWFolder.mCollectionID = (int)mCollectionCombo.GetItemData(lCurSel);
	else
		lTWFolder.mCollectionID = 0;

	lTWFolder.mIncludeSubfolders = (m_InclSubfoldersCheckBtn.GetCheck() == BST_CHECKED ? 1:0);

	lTWFolder.mCheckFrequency = m_FrequencyCombo.GetCurSel();

	if(lTWFolder.mCheckFrequency < 0)
		lTWFolder.mCheckFrequency = 0;
	
	lTWFolder.mUseTitle = IsDlgButtonChecked(IDC_WF_TITLE_CHECK);
	lTWFolder.mUseAuthor = IsDlgButtonChecked(IDC_WF_AUTHOR_CHECK); 
	lTWFolder.mUseSubject = IsDlgButtonChecked(IDC_WF_SUBJECT_CHECK); 
	lTWFolder.mUseCategories = IsDlgButtonChecked(IDC_WF_CAT_CHECK);
	lTWFolder.mUseComments = IsDlgButtonChecked(IDC_WF_COMMENTS_CHECK);
	lTWFolder.mUseTags = IsDlgButtonChecked(IDC_WF_TAGS_CHECK);
	CString lTmp;
	GetDlgItemText(IDC_WF_TITLE_EDIT, lTmp);
	lTWFolder.mTitle = lTmp;
	GetDlgItemText(IDC_WF_AUTHOR_EDIT, lTmp);
	lTWFolder.mAuthor = lTmp;
	GetDlgItemText(IDC_WF_SUBJECT_EDIT, lTmp);
	lTWFolder.mSubject = lTmp; 
	GetDlgItemText(IDC_WF_CAT_EDIT, lTmp);
	lTWFolder.mCategories = lTmp;
	GetDlgItemText(IDC_WF_COMMENTS_EDIT, lTmp);
	lTWFolder.mComments = lTmp;

	lTWFolder.AddNewWatchedFolder(&mSelectedExtensions.mIDs, &mSelectedTags.mIDs);
	OnOK();
}

void CWatchedFolderSettingsDlg::OnEnChangeEdit7()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}

void CWatchedFolderSettingsDlg::OnBnClickedWfTitleCheck()
{
	GetDlgItem(IDC_WF_TITLE_EDIT)->EnableWindow( IsDlgButtonChecked(IDC_WF_TITLE_CHECK));
}

void CWatchedFolderSettingsDlg::OnBnClickedWfAuthorCheck()
{
	GetDlgItem(IDC_WF_AUTHOR_EDIT)->EnableWindow( IsDlgButtonChecked(IDC_WF_AUTHOR_CHECK));
}

void CWatchedFolderSettingsDlg::OnBnClickedWfSubjectCheck()
{
	GetDlgItem(IDC_WF_SUBJECT_EDIT)->EnableWindow( IsDlgButtonChecked(IDC_WF_SUBJECT_CHECK));
}

void CWatchedFolderSettingsDlg::OnBnClickedWfCatCheck()
{
	GetDlgItem(IDC_WF_CAT_EDIT)->EnableWindow( IsDlgButtonChecked(IDC_WF_CAT_CHECK));
}

void CWatchedFolderSettingsDlg::OnBnClickedWfTagsCheck()
{
	GetDlgItem(IDC_WF_TAGS_EDIT)->EnableWindow( IsDlgButtonChecked(IDC_WF_TAGS_CHECK));
	GetDlgItem(IDC_WF_EDIT_TAGS_BTN)->EnableWindow( IsDlgButtonChecked(IDC_WF_TAGS_CHECK));
}

void CWatchedFolderSettingsDlg::OnBnClickedWfCommentsCheck()
{
	GetDlgItem(IDC_WF_COMMENTS_EDIT)->EnableWindow( IsDlgButtonChecked(IDC_WF_COMMENTS_CHECK));
}
