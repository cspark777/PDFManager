#include "stdafx.h"
#include "Define.h"
#include "Benubird.h"
#include "CConvertToPDFDlg.h"
#include "MainFrm.h"
#include "LibraryView.h"
#include "Winspool.h"
#include "COptionsDefs.h"
#include "CRegistry.h"
#include "CPDFPropertiesDlg.h"
#include "CProgressDlg.h"
#include "CBRepository.h"
//#include "DebenuPDFLibraryAX0912.h"
//#include "QuickPDFAX0715.h"
//#include "QuickPDFAX0714.h"
//#include "QuickPDFDLL0712.h"

/////////////////////////////////////////////////////////////////////
// CConvertToPDFDlg dialog
IMPLEMENT_DYNAMIC(CConvertToPDFDlg, CDialog)

CConvertToPDFDlg::CConvertToPDFDlg(CStringList* inFileList)
	: CDialog(CConvertToPDFDlg::IDD, NULL)
{
	mFileList = inFileList;

	CMDIChildWnd * pChild = ((CMDIFrameWnd*)(AfxGetApp()->m_pMainWnd))->MDIGetActive();
	CLibraryDoc* pDoc = DYNAMIC_DOWNCAST(CLibraryDoc, pChild->GetActiveDocument ());
	m_pParamCDB = pDoc->m_pParamCDB;
}

CConvertToPDFDlg::~CConvertToPDFDlg()
{
}

void CConvertToPDFDlg::Set_ParamForCDB(PARAM_FOR_CDB *pParamCDB,LIBRARY_VIEW_INFO *pViewInfo)
{
	m_pParamCDB = pParamCDB;
	m_pViewInfo = pViewInfo;
	m_nRepository = pParamCDB->nIsUseRepository;
}

void CConvertToPDFDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FILE_LIST, m_FileList);
	DDX_Control(pDX, IDC_COLL_COMBO, m_CollectionCombo);
	DDX_Control(pDX, IDC_OPEN_CHECK, m_OpenPDFCheck);
}

BEGIN_MESSAGE_MAP(CConvertToPDFDlg, CDialog)
	ON_BN_CLICKED(IDC_PROPERTIES_BUTTON, &CConvertToPDFDlg::OnBnClickedPropertiesButton)
	ON_BN_CLICKED(IDC_DELETE_BUTTON, &CConvertToPDFDlg::OnBnClickedDeleteButton)
	ON_BN_CLICKED(IDOK, &CConvertToPDFDlg::OnBnClickedOk)
END_MESSAGE_MAP()

BOOL CConvertToPDFDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	int lIdx;
	lIdx = m_CollectionCombo.AddString(_T(" - none - "));
	m_CollectionCombo.SetItemData(lIdx, -1);
	m_CollectionCombo.SetCurSel(lIdx);

	CTCollections lCol(m_pParamCDB->pDB);
	if(	lCol.GetCollectionsByNames(-2))
	{
		while(lCol.mDataIsValid)
		{
			lIdx = m_CollectionCombo.AddString(lCol.mName);
			m_CollectionCombo.SetItemData(lIdx, lCol.mID);
			lCol.LoadNextRow();
		}
	}

	if (mFileList != NULL)
	{
		CConvertToPDFDlg::InitFiles(mFileList);
	}
	else
	{
		CConvertToPDFDlg::InitFiles();
	}
	EnableControls();

	return TRUE;
}

BOOL CConvertToPDFDlg::InitFiles()
{
	CMDIChildWnd * pChild = ((CMDIFrameWnd*)(AfxGetApp()->m_pMainWnd))->MDIGetActive();
	CLibraryView* pView = DYNAMIC_DOWNCAST(CLibraryView, pChild->GetActiveView ());
	CLibraryDoc* pDoc = DYNAMIC_DOWNCAST(CLibraryDoc, pChild->GetActiveDocument ());

	CTFiles lTFiles(m_pParamCDB);
	int lCount = pView->GetListSelectedCount();
	if (lCount > 0)
	{
		POSITION lPos = pView->GetListFirstSelectedItemPosition();
		while(lPos)
		{
			int lSelItemId = pView->GetListNextSelectedItem(lPos);

			// check this file
			if (pDoc->CanConvertToPDF(pDoc->sFilesArray[lSelItemId].mPath))
			{
				PDF_Entry* lEntry = new PDF_Entry;

				lEntry->mPath = pDoc->sFilesArray[lSelItemId].mPath;
				lEntry->mName = pDoc->sFilesArray[lSelItemId].mName;
				lEntry->mTitle = pDoc->sFilesArray[lSelItemId].mTitle;
				lEntry->mAuthor = pDoc->sFilesArray[lSelItemId].mAuthor;
				lEntry->mSubject = pDoc->sFilesArray[lSelItemId].mSubject;
				lEntry->mKeywords = pDoc->sFilesArray[lSelItemId].mTags; // Added to support transfer of keywords/tags - KDA 29 Jan, 2009
				lEntry->mComments = pDoc->sFilesArray[lSelItemId].mComments; // Added to support transfer of comments - KDA 29 Jan, 2009
				lEntry->mFastWebWiew = true;
				lEntry->mPDFVersion = 6;
//				lEntry->mSelItemIdx = lSelItemId; // Used to refresh the main view after deleting entries

				int lIndex = m_FileList.AddString(lEntry->mName);
				m_FileList.SetItemData(lIndex, (DWORD_PTR)lEntry);
			}
		}
		
		if (m_FileList.GetCount()>0)
			m_FileList.SetSel(0);
	}

	EnableControls();
	return TRUE;
}

