// BenubirdShellExt.cpp : Implementation of CBenubirdShellExt
#include "stdafx.h"
#include "BenubirdShell.h"
#include "BenubirdShellExt.h"
#include "sn.h"
#include <atlconv.h>  // for ATL string conversion macros


/////////////////////////////////////////////////////////////////////////////
// CBenubirdShellExt 
 
STDMETHODIMP CBenubirdShellExt::Initialize ( 
  LPCITEMIDLIST pidlFolder,
  LPDATAOBJECT pDataObj,
  HKEY hProgID )
{
FORMATETC fmt = { CF_HDROP, NULL, DVASPECT_CONTENT,
                  -1, TYMED_HGLOBAL };
STGMEDIUM stg = { TYMED_HGLOBAL };
HDROP     hDrop;
 
  // Look for CF_HDROP data in the data object. If there

  // is no such data, return an error back to Explorer.

  if ( FAILED( pDataObj->GetData ( &fmt, &stg ) )) 
    return E_INVALIDARG;
 
  // Get a pointer to the actual data.

  hDrop = (HDROP) GlobalLock ( stg.hGlobal );
 
  // Make sure it worked.

  if ( NULL == hDrop )
    return E_INVALIDARG;

  // Sanity check ?make sure there is at least one filename.

UINT uNumFiles = DragQueryFile ( hDrop, 0xFFFFFFFF, NULL, 0 );
HRESULT hr = S_OK;
  
  if ( 0 == uNumFiles )
    {
    GlobalUnlock ( stg.hGlobal );
    ReleaseStgMedium ( &stg );
    return E_INVALIDARG;
    }
 
  // Get the name of the first file and store it in our

  // member variable m_szFile.

  UINT lCount = DragQueryFile (hDrop, 0xFFFFFFFF, NULL, 0);

  if (lCount > 0)
  {
	  for (UINT i = 0; i < lCount; i++)
	  {
		if ( 0 != DragQueryFile ( hDrop, i, m_szFile, MAX_PATH ) )
		{
			m_FileList.AddTail(m_szFile);
		}
	  }
  }
//  if ( 0 == DragQueryFile ( hDrop, 0, m_szFile, MAX_PATH ) )
//    hr = E_INVALIDARG;
 
  GlobalUnlock ( stg.hGlobal );
  ReleaseStgMedium ( &stg );
 
  return hr;

}

BOOL CanConvertToPDF(CString inFilePath)
{
	TCHAR lDrv[_MAX_DRIVE];
	TCHAR lFld[_MAX_DIR];
	TCHAR lNam[_MAX_FNAME];
	TCHAR lExt[_MAX_EXT];
	_tsplitpath_s(inFilePath, lDrv, lFld, lNam, lExt);

	CString lExtStr = lExt;
	if ((lExtStr == _T(".doc")) ||
		(lExtStr == _T(".xls")) ||
		(lExtStr == _T(".ppt")) ||
		(lExtStr == _T(".rtf")) ||
		(lExtStr == _T(".docm")) ||
		(lExtStr == _T(".xps")) ||
		(lExtStr == _T(".jpg")) ||
		(lExtStr == _T(".gif")) ||
		(lExtStr == _T(".tif")) ||
		(lExtStr == _T(".bmp")) ||
		(lExtStr == _T(".png")) ||
		(lExtStr == _T(".ico")) ||
		(lExtStr == _T(".html")) ||
		(lExtStr == _T(".txt")) ||
		(lExtStr == _T(".csv")) ||
		(lExtStr == _T(".docx")) ||
		(lExtStr == _T(".dot")) ||
		(lExtStr == _T(".dotx")) ||
		(lExtStr == _T(".egt")) ||
		(lExtStr == _T(".odt")) ||
		(lExtStr == _T(".ott")) ||
		(lExtStr == _T(".wpd")) ||
		(lExtStr == _T(".wps")) ||
		(lExtStr == _T(".wpt")) ||
		(lExtStr == _T(".wri")) ||
		(lExtStr == _T(".xhtml")) ||
		(lExtStr == _T(".ps")) ||
		(lExtStr == _T(".odp")) ||
		(lExtStr == _T(".otp")) ||
		(lExtStr == _T(".pps")) ||
		(lExtStr == _T(".sti")) ||
		(lExtStr == _T(".sxi")) ||
		(lExtStr == _T(".ods")) ||
		(lExtStr == _T(".ots")) ||
		(lExtStr == _T(".stc")) ||
		(lExtStr == _T(".sxc")) ||
		(lExtStr == _T(".xlk")) ||
		(lExtStr == _T(".xlsx")) ||
		(lExtStr == _T(".psd")) ||
		(lExtStr == _T(".tiff")) ||
		(lExtStr == _T(".sxw")) ||
		(lExtStr == _T(".htm")) ||
		(lExtStr == _T(".jpeg")))
		return TRUE;
	else 
		return FALSE;
}


