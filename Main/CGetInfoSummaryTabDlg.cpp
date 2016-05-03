// CGetInfoSummaryTabDlg.cpp: implementation file

#include "stdafx.h"
#include "CGetInfoSummaryTabDlg.h"
#include "CBBTabPanel.h"
#include "CGetInfoDlg.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// Messages
BEGIN_MESSAGE_MAP(CGetInfoSummaryTabDlg, CBBTabPanel)

	ON_BN_CLICKED(IDC_INFO_CHANGE_OPEN_WITH_BTN, &CGetInfoSummaryTabDlg::OnBnClickedInfoChangeOpenWithBtn)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
// Constructor
CGetInfoSummaryTabDlg::CGetInfoSummaryTabDlg(CGetInfoData *inSelFilesData, CWnd* pParent /*=NULL*/)
: CBBTabPanel(pParent)
{
	mSelFilesData = inSelFilesData;
	//{{AFX_DATA_INIT(CGetInfoSummaryTabDlg)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

//////////////////////////////////////////////////////////////////////////
// Destructor
CGetInfoSummaryTabDlg::~CGetInfoSummaryTabDlg()
{
}

//////////////////////////////////////////////////////////////////////////
// Data exchange
void CGetInfoSummaryTabDlg::DoDataExchange(CDataExchange* pDX)
{
	CBBTabPanel::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGetInfoSummaryTabDlg)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_SUMMARY_SIZE_STATIC, m_SizeStatic);
	DDX_Control(pDX, IDC_SUMMARY_SIZE_ON_DISK_STATIC, m_SizeOnDiskStatic);
	DDX_Control(pDX, IDC_SUMMARY_CREATED_STATIC, m_CreatedStatic);
	DDX_Control(pDX, IDC_SUMMARY_MODIFIED_STATIC, m_ModifiedStatic);
	DDX_Control(pDX, IDC_SUMMARY_ACCESSED_STATIC, m_AccessedStatic);
	DDX_Control(pDX, IDC_SUMMARY_OPENSWITH_STATIC, m_OpensWithStatic);
	DDX_Control(pDX, IDC_SUMMARY_FILETYPE_STATIC, m_FileTypeStatic);
}

//////////////////////////////////////////////////////////////////////////
// CGetInfoSummaryTabDlg message handlers
bool CGetInfoSummaryTabDlg::ValidateAndProcess()
{
	return true;
}

