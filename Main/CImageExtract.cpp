// FileThumbExtract.cpp : Implementation of CImageExtract
#include "stdafx.h"
#include "Benubird.h"
#include "CImageExtract.h"
#include <shlobj.h>
#include <shlwapi.h>
#include <wchar.h>
#include <shlwapi.h>
#include "CUtils.h"

extern DWORD g_dwWinVer;

/////////////////////////////////////////////////////////////////////////////
// CImageExtract
/////////////////////////////////////////////////////////////////////////////
CImageExtract::CImageExtract()
{
	m_Width = m_Height = 120;
	m_pMalloc = NULL;
	m_hThumbnail = NULL;
	m_pBitmapData = NULL;

	::GetTempPathW(1000, m_wsBuffer);
	m_wsTempPath = m_wsBuffer;
	SHGetMalloc(&m_pMalloc);

	mDefaultSmallIcon = (HICON)::LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_UNKNOWN_FILE_ICON), IMAGE_ICON, 16, 16, 0);
	mDefaultLargeIcon = (HICON)::LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_UNKNOWN_FILE_ICON), IMAGE_ICON, 32, 32, 0);
	mEmptyFileIcon = (HICON)::LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_EMPTY_FILE_ICON), IMAGE_ICON, 32, 32, 0);
}

CImageExtract::~CImageExtract()
{
	DestroyIcon(mDefaultSmallIcon);
	DestroyIcon(mDefaultLargeIcon);
	DestroyIcon(mEmptyFileIcon);
	FreeThumbnail();
}

// Philip modified, we want to create 24-bit bitmap info (desired format)
PBITMAPINFO CreateBitmapInfoStruct(HWND hwnd, HBITMAP hBmp, WORD nBits) { 
   BITMAP bmp; 
   PBITMAPINFO pbmi; 
   WORD    cClrBits; 
   
   // Retrieve the bitmap's color format, width, and height. 
   if (!GetObject(hBmp, sizeof(BITMAP), (LPSTR)&bmp)) return NULL; 
   
	 if(nBits==0) {
		 // Convert the color format to a count of bits. 
		 cClrBits = (WORD)(bmp.bmPlanes * bmp.bmBitsPixel); 
		 if (cClrBits == 1) 
				cClrBits = 1; 
		 else if (cClrBits <= 4) 
				cClrBits = 4; 
		 else if (cClrBits <= 8) 
				cClrBits = 8; 
		 else if (cClrBits <= 16) 
				cClrBits = 16; 
		 else if (cClrBits <= 24) 
				cClrBits = 24; 
		 else cClrBits = 32; 
	 }
	 else
		 cClrBits = nBits; 
   
   // Allocate memory for the BITMAPINFO structure. (This structure 
   // contains a BITMAPINFOHEADER structure and an array of RGBQUAD 
   // data structures.) 
   
   if (cClrBits < 24) {
      pbmi = (PBITMAPINFO) LocalAlloc(LPTR, sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * (1<< cClrBits)); 
   } else {
      // There is no RGBQUAD array for the 24-bit-per-pixel format. 
      pbmi = (PBITMAPINFO) LocalAlloc(LPTR, sizeof(BITMAPINFOHEADER)); 
   }
   
   // Initialize the fields in the BITMAPINFO structure. 
   
   pbmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER); 
   pbmi->bmiHeader.biWidth = bmp.bmWidth; 
   pbmi->bmiHeader.biHeight = bmp.bmHeight; 
   pbmi->bmiHeader.biPlanes = bmp.bmPlanes; 
   pbmi->bmiHeader.biBitCount = bmp.bmBitsPixel; 
   if (cClrBits < 24) pbmi->bmiHeader.biClrUsed = (1<<cClrBits); 
   
   // If the bitmap is not compressed, set the BI_RGB flag. 
   pbmi->bmiHeader.biCompression = BI_RGB; 
   
   // Compute the number of bytes in the array of color 
   // indices and store the result in biSizeImage. 
   // For Windows NT/2000, the width must be DWORD aligned unless 
   // the bitmap is RLE compressed. This example shows this. 
   // For Windows 95/98, the width must be WORD aligned unless the 
   // bitmap is RLE compressed.
   pbmi->bmiHeader.biSizeImage = ((pbmi->bmiHeader.biWidth * cClrBits +31) & ~31) / 8 * pbmi->bmiHeader.biHeight; 
   // Set biClrImportant to 0, indicating that all of the 
   // device colors are important. 
   pbmi->bmiHeader.biClrImportant = 0; 
   return pbmi; 
} 

