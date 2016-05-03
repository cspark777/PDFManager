// CGetInfoInfoTabDlg.cpp: implementation file

#include "stdafx.h"
#include "CGetInfoInfoTabDlg.h"
#include "CBBTabPanel.h"
#include "CGetInfoDlg.h"
#include "CCategoriesSelectionDlg.h"
#include "CTagsSelectionDlg.h"
#include "MainFrm.h"
#include "CPDFPropertiesFrame.h"
#include "CGetInfoPDFTabDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// Messages
BEGIN_MESSAGE_MAP(CGetInfoInfoTabDlg, CBBTabPanel)

	ON_BN_CLICKED(IDC_INFO_EDIT_CATEGORIES_BTN, &CGetInfoInfoTabDlg::OnBnClickedInfoEditCategoriesBtn)
	ON_BN_CLICKED(IDC_INFO_EDIT_TAGS_BTN, &CGetInfoInfoTabDlg::OnBnClickedInfoEditTagsBtn)
	ON_BN_CLICKED(IDC_INF_ADDITIONAL_METADATA_BTN, &CGetInfoInfoTabDlg::OnBnClickedInfAdditionalMetadataBtn)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
// Constructor
CGetInfoInfoTabDlg::CGetInfoInfoTabDlg(CGetInfoData *inSelFilesData, CWnd* pParent /*=NULL*/)
: CBBTabPanel(pParent)
{
	mSelFilesData = inSelFilesData;
	//{{AFX_DATA_INIT(CGetInfoInfoTabDlg)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

//////////////////////////////////////////////////////////////////////////
// Destructor
CGetInfoInfoTabDlg::~CGetInfoInfoTabDlg()
{
	mFileIdx = -1;
}

//////////////////////////////////////////////////////////////////////////
// Data exchange
void CGetInfoInfoTabDlg::DoDataExchange(CDataExchange* pDX)
{
	CBBTabPanel::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGetInfoInfoTabDlg)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

//////////////////////////////////////////////////////////////////////////
// CGetInfoInfoTabDlg message handlers
bool CGetInfoInfoTabDlg::ValidateAndProcess()
{
// If the files is a PDF then overwrite with PDF metadata prior to updating -- KDA 11 Feb, 2009
// -----------------------------------------------------------------------------------------
	if(((CGetInfoDlg*)GetParent())->IsPDF()==true)
	{
// Extract the PDF metadata from the PDF tab in the Info tabbed dialog
		CGetInfoPDFTabDlg* pPDFPanel = (CGetInfoPDFTabDlg*) ((CGetInfoDlg*)GetParent())->GetPDFPanel();
		CPDFPropertiesFrame* pPDFPropertiesFrame = (CPDFPropertiesFrame*) pPDFPanel->GetPDFPropertiesFrame();

		CString strTitle, strAuthor, strSubject, strKeywords;
		pPDFPropertiesFrame->GetDlgItemText(IDC_TITLE_EDIT, strTitle);
		pPDFPropertiesFrame->GetDlgItemText(IDC_AUTHOR_EDIT, strAuthor);
		pPDFPropertiesFrame->GetDlgItemText(IDC_SUBJECT_EDIT, strSubject);
		pPDFPropertiesFrame->GetDlgItemText(IDC_TAGS_EDIT, strKeywords);

// Prefill the dialog with PDF metadata so processing can continue as normal
		GetDlgItem(IDC_INFO_TITLE_EDIT)->SetWindowText(strTitle);
		GetDlgItem(IDC_INFO_AUTHOR_EDIT)->SetWindowText(strAuthor);
		GetDlgItem(IDC_INFO_SUBJECT_EDIT)->SetWindowText(strSubject);
		GetDlgItem(IDC_INFO_TAGS_EDIT)->SetWindowText(strKeywords);
	}
// -----------------------------------------------------------------------------------------

	CString lStr;
	CTFiles *lTFiles = mSelFilesData->GetDataTFiles(mFileIdx);
	GetDlgItemText(IDC_INFO_TITLE_EDIT, lStr);
	if(lStr != lTFiles->mTitle)
	{
		lTFiles->mTitle = lStr;
		lTFiles->mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_TITLE] = true;
	}

	GetDlgItemText(IDC_INFO_AUTHOR_EDIT, lStr);
	if(lStr != lTFiles->mAuthor)
	{
		lTFiles->mAuthor = lStr;
		lTFiles->mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_AUTHOR] = true;
	}

	GetDlgItemText(IDC_INFO_CATEGORIES_EDIT, lStr);
	if(lStr != lTFiles->mCategory)
	{
		lTFiles->mCategory = lStr;
		lTFiles->mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_CATEGORY] = true;
	}

	GetDlgItemText(IDC_INFO_SUBJECT_EDIT, lStr);
	if(lStr != lTFiles->mSubject)
	{
		lTFiles->mSubject = lStr;
		lTFiles->mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_SUBJECT] = true;
	}
	/*
	GetDlgItemText(IDC_INFO_TAGS_EDIT, lStr);
	if(lStr != lTFiles->mKeywords)
	{
		lTFiles->mKeywords = lStr;
		lTFiles->mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_KEYWORDS] = true;
	}
	*/
	GetDlgItemText(IDC_INFO_COMMENTS_EDIT, lStr);
	if(lStr != lTFiles->mComments)
	{
		lTFiles->mComments = lStr;
		lTFiles->mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_COMMENTS] = true;
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////
// Initialization
BOOL CGetInfoInfoTabDlg::OnInitDialog() 
{
	CBBTabPanel::OnInitDialog();
	Update(((CGetInfoDlg*)GetParent())->GetItem());
	
	return TRUE;
}

