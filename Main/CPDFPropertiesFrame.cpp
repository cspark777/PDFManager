#include "stdafx.h"

#include "Define.h"
#include "Benubird.h"
#include "CPDFPropertiesFrame.h"
#include "COptionsDefs.h"
#include "CRegistry.h"
#include "MainFrm.h"
#include "DebenuPDFLibraryAX0912.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CPDFPropertiesFrame, CDialog)

/////////////////////////////////////////////////////////////////////////
// static  
BOOL CPDFPropertiesFrame::LoadPDFMetadata(CString inFile, PDF_Entry* outEntry)
{
	outEntry->mTitle = _T("");
	outEntry->mAuthor = _T("");
	outEntry->mSubject = _T("");
	outEntry->mKeywords = _T("");
	outEntry->mFastWebWiew = 0;
	outEntry->mPDFVersion = 6;

	/*
	0 = PDF Version
	1 = Author
	2 = Title
	3 = Subject
	4 = Keywords
	5 = Creator
	6 = Producer
	7 = Creation date
	8 = Modification date
	*/

	DebenuPDFLibraryAX0912 QP;
	QP.UnlockKey(_T(QUICKPDF_LICENSE_KEY));
	if(QP.Unlocked() != 1)
	{
		return FALSE;
	}
	try
	{
		long rtn = QP.LoadFromFile(inFile.GetString(),L""); //load a local file//TODO with password
		CString strMetadata;
		try
		{
			strMetadata = QP.GetInformation(0).c_str();
			if (strMetadata)
			{
				int lDot = strMetadata.Find(_T("."));
				if (lDot != -1)
				{
					strMetadata.Delete(0, lDot+1);
				}
				outEntry->mPDFVersion = _wtoi(strMetadata);
			}
		}
		catch(...)
		{
		}

	// Get Author
		strMetadata = QP.GetInformation(1).c_str();
		if (strMetadata)
		{
			outEntry->mAuthor = strMetadata;
		}
	
	// Get Title
		strMetadata = QP.GetInformation(2).c_str();
		if (strMetadata)
		{
			outEntry->mTitle = strMetadata; 
		}

	// Get Subject
		strMetadata = QP.GetInformation(3).c_str();
		if (strMetadata)
		{
			outEntry->mSubject = strMetadata;
		}

	// Get Keywords
		strMetadata = QP.GetInformation(4).c_str();
		if (strMetadata)
		{
			outEntry->mKeywords = strMetadata;
		}
	}
	catch(...)
	{
		return FALSE;
	}

	return TRUE;
}

BOOL CPDFPropertiesFrame::SavePDFMetadata(CString inFile, PDF_Entry* inEntry)
{
	try
	{
		DebenuPDFLibraryAX0912 QP;
		QP.UnlockKey(_T(QUICKPDF_LICENSE_KEY));
		if(QP.Unlocked() != 1)
		{
			return FALSE;
		}
		long lRetValue = QP.LoadFromFile(inFile.GetString(), L""); //load a local file//TODO with password
		
	// Write version
		CString lStr;
		lStr.Format(_T("1.%d"), inEntry->mPDFVersion);
		lRetValue = QP.SetInformation(0, lStr.GetString());

	// Write Author
		lRetValue = QP.SetInformation(1, inEntry->mAuthor.GetString());

	// Write Title
		lRetValue = QP.SetInformation(2, inEntry->mTitle.GetString());

	// Write Subject
		lRetValue = QP.SetInformation(3, inEntry->mSubject.GetString());

	// Write Keywords
		lRetValue = QP.SetInformation(4, inEntry->mKeywords.GetString());

	// Save file
		lRetValue = QP.SaveToFile(inFile.GetString());
	}
	catch(...)
	{
		return FALSE;
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////
// CPDFPropertiesFrame.cpp 
/////////////////////////////////////////////////////////////////////////
// Message map
BEGIN_MESSAGE_MAP(CPDFPropertiesFrame, CDialog)
	ON_BN_CLICKED(IDC_WF_EDIT_TAGS_BTN, &CPDFPropertiesFrame::OnBnClickedWfEditTagsBtn)
	ON_CBN_SELCHANGE(IDC_VERSION_COMBO, &CPDFPropertiesFrame::OnCbnSelchangeVersionCombo)
END_MESSAGE_MAP()

//*****************************************************************************
CPDFPropertiesFrame::CPDFPropertiesFrame(PDF_Entry* inEntry)
	: CDialog(CPDFPropertiesFrame::IDD, NULL)
{
	mEntry = inEntry;
}

//*****************************************************************************
CPDFPropertiesFrame::~CPDFPropertiesFrame()
{
}
//*****************************************************************************
void CPDFPropertiesFrame::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_VERSION_COMBO, m_VersionCombo);
}

BOOL CPDFPropertiesFrame::ValidateAndProcess()
{
	CString strTitle, strAuthor, strSubject, strKeywords;
	GetDlgItemText(IDC_TITLE_EDIT, strTitle);
	GetDlgItemText(IDC_AUTHOR_EDIT,  strAuthor);
	GetDlgItemText(IDC_SUBJECT_EDIT,  strSubject);
	GetDlgItemText(IDC_TAGS_EDIT,  strKeywords);

	if(strSubject != mEntry->mSubject || strTitle != mEntry->mTitle || strAuthor != mEntry->mAuthor || strKeywords != mEntry->mKeywords) {
		GetDlgItemText(IDC_TITLE_EDIT, mEntry->mTitle);
		GetDlgItemText(IDC_AUTHOR_EDIT,  mEntry->mAuthor);
		GetDlgItemText(IDC_SUBJECT_EDIT,  mEntry->mSubject);
		GetDlgItemText(IDC_TAGS_EDIT,  mEntry->mKeywords);
		mEntry->mFastWebWiew = IsDlgButtonChecked(IDC_FWW_CHECK);
		mEntry->mPDFVersion = m_VersionCombo.GetCurSel()+3;
		return TRUE;
	}
	else 
		return FALSE;
}

BOOL CPDFPropertiesFrame::OnInitDialog()
{
	CDialog::OnInitDialog();
	Update();
	return TRUE;
}

void CPDFPropertiesFrame::Update()
{
	SetDlgItemText(IDC_TITLE_EDIT, mEntry->mTitle);
	SetDlgItemText(IDC_AUTHOR_EDIT,  mEntry->mAuthor);
	SetDlgItemText(IDC_SUBJECT_EDIT,  mEntry->mSubject);
	SetDlgItemText(IDC_TAGS_EDIT,  mEntry->mKeywords);
	CheckDlgButton(IDC_FWW_CHECK, mEntry->mFastWebWiew);
	m_VersionCombo.SetCurSel(mEntry->mPDFVersion-3);

	m_VersionCombo.SetCurSel(1);
	m_VersionCombo.EnableWindow(FALSE);
}

void CPDFPropertiesFrame::OnBnClickedWfEditTagsBtn()
{
}

void CPDFPropertiesFrame::OnCbnSelchangeVersionCombo()
{
	// TODO: Add your control notification handler code here
}