BOOL CConvertToPDFDlg::InitFiles(CStringList* inFileList)
{
	CString lFile;
	POSITION lPos;
	lPos = inFileList->GetHeadPosition();
	while(lPos)
	{
		PDF_Entry* lEntry = new PDF_Entry;
		lEntry->mPath = inFileList->GetNext(lPos);
		CPath lPath(lEntry->mPath);
		lEntry->mName = lPath.GetFileName();
		lEntry->mTitle = _T("");
		lEntry->mAuthor = _T("");
		lEntry->mSubject = _T("");
		lEntry->mKeywords = _T("");
		lEntry->mComments = _T(""); // Added to support transfer of comments - KDA - 29 Jan, 2009
		lEntry->mFastWebWiew = true;
		lEntry->mPDFVersion = 6;

		int lIndex = m_FileList.AddString(lEntry->mName);
		m_FileList.SetItemData(lIndex, (DWORD_PTR)lEntry);
	}
	
	if (m_FileList.GetCount()>0) 
		m_FileList.SetSel(0);

	EnableControls();
	return TRUE;
}

void CConvertToPDFDlg::EnableControls()
{	
	int lIndex = m_FileList.GetCurSel();
	int lCount = m_FileList.GetCount();

	if (lCount > 1)
	{
		GetDlgItem(IDC_DELETE_BUTTON)->ShowWindow(SW_SHOW);
	}
	else
	{
		GetDlgItem(IDC_DELETE_BUTTON)->ShowWindow(SW_HIDE);
	}

	if ((lIndex == LB_ERR) && (lCount > 0))
	{
		m_FileList.SetCurSel(0);
	}
}

void CConvertToPDFDlg::OnBnClickedPropertiesButton()
{
	DWORD_PTR lData = m_FileList.GetItemData(m_FileList.GetCurSel());
	if (lData == NULL) return;
	PDF_Entry* lEntry = (PDF_Entry*) lData;

	CPDFPropertiesDlg lDlg(lEntry);
	lDlg.DoModal();
}

void CConvertToPDFDlg::OnBnClickedDeleteButton()
{
	if (m_FileList.GetCount() <= 1) return;

	for (int i = m_FileList.GetCount()-1 ;i >= 0 ;i--)
	{
		if (m_FileList.GetSel(i) > 0)
		{
			// destroy data and delete the item
			DWORD_PTR lData = m_FileList.GetItemData(i);
			if (lData) delete((PDF_Entry*)lData);
			m_FileList.DeleteString(i);
		}
	}
	
	EnableControls();
}

