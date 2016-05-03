#pragma once

#include "CDB.h"


class COrganizer
{
public:
	static bool Start();
	static void End();
public:
	COrganizer();
	~COrganizer();

	bool GetBenubirdFolder(LPTSTR fldBuf, int maxBuf, bool create = TRUE);
	bool Initialize();

	int OrganizeFiles(CStringList & inFilePaths, CTWatchedFolder_Entry * inFolderEntry = NULL, CIntArray* outAddedFilesArray = NULL, bool inUseProgMon = true);
	bool OrganizeFile(CString inFilePath);

	bool FindFilesByName(CString inFileName, CStringList & outFileNames);

private:
	CString m_B_path;
	TCHAR m_B_drv[_MAX_DRIVE];
	TCHAR m_B_fld[_MAX_DIR];
	/*!
		Check if the file path is the B-Folder or it's subfolder
	*/
	bool IsInOrganizedFolder(CString inFilePath);

	friend class CImportUtils;
};

extern COrganizer* gOrganizer;