// COptionsPDFDlg.cpp 
//*****************************************************************************

#include "stdafx.h"
#include "Benubird.h"
#include "COptionsPDFDlg.h"
#include "COptionsDefs.h"
#include "CRegistry.h"
#include "MainFrm.h"
#include "CAlertDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(COptionsPDFDlg, CDialog)

//*****************************************************************************
// Message map
BEGIN_MESSAGE_MAP(COptionsPDFDlg, CDialog)
END_MESSAGE_MAP()

//*****************************************************************************
COptionsPDFDlg::COptionsPDFDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COptionsPDFDlg::IDD, pParent)
{
}

//*****************************************************************************
COptionsPDFDlg::~COptionsPDFDlg()
{
}
//*****************************************************************************
void COptionsPDFDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_OPT_PDF_HYPERLINKS_CHECK, m_HyperlinkCheck);
	DDX_Control(pDX, IDC_OPT_PDF_BOOKMARKS_CHECK, m_BookmarksCheck);
	DDX_Control(pDX, IDC_OPT_PDF_EMBED_SUBSET_CHECK, m_EmbedSubsetCheck);
	DDX_Control(pDX, IDC_OPT_PDF_DISPLAY_INFOTAB_CHECK, m_DisplayInfoTabCheck);
	DDX_Control(pDX, IDC_OPT_PDF_GSETTINGS_COMBO, m_GSettingsCombo);
	DDX_Control(pDX, IDC_OPT_PDF_OUTPUT_EDIT, m_PDFOutputEdit);
	DDX_Control(pDX, IDC_OPT_PDF_RENDER_WITH_QUICKPDF_CHECK, m_RenderWithQuickPDFCheck);
}

//*****************************************************************************
BOOL COptionsPDFDlg::ValidateAndProcess()
{
	// comments in OnInitDialog() method
	int lIntValue;
	CWinApp* lApp = AfxGetApp();

	TCHAR lDrv[_MAX_DRIVE];
	TCHAR lFld[_MAX_DIR];
	TCHAR lNam[_MAX_FNAME];
	TCHAR lExt[_MAX_EXT];
	
	// Check if the selected PDF output folder exists
	// ---------------------------------------------------------
	CString lStrPDFOutputPath;
	m_PDFOutputEdit.GetWindowText(lStrPDFOutputPath);
/* 
	//check if the path is valid
	if (_wsplitpath_s(lStrPDFOutputPath, lDrv, lFld, lNam, lExt) != 0 || lDrv[0] == '\0')
	{
		Alert(kAlertWarning, _T("Invalid folder"), _T("OK"));
		m_PDFOutputEdit.SetFocus();
		return FALSE;
	}
	CFileFind lRepositoryFinder;
	if (!lRepositoryFinder.FindFile(lStrPDFOutputPath + _T("\\*.*")))
	{
		CWnd lMessage;
		if (lMessage.MessageBox(_T("The PDF output folder does not exist, do you wish\nto create it?"), _T("Create PDF output folder"), MB_YESNO | MB_ICONQUESTION) == IDNO)
		{
			m_PDFOutputEdit.SetFocus();
			return FALSE;
		}

		//try to create the directory
		if (::SHCreateDirectoryEx(NULL, lStrPDFOutputPath, NULL) != ERROR_SUCCESS)
		{
			Alert(kAlertError, _T("Benubird was unable to create the PDF output folder.\nPlease contact support@debenu.com"), _T("OK"));
			m_PDFOutputEdit.SetFocus();
			return FALSE;
		}
	}
*/
	// Write the PDF output path to the registry
	lApp->WriteProfileString(kOptions, kPDFOutputFolder, lStrPDFOutputPath);

	// ----------------------------------------------------------------

	lIntValue = m_DisplayInfoTabCheck.GetCheck();
	lApp->WriteProfileInt(kOptions, kPDFDisplayInfoTab, lIntValue);

	lIntValue = m_RenderWithQuickPDFCheck.GetCheck();
	lApp->WriteProfileInt(kOptions, kPDFRenderWithQuickPDF, lIntValue);

	//lIntValue = m_UseInfoCheck.GetCheck();
	//lApp->WriteProfileInt(kOptions, kPDFUseInfo, lIntValue);

	/*lIntValue = m_HyperlinkCheck.GetCheck();
	lApp->WriteProfileInt(kOptions, kPDFHyperlink, lIntValue);

	lIntValue = m_BookmarksCheck.GetCheck();
	lApp->WriteProfileInt(kOptions, kPDFBookmark, lIntValue);

	lIntValue = m_EmbedSubsetCheck.GetCheck();
	lApp->WriteProfileInt(kOptions, kPDFEmbedSubset, lIntValue);

	lIntValue = m_GSettingsCombo.GetCurSel();
	lApp->WriteProfileInt(kOptions, kPDFGSettings, lIntValue);
	*/

	CRegistry lReg(FALSE);
	CString lProfileKey = kPDFDefaultProfileKey;
	//if (!lReg.Open(lProfileKey)) return FALSE;
	if (!lReg.CreateKey(lProfileKey)) return FALSE;

	try
	{
		CString lStr;
		lStr.Format(_T("%d"), m_GSettingsCombo.GetCurSel());
		lReg.Write(_T("Graphics Configuration"), lStr);

		if (m_HyperlinkCheck.GetCheck() == 1)
		{
			lReg.Write(_T("AnalyzeUrl"), _T("1"));
			lReg.Write(_T("UseLinkColor"), _T("1"));
			lReg.Write(_T("BorderType"), _T("1"));
		}
		else
			lReg.Write(_T("AnalyzeUrl"), _T("0"));

		/*if (m_BookmarksCheck.GetCheck()  == 1)
		{
			lReg.Write(_T("Bookmarks Detection Enabled"), _T("1"));
			lReg.Write(_T("Bookmarks Number of Levels to Consider"), _T("2"));
		}
		else
			lReg.Write(_T("Bookmarks Detection Enabled"), _T("0"));
		*/

		if (m_EmbedSubsetCheck.GetCheck()  == 1)
		{
			lReg.Write(_T("Embed Font Subset"), _T("1"));
			lReg.Write(_T("Embed All Fonts"), _T("1"));
		}
		else
		{
			lReg.Write(_T("Embed Font Subset"), _T("0"));
			lReg.Write(_T("Embed All Fonts"), _T("1"));
		}
	}
	catch(...)
	{
		return FALSE;
	}

	return TRUE;
}