/////////////////////////////////////////////////////////////////////////
// static
/* tato fcia je zatial lokalne tu. Ide o pracu s IQuickPDF kniznicou. Konvertovanie images a inych
suborov do PDF je rozdielne. Kod je zatial scasti zdvojeny a treba do buducnosti spojit!
Pozri do fcie: CMainFrame::ProcessNovaPDFCmdLine */
BOOL CConvertToPDFDlg::ConvertImageToPDF(CString inFile, PDF_Entry* inEntry, int inCollection)
{
	CPath lImagePath(inEntry->mName);
	CString lImageExt = lImagePath.GetExtName();
	lImageExt.MakeLower();
	if ((lImageExt != _T("jpg")) &&
		(lImageExt != _T("gif")) &&
		(lImageExt != _T("tif")) &&
		(lImageExt != _T("bmp")) &&
		(lImageExt != _T("png")) &&
		(lImageExt != _T("ico")) &&
		(lImageExt != _T("tiff")) &&
		(lImageExt != _T("jpeg")))
	{
		return FALSE;
	}
 
	DebenuPDFLibraryAX0912 QP;

	QP.UnlockKey(_T(QUICKPDF_LICENSE_KEY));
	if(QP.Unlocked() != 1)
	{
		return FALSE;
	}

	BOOL lResult = true;
	CString lConversionPath = _T("");
	try
	{
		// XP:	  C:\Documents and Settings\<user>\Application Data
		// Vista: C:\Users\<user>\AppData\Roaming
		TCHAR lFolder[MAX_PATH];
		SHGetSpecialFolderPath(0, lFolder, CSIDL_APPDATA, FALSE);
		lConversionPath.Format(_T("%s\\Benubird\\Conversion"), lFolder);
		::CreateDirectory(lConversionPath, NULL);
		//if( _mkdir( "\\testtmp" ) == 0 )
		if(m_nRepository==1)
		{
			lConversionPath.Format(_T("%s\\Benubird\\Conversion\\%s.pdf"), lFolder, lImagePath.GetFileTitle());
		}
		else
		{
			CPath lFullPath(inFile);
			lConversionPath.Format(_T("%s%s%s.pdf"), lFullPath.GetDrive(), lFullPath.GetDir(),lImagePath.GetFileTitle());
		}

		// Create a temp file
		if(QP.NewDocument()==0)
		{
			AfxMessageBox(_T("Unable to create new PDF document - please contact Debenu support - support@debenu.com"));
			return FALSE;
		}

		// Add image
		if(QP.AddImageFromFile(inFile.GetString(), 1)==0)
		{
			AfxMessageBox(_T("Unable to add image to PDF document - please contact Debenu support - support@debenu.com"));
			return FALSE;
		}
		long lWidth = QP.ImageWidth();
		long lHeight = QP.ImageHeight();
		
		if(QP.SetPageDimensions(lWidth, lHeight)==0)
		{
			AfxMessageBox(_T("Unable to set page dimensions in PDF document - please contact Debenu support - support@debenu.com"));
			return FALSE;
		}

		if(QP.DrawImage(0, lHeight, lWidth, lHeight)==0)
		{
			AfxMessageBox(_T("Unable to draw image in PDF document - please contact Debenu support - support@debenu.com"));
			return FALSE;
		}

		// Save file
		//CComBSTR lPDFFile = lConversionPath;
		if(QP.SaveToFile(lConversionPath.GetString())==0)
		{
			AfxMessageBox(_T("Unable to save PDF document - please contact Debenu support - support@debenu.com"));
			return FALSE;
		}
		
		// Save metadata to temp file
		CPDFPropertiesFrame::SavePDFMetadata(lConversionPath, inEntry);
		
		// Load the file into the repository
		CStringList lListOfFiles;
		lConversionPath.Replace(_T("\\\\"), _T("\\"));
		lListOfFiles.AddTail(lConversionPath);

		CFileProperties_Entry lEntry;

		lEntry.mCollectionID = inCollection;
		
		lEntry.mUseTitle = true;
		lEntry.mTitle = inEntry->mTitle;
		
		lEntry.mUseAuthor = true;
		lEntry.mAuthor = inEntry->mAuthor;

		lEntry.mUseSubject = true;
		lEntry.mSubject = inEntry->mSubject;

		lEntry.mUseCategories = false;

	// Added to support transfer of comments and tags
		lEntry.mUseComments = true;
		lEntry.mComments = inEntry->mComments;

		lEntry.mUseTags = true;
		lEntry.mTags = inEntry->mKeywords;

		int lCount;
		if(m_nRepository==1)
		{
			lCount = m_pViewInfo->pRepository->AddFiles(lListOfFiles, &lEntry, TRUE);
		}
		else
		{
			CTFiles lTFiles(m_pParamCDB);
			lCount = lTFiles.AddFiles(lListOfFiles, &lEntry);
		}

		if (lCount == 0)
		{ // nothing was added
			CString lStr;
			lStr.Format(_T("There has been a problem with PDF creation. File: %s."), lImagePath.GetFileName());
			AfxMessageBox(lStr);
		}
	}
	catch(...)
	{
		lResult = false;
	}

	// delete the temp file
	if(m_nRepository==1)
	{
		CFileFind lFinder;
		if (lFinder.FindFile(lConversionPath) == TRUE)
		{
			DeleteFile(lConversionPath);
		}
	}

	return lResult;
}

