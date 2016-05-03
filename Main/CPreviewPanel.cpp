#include "stdafx.h"
#include "Define.h"
#include "Benubird.h"
#include "CPreviewPanel.h"
#include "MainFrm.h"
#include "LibraryView.h"
#include "CDB.h"
#include "COptionsDefs.h"
#include "WinSpool.h"
//#include "include/novaOptions.h"
//#include "include/novapi.h"
#include "CRegistry.h"
//#include "BCGPResistry.h"

enum MarkupDocFlags
{
	TXT_UTF16LEFILE = 1,
	TXT_UTF8PREAMBLE = 4,
	TXT_IGNORECASE = 8,
	TXT_UTF16BEFILE = 16,
	TXT_TRIMWHITESPACE = 32,
	TXT_COLLAPSEWHITESPACE = 64
};

struct BOM_TABLE 
{ 
	CONST CHAR* pszBom; 
	INT nBomLen; 
	TCHAR* pszBomEnc; 
	INT nBomFlag; 
} btUtfIds[] = {
	{ "\xef\xbb\xbf", 3, L"UTF-8", TXT_UTF8PREAMBLE },
	{ "\xff\xfe", 2, L"UTF-16LE", TXT_UTF16LEFILE },
	{ "\xfe\xff", 2, L"UTF-16BE", TXT_UTF16BEFILE },
	{ NULL,0,NULL,0 } 
};

//name of novaPDF Printer
#define PRINTER_NAME    L"novaPDF v7"

//Print profile name
#define PROFILE_NAME       L"TEST Profile"
#define PROFILE_IS_PUBLIC  0
/*
#pragma message("Import MSO.DLL")
#import "C:\\Program Files\\Common Files\\Microsoft Shared\\OFFICE12\\MSO.DLL" rename_namespace("MSO"), auto_rename
#pragma message("Import VBE6EXT.OLB")
#import "C:\\Program Files\\Common Files\\Microsoft Shared\\VBA\\VBA6\\VBE6EXT.OLB" rename_namespace("VBE6EXT")
#pragma message("Import MS Word API")
#import "C:\\Program Files\\Microsoft Office\\OFFICE12\\MSWORD.OLB" rename_namespace("MSWORD"), auto_rename
*/


static COleVariant vTrue( (short)TRUE ), vFalse( (short)FALSE ), vOpt( (long)DISP_E_PARAMNOTFOUND, VT_ERROR );
//*****************************************************************************
// CPreviewPanel
//*****************************************************************************
IMPLEMENT_SERIAL(CPreviewPanel, CBBDialogBar, VERSIONABLE_SCHEMA | 1)

CPreviewPanel::CPreviewPanel(CWnd* pParent /*=NULL*/)
{
	mFileID = -1;
}

CPreviewPanel::~CPreviewPanel()
{
}

void CPreviewPanel::DoDataExchange(CDataExchange* pDX)
{
	CBBDialogBar::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CPreviewPanel, CBBDialogBar)
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()

BOOL CPreviewPanel::OnEraseBkgnd(CDC* pDC) 
{
	CRect lClientRect;
	GetClientRect(lClientRect);
	CBCGPVisualManager::GetInstance()->OnFillBarBackground(pDC, this, lClientRect, lClientRect);
	return TRUE;
}

LRESULT CPreviewPanel::HandleInitDialog(WPARAM wParam, LPARAM lParam)
{
	if (!CBBDialogBar::HandleInitDialog (wParam, lParam))
	{
		return FALSE;
	}
	OnInitDialog ();
	return TRUE;
}

BOOL CPreviewPanel::OnInitDialog ()
{
	mPreviewBitmap.SubclassDlgItem(IDC_PREVIEW_STATIC, this);
	mPreviewIcon.SubclassDlgItem(IDC_PREVIEW_ICON, this);
	
	mPreviewIcon.ShowWindow(SW_HIDE);
	mPreviewBitmap.ShowWindow(SW_HIDE);

	return TRUE;
}

