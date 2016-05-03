#pragma once

#include "stdafx.h"
#include "cdb.h"

class CExportUtils
{
public:
	static bool ExportFilesToFolder(int inCollectionID, CString inFolder , bool inIncludeDB = false);
	static bool ExportFilesToZip(int inCollectionID, CString inZipPath, CString inPassword, bool inIncludeDB = false);

private:
	static bool PrepareFileList(int inCollectionID, CTFiles* outFilesTable);
	
};