//////////////////////////////////////////////////////////////////////////
// Initialization
BOOL CGetInfoSummaryTabDlg::OnInitDialog() 
{
	CBBTabPanel::OnInitDialog();
	Update(((CGetInfoDlg*)GetParent())->GetItem());

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

// Cancel
void CGetInfoSummaryTabDlg::OnCancel() 
{
	((CGetInfoDlg*)GetParent())->OnCancel();
}

BOOL CGetInfoSummaryTabDlg::PreTranslateMessage(MSG* pMsg)
{
	return CBBTabPanel::PreTranslateMessage(pMsg);
}

#include "CAlertDlg.h"

void CGetInfoSummaryTabDlg::OnBnClickedInfoChangeOpenWithBtn()
{
	CString lLocation;
	CString lName;
	CString lCommand;
	
	GetDlgItem(IDC_SUMMARY_LOCATION_STATIC)->GetWindowText(lLocation);
	GetDlgItem(IDC_SUMMARY_FILE_STATIC)->GetWindowText(lName);

	lCommand = _T("SHELL32.DLL OpenAs_RunDLL ") + lLocation + lName;
	
	ShellExecute( 0 , _T(""), _T("RUNDLL32.EXE"), lCommand, _T(""), 2);
}

void CGetInfoSummaryTabDlg::Update(int inFileID)
{
	CTFiles *lTFiles = mSelFilesData->GetDataTFiles(inFileID);
	if(lTFiles->mDataIsValid)
	{
		CString lStr;
		CString lLocation;

		CString lFilename;
		CString lSize;
		CString lSizeOnDisk;
		lStr.Format(_T("%s"), lTFiles->mPath);

		SetDlgItemText(IDC_SUMMARY_FILE_STATIC, lTFiles->mName);

			GetDlgItem(IDC_SUMMARY_FILETYPE_STATIC)->SetWindowText(lStr.Right(lStr.GetLength() - lStr.ReverseFind('.') - 1));

			lLocation = lStr.Left(lStr.ReverseFind('\\') + 1);
			GetDlgItem(IDC_SUMMARY_LOCATION_STATIC)->SetWindowText(lLocation);

			lSize.Format(_T("%d"), GetFileSize(lTFiles->mPath));
			m_SizeStatic.SetWindowText(lSize);

			lSizeOnDisk.Format(_T("%d"), GetSizeOnDisk(lTFiles->mPath));
			m_SizeOnDiskStatic.SetWindowText(lSizeOnDisk);
			
			CTime lTimeCr(lTFiles->mFindData.ftCreationTime);
			CString lTimeStr = lTimeCr.Format(_T("%d %B %Y, %H:%M:%S")); 
			m_CreatedStatic.SetWindowText(lTimeStr);

			CTime lTimeMod(lTFiles->mFindData.ftLastWriteTime);
			lTimeStr = lTimeMod.Format(_T("%d %B %Y, %H:%M:%S")); 
			m_ModifiedStatic.SetWindowText(lTimeStr);

			CTime lTimeAcc(lTFiles->mFindData.ftLastAccessTime);
			lTimeStr = lTimeAcc.Format(_T("%d %B %Y, %H:%M:%S")); 
			m_AccessedStatic.SetWindowText(lTimeStr);
		}
	return;
/*
	CString lLocation;

	CTFiles lTFiles;
	CString lFilename;
	CString lSize;
	CString lSizeOnDisk;
	
	mFileID = inFileID;
	if (lTFiles.GetFileByID(inFileID))
	{
		if (lTFiles.mDataIsValid)	
		{
			lStr.Format(_T("%s"), lTFiles.mPath);
			lFilename = lStr.Right(lStr.GetLength() - lStr.ReverseFind('\\') - 1);
			GetDlgItem(IDC_SUMMARY_FILE_STATIC)->SetWindowText(lFilename);

			GetDlgItem(IDC_SUMMARY_FILETYPE_STATIC)->SetWindowText(lStr.Right(lStr.GetLength() - lStr.ReverseFind('.') - 1));

			lLocation = lStr.Left(lStr.ReverseFind('\\') + 1);
			GetDlgItem(IDC_SUMMARY_LOCATION_STATIC)->SetWindowText(lLocation);

			lSize.Format(_T("%d"), GetFileSize(lLocation + lFilename));
			m_SizeStatic.SetWindowText(lSize);

			lSizeOnDisk.Format(_T("%d"), GetSizeOnDisk(lLocation + lFilename));
			m_SizeOnDiskStatic.SetWindowText(lSizeOnDisk);
			
			CTime lTimeCr(lTFiles.mFindData.ftCreationTime);
			CString lTimeStr = lTimeCr.Format(_T("%d %B %Y, %H:%M:%S")); 
			m_CreatedStatic.SetWindowText(lTimeStr);

			CTime lTimeMod(lTFiles.mFindData.ftLastWriteTime);
			lTimeStr = lTimeMod.Format(_T("%d %B %Y, %H:%M:%S")); 
			m_ModifiedStatic.SetWindowText(lTimeStr);

			CTime lTimeAcc(lTFiles.mFindData.ftLastAccessTime);
			lTimeStr = lTimeAcc.Format(_T("%d %B %Y, %H:%M:%S")); 
			m_AccessedStatic.SetWindowText(lTimeStr);
		}
	}
*/
}

long CGetInfoSummaryTabDlg::GetFileSize(LPCTSTR inPath)
{
	CFile lFile;
	ULONGLONG lByte = 0;
	if(lFile.Open(inPath, CFile::modeRead | CFile::shareDenyNone))
	{
		lByte = lFile.GetLength();
		lFile.Close();
	}
	return lByte;
}

long CGetInfoSummaryTabDlg::GetSizeOnDisk(LPCTSTR inPath)
{
	long lClusterSize;
	long lSizeOnDisk;
	CString lPath;
	CString lRoot;
	DWORD dwSectPerClust, dwBytesPerSect, dwFreeClusters, dwTotalClusters;
	lPath = inPath;
	lRoot = lPath.Left(lPath.Find(_T("\\")) + 1);
	::GetDiskFreeSpace(lRoot, &dwSectPerClust, &dwBytesPerSect, &dwFreeClusters, &dwTotalClusters);
	lClusterSize = dwSectPerClust * dwBytesPerSect;
	lSizeOnDisk = lClusterSize * ((GetFileSize(lPath) + lClusterSize - 1) / lClusterSize);
	return lSizeOnDisk;
}