void CPreviewPanel::Resize (BOOL bFull)
{
	CRect lRectClient;
	GetClientRect(lRectClient);
	mPreviewBitmap.SetWindowPos(NULL, lRectClient.left, lRectClient.top, lRectClient.Width(), lRectClient.Height(), SWP_NOZORDER | SWP_NOACTIVATE);
	mPreviewIcon.SetWindowPos(NULL, lRectClient.left, lRectClient.top, lRectClient.Width(), lRectClient.Height(), SWP_NOZORDER | SWP_NOACTIVATE);
	
	Refresh(TRUE);
}

void CPreviewPanel::Refresh(BOOL inForceRedraw)
{
	CMDIChildWnd * pChild = ((CMDIFrameWnd*)(AfxGetApp()->m_pMainWnd))->MDIGetActive();
	if(pChild == NULL)
		return;
	CLibraryView* pView = DYNAMIC_DOWNCAST(CLibraryView, pChild->GetActiveView ());
	CLibraryDoc* pDoc = DYNAMIC_DOWNCAST(CLibraryDoc, pChild->GetActiveDocument ());

	if (pView == NULL || pDoc == NULL)
		return;

	POSITION lPos = NULL;
	lPos = pView->mDocumentFileList.GetFirstSelectedItemPosition();

	int lIndex = ((int)lPos) - 1;
	if (lIndex < 0) 
	{
		mPreviewBitmap.ShowWindow(SW_HIDE);
		mPreviewIcon.ShowWindow(SW_HIDE);
		return;
	}

	CString basePath=L"";
	try
	{
		// store the last preview file identifier
		mFileID = pDoc->sFilesArray[lIndex].mID;

		// prepare the preview
		CString lPath = pDoc->sFilesArray[lIndex].mPath;
		if(lPath.GetLength() == 0)
		{
			CTFiles lTFiles(pDoc->m_pParamCDB);
			lTFiles.GetFileByID(mFileID);
			lPath = lTFiles.mPath;
		}

		if(lPath.GetLength() == 0)
		{
			// Can't find file path
			return;
		}

		//if pdf , do with QuickPDF
		CWinApp* lApp = AfxGetApp();
		int lIntValue = lApp->GetProfileInt(kOptions, kPDFRenderWithQuickPDF, BST_CHECKED);

		if(lIntValue == 1 && lPath.GetLength() > 4) {
			
			/*			
			/// For MS WORD documents
			
			char szOriginalPrnName[MAX_PATH];
			DWORD dwSize = MAX_PATH;
			bool bHasDefaultPrinter = true;
			if(!GetDefaultPrinter((LPWSTR) szOriginalPrnName,&dwSize))
			{
				int d = GetLastError();
				if(d == ERROR_INSUFFICIENT_BUFFER || d == ERROR_FILE_NOT_FOUND)
				bHasDefaultPrinter = false;
			}

			BOOL lSet = SetDefaultPrinter(_T("novaPDF v7"));

			if (lSet == FALSE)
			{
				AfxMessageBox(_T("Unable to select the Benubird printer (novaPDF). Please reinstall Benubird - if the problem persists contact Debenu support - support@debenu.com"));
				return;
			}

			if(!CString(lPath.GetBuffer(MAX_PATH) + lPath.GetLength()-5).CompareNoCase( L".docx" ) ||
				!CString(lPath.GetBuffer(MAX_PATH) + lPath.GetLength()-4).CompareNoCase( L".doc" )){

				// Run Microsoft Word as COM-server
				MSWORD::_ApplicationPtr itfWordApp(L"Word.Application");	
				MSWORD::_DocumentPtr itfWordDoc;
				// Open the document from a file
				itfWordDoc = itfWordApp->Documents->Open(COleVariant(lPath), COleVariant(VARIANT_FALSE), COleVariant(VARIANT_TRUE));
				INovaPdfOptions *m_novaOptions;
 				HRESULT hr = S_OK;
				m_novaOptions = 0;
				hr = CoCreateInstance(__uuidof(NovaPdfOptions), NULL, CLSCTX_INPROC_SERVER, __uuidof(INovaPdfOptions), (LPVOID*) &m_novaOptions);
				if (SUCCEEDED(hr)) {
					hr = m_novaOptions -> InitializeSilent(PRINTER_NAME, L"", L"", L"");
					if (SUCCEEDED(hr)) {
						
						basePath = lPath;
						lPath = L"C:\\Windows\\Temp\\test.pdf";

						m_novaOptions -> StartUpdateProfiles();
						m_novaOptions -> AddProfile(PROFILE_NAME, PROFILE_IS_PUBLIC);
						// disable the "Save PDF file as" prompt 
						hr = m_novaOptions -> SetOptionLong(NOVAPDF_SAVE_PROMPT, FALSE, PROFILE_NAME, PROFILE_IS_PUBLIC);
						// set generated Pdf files destination folder ("D:\")
						hr = m_novaOptions -> SetOptionString(NOVAPDF_SAVE_FOLDER, L"C:\\Windows\\Temp\\", PROFILE_NAME, PROFILE_IS_PUBLIC);
						hr = m_novaOptions -> SetOptionString(NOVAPDF_SAVE_FILE, L"test.pdf", PROFILE_NAME, PROFILE_IS_PUBLIC);
						// if file exists in the destination folder, append a counter to the end of the file name
						hr = m_novaOptions -> SetOptionLong(NOVAPDF_SAVE_CONFLICT_STRATEGY, FILE_CONFLICT_STRATEGY_OVERWRITE,PROFILE_NAME, PROFILE_IS_PUBLIC);
						// don't detect URLs
						hr = m_novaOptions -> SetOptionLong(NOVAPDF_URL_ANALIZE, (LONG)FALSE, PROFILE_NAME, PROFILE_IS_PUBLIC);
						hr = m_novaOptions -> SetOptionLong(NOVAPDF_SAVE_DESTINATION, (LONG)FALSE, PROFILE_NAME, PROFILE_IS_PUBLIC);
						hr = m_novaOptions -> SetOptionLong(NOVAPDF_SAVE_ADV_DLG, (LONG)FALSE, PROFILE_NAME, PROFILE_IS_PUBLIC);
						hr = m_novaOptions -> SetOptionLong(NOVAPDF_SAVE_ADVANCED_STRATEGY , FILE_CONFLICT_STRATEGY_OVERWRITE, PROFILE_NAME, PROFILE_IS_PUBLIC);
						hr = m_novaOptions -> SetOptionLong(NOVAPDF_ACTION_OPEN_DOCUMENT , (LONG)FALSE, PROFILE_NAME, PROFILE_IS_PUBLIC);
						hr = m_novaOptions -> SetOptionLong(NOVAPDF_ACTION_USE_DEFAULT_VIEWER , (LONG)FALSE, PROFILE_NAME, PROFILE_IS_PUBLIC);
						hr = m_novaOptions -> SetOptionLong(NOVAPDF_SILENT_PRINT, (LONG)TRUE, PROFILE_NAME, PROFILE_IS_PUBLIC);
						
						hr = m_novaOptions -> SetOptionLong(NOVAPDF_BMARK_EN_AUTO_DET_BMARKS, (LONG)FALSE, PROFILE_NAME, PROFILE_IS_PUBLIC);
						hr = m_novaOptions -> SetOptionLong(NOVAPDF_WM_ENABLE, (LONG)FALSE, PROFILE_NAME, PROFILE_IS_PUBLIC);
						hr = m_novaOptions -> SetOptionLong(NOVAPDF_OV_ENABLE, (LONG)FALSE, PROFILE_NAME, PROFILE_IS_PUBLIC);
						hr = m_novaOptions -> SetOptionLong(NOVAPDF_SIG_ENABLE, (LONG)FALSE, PROFILE_NAME, PROFILE_IS_PUBLIC);
						
						// mark finish changing options so they are saved for the printer
						m_novaOptions -> EndUpdateProfiles();
						
						m_novaOptions -> SetActiveProfile(PROFILE_NAME, PROFILE_IS_PUBLIC);
						hr = m_novaOptions -> RegisterEventWindow((LONG) GetSafeHwnd());
						hr = m_novaOptions -> SetDefaultPrinter();
						
						// Print all pages of the document
						
						itfWordDoc->PrintOut();
						WaitForSingleObject(itfWordDoc, 15000);
						m_novaOptions -> UnRegisterEventWindow();
						m_novaOptions -> RestoreDefaultPrinter();
						
						if(bHasDefaultPrinter) // Restore the previous printer
						SetDefaultPrinter((LPCWSTR) szOriginalPrnName);
						m_novaOptions -> DeleteProfile(PROFILE_NAME, PROFILE_IS_PUBLIC);												
					}else{
						return ;
					}
				}else{
					return ;
				}

				if (m_novaOptions) {
					m_novaOptions -> Release();
				}
				// Close the document and Microsoft Word application
				itfWordDoc->Close();
						
				itfWordApp->Quit();
			}*/

			///	For TXT documents
			if(!CString(lPath.GetBuffer(MAX_PATH) + lPath.GetLength()-4).CompareNoCase( L".txt" ) ||
			   !CString(lPath.GetBuffer(MAX_PATH) + lPath.GetLength()-4).CompareNoCase( L".inf" ) ||
			   !CString(lPath.GetBuffer(MAX_PATH) + lPath.GetLength()-4).CompareNoCase( L".xml" ) ||
			   !CString(lPath.GetBuffer(MAX_PATH) + lPath.GetLength()-5).CompareNoCase( L".html" ) ||
			   !CString(lPath.GetBuffer(MAX_PATH) + lPath.GetLength()-4).CompareNoCase( L".htm" ) ||
			   !CString(lPath.GetBuffer(MAX_PATH) + lPath.GetLength()-4).CompareNoCase( L".asp" ) ||
			   !CString(lPath.GetBuffer(MAX_PATH) + lPath.GetLength()-2).CompareNoCase( L".c" ) ||
			   !CString(lPath.GetBuffer(MAX_PATH) + lPath.GetLength()-2).CompareNoCase( L".h" ) ||
			   !CString(lPath.GetBuffer(MAX_PATH) + lPath.GetLength()-4).CompareNoCase( L".cpp" ) ||
			   !CString(lPath.GetBuffer(MAX_PATH) + lPath.GetLength()-3).CompareNoCase( L".rc" ) ||
			   !CString(lPath.GetBuffer(MAX_PATH) + lPath.GetLength()-4).CompareNoCase( L".php" ) ||
			   !CString(lPath.GetBuffer(MAX_PATH) + lPath.GetLength()-3).CompareNoCase( L".pl" ) ||
			   !CString(lPath.GetBuffer(MAX_PATH) + lPath.GetLength()-3).CompareNoCase( L".cs" ) ||
			   !CString(lPath.GetBuffer(MAX_PATH) + lPath.GetLength()-4).CompareNoCase( L".css" ) ||
			   !CString(lPath.GetBuffer(MAX_PATH) + lPath.GetLength()-3).CompareNoCase( L".js" ) ||
			   !CString(lPath.GetBuffer(MAX_PATH) + lPath.GetLength()-4).CompareNoCase( L".jsp" ) ||
			   !CString(lPath.GetBuffer(MAX_PATH) + lPath.GetLength()-4).CompareNoCase( L".ini" )){
				
				basePath = lPath;
				if ( ! LoadFile(lPath.GetBuffer(MAX_PATH)))
					return;
				
				try{
					int iDAFileHandle = NULL, iPageRef;
					DebenuPDFLibraryAX0912 mQP;
					
					wchar_t wszRes[1600];
					
					mQP.UnlockKey(_T(QUICKPDF_LICENSE_KEY));
					
					if ( !mQP.Unlocked())
						return;

					mQP.SetPDFAMode(2);
					// Set page origin for drawing co-ordinates to top left of page
					mQP.SetOrigin(1);
					// Now lets add a bunch of different objects to the PDF which
					// are supported by the PDF/A specification.
					// Add some document properties to the PDF
					mQP.SetInformation(0, L"1.9");
					mQP.SetInformation(1, L"John Smith");
					mQP.SetInformation(2, L"The Life and Times of John Smith");
					mQP.SetInformation(3, L"A very special book");
					mQP.SetInformation(4, L"book, paperback, e-book");
					mQP.SetInformation(5, L"Mother Earth");
					mQP.SetInformation(6, L"Humanity");
					// Embed a font that will be used for drawing some sample text

					if ( isCJKFont()){
						mQP.AddCJKFont(1);
						mQP.AddCJKFont(5);					
					}
					wstring strTemp = m_wszContent;
					int nTextLen = strTemp.length();
					strTemp.copy(wszRes, 1600, 0);

					if ( nTextLen < 1600 )
						wszRes[nTextLen] = 0;
					mQP.SetPageSize(L"B5");
					mQP.SetTextColor(0,0,0);
					mQP.DrawTextBox(25, 25, 400,680, strTemp, 1);

					//  Gets the temp path env string (no guarantee it's a valid path).
					DWORD dwRetVal = 0;
					TCHAR strTempPathBuffer[MAX_PATH];
					dwRetVal = GetTempPath(MAX_PATH,          // length of the buffer
						strTempPathBuffer); // buffer for path 
					if (dwRetVal > MAX_PATH || (dwRetVal == 0))
						return;
					CString tempFilePath;
					tempFilePath = strTempPathBuffer;
					tempFilePath += _T("\\Benubird_Preview.pdf");
					if ( mQP.SaveToFile(tempFilePath.GetBuffer(MAX_PATH)) < 0 )
						return;
					mQP.DACloseFile(iDAFileHandle);
					lPath = tempFilePath;
				}catch(...)
				{}
			}

			if(!CString(lPath.GetBuffer(MAX_PATH) + lPath.GetLength()-4).CompareNoCase(L".pdf")) {

				int iDAFileHandle = NULL, iPageRef;

				DebenuPDFLibraryAX0912 mQP;
				mQP.UnlockKey(_T(QUICKPDF_LICENSE_KEY));
				if(mQP.Unlocked()) {

					iDAFileHandle= mQP.DAOpenFile(lPath.GetBuffer(MAX_PATH), _T(""));
					if(iDAFileHandle != NULL) {
						iPageRef = mQP.DAFindPage(iDAFileHandle, 1);

						RECT rect;
						mPreviewBitmap.GetWindowRect(&rect);
						int iHeight = mQP.DAGetPageHeight(iDAFileHandle, iPageRef);
						int iWidth = mQP.DAGetPageWidth(iDAFileHandle, iPageRef);
						int xRatio = ((double)(rect.right - rect.left)/ iWidth)*75;
						int yRatio = ((double)(rect.bottom - rect.top)/ iHeight)*75;

						std::vector<BYTE> bitmap;
						bitmap = mQP.DARenderPageToVariant(iDAFileHandle, iPageRef, 0, xRatio > yRatio ? yRatio : xRatio);
						BITMAPFILEHEADER& bfh = (BITMAPFILEHEADER&)bitmap[0];
						BITMAPINFO& bi = (BITMAPINFO&)bitmap[sizeof(BITMAPFILEHEADER)];
						BITMAPINFOHEADER& bih = bi.bmiHeader; 
						BYTE* bytearray = &bitmap[bfh.bfOffBits];

						HBITMAP dib = CreateDIBitmap( mPreviewBitmap.GetDC()->m_hDC, &bih, CBM_INIT, (void*)bytearray, &bi, DIB_RGB_COLORS );
						mPreviewBitmap.SetBitmap(dib);
						mPreviewIcon.ShowWindow(SW_HIDE);
						mPreviewBitmap.ShowWindow(SW_SHOW);

						mQP.DACloseFile(iDAFileHandle);
						if(basePath.GetLength()!=0){
							BOOL ret = ::DeleteFile(lPath);
						}
						return;
					}
				}
			}
			else {						
				mThumbExtract.SetPath(lPath);
				CRect lRectClient;
				GetClientRect(lRectClient);
				mThumbExtract.SetThumbnailSize(lRectClient.Width(), lRectClient.Height());	
				HBITMAP lBitmap = NULL;
				bool lExtractThumbnail = mThumbExtract.ExtractThumbnail(lBitmap);

				if (lExtractThumbnail && (lBitmap != NULL))
				{
					mPreviewBitmap.SetBitmap((HBITMAP)lBitmap);
					mPreviewIcon.ShowWindow(SW_HIDE);
					mPreviewBitmap.ShowWindow(SW_SHOW);
				}
				else
				{ // put inside the image based on extension
					lIcon = mThumbExtract.ExtractIcon(pDoc->sFilesArray[lIndex].mPath, false);
					mPreviewIcon.SetIcon(lIcon);
					mPreviewBitmap.ShowWindow(SW_HIDE);
					mPreviewIcon.ShowWindow(SW_SHOW);
					DestroyIcon(lIcon);
				}
			}
		}
		
	}
	catch(...)
	{
	}
}

