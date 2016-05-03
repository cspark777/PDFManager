
#include "CBRepository.h"
#include "CDB.h"
#include "CProgressDlg.h"
#include "COptionsDefs.h"


#ifndef _BNB_ASSISTANT
//#include "CUtils.h"
#include "../../Main/MainFrm.h"
#include "../../Main/CGetInfoDlg.h"
#include "../../Main/CMultipleProfileInformationDlg.h"
#endif

extern CString GUIDgen();

CBRepository::CBRepository()
{
	
}

CBRepository::~CBRepository()
{
}


void CBRepository::Set_ParamForCDB(PARAM_FOR_CDB *pParamCDB)
{
	TCHAR strPath[0x100];
	memset(strPath, 0, 0x100*sizeof(TCHAR));

	m_pParamCDB = pParamCDB;
}

bool CBRepository::InitializePath(LPCTSTR path)
{
	m_strRepositoryPath = path;
	return true;
}

int CBRepository::AddFiles(CStringList & inFileList, CFileProperties_Entry * inPropEntry, bool inSilent)
{
	CFileProperties_Entry lPropEntry;
	if (inPropEntry != NULL)
		lPropEntry = *inPropEntry;

	int lNumFiles = 0;

	POSITION lPos = inFileList.GetHeadPosition();
#ifndef _BNB_ASSISTANT

	CProgressDlg* lProgressDlg = NULL;
	bool lProgMon = (inSilent == false && inFileList.GetCount() > 1);

	if (lProgMon)
	{
		lProgressDlg = new CProgressDlg;
		lProgressDlg->SetMax((long)inFileList.GetCount());
		lProgressDlg->SetCaption(_T("Importing files..."));
		lProgressDlg->CreateProgressDlg();
	}
#endif //_BNB_ASSISTANT

	CIntArray lAddedIds;

	while (lPos)
	{
		CString lSourceFilePath = inFileList.GetNext(lPos);
//		lSourceFilePath.MakeLower(); // Make search case insensitive...

		if (!IsAddThisFileEnabled(lSourceFilePath))
			continue;

		//if (lSourceFilePath.Left(m_Folder.GetLength()).CompareNoCase(m_Folder) == 0)
		//	continue;

		CTFiles lFiles(m_pParamCDB);
		int lFileID = lFiles.GetNextFileID();

		TCHAR lFName[_MAX_FNAME];
		TCHAR lFExt[_MAX_EXT];

		_tsplitpath(lSourceFilePath, NULL, NULL, lFName, lFExt);
/*
		CString strMsg;
		strMsg.Format(_T("try to add library file called %s.bb , are you sure?"),lFName);
		if(wcscmp(lFExt,_T(".bb")) == 0)
		{
			int nRet = MessageBox(NULL,strMsg.GetBuffer() ,_T("Warning"),MB_OKCANCEL);
			if(nRet == IDCANCEL)
			{
				continue;
			}
		}
*/
		if (lPropEntry.mFileTypes.GetCount() > 0)
		{
			if (lPropEntry.mFileTypes.GetIdxByStr(lFExt) == -1)
				continue;
		}

		CString lOrigFileName;
		lOrigFileName.Format(_T("%s%s"), lFName, lFExt);

#ifndef _BNB_ASSISTANT
		if (lProgMon)
		{
			CString lStrMsg;
			lStrMsg.Format(_T("Adding file %s"), /*lOrigFileName*/lSourceFilePath);
			lProgressDlg->SetMessage(lStrMsg);
			lProgressDlg->StepIt();
			if (lProgressDlg->IsCancelled())
				break;
		}
#endif //_BNB_ASSISTANT

		//skontrolujem ci som uz tento subor pridaval
		CFileFind lFinder;
		if (lFinder.FindFile(lSourceFilePath))
		{
			lFinder.FindNextFile();
//			AfxMessageBox(lFinder.GetFileName()); // for debugging only

			CAddedFiles lAddedFiles(m_pParamCDB);
			if (lAddedFiles.WasFileAdded(lSourceFilePath))
				continue;
		}

//		CString outPath;
//		GetUserPath(kPathRepository, outPath, false);

		// --------------------------------------------------
		CString lFilePath;
		if(m_pParamCDB->nIsUseRepository == 1) // Copy the file to the workspace
		{
			lFilePath = GetFilePath(lOrigFileName, -1);
			::CopyFile(lSourceFilePath, lFilePath, false);

		}
		else
		{
			lFilePath = lSourceFilePath; // Use the original path for direct linking and do not copy the file
		}
		// --------------------------------------------------

		CStringList lFileList;
		lFileList.AddHead(lFilePath);

		int lNum = lFiles.AddFile(lFilePath, lOrigFileName, &lPropEntry);

		if (lNum == 0)
		{

			// ------------------------------------
			if(m_pParamCDB->nIsUseRepository == 1) // Delete the file from the workspace
			{
				::DeleteFile(lFilePath);
			}
			// ------------------------------------

		}
		else
		{
			lNumFiles += lNum;
			lAddedIds.Add(lFileID);
			CAddedFiles lAddedFiles(m_pParamCDB);
			lAddedFiles.AddFile(lFileID, lSourceFilePath);
		}
	}
#ifndef _BNB_ASSISTANT
	if (lProgMon)
	{
		lProgressDlg->DestroyWindow();
		delete lProgressDlg; 
	}
#endif //_BNB_ASSISTANT

#ifndef _BNB_ASSISTANT

	if (!inSilent)
	{
		if (lNumFiles > 0)
		{
			if(lAddedIds.GetCount() == 1)
			{
				CGetInfoDlg lGetInfoDlg(&lAddedIds);
				lGetInfoDlg.Set_ParamForCDB(m_pParamCDB);
				lGetInfoDlg.DoModal();
			}
			else
			{
				CMultipleProfileInformationDlg lDlg(&lAddedIds);
				lDlg.Set_ParamForCDB(m_pParamCDB);
				lDlg.DoModal();
			}
		}
	}
#endif //_BNB_ASSISTANT
	return lNumFiles;
}