// Cancel
void CGetInfoInfoTabDlg::OnCancel() 
{
	((CGetInfoDlg*)GetParent())->OnCancel();
}

BOOL CGetInfoInfoTabDlg::PreTranslateMessage(MSG* pMsg)
{
	return CBBTabPanel::PreTranslateMessage(pMsg);
}



void CGetInfoInfoTabDlg::OnBnClickedInfoEditCategoriesBtn()
{
	CCategoriesSelectionDlg lDlg;
	lDlg.DoModal();
}

void CGetInfoInfoTabDlg::OnBnClickedInfoEditTagsBtn()
{
	if(mFileIdx >= 0)
	{
	//	mSelFilesData->mIDDataTags[mFileIdx]
		CTagsSelectionDlg lDlg(&(mSelFilesData->mIDDataTags[mFileIdx]));
		if(lDlg.DoModal() == IDOK)
		{
			CIDData *lTagsIDData = mSelFilesData->GetDataIDDataTags(mFileIdx);
			CString lStr = _T("");	
			lTagsIDData->GetStringsFormatted(lStr);
			GetDlgItem(IDC_INFO_TAGS_EDIT)->SetWindowText(lStr);
		}
	}
}

void CGetInfoInfoTabDlg::OnBnClickedInfAdditionalMetadataBtn()
{
	AfxMessageBox(_T("Show additional metadata dialog."));
}

void CGetInfoInfoTabDlg::Update(int inFileIdx)
{
	CString lStr;
	CString lAuthor;
	CString lTitle;
	CString lSubject;
	
	CString lFilename;

	mFileIdx	= inFileIdx;

	CTFiles *lTFiles = mSelFilesData->GetDataTFiles(inFileIdx);
	if (lTFiles->mDataIsValid)	
	{
		CString lStr;
		lStr.Format(_T("%s"), lTFiles->mPath);

		lFilename = lStr.Right(lStr.GetLength() - lStr.ReverseFind('\\') - 1);
		GetDlgItem(IDC_INFO_FILE_STATIC)->SetWindowText(lTFiles->mName);

		lTitle.Format(_T("%s"), lTFiles->mTitle);
		GetDlgItem(IDC_INFO_TITLE_EDIT)->SetWindowText(lTitle);

		lAuthor.Format(_T("%s"), lTFiles->mAuthor);
		GetDlgItem(IDC_INFO_AUTHOR_EDIT)->SetWindowText(lAuthor);

		lSubject.Format(_T("%s"), lTFiles->mSubject);
		GetDlgItem(IDC_INFO_SUBJECT_EDIT)->SetWindowText(lSubject);

		SetDlgItemText(IDC_INFO_CATEGORIES_EDIT, lTFiles->mCategory);
		SetDlgItemText(IDC_INFO_COMMENTS_EDIT, lTFiles->mComments);
	}

// If the file is a PDF then prevent the user from editing the Windows metadata - KDA 11 Feb, 2009
	if(((CGetInfoDlg*)GetParent())->IsPDF()==true)
	{
		GetDlgItem(IDC_INFO_TITLE_EDIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_INFO_AUTHOR_EDIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_INFO_SUBJECT_EDIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_INFO_TAGS_EDIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_INFO_EDIT_TAGS_BTN)->EnableWindow(FALSE);
		GetDlgItem(IDC_PDF_INFO_TEXT)->ShowWindow(TRUE);
	}
	
	CIDData *lTagsIDData = mSelFilesData->GetDataIDDataTags(inFileIdx);
	lStr = _T("");	
	lTagsIDData->GetStringsFormatted(lStr);
	GetDlgItem(IDC_INFO_TAGS_EDIT)->SetWindowText(lStr);

//	SetDlgItemText(IDC_INFO_CATEGORIES_EDIT, _T("Not working"));
	CheckDlgButton(IDC_GETINFO_INFO_APPLY_METADATA_CHECK, TRUE);
}