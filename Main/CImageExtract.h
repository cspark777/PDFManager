// FileThumbExtract.h : Declaration of the CImageExtract

#ifndef __FILETHUMBEXTRACT_H_
#define __FILETHUMBEXTRACT_H_

#include "resource.h"       // main symbols
#include <string>
using namespace std;

/////////////////////////////////////////////////////////////////////////////
// CImageExtract
/////////////////////////////////////////////////////////////////////////////
class CImageExtract 
{
public:
	CImageExtract();
	~CImageExtract();

protected:
	CString m_wsDir,m_wsFile,m_wsPath,m_wsTempPath;
	long m_Width,m_Height;
	WCHAR m_wsBuffer[1000];
	CComPtr<IMalloc> m_pMalloc;
	HBITMAP m_hThumbnail;
	LPBYTE m_pBitmapData;

	// files icons
	HICON mDefaultSmallIcon;
	HICON mDefaultLargeIcon;
	HICON mEmptyFileIcon;

public:
	HRESULT CreateThumbnail(DWORD dwWidth, DWORD dwHeight);
	HRESULT CreateThumbnailVista(DWORD dwWidth, DWORD dwHeight);
	void SaveToFile(/*[in]*/ CString BMPPath);
	void FreeThumbnail();
	void ResolvePath();
	HICON ExtractIcon(LPCTSTR inFileName, bool inSmall);
	bool ExtractThumbnail(/*[out,retval]*/ HBITMAP& phBitmap);
	void SetThumbnailSize(/*[in]*/ long Width,/*[in]*/ long Height);
	void SetFile(/*[in]*/ CString File);
	void SetDir(/*[in]*/ CString Dir);
	void SetPath(/*[in]*/ CString Path);
	CString GetPath();
	CString GetDir();
	CString GetFile();
};

#endif //__FILETHUMBEXTRACT_H_
