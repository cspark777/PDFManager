// CEditFilePropertiesDlg.cpp : implementation file
//



#include "stdafx.h"
#include "Benubird.h"
#include "CEditFilePropertiesDlg.h"
#include "CTagsSelectionDlg.h"


// CEditFilePropertiesDlg dialog

IMPLEMENT_DYNAMIC(CEditFilePropertiesDlg, CDialog)

CEditFilePropertiesDlg::CEditFilePropertiesDlg(CFileProperties_Entry * inFileInfo, CIDData* inIDDataTags, CWnd* pParent /*=NULL*/)
	: CDialog(CEditFilePropertiesDlg::IDD, pParent)
{
	mFileInfo = inFileInfo;
	mIDDataTags     = inIDDataTags;

	mOwnUserInfo = false;
	if (mFileInfo == NULL)
	{
		mFileInfo = new CFileProperties_Entry;
		mOwnUserInfo = true;
	}
}

CEditFilePropertiesDlg::~CEditFilePropertiesDlg()
{
	if (mOwnUserInfo && mFileInfo)
		delete mFileInfo;
}

void CEditFilePropertiesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CEditFilePropertiesDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CEditFilePropertiesDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_WF_EDIT_TAGS_BTN, &CEditFilePropertiesDlg::OnBnClickedWfEditTagsBtn)
	ON_BN_CLICKED(IDC_WF_TITLE_CHECK, &CEditFilePropertiesDlg::OnBnClickedWfTitleCheck)
	ON_BN_CLICKED(IDC_WF_AUTHOR_CHECK, &CEditFilePropertiesDlg::OnBnClickedWfAuthorCheck)
	ON_BN_CLICKED(IDC_WF_SUBJECT_CHECK, &CEditFilePropertiesDlg::OnBnClickedWfSubjectCheck)
	ON_BN_CLICKED(IDC_WF_CAT_CHECK, &CEditFilePropertiesDlg::OnBnClickedWfCatCheck)
	ON_BN_CLICKED(IDC_WF_TAGS_CHECK, &CEditFilePropertiesDlg::OnBnClickedWfTagsCheck)
	ON_BN_CLICKED(IDC_WF_COMMENTS_CHECK, &CEditFilePropertiesDlg::OnBnClickedWfCommentsCheck)
	ON_BN_CLICKED(IDCANCEL, &CEditFilePropertiesDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CEditFilePropertiesDlg message handlers

BOOL CEditFilePropertiesDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	//update the file info
	//load data from file info

	for (int i = 0; i < mIDDataTags->GetCount(); i++)
	{
		int lID;
		CString lStr;
		mIDDataTags->GetDataByIdx(i, lID, lStr);
		mIDDataTagsOrig.AddData(lID, lStr);
	}

	CheckDlgButton(IDC_WF_TITLE_CHECK, mFileInfo->mUseTitle);
	SetDlgItemText(IDC_WF_TITLE_EDIT, mFileInfo->mTitle);
	OnBnClickedWfTitleCheck();

	CheckDlgButton(IDC_WF_AUTHOR_CHECK, mFileInfo->mUseAuthor);
	SetDlgItemText(IDC_WF_AUTHOR_EDIT, mFileInfo->mAuthor);
	OnBnClickedWfAuthorCheck();

	CheckDlgButton(IDC_WF_SUBJECT_CHECK, mFileInfo->mUseSubject);
	SetDlgItemText(IDC_WF_SUBJECT_EDIT, mFileInfo->mSubject);
	OnBnClickedWfSubjectCheck();

	CheckDlgButton(IDC_WF_CAT_CHECK, mFileInfo->mUseCategories);
	SetDlgItemText(IDC_WF_CAT_EDIT, mFileInfo->mCategories);
	OnBnClickedWfCatCheck();

	CheckDlgButton(IDC_WF_COMMENTS_CHECK, mFileInfo->mUseComments);
	SetDlgItemText(IDC_WF_COMMENTS_EDIT, mFileInfo->mComments);
	OnBnClickedWfCommentsCheck();

	CheckDlgButton(IDC_WF_TAGS_CHECK, mFileInfo->mUseTags);
	SetDlgItemText(IDC_WF_TAGS_EDIT, mFileInfo->mTags);
	OnBnClickedWfTagsCheck();
/*
	CString lStr;
	mFileInfo->mTags.GetStringsFormatted(lStr);
	SetDlgItemText(IDC_WF_TAGS_EDIT, lStr);
*/
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CEditFilePropertiesDlg::OnBnClickedOk()
{
	if (mFileInfo->mUseTitle = IsDlgButtonChecked(IDC_WF_TITLE_CHECK))
		GetDlgItemText(IDC_WF_TITLE_EDIT, mFileInfo->mTitle);
	if (mFileInfo->mUseAuthor = IsDlgButtonChecked(IDC_WF_AUTHOR_CHECK))
		GetDlgItemText(IDC_WF_AUTHOR_EDIT, mFileInfo->mAuthor);
	if (mFileInfo->mUseSubject = IsDlgButtonChecked(IDC_WF_SUBJECT_CHECK))
		GetDlgItemText(IDC_WF_SUBJECT_EDIT, mFileInfo->mSubject);
	if (mFileInfo->mUseCategories = IsDlgButtonChecked(IDC_WF_CAT_CHECK))
		GetDlgItemText(IDC_WF_CAT_EDIT, mFileInfo->mCategories);
	if (mFileInfo->mUseComments = IsDlgButtonChecked(IDC_WF_COMMENTS_CHECK))
		GetDlgItemText(IDC_WF_COMMENTS_EDIT, mFileInfo->mComments);
	if (mFileInfo->mUseTags = IsDlgButtonChecked(IDC_WF_TAGS_CHECK))
		GetDlgItemText(IDC_WF_TAGS_EDIT, mFileInfo->mTags);

	OnOK();
}

void CEditFilePropertiesDlg::OnBnClickedWfEditTagsBtn()
{
	CTagsSelectionDlg lDlg(mIDDataTags);
	if(IDOK == lDlg.DoModal())
	{
		CString lStr;
		mIDDataTags->GetStringsFormatted(lStr);
		SetDlgItemText(IDC_WF_TAGS_EDIT, lStr);
	}
}

void CEditFilePropertiesDlg::OnBnClickedWfTitleCheck()
{
	GetDlgItem(IDC_WF_TITLE_EDIT)->EnableWindow(IsDlgButtonChecked(IDC_WF_TITLE_CHECK));
}

void CEditFilePropertiesDlg::OnBnClickedWfAuthorCheck()
{
	GetDlgItem(IDC_WF_AUTHOR_EDIT)->EnableWindow(IsDlgButtonChecked(IDC_WF_AUTHOR_CHECK));
}

void CEditFilePropertiesDlg::OnBnClickedWfSubjectCheck()
{
	GetDlgItem(IDC_WF_SUBJECT_EDIT)->EnableWindow(IsDlgButtonChecked(IDC_WF_SUBJECT_CHECK));
}

void CEditFilePropertiesDlg::OnBnClickedWfCatCheck()
{
	GetDlgItem(IDC_WF_CAT_EDIT)->EnableWindow(IsDlgButtonChecked(IDC_WF_CAT_CHECK));
}

void CEditFilePropertiesDlg::OnBnClickedWfTagsCheck()
{
	GetDlgItem(IDC_WF_TAGS_EDIT)->EnableWindow(IsDlgButtonChecked(IDC_WF_TAGS_CHECK));
	GetDlgItem(IDC_WF_EDIT_TAGS_BTN)->EnableWindow(IsDlgButtonChecked(IDC_WF_TAGS_CHECK));
}

void CEditFilePropertiesDlg::OnBnClickedWfCommentsCheck()
{
	GetDlgItem(IDC_WF_COMMENTS_EDIT)->EnableWindow(IsDlgButtonChecked(IDC_WF_COMMENTS_CHECK));
}

void CEditFilePropertiesDlg::OnBnClickedCancel()
{
	mIDDataTags->RemoveAll();
	for (int i = 0; i < mIDDataTagsOrig.GetCount(); i++)
	{
		int lID;
		CString lStr;
		mIDDataTagsOrig.GetDataByIdx(i, lID, lStr);
		mIDDataTags->AddData(lID, lStr);
	}
	OnCancel();
}
