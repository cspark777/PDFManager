// CMultipleProfileInformationDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Benubird.h"
#include "CMultipleProfileInformationDlg.h"
#include "CTagsSelectionDlg.h"
#include "MainFrm.h"
#include "LibraryView.h"

// CMultipleProfileInformationDlg dialog

IMPLEMENT_DYNAMIC(CMultipleProfileInformationDlg, CDialog)

CMultipleProfileInformationDlg::CMultipleProfileInformationDlg(CIntArray *inFileIDsArray, CWnd* pParent /*=NULL*/)
	: CDialog(CMultipleProfileInformationDlg::IDD, pParent)
{
	mFileIDs = inFileIDsArray;
}


CMultipleProfileInformationDlg::~CMultipleProfileInformationDlg()
{
}

void CMultipleProfileInformationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMultipleProfileInformationDlg, CDialog)
	ON_BN_CLICKED(IDC_MPI_TITLE_CHECK, &CMultipleProfileInformationDlg::OnBnClickedMpiTitleCheck)
	ON_BN_CLICKED(IDC_MPI_AUTHOR_CHECK, &CMultipleProfileInformationDlg::OnBnClickedMpiAuthorCheck)
	ON_BN_CLICKED(IDC_MPI_SUBJECT_CHECK, &CMultipleProfileInformationDlg::OnBnClickedMpiSubjectCheck)
	ON_BN_CLICKED(IDC_MPI_CATEGORY_CHECK, &CMultipleProfileInformationDlg::OnBnClickedMpiCategoryCheck)
	ON_BN_CLICKED(IDC_MPI_TAGS_CHECK, &CMultipleProfileInformationDlg::OnBnClickedMpiTagsCheck)
	ON_BN_CLICKED(IDC_MPI_EDIT_TAGS_BTN, &CMultipleProfileInformationDlg::OnBnClickedMpiEditTagsBtn)
	ON_BN_CLICKED(IDOK, &CMultipleProfileInformationDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_MPI_COMMENTS_CHECK, &CMultipleProfileInformationDlg::OnBnClickedMpiCommentsCheck)
END_MESSAGE_MAP()


// CMultipleProfileInformationDlg message handlers

void CMultipleProfileInformationDlg::OnBnClickedMpiTitleCheck()
{
	GetDlgItem(IDC_MPI_TITLE_EDIT)->EnableWindow( IsDlgButtonChecked(IDC_MPI_TITLE_CHECK));
}

void CMultipleProfileInformationDlg::OnBnClickedMpiAuthorCheck()
{
	GetDlgItem(IDC_MPI_AUTHOR_EDIT)->EnableWindow( IsDlgButtonChecked(IDC_MPI_AUTHOR_CHECK));
}

void CMultipleProfileInformationDlg::OnBnClickedMpiSubjectCheck()
{
	GetDlgItem(IDC_MPI_SUBJECT_EDIT)->EnableWindow( IsDlgButtonChecked(IDC_MPI_SUBJECT_CHECK));
}

void CMultipleProfileInformationDlg::OnBnClickedMpiCategoryCheck()
{
	GetDlgItem(IDC_MPI_CATEGORY_EDIT)->EnableWindow( IsDlgButtonChecked(IDC_MPI_CATEGORY_CHECK));
}

void CMultipleProfileInformationDlg::OnBnClickedMpiTagsCheck()
{
	GetDlgItem(IDC_MPI_TAGS_EDIT)->EnableWindow( IsDlgButtonChecked(IDC_MPI_TAGS_CHECK));
	GetDlgItem(IDC_MPI_EDIT_TAGS_BTN)->EnableWindow( IsDlgButtonChecked(IDC_MPI_TAGS_CHECK));
}

void CMultipleProfileInformationDlg::OnBnClickedMpiEditTagsBtn()
{
	CTagsSelectionDlg lDlg(&mIDDataTags);
	if(IDOK == lDlg.DoModal())
	{
		CString lStr;
		mIDDataTags.GetStringsFormatted(lStr);
		SetDlgItemText(IDC_MPI_TAGS_EDIT, lStr);
	}
}