BOOL CreateBMPFile(HWND hwnd, CString pszFile, PBITMAPINFO pbi, HBITMAP hBMP, HDC hDC) { 
   HANDLE hf;                 // file handle 
   BITMAPFILEHEADER hdr;       // bitmap file-header 
   PBITMAPINFOHEADER pbih;     // bitmap info-header 
   LPBYTE lpBits;              // memory pointer 
   DWORD dwTotal;              // total count of bytes 
   DWORD cb;                   // incremental count of bytes 
   BYTE *hp;                   // byte pointer 
   DWORD dwTmp; 
   
   pbih = (PBITMAPINFOHEADER) pbi; 
   lpBits = (LPBYTE) GlobalAlloc(GMEM_FIXED, pbih->biSizeImage);
   
   if (!lpBits) return FALSE;
   
   // Retrieve the color table (RGBQUAD array) and the bits 
   // (array of palette indices) from the DIB. 
   if (!GetDIBits(hDC, hBMP, 0, (WORD) pbih->biHeight, lpBits, pbi, 
      DIB_RGB_COLORS)) 
   {
      return FALSE;
   }
   
   // Create the .BMP file. 
   hf = CreateFileW(pszFile, GENERIC_READ | GENERIC_WRITE, (DWORD) 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, (HANDLE) NULL); 
   if (hf == INVALID_HANDLE_VALUE) return FALSE;
   hdr.bfType = 0x4d42;        // 0x42 = "B" 0x4d = "M" 
   // Compute the size of the entire file. 
   hdr.bfSize = (DWORD) (sizeof(BITMAPFILEHEADER) + 
      pbih->biSize + pbih->biClrUsed 
      * sizeof(RGBQUAD) + pbih->biSizeImage); 
   hdr.bfReserved1 = 0; 
   hdr.bfReserved2 = 0; 
   
   // Compute the offset to the array of color indices. 
   hdr.bfOffBits = (DWORD) sizeof(BITMAPFILEHEADER) + 
      pbih->biSize + pbih->biClrUsed 
      * sizeof (RGBQUAD); 
   
   // Copy the BITMAPFILEHEADER into the .BMP file. 
   if (!WriteFile(hf, (LPVOID) &hdr, sizeof(BITMAPFILEHEADER), 
      (LPDWORD) &dwTmp,  NULL)) 
   {
      return FALSE;
   }
   
   // Copy the BITMAPINFOHEADER and RGBQUAD array into the file. 
   if (!WriteFile(hf, (LPVOID) pbih, sizeof(BITMAPINFOHEADER) + pbih->biClrUsed * sizeof (RGBQUAD), (LPDWORD) &dwTmp, NULL)) return FALSE;
   
   // Copy the array of color indices into the .BMP file. 
   dwTotal = cb = pbih->biSizeImage; 
   hp = lpBits; 
   if (!WriteFile(hf, (LPSTR) hp, (int) cb, (LPDWORD) &dwTmp,NULL)) 
      return FALSE;
   
   // Close the .BMP file. 
   if (!CloseHandle(hf)) 
      return FALSE;
   
   // Free memory. 
   GlobalFree((HGLOBAL)lpBits);

   return TRUE;
}

#define SHORTCUT_TEMPLATE  L"[InternetShortcut]\nURL=%s\n"
#define TEMP_URL           L"TEMP.URL"