CString CBRepository::GetFilePath(CString inFileName, int inFileID)
{
	CTFiles lFiles(m_pParamCDB);
	int lFileID = inFileID;
	if (lFileID == -1)
		lFileID = lFiles.GetNextFileID();

	CString lFileName;
	lFileName.Format(_T("%06d_%s"), lFileID, inFileName);

	//repository folder index
	
	::SHCreateDirectoryEx(NULL, m_strRepositoryPath, NULL);

	CString lFilePath = m_strRepositoryPath + lFileName;

	TCHAR lDrv[_MAX_DRIVE];
	TCHAR lFld[_MAX_DIR];
	TCHAR lNam[_MAX_FNAME];
	TCHAR lExt[_MAX_EXT];

	_tsplitpath(lFilePath, lDrv, lFld, lNam, lExt);

	int i = 1;

	CFileFind lFinder;
	while (lFinder.FindFile(lFilePath))
	{
		//taky subor uz existuje, tak pridame <filename>_(x).<ext>
		lFilePath.Format(_T("%s%s%s(%d)%s"), lDrv, lFld, lNam, i, lExt);
		
		i++;
	}
	return lFilePath;
}

bool CBRepository::GetUserPath(int inType, CString & outPath, bool inCreate)
{
	CString lStrPrefName;
	switch (inType)
	{
	case kPathRepository:
		lStrPrefName = kRepositoryFolder;
		break;
	case kPathWorkspace:
		lStrPrefName = kWorkspaceFolder;
		break;
	default:
		return false;
	}

	//bool regFound = false;

	CString lStrFld;

#ifdef _BNB_ASSISTANT // Only applies to the repository -- not the workspace

	HKEY keyHandle;
    DWORD size, Type;
    if( RegOpenKeyEx(HKEY_CURRENT_USER, 
	 _T("SOFTWARE\\Debenu\\Benubird PDF\\Options"), 0, KEY_READ, &keyHandle) == ERROR_SUCCESS)
     {
        size=1023;
        RegQueryValueEx( keyHandle, lStrPrefName, NULL, &Type, (LPBYTE)lStrPrefName.GetBufferSetLength(1024), &size);
         lStrPrefName.ReleaseBuffer();
     }     
     RegCloseKey(keyHandle);

	 lStrFld = lStrPrefName;
#else
	CWinApp* lApp = AfxGetApp();
	lStrFld = lApp->GetProfileString(kOptions, lStrPrefName, _T(""));

	if (lStrFld.GetLength() == 0)
	{
		TCHAR lPath[MAX_PATH+1];
		memset(lPath, 0, sizeof(lPath));

		if (::SHGetFolderPath(NULL, CSIDL_APPDATA | (inCreate == true ? CSIDL_FLAG_CREATE : 0), NULL, SHGFP_TYPE_CURRENT, lPath) != S_OK)
			lStrFld = _T("C:");
		else
			lStrFld = lPath;

		switch (inType)
		{
		case kPathRepository:
			lStrFld += _T("\\Benubird\\Repository");
			break;
		case kPathWorkspace:
			lStrFld += _T("\\Benubird\\Workspace");
			break;
		}

	}
#endif

	CFileFind lFinder;
	if (!lFinder.FindFile(lStrFld))
	{
		//just for sure... create the directory
		::SHCreateDirectoryEx(NULL, lStrFld, NULL);
/*		if (iRet != ERROR_SUCCESS || iRet != ERROR_ALREADY_EXISTS)
		{
			if (inCreate)
				return false;
		}*/
	}

	if (lStrFld.Right(1) != _T("\\"))
		lStrFld.AppendChar('\\');

	outPath = lStrFld;

	return true;

}
void CBRepository::SetRepositoryPath(LPCTSTR path)
{
	m_strRepositoryPath = path;
}