HRESULT CBenubirdShellExt::QueryContextMenu (
  HMENU hmenu, UINT uMenuIndex, UINT uidFirstCmd,
  UINT uidLastCmd, UINT uFlags )
{
	// If the flags include CMF_DEFAULTONLY then we shouldn't do anything.

	if ( uFlags & CMF_DEFAULTONLY )
		return MAKE_HRESULT ( SEVERITY_SUCCESS, FACILITY_NULL, 0 );


// Calculate the number of menu items
	UINT uintMenuItemCount = GetMenuItemCount(hmenu);
	UINT iMenuItemID = 0;
	UINT fType;
	MENUITEMINFO menuItemInfo;
		
// Loop through until we find the first separator
	for(iMenuItemID = 1; iMenuItemID < uintMenuItemCount-1; iMenuItemID++)
	{
		memset(&menuItemInfo, 0, sizeof menuItemInfo);
		menuItemInfo.cbSize = sizeof(MENUITEMINFO);
		menuItemInfo.fMask = MIIM_TYPE;
		GetMenuItemInfo(hmenu, iMenuItemID, TRUE, &menuItemInfo);	
		fType = menuItemInfo.fType;

		if(menuItemInfo.fType == MFT_SEPARATOR)
			break;
	}

// Create our own submenu
	HMENU lSubMenu = CreateMenu();
	InsertMenu(lSubMenu, 0, MF_BYPOSITION, uidFirstCmd, _T("Add to Benubird PDF"));
	InsertMenu(lSubMenu, 1, MF_BYPOSITION, uidFirstCmd+1, _T("Add to Benubird PDF as PDF"));

	InsertMenu(hmenu, iMenuItemID, MF_SEPARATOR | MF_BYPOSITION, NULL, _T("") );
	InsertMenu(hmenu, iMenuItemID+1, MF_POPUP | MF_BYPOSITION, (UINT_PTR)lSubMenu, _T("Benubird PDF") );
	InsertMenu(hmenu, iMenuItemID+2, MF_SEPARATOR | MF_BYPOSITION, NULL, _T("") );

	// can convert to PDF?
	BOOL lCanConvert = TRUE; 
	POSITION lPos = m_FileList.GetHeadPosition();	
	while(lPos && lCanConvert)
	{
		CString lTmpStr = m_FileList.GetNext(lPos);
		lCanConvert = lCanConvert && CanConvertToPDF(lTmpStr);
	}
	if (lCanConvert == FALSE) 
		EnableMenuItem(lSubMenu, 1, MF_BYPOSITION | MF_GRAYED);
	
	if (sn::state() == sn::eStateExpired)
	{
		//EnableMenuItem(lSubMenu, 0, MF_BYPOSITION | MF_GRAYED);
		//EnableMenuItem(lSubMenu, 1, MF_BYPOSITION | MF_GRAYED);
	}

	//InsertMenu ( hmenu, uMenuIndex, MF_BYPOSITION,
	//           uidFirstCmd, _T("Add to Benubird PDF") );
 
	return MAKE_HRESULT ( SEVERITY_SUCCESS, FACILITY_NULL, 3 );
}


/*HRESULT CBenubirdShellExt::GetCommandString (
  UINT idCmd, UINT uFlags, UINT* pwReserved,
  LPSTR pszName, UINT cchMax )*/

HRESULT CBenubirdShellExt::GetCommandString (
  UINT_PTR idCmd, UINT uFlags, UINT* pwReserved,
  LPSTR pszName, UINT cchMax )
{
USES_CONVERSION;
 
  // Check idCmd, it must be 0 since we have only one menu item.
OutputDebugString(_T("PPPPPPPPPPPPPP KKKKKKKKKKKKKKKKK"));
TCHAR str[0x100] = {0};
sprintf(str,_T("AAAAAAAAAAAAAAAAAAAAAAAAA  %d"),idCmd);
OutputDebugString(str);

  if ( 0 != idCmd )
    return E_INVALIDARG;
 
  // If Explorer is asking for a help string, copy our string into the

  // supplied buffer.

  if ( uFlags & GCS_HELPTEXT )
    {
    LPCTSTR szText = _T("Add files to Benubird library");
 
    if ( uFlags & GCS_UNICODE )
      {
      // We need to cast pszName to a Unicode string, and then use the

      // Unicode string copy API.

      lstrcpynW ( (LPWSTR) pszName, T2CW(szText), cchMax );
      }
    else
      {
      // Use the ANSI string copy API to return the help string.

      lstrcpynA ( pszName, T2CA(szText), cchMax );
      }
 
    return S_OK;
    } 
 
  return E_INVALIDARG;
}