BOOL CreateURLShortcut(LPCWSTR pszURL, LPCWSTR pszShortcutFile) {
   WCHAR pszShortcutContents[1024];

   HANDLE hf = CreateFileW(pszShortcutFile, GENERIC_READ | GENERIC_WRITE, (DWORD) 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, (HANDLE) NULL); 
   if(hf == INVALID_HANDLE_VALUE) return FALSE;
   int iCharCount = swprintf(pszShortcutContents, SHORTCUT_TEMPLATE, pszURL) + 1;
   DWORD dwWritten = 0;
   if(!WriteFile(hf, pszShortcutContents, sizeof(WCHAR)*iCharCount, &dwWritten, NULL)) return FALSE;
   CloseHandle(hf);
   return TRUE;
}

// Creates a typedef
typedef HRESULT ( WINAPI *SHCreateItemFromParsingNameFunc )( PCWSTR pszPath, IBindCtx *pbc, REFIID riid, void **ppv );

HRESULT CImageExtract::CreateThumbnailVista(DWORD dwWidth, DWORD dwHeight)
{
    HRESULT hr = E_NOINTERFACE;

	try
	{
	// Load shell32 dll for support XP and Vista as a single binary
	HMODULE hModule = NULL;
	SHCreateItemFromParsingNameFunc SHCreateItemFromParsingNamePtr = NULL;
	hModule = LoadLibrary( L"Shell32.dll" );
	if(hModule)
	{
		// Get procedure address
		SHCreateItemFromParsingNamePtr = ( SHCreateItemFromParsingNameFunc ) GetProcAddress( hModule, "SHCreateItemFromParsingName" );
		if(SHCreateItemFromParsingNamePtr == NULL)
			return -1;
	} else
		return -1;

    hr = CoInitialize(NULL);
    if (FAILED(hr))
		return hr;
	/*
	IThumbnailCache *ptc;
	hr = CoCreateInstance(CLSID_LocalThumbnailCache, NULL, CLSCTX_INPROC, IID_PPV_ARGS(&ptc));

    SIZE size;
    size.cx = dwWidth;
    size.cy = dwHeight;
	IShellItem *psi;
	ISharedBitmap* pBitmap = NULL;
	WTS_CACHEFLAGS thumbnailFlags;
	WTS_THUMBNAILID thumbnailID;

	m_hThumbnail = NULL;

	// extract thumbnail image
	hr = SHCreateItemFromParsingNamePtr(m_wsPath.GetBuffer(MAX_PATH), NULL, IID_PPV_ARGS(&psi));
	if (SUCCEEDED(hr))
	{
		hr = ptc->GetThumbnail(psi, 96, WTS_EXTRACT, &pBitmap, &thumbnailFlags, &thumbnailID);
		if (hr == E_PENDING) {
			IRunnableTask* pRun = NULL;
			HRESULT hr = S_OK;
			ULONG lRunState = 0;
			hr = ptc->QueryInterface(IID_IRunnableTask, (void**)&pRun);
			if (SUCCEEDED(hr) && pRun) {
				while (TRUE) {
					lRunState = pRun->IsRunning();
					if (lRunState == IRTIR_TASK_FINISHED || lRunState == IRTIR_TASK_NOT_RUNNING)
						break;
					MessageBox(NULL, L"An thumbnail is running in background.", L"Benubird", MB_ICONSTOP);
					Sleep(10);
				}
				if (FALSE)
					pRun->Kill(0);

				pRun->Release();
				pRun = NULL;
			}
			hr = S_OK;
		}
		if (SUCCEEDED(hr)) {
			hr = pBitmap->Detach(&m_hThumbnail);
			pBitmap->Release();
		} else {
//			MessageBox(NULL, L"An error occured during extract thumbnail of a file", L"Benubird", MB_ICONSTOP);
		}
		ptc->Release();
	}

	// try to extract icon if failed
	if (m_hThumbnail == NULL || FAILED(hr))
	{*/
		// Getting the IShellItemImageFactory interface pointer for the file.
		IShellItemImageFactory *pImageFactory;
		hr = SHCreateItemFromParsingNamePtr(m_wsPath, NULL, IID_PPV_ARGS(&pImageFactory));
		if (SUCCEEDED(hr))
		{
			SIZE size = { 48, 48 };

			//sz - Size of the image, SIIGBF_BIGGERSIZEOK - GetImage will stretch down the bitmap (preserving aspect ratio)
			HBITMAP hbmp = NULL;
			hr = pImageFactory->GetImage(size, SIIGBF_BIGGERSIZEOK, &hbmp);
			if (SUCCEEDED(hr))
				m_hThumbnail = hbmp;
			else {
				MessageBox(NULL, L"An error occured during extract icon of a file", L"Benubird", MB_ICONSTOP);
				m_hThumbnail = NULL;
			}

			pImageFactory->Release();
//		}

		// Free loaded library
		FreeLibrary( hModule );
	}

	CoUninitialize();
	}
	catch(...)
	{
		MessageBox(NULL, L"An error occured during extract preview", L"Benubird", MB_ICONSTOP);
	}

	return hr;
}