void CConvertToPDFDlg::OnBnClickedOk()
{
	if (m_FileList.GetCount() <= 0)
	{
		OnOK();
	}

	// Take note of the existing printer
	// ------------------------------------------------
	char szOriginalPrnName[MAX_PATH];
    DWORD dwSize = MAX_PATH;
	bool bHasDefaultPrinter = true;
	if(!GetDefaultPrinter((LPWSTR) szOriginalPrnName,&dwSize))
	{
		int d = GetLastError();
		if(d == ERROR_INSUFFICIENT_BUFFER || d == ERROR_FILE_NOT_FOUND)
		bHasDefaultPrinter = false;
	}
	// ------------------------------------------------

	BOOL lSet = SetDefaultPrinter(_T("Add to Benubird PDF as PDF"));
	if (lSet == FALSE)
	{
		AfxMessageBox(_T("Unable to select the Benubird printer (novaPDF). Please reinstall Benubird - if the problem persists contact Debenu support - support@debenu.com"));
		return;
	}

	int lCollection = (int) m_CollectionCombo.GetItemData(m_CollectionCombo.GetCurSel());
	int lOpenPDF = m_OpenPDFCheck.GetCheck();	
	CString lParams;
	lParams.Format(_T("%s %d"), _T("%1"), lCollection);

	/////////////////////////////////////////////////////////////////////
	// send the list of files to "Benubird PDF Pro" printer

	CProgressDlg* lProgressDlg = new CProgressDlg;
	lProgressDlg->SetMax(m_FileList.GetCount());
	lProgressDlg->SetCaption(_T("Converting files to PDF..."));
	lProgressDlg->CreateProgressDlg();

	CString lErrorStr = _T("");
	for (int i = 0; i < m_FileList.GetCount(); i++)
	{
		PDF_Entry* lEntry = NULL;
		try
		{
			DWORD_PTR lData = m_FileList.GetItemData(i);
			if (lData == NULL) throw(0);
			lEntry = (PDF_Entry*) lData;

			CString lPathStr =  lEntry->mPath;
			lPathStr.Replace(_T("\\\\"), _T("\\"));

			lProgressDlg->SetMessage(lEntry->mName);

			BOOL lConvert = ConvertImageToPDF(lPathStr, lEntry, lCollection);
			if (lConvert == FALSE)
			{			
				/////////////////////////////////////////////////////////////////////
				// Set the default profile for novaPDF for the current file

				CRegistry lDefReg(FALSE);
				lDefReg.DeleteKey(kPDFDefaultProfileKey,0); // Clear any existing keys...
				if (!lDefReg.CreateKey(kPDFDefaultProfileKey)) throw(0);

				lDefReg.Write(_T("Document Author"), lEntry->mAuthor);
				lDefReg.Write(_T("Document Keywords"), lEntry->mKeywords);
				lDefReg.Write(_T("Document Subject"), lEntry->mSubject);
				lDefReg.Write(_T("Document Title"), lEntry->mTitle);
//				CString lVersion;
//				lVersion.Format(_T("%d"), lEntry->mPDFVersion);
//				lDefReg.Write(_T("PDFVersion"), lVersion);
				lDefReg.Write(_T("CollectionID"), lCollection); // Write this and pickup on the 
				CPath lmyPath(lPathStr);
				CString strFileName = lmyPath.GetLocation() + lmyPath.GetFileTitle() +  _T(".pdf");
				lDefReg.Write(_T("Path"), strFileName);
				lDefReg.Write(_T("Internal"), 1);
				
				lDefReg.Close();

			// if we need to wait for the end of the print process, use the following code:
				SHELLEXECUTEINFO sei;
				ZeroMemory (&sei, sizeof (SHELLEXECUTEINFO));
				sei.cbSize = sizeof (SHELLEXECUTEINFO);
				sei.lpVerb = _T("print");
				sei.lpFile = lEntry->mPath;
				sei.lpParameters = NULL;
				sei.nShow = SW_NORMAL;
				//sei.nShow = SW_HIDE;
				sei.fMask = SEE_MASK_NOCLOSEPROCESS;

				DWORD lExitCode = 1;
				BOOL lResult = ShellExecuteEx(&sei); // Returns TRUE if successful, or FALSE otherwise. Call GetLastError for error information. 
				if (lResult == FALSE)
					throw(0);

				lExitCode /*DWORD lWait*/ = WaitForSingleObject(sei.hProcess, 40000/*40 sec*/ /*INFINITE*/ );
			}
// 			::PostMessage(HWND_BROADCAST, BENUBIRD_ASSISTANT_REFRESH_FILE, (WPARAM) false, (LPARAM) lEntry->mSelItemIdx );
		}
		catch(...)
		{
			CString lStr;
			lStr.Format(_T("Could not convert %s to PDF. Open its native application and choose File. Print. 'Add to Benubird PDF as PDF' printer."), lEntry->mName);
			AfxMessageBox(lStr);
		}

		lProgressDlg->StepIt();
		MSG	msg;
		while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) 
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);  
		}
		if (lProgressDlg->IsCancelled())
		{
			AfxMessageBox(_T("Conversion has been cancelled."));
			break;
		} 

	}

	delete lProgressDlg;
	lProgressDlg = NULL;

	/////////////////////////////////////////////////////////////////////
	// return the previous settings

	if(bHasDefaultPrinter) // Restore the previous printer
		SetDefaultPrinter((LPCWSTR) szOriginalPrnName);

	OnOK();
}