//*****************************************************************************
BOOL COptionsPDFDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	int lIntValue;
	CWinApp* lApp = AfxGetApp();

	////////////////////////////////////////////////////////////////////////
	//Viewing properties

	// Display PDF Info tab in file Properties dialog
	lIntValue = lApp->GetProfileInt(kOptions, kPDFDisplayInfoTab, BST_CHECKED);
	m_DisplayInfoTabCheck.SetCheck(lIntValue);

	// Render PDF preview with QuickPDF library
	lIntValue = lApp->GetProfileInt(kOptions, kPDFRenderWithQuickPDF, BST_CHECKED);
	m_RenderWithQuickPDFCheck.SetCheck(lIntValue);

	// Setup PDF Output Folder
	CString lStrPathRep = lApp->GetProfileString(kOptions, kPDFOutputFolder, _T(""));
	if (lStrPathRep.IsEmpty())
	{
		// Use the Repository location if none is specified			
		lStrPathRep = AfxGetApp()->GetProfileString(kOptions, kRepositoryFolder, _T(""));
		/*TCHAR szPath[MAX_PATH];
		if(!(SHGetSpecialFolderPath(NULL, szPath, CSIDL_DESKTOP, FALSE)))  // Desktop
		{
			throw(0);
		}*/
	  //lStrPathRep = CString(szPath) + _T("//") + lPath.GetFileTitle() + _T(".pdf");
	  //lStrPathRep = CString(szPath) + _T("\\");
	}

	// Double-check that all backslashes have been removed from the path
	// -------------------------------
	CPath tempPath;
	lStrPathRep = tempPath.RemoveBackSlash(lStrPathRep);
	// -------------------------------

	m_PDFOutputEdit.SetWindowText(lStrPathRep);
	m_PDFOutputEdit.EnableFolderBrowseButton();

	// Use PDF Info in the files list
	//lIntValue = lApp->GetProfileInt(kOptions, kPDFUseInfo, BST_CHECKED);
	//m_UseInfoCheck.SetCheck(lIntValue);


	////////////////////////////////////////////////////////////////////////
	/*The settings for Nova PDF printer driver options are stored within the following registry key: 
	HKEY_CURRENT_USER\Software\Softland\novaPDF\Printers\novaPDF Pro\Profiles\Default Profile */

	lIntValue = 0; //lApp->GetProfileInt(kOptions, kPDFHyperlink, BST_CHECKED);
	m_HyperlinkCheck.SetCheck(lIntValue);

	lIntValue = 0; //lApp->GetProfileInt(kOptions, kPDFBookmark, BST_CHECKED);
	m_BookmarksCheck.SetCheck(lIntValue);

	lIntValue = 0; //lApp->GetProfileInt(kOptions, kPDFEmbedSubset, BST_CHECKED);
	m_EmbedSubsetCheck.SetCheck(lIntValue);

	lIntValue = 0; //lApp->GetProfileInt(kOptions, kPDFGSettings, 0);
	m_GSettingsCombo.SetCurSel(lIntValue);

	CString lProfileKey = kPDFDefaultProfileKey;
	CRegistry lReg(FALSE);
	if (!lReg.Open(lProfileKey)) return TRUE;

	CString lStringValue;
	try
	{
		////////////////////////////////////////////////////////////////////////
		//Creation properties

		// lProfileKey\\Graphics Configuration (int)
		// 0 - Compression (default) 1 ?Downsample 2 - Greyscale 3 ?Monochrome 4 ?None
		if (lReg.Read(_T("Graphics Configuration"), lStringValue))
		{
			if (lStringValue == _T("0")) m_GSettingsCombo.SetCurSel(0);
			else if (lStringValue == _T("1")) m_GSettingsCombo.SetCurSel(1);
			else if (lStringValue == _T("2")) m_GSettingsCombo.SetCurSel(2);
			else if (lStringValue == _T("3")) m_GSettingsCombo.SetCurSel(3);
			else if (lStringValue == _T("4")) m_GSettingsCombo.SetCurSel(4);
		}

		// If hyperlink detection is enabled then activate by setting; 
		//	lProfileKey\AnalyzeUrl = 1 
		//	lProfileKey\UseLinkColor = 1 
		//	lProfileKey\BorderType = 1 
		// Else 
		//	lProfileKey\AnalyzeUrl = 0;

		if (lReg.Read(_T("AnalyzeUrl"), lStringValue)) 
		{
			if (lStringValue == _T("0")) m_HyperlinkCheck.SetCheck(BST_UNCHECKED);
			else m_HyperlinkCheck.SetCheck(BST_CHECKED);
		}

		// If "Automatically detect bookmarks" is enabled then activate by setting: 
		//	lProfileKey\Bookmarks Detection Enabled = 1 
		//	lProfileKey\Bookmarks Number of Levels to Consider = 2 
		// else disable by:
		//	lProfileKey\Bookmarks Detection Enabled = 0		
		
		/*if (lReg.Read(_T("Bookmarks Detection Enabled"), lStringValue)) 
		{
			if (lStringValue == _T("0")) m_BookmarksCheck.SetCheck(BST_UNCHECKED);
			else m_BookmarksCheck.SetCheck(BST_CHECKED);
		}*/

		// If "Only embed subsets of fonts" is enabled (default) then activate by setting:
		//	lProfileKey\Embed Font Subset = 1 
		//	lProfileKey\Embed All Fonts = 0 
		// else: 
		//	lProfileKey\Embed Font Subset = 0 
		//	lProfileKey\Embed All Fonts = 1
		if (lReg.Read(_T("Embed Font Subset"), lStringValue)) 
		{
			if (lStringValue == _T("0")) m_EmbedSubsetCheck.SetCheck(BST_UNCHECKED);
			else m_EmbedSubsetCheck.SetCheck(BST_CHECKED);
		}
	}
	catch(...)
	{
		return FALSE;
	}


	return TRUE;
}