HRESULT CImageExtract::CreateThumbnail(DWORD dwWidth, DWORD dwHeight)
{
/*    LPITEMIDLIST pidList = NULL;
    DWORD dwPriority = 0;
    DWORD dwFlags = IEIFLAG_SCREEN | IEIFLAG_ASYNC;
    HRESULT hr = E_NOINTERFACE;
    SIZE size;
    WCHAR szBuffer[MAX_PATH];
    IRunnableTask  *prtURL = NULL;
    IExtractImage *peiURL = NULL;
    IShellFolder  *psfWorkDir = NULL;
    IShellFolder  *psfDesktop = NULL;

    size.cx = dwWidth;
    size.cy = dwHeight;

    int nTries = 0;

    try {
        CoInitialize(NULL);

        hr = SHGetDesktopFolder(&psfDesktop);
        if(SUCCEEDED(hr)) {
			hr = psfDesktop->ParseDisplayName(NULL, NULL, (LPWSTR)m_wsDir.GetBuffer(), NULL, &pidList, NULL);
            if(SUCCEEDED(hr)) {
                hr = psfDesktop->BindToObject(pidList, NULL, IID_IShellFolder, (void **)&psfWorkDir);
                if(SUCCEEDED(hr)) {
					hr = psfWorkDir->ParseDisplayName(NULL, NULL, (LPWSTR)m_wsFile.GetBuffer(), NULL, &pidList, NULL);
                    if(SUCCEEDED(hr)) {
                        LPCITEMIDLIST pidl = pidList;
                        hr = psfWorkDir->GetUIObjectOf(NULL, 1, &pidl, IID_IExtractImage, NULL,
                                                            (void **)&peiURL);
                        if(SUCCEEDED(hr)) {
                            hr = peiURL->GetLocation(szBuffer, MAX_PATH, &dwPriority, &size, 16, &dwFlags);
							if (hr == E_PENDING) {
								IRunnableTask* pRun = NULL;
								HRESULT hr = S_OK;
								ULONG lRunState = 0;
								hr = peiURL->QueryInterface(IID_IRunnableTask, (void**)&pRun);
								if (SUCCEEDED(hr) && pRun) {
									while (TRUE) {
										lRunState = pRun->IsRunning();
										if (lRunState == IRTIR_TASK_FINISHED || lRunState == IRTIR_TASK_NOT_RUNNING)
											break;
										Sleep(10);
									}
									if (FALSE)
										pRun->Kill(0);

									pRun->Release();
									pRun = NULL;
								}
								hr = S_OK;
							}
                            if(SUCCEEDED(hr)) {
                                hr = peiURL->Extract(&m_hThumbnail);
                            }
                        }
                    }
                }
            }
        }

		IMalloc *pMalloc;
		SHGetMalloc(&pMalloc);
		pMalloc->Free(pidList);
        if(peiURL != NULL)
            peiURL->Release();
        if(psfDesktop != NULL)
            psfDesktop->Release();
        if(psfWorkDir != NULL)
            psfWorkDir->Release();

        CoUninitialize();
    }
    catch(...) {
       return E_ABORT;
    }

    return hr;
	*/
	LPITEMIDLIST pidlItems = NULL, pidlURL = NULL, pidlWorkDir = NULL;
	HRESULT hr;
	WCHAR pszPath[MAX_PATH];
	DWORD dwPriority = 0;
	//DWORD dwFlags = IEIFLAG_ASPECT | IEIFLAG_ORIGSIZE;
	DWORD dwFlags = IEIFLAG_SCREEN | IEIFLAG_ORIGSIZE;
	
    hr = CoInitialize(NULL);
    if (FAILED(hr))
		return hr;

	/*
	{ Specify Flag bits for GetLocation() method. }
	Flags :=IEIFLAG_SCREEN   // Used to tell the object to render
			or               // as if for the screen.
			IEIFLAG_OFFLINE; // Used to tell the object to use
							// only local content for rendering. 
	*/
	SIZE size = { dwWidth, dwHeight };
	IExtractImage* peiURL = NULL;
	IShellFolder* psfDesktop = NULL;
	IShellFolder* psfWorkDir = NULL;
	CString wsDir,wsFile,wsTempFile; 

	FreeThumbnail();

	try
	{
		hr = SHGetDesktopFolder(&psfDesktop);
		if(FAILED(hr)) throw 0;

		// create shortcut if URL
		if(PathIsURLW(m_wsPath)) 
		{
		wsTempFile = m_wsTempPath; wsTempFile += TEMP_URL;
		if(!CreateURLShortcut(m_wsPath, wsTempFile)) throw 0;
		wsDir = m_wsTempPath;
		wsFile = TEMP_URL;
		}
		else {
		wsDir = m_wsDir;
		wsFile = m_wsFile;
		}
		// get working directory
		wcscpy(m_wsBuffer,wsDir);
		hr = psfDesktop->ParseDisplayName(NULL, NULL, m_wsBuffer, NULL, &pidlWorkDir, NULL);
		if(FAILED(hr)) throw 0;
		hr = psfDesktop->BindToObject(pidlWorkDir, NULL, IID_IShellFolder, (LPVOID*)&psfWorkDir);
		if(FAILED(hr)) throw 0;
		psfDesktop->Release();
		psfDesktop = NULL;
		m_pMalloc->Free(pidlWorkDir);
		pidlWorkDir = NULL;

		// retrieve link information
		wcscpy(m_wsBuffer,wsFile);
		hr = psfWorkDir->ParseDisplayName(NULL, NULL, m_wsBuffer, NULL, &pidlURL, NULL);
		if(FAILED(hr)) throw 0;

		// query IExtractImage
		hr = psfWorkDir->GetUIObjectOf(NULL, 1, (LPCITEMIDLIST*)&pidlURL, IID_IExtractImage, NULL, (LPVOID*)&peiURL);
		if(FAILED(hr)) throw 0;

		// define thumbnail properties
		hr = peiURL->GetLocation(pszPath, MAX_PATH, &dwPriority, &size, 16/*CUtils::GetNumColorBit()/*lucia - povodne bolo 16*/, &dwFlags);
		if ((hr != S_OK) && (hr != E_PENDING)) throw 0;

		// generate thumbnail
		HBITMAP lThumbnail = NULL;
		hr = peiURL->Extract(&lThumbnail);
		if(FAILED(hr)) throw 0;

		/*TBitmap lBmp;
		try
		  OleCheck(XtractImg.Extract(BitmapHandle));
		  Bmp.Handle := BitmapHandle;
		  Image1.Picture.Assign(Bmp);
		finally
		  Bmp.Free;
		end;
		end;*/


		// mImage.Attach(*pThumbnail);
		m_hThumbnail = lThumbnail;

		// clean-up IExtractImage
		peiURL->Release();
		peiURL = NULL;
	}
	catch(...)
	{
	}

	// free allocated structures
	if(peiURL != NULL) peiURL->Release();
	if(pidlURL != NULL) m_pMalloc->Free(pidlURL);
	if(pidlWorkDir != NULL) m_pMalloc->Free(pidlWorkDir);
	if(psfDesktop != NULL) psfDesktop->Release();
	if(psfWorkDir != NULL) psfWorkDir->Release();

	CoUninitialize();

	return hr;
}