void CMultipleProfileInformationDlg::OnBnClickedOk()
{
	bool lUpdateTitle = IsDlgButtonChecked(IDC_MPI_TITLE_CHECK);
	bool lUpdateAuthor = IsDlgButtonChecked(IDC_MPI_AUTHOR_CHECK);
	bool lUpdateSubject = IsDlgButtonChecked(IDC_MPI_SUBJECT_CHECK);
	bool lUpdateCategory = IsDlgButtonChecked(IDC_MPI_CATEGORY_CHECK);
	bool lUpdateTags = IsDlgButtonChecked(IDC_MPI_TAGS_CHECK);
	bool lUpdateComments = IsDlgButtonChecked(IDC_MPI_COMMENTS_CHECK);
	if( !(lUpdateTitle || lUpdateAuthor || lUpdateSubject || lUpdateCategory || lUpdateTags || lUpdateComments))
	{
		OnOK();
		return;
	}

	int lNumFiles = mFileIDs->GetCount();
	CString lTitleStr = _T(""), lAuthorStr = _T(""), lSubjectStr = _T(""), lCategoryStr = _T(""), lTagsStr = _T(""), lCommentsStr = _T("");
	GetDlgItemText(IDC_MPI_TITLE_EDIT, lTitleStr);
	GetDlgItemText(IDC_MPI_AUTHOR_EDIT, lAuthorStr);
	GetDlgItemText(IDC_MPI_SUBJECT_EDIT, lSubjectStr);
	GetDlgItemText(IDC_MPI_CATEGORY_EDIT, lCategoryStr);	
	GetDlgItemText(IDC_MPI_COMMENTS_EDIT, lCommentsStr);

	int lNumChangedFiles = 0;
	int lFileID;
	CTFiles lTFiles(m_pParamCDB);
	for(int i = 0; i<lNumFiles; i++)
	{
		lFileID = mFileIDs->GetAt(i);
		lTFiles.GetFileByID(lFileID);
		if(lUpdateTitle == true)
		{
			lTFiles.mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_TITLE] = true;
			lTFiles.mTitle = lTitleStr;
		}
		else
			lTFiles.mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_TITLE] = false;

		if(lUpdateAuthor == true)
		{
			lTFiles.mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_AUTHOR] = true;
			lTFiles.mAuthor = lAuthorStr;
		}
		else
			lTFiles.mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_AUTHOR] = false;

		if(lUpdateSubject == true)
		{
			lTFiles.mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_SUBJECT] = true;
			lTFiles.mSubject = lSubjectStr;
		}
		else
			lTFiles.mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_SUBJECT] = false;

		if(lUpdateCategory == true)
		{
			lTFiles.mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_CATEGORY] = true;
			lTFiles.mCategory = lCategoryStr;
		}
		else
			lTFiles.mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_CATEGORY] = false;

		if(lUpdateTags == true)
		{
			m_pDB->DeAssociateTagsFromFile(lFileID);
			int lCount =  mIDDataTags.GetCount();
			for(int j = 0; j<lCount; j++)
			{
				m_pDB->AddTagToFile(mIDDataTags.mIDs.GetAt(j), lFileID);
			}
			//lTFiles.AddTagsFromKeywords(iFileID, 
		}

		if(lUpdateComments == true)
		{
			lTFiles.mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_COMMENTS] = true;
			lTFiles.mComments = lCommentsStr;
		}
		else
			lTFiles.mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_COMMENTS] = false;

		lTFiles.UpdateTablebyID();

		// If we're using the direct linking, then update the database when any file changes its data
		// ----------------------------------------------
		if(!(m_pParamCDB->nIsUseRepository == 1))
		{
			lTFiles.UpdateFileOnDiskFromMembers();
		}
		// ----------------------------------------------

//		lTFiles.UpdateFileFromDisk(true); // don't need to refresh files due to final Refresh()

		lNumChangedFiles++;
	}

	if(lNumChangedFiles > 0)
	{
//?		((CMainFrame*)AfxGetApp()->m_pMainWnd)->Refresh();
	}

	OnOK();
}

BOOL CMultipleProfileInformationDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	//
	CTFiles lTFiles(m_pParamCDB);
	int lNumFiles = mFileIDs->GetCount();
	int lFileID;
	bool lSemafor = true;
	CString lTitleStr = _T(""), lAuthorStr = _T(""), lSubjectStr = _T(""), lCategoryStr = _T(""), lTagsStr = _T(""), lCommentsStr = _T("");
	CTTags lTTags(m_pParamCDB->pDB);
	CIDData lIDDataTags;
	for(int i = 0; i<lNumFiles; i++)
	{
		lFileID = mFileIDs->GetAt(i);
		if(	lTFiles.GetFileByID(lFileID) && lTFiles.mDataIsValid)
		{
			if(lSemafor)
			{
				lSemafor = false;
				lTitleStr = lTFiles.mTitle;
				lAuthorStr = lTFiles.mAuthor;
				lSubjectStr = lTFiles.mSubject;
				lCategoryStr = lTFiles.mCategory;
				lCommentsStr = lTFiles.mComments;
			}
			else
			{
				if(lTitleStr.Compare(lTFiles.mTitle) != 0)
					lTitleStr = _T("");
				if(lAuthorStr.Compare(lTFiles.mAuthor) != 0)
					lAuthorStr = _T("");
				if(lSubjectStr.Compare(lTFiles.mSubject) != 0)
					lSubjectStr = _T("");
				if(lCategoryStr.Compare(lTFiles.mCategory) != 0)
					lCategoryStr = _T("");
				if(lCommentsStr.Compare(lTFiles.mComments) != 0)
					lCommentsStr = _T("");
			}
		}

		// find tags now
		if(i == 0)
			lTTags.GetCIDDataByFileID(mIDDataTags, lFileID);
		else if(mIDDataTags.GetCount()>0)
		{
			lTTags.GetCIDDataByFileID(lIDDataTags, lFileID);
			if(! mIDDataTags.HasEqualIDs(lIDDataTags))
				mIDDataTags.RemoveAll();
		}	
	}
	SetDlgItemText(IDC_MPI_TITLE_EDIT, lTitleStr);
	SetDlgItemText(IDC_MPI_AUTHOR_EDIT, lAuthorStr);
	SetDlgItemText(IDC_MPI_SUBJECT_EDIT, lSubjectStr);
	SetDlgItemText(IDC_MPI_CATEGORY_EDIT, lCategoryStr);
	mIDDataTags.GetStringsFormatted(lTagsStr);
	SetDlgItemText(IDC_MPI_TAGS_EDIT, lTagsStr);
	SetDlgItemText(IDC_MPI_COMMENTS_EDIT, lCommentsStr);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CMultipleProfileInformationDlg::OnBnClickedMpiCommentsCheck()
{
	GetDlgItem(IDC_MPI_COMMENTS_EDIT)->EnableWindow( IsDlgButtonChecked(IDC_MPI_COMMENTS_CHECK));
}

void CMultipleProfileInformationDlg::Set_ParamForCDB(PARAM_FOR_CDB *pParamCDB)
{
	m_pParamCDB = pParamCDB;
	m_pDB = m_pParamCDB->pDB;
}