void CPreviewPanel::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND, ID_OPEN);
	CBBDialogBar::OnLButtonDblClk(nFlags, point);
}
BOOL CPreviewPanel::isCJKFont()
{
	return m_bCJKFontFlag;
}

BOOL CPreviewPanel::LoadFile(TCHAR* wszFilePath)
{
	m_bCJKFontFlag = FALSE;
	FILE *fpDoc = _wfopen(wszFilePath, L"rb" );
	if ( ! fpDoc )
		return FALSE;

	// Prepare file
	BOOL bSuccess = TRUE;
	INT nBomLen = 0;
	INT nDocFlags = 0;
	INT nFileByteLen, nFileCharUnitSize = 1; // unless UTF-16 BOM
	wstring strEncoding;

	// Get file length
	fseek( fpDoc, 0, SEEK_END );
	nFileByteLen = ftell( fpDoc );
	fseek( fpDoc, 0, SEEK_SET );

	// Read the top of the file to check BOM and encoding
	INT nReadTop = 1024;
	if ( nFileByteLen < nReadTop )
		nReadTop = (INT)nFileByteLen;
	if ( nReadTop )
	{
		CHAR* pFileTop = new CHAR[nReadTop];
		if ( nReadTop )
			bSuccess = ( fread( pFileTop, nReadTop, 1, fpDoc ) == 1 );
		if ( bSuccess )
		{
			// Check for Byte Order Mark (preamble)
			int nBomCheck = 0;
			nDocFlags &= ~( TXT_UTF16LEFILE | TXT_UTF8PREAMBLE );
			while ( btUtfIds[nBomCheck].pszBom )
			{
				while ( nBomLen < btUtfIds[nBomCheck].nBomLen )
				{
					if ( nBomLen >= nReadTop || pFileTop[nBomLen] != btUtfIds[nBomCheck].pszBom[nBomLen] )
						break;
					++nBomLen;
				}
				if ( nBomLen == btUtfIds[nBomCheck].nBomLen )
				{
					nDocFlags |= btUtfIds[nBomCheck].nBomFlag;
					if ( nBomLen == 2 )
						nFileCharUnitSize = 2;
					strEncoding = btUtfIds[nBomCheck].pszBomEnc;
					m_bCJKFontFlag = TRUE;
					break;
				}
				++nBomCheck;
				nBomLen = 0;
			}

			if ( nReadTop > nBomLen )
				fseek( fpDoc, nBomLen, SEEK_SET );
		} else
			return bSuccess;
		delete [] pFileTop;
	}

	UINT nOpFileByteLen = 0;
	UINT nFileByteOffset = (LONG)nBomLen;

	// Read text content according to the encoding
	LONG nBytesRemaining = nFileByteLen - nFileByteOffset;
	if ( nDocFlags & (TXT_UTF16LEFILE | TXT_UTF16BEFILE) )
	{
		m_bCJKFontFlag = TRUE;
		int nLen = nBytesRemaining/2 + nBytesRemaining/100;
		WCHAR* pUTF16Buffer = new WCHAR[nLen];
		bSuccess = fread( pUTF16Buffer, nBytesRemaining, 1, fpDoc );


		if ( strEncoding.compare(L"UTF-16BE") == 0 )
		{
			for ( int i = 0; i < nLen; i ++ )
			{
				WCHAR temp = (( pUTF16Buffer[i] & 0xff00) >> 8) | ((pUTF16Buffer[i] & 0x00ff) << 8);
				pUTF16Buffer[i] = temp;
			}
		}

		pUTF16Buffer[nBytesRemaining/2] = L'\0';
		m_wszContent = pUTF16Buffer;
	}
	else // single or multibyte file (i.e. not UTF-16)
	{
		char* pBuffer = new char[nBytesRemaining];
		bSuccess = fread( pBuffer, nBytesRemaining, 1, fpDoc );
		pBuffer[nBytesRemaining] = '\0';
		INT nLen = 0;
		nLen = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, pBuffer, -1, NULL, NULL);

		if ( nLen == nBytesRemaining + 1 )
			m_bCJKFontFlag = FALSE;
		else{			
			m_bCJKFontFlag = TRUE;
		}

		// module for searching the point of the first ANSI code
		int nSpacePoint = 0;
		WCHAR wSpace = pBuffer[nSpacePoint];
		while( (wSpace & 0x00ff) != 0x0020)
		{
			nSpacePoint++;
			wSpace = ((WCHAR*)pBuffer)[nSpacePoint];
		}

		if ( (nLen == (nSpacePoint + 1) * 2) || (nLen == 0) ){

			int nLenTemp = 0;
			WCHAR wTemp = pBuffer[nLenTemp];
			while((wTemp & 0x00FF) != 0x0000)
			{
				nLenTemp++;
				wTemp = ((WCHAR*)pBuffer)[nLenTemp];
			}

			/// for ANCI code
			nLen = MultiByteToWideChar( CP_ACP, MB_ERR_INVALID_CHARS, pBuffer, -1, NULL, NULL);
			if ((nLen > 0) && (nLen != (nSpacePoint + 1) * 2))
			{
				WCHAR* pwszUTF16 = new WCHAR[nLen];
				nLen = MultiByteToWideChar(CP_ACP, MB_ERR_INVALID_CHARS, pBuffer, nBytesRemaining, pwszUTF16, nLen);
				pwszUTF16[nLen] = L'\0';		
				m_wszContent = pwszUTF16;
			}else{
				WCHAR* pwszUTFBuffer = new WCHAR[nLenTemp];
				char *psz = new char[nLenTemp*2+2];
				memset(psz, 0, nLenTemp * 2 +2);
				memcpy( psz, pBuffer, nLenTemp * 2);
				m_wszContent = (wchar_t*)psz;
				delete[] psz;
			}
		}
		else{
			WCHAR* pwszUTF16 = new WCHAR[nLen];
			nLen = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, pBuffer, nBytesRemaining, pwszUTF16, nLen);
			pwszUTF16[nLen] = L'\0';		
			m_wszContent = pwszUTF16;
			delete[] pwszUTF16;
		}
	}
	fclose(fpDoc);
	return TRUE;
}