BOOL CreateBitmapFile(HBITMAP hThumbnail, CString pszThumbnail) {

   // create bitmap
   PBITMAPINFO pBitmapInfo = CreateBitmapInfoStruct(NULL, hThumbnail,0);
   if(pBitmapInfo == NULL) return FALSE;

   HDC hDC = GetDC(NULL);
   CreateBMPFile(NULL, pszThumbnail, pBitmapInfo, hThumbnail, hDC);
   ReleaseDC(NULL, hDC);
   LocalFree(pBitmapInfo);

   return TRUE;
}

void CImageExtract::ResolvePath()
{ 
	if(PathIsURLW(m_wsPath))
		return; // dont resolve URL 
	// resolve full-path to dir and file
	wcscpy(m_wsBuffer,m_wsPath);
	::PathRemoveFileSpecW(m_wsBuffer); 
	m_wsDir = m_wsBuffer;
	m_wsFile  = ::PathFindFileNameW(m_wsPath);
}

void CImageExtract::SetPath(CString Path)
{
	m_wsPath = Path;
	ResolvePath();
}

void CImageExtract::SetDir(CString Dir)
{
	m_wsDir = Dir;
}

void CImageExtract::SetFile(CString File)
{
	m_wsFile = File;
	if(::PathIsRelativeW(m_wsFile)) {
		::PathCombineW(m_wsBuffer,m_wsDir,m_wsFile);
		m_wsPath = m_wsBuffer;
	}
	else { // file is absolute path
		m_wsPath = m_wsFile;
	}
	ResolvePath();
}