void StartBenubird()
{
	HWND wnd = NULL;
	wnd = FindWindowA("Benubird PDF Class Name",NULL);

	HWND wnd1 = NULL;
	wnd1 = FindWindowA("Benubird PDF Pro Class Name",NULL);

	if((wnd != NULL)||(wnd1 != NULL))
	{
		OutputDebugString(_T("------------------------  Benubird already run"));
		return;
	}

	OutputDebugString(_T("------------------------  Benubird new run"));

  char szDLLPath[MAX_PATH] = "";
  HMODULE hinst;
  GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, "", &hinst);
  ::GetModuleFileName((HINSTANCE)hinst, szDLLPath, sizeof(szDLLPath) - 1);

  // Extract directory
  PathRemoveFileSpec(szDLLPath);
  CString strDLLDirectory = szDLLPath;
  strDLLDirectory = "\"" + strDLLDirectory + "\\BenubirdPDF.exe\"";
  ::ShellExecute(NULL, "open", strDLLDirectory, NULL, NULL, SW_SHOW);
}

HRESULT CBenubirdShellExt::InvokeCommand (
  LPCMINVOKECOMMANDINFO pCmdInfo )
{
	// If lpVerb really points to a string, ignore this function call and bail out.
	if ( 0 != HIWORD( pCmdInfo->lpVerb ) )
		return E_INVALIDARG;
 
	StartBenubird(); // Start Benubird prior to adding files so that 
			// it remains open after the process is complete

	// Get the command index - the only valid one is 0/1.
	switch ( LOWORD( pCmdInfo->lpVerb ) )
	{

		case 0:
		{	
			if (!m_BenubirdStarted)
			{
				m_Benubird.CreateDispatch(_T("Benubird.Application"));
				m_Benubird.Show();
				m_BenubirdStarted = TRUE;
			}

			POSITION lPos = m_FileList.GetHeadPosition();	
			while(lPos)
			{
				CString lTmpStr = m_FileList.GetNext(lPos);
				m_Benubird.AddFile(lTmpStr);
				//m_Benubird.AddFile(CComBSTR(lTmpStr));
			}

			//	  BSTR  lBstr = SysAllocString(lFile);
			//	  lBenubird.AddFile(m_szFile);
			//	  SysFreeString(lBstr);
			//      wsprintf ( szMsg, _T("The selected file was:\n\n%s"), m_szFile );
			//      MessageBox ( pCmdInfo->hwnd, szMsg, _T("SimpleShlExt"), 
			//                   MB_ICONINFORMATION );
			m_Benubird.ReleaseDispatch();
			m_BenubirdStarted = false;
			

			return S_OK;
		}
		break;

		case 1:
		{
			if (!m_BenubirdStarted)
			{
				m_Benubird.CreateDispatch(_T("Benubird.Application"));
				m_Benubird.Show();
				m_BenubirdStarted = TRUE;
			}

			CString lStr = _T("");
			POSITION lPos = m_FileList.GetHeadPosition();	
			while(lPos)
			{
				CString lTmpStr = m_FileList.GetNext(lPos);
				if (lStr.IsEmpty())
					lStr = lTmpStr;
				else
					lStr += _T("####") + lTmpStr;
			}
//			m_Benubird.ConvertToPDF(CComBSTR(lStr));
			m_Benubird.ConvertToPDF(lStr);

			m_Benubird.ReleaseDispatch();
			m_BenubirdStarted = false;


			/*
			long lNumFiles = m_F ileList.GetCount();
			long lIndex = 0;
			LPCTSTR* lFileList = (LPCTSTR*) malloc(lNumFiles * sizeof(LPCTSTR));
			POSITION lPos = m_FileList.GetHeadPosition();	
			while(lPos)
			{
				CString lTmpStr = m_FileList.GetNext(lPos);
				WCHAR* lStr = (WCHAR*)malloc(MAX_PATH*sizeof(WCHAR));
				wcscpy(lStr, lTmpStr);
				lFileList[lIndex++] = lStr; 
 			}
*/

//			m_Benubird.ConvertToPDF(arr);
 

			//BSTR  lBstr = SysAllocString(lFile);
			//	  lBenubird.AddFile(m_szFile);
			//	  SysFreeString(lBstr);
			//      wsprintf ( szMsg, _T("The selected file was:\n\n%s"), m_szFile );
			//      MessageBox ( pCmdInfo->hwnd, szMsg, _T("SimpleShlExt"),
			//                   MB_ICONINFORMATION );

//			StartBenubird();
			return S_OK;
		}
		break;

		default:
			return E_INVALIDARG;
		break;
	}
}

STDMETHODIMP CBenubirdShellExt::HandleMenuMsg ( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
  AFX_MANAGE_STATE(AfxGetStaticModuleState());
  return HandleMenuMsg2(uMsg, wParam, lParam, NULL);
}

STDMETHODIMP CBenubirdShellExt::HandleMenuMsg2 ( UINT uMsg, WPARAM wParam, LPARAM lParam,
                                              LRESULT* pResult )
{
  AFX_MANAGE_STATE(AfxGetStaticModuleState());
	switch(uMsg)
	{
		case WM_INITMENUPOPUP:
			//g_hMenu = (HMENU) wParam;
			break;

	    case WM_DRAWITEM:

			//DrawMenuItem((LPDRAWITEMSTRUCT) lParam);
			break;

		case WM_MEASUREITEM:
			//MeasureItem((LPMEASUREITEMSTRUCT) lParam);
			break;
	}
	 
	return S_OK;
}

