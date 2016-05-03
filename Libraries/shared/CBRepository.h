#pragma once

#include "cdb.h"

class CBRepository
{
public: 
	enum {kPathRepository, kPathWorkspace};
	static bool GetUserPath(int inType, CString & outPath, bool inCreate);

public:

	PARAM_FOR_CDB *m_pParamCDB;
	void Set_ParamForCDB(PARAM_FOR_CDB *pParamCDB);
public:
	CBRepository();
	~CBRepository();

	bool InitializePath(LPCTSTR path);
	void SetRepositoryPath(LPCTSTR path);
	int AddFiles(CStringList & inFileList, CFileProperties_Entry * inPropEntry = NULL, bool inSilent = false);

	CString GetFilePath(CString inFileName, int inFileID = -1);

public:
	CString m_strRepositoryPath; 
};

