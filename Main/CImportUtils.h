#pragma once

#include "stdafx.h"
#include "CDB.h"

class CImportUtils
{
public:
	static int ImportFilesFromZip(CString inZipPath, CString inPassword, CFileProperties_Entry * inFileProperties);
	static int ImportFilesFromFolder(CString & inFolder, bool inIncludeSubfolders, CFileProperties_Entry* inFileProperties);

private:
	static int ImportFromFileList(CStringList & inFileList, CFileProperties_Entry* inFileProperties);

};