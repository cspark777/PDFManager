#include "stdafx.h"
#include "CUtils.h"
#include "MainFrm.h"
#include "CRegistry.h"
#include "CTagsPanel.h"
#include "CImportFolderDlg.h"
#include "CProgressDlg.h"
#include "COptionsDefs.h"
#include "CBRepository.h"
#include "CBWorkspace.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//*****************************************************************************
// class CUtils 
//*****************************************************************************
LPCTSTR CUtils::UFileSize2Str(CString &outStr, DWORD inHi, DWORD inLo)
{
	double Hi = inHi;
	double Lo = inLo;
	if(inHi == 0)
	{
		if(inLo < 1024)
			outStr.Format(_T("%.f B"), Lo);
		else if(inLo < (1024 * 1024))
			outStr.Format(_T("%.000f KB"), Lo/1024);
		else 
			outStr.Format(_T("%.000f MB"), Lo/(1024 *1024));
	}
	else
	{
		outStr.Format(_T("%.000f GB"), ((Hi * MAXDWORD) + Lo)/(1024 * 1024 * 1024));	
	}
	return outStr;
}

BOOL CUtils::IsPDF(CString inFilePath)
{
	CPath lPath(inFilePath);
	CString lFileExt = lPath.GetExtName();
	lFileExt.MakeLower();
	if (lFileExt == _T("pdf"))
	{
		return TRUE;
	}

	return FALSE;
}


//*************************************************************************************************
// File list must contain full path and filename strings separated by "\0" and
// terminated  by "\0\0". Or a single path and filename terminated by "\0\0". 
BOOL CUtils::SendFileToTheRecycleBin(LPCTSTR lpszFilePathList)
{
	SHFILEOPSTRUCT shfos;

	shfos.hwnd = AfxGetMainWnd()->GetSafeHwnd();
	shfos.wFunc = FO_DELETE;
	shfos.pFrom = lpszFilePathList;
	shfos.pTo = NULL;
	shfos.fFlags = FOF_ALLOWUNDO | FOF_NOCONFIRMATION | FOF_SILENT | FOF_NOERRORUI;
	shfos.fAnyOperationsAborted = FALSE;
	shfos.hNameMappings = NULL;
	shfos.lpszProgressTitle = NULL;

	return ((SHFileOperation(&shfos) == 0) && (shfos.fAnyOperationsAborted == 0));
}