CString CImageExtract::GetPath()
{
	return m_wsPath; 
}

CString CImageExtract::GetDir()
{
	return m_wsDir; 
}

CString CImageExtract::GetFile()
{
	return m_wsFile; 
}

void CImageExtract::SetThumbnailSize(long Width, long Height)
{
	m_Width = Width; m_Height = Height;
}

HICON CImageExtract::ExtractIcon(LPCTSTR inFileName, bool inSmall)
{
	// file exist?
	WIN32_FIND_DATA lFindData;
	HANDLE lHFind = FindFirstFile(inFileName, &lFindData);	
	if (lHFind == INVALID_HANDLE_VALUE) 
	{
		if (inSmall) return mDefaultSmallIcon;
		else return mDefaultLargeIcon;
	}
	else
		FindClose(lHFind);

	SHFILEINFO sfi; 
	if (inSmall)
	{
		::SHGetFileInfo(inFileName, FILE_ATTRIBUTE_NORMAL, &sfi, sizeof(sfi),SHGFI_USEFILEATTRIBUTES |  SHGFI_ICON|SHGFI_SMALLICON); 
		if (sfi.hIcon != NULL)return sfi.hIcon;
		else return mDefaultSmallIcon;
	}
	else
	{
		::SHGetFileInfo(inFileName, FILE_ATTRIBUTE_NORMAL, &sfi, sizeof(sfi), SHGFI_USEFILEATTRIBUTES | SHGFI_ICON|SHGFI_LARGEICON); 
		if (sfi.hIcon != NULL)return sfi.hIcon;
		else return mDefaultLargeIcon;
	}
}

bool CImageExtract::ExtractThumbnail(HBITMAP& phBitmap)
{
	HRESULT hr; 

	if(g_dwWinVer == WIN_VER_6 || g_dwWinVer == WIN_VER_7)
		hr = CreateThumbnailVista(m_Width, m_Height);
	else
		hr = CreateThumbnail(m_Width, m_Height);
	if(FAILED(hr)) 
		return false;

	phBitmap = m_hThumbnail;
	return true;
}

void CImageExtract::FreeThumbnail()
{
	try
	{
		if(m_hThumbnail != NULL) {
//			m_pMalloc->Free(m_hThumbnail);
			m_hThumbnail = NULL;
		}
		if(m_pBitmapData != NULL)	{ 
			::GlobalFree(m_pBitmapData); // delete [] m_pBitmapData; 
			m_pBitmapData = NULL;
		}
	}
	catch(...)
	{
	}
}

void CImageExtract::SaveToFile(CString BMPPath)
{
	CreateBitmapFile(m_hThumbnail, BMPPath);
	FreeThumbnail();
}

/*
void CImageExtract::get_Stride(long *pVal)
{
	return((m_Width * 24 +31) & ~31);
}*/

