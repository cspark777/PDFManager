
#pragma once
#include "afxwin.h"

#include "Define.h"
#include "DebenuPDFLibraryAX0912.h"

BOOL LoadPDFMetadata(CString inFile, PDF_Entry* outEntry)
{
	outEntry->mTitle = _T("");
	outEntry->mAuthor = _T("");
	outEntry->mSubject = _T("");
	outEntry->mKeywords = _T("");
	outEntry->mFastWebWiew = 0;
	outEntry->mPDFVersion = 6;

	/*
	0 = PDF Version
	1 = Author
	2 = Title
	3 = Subject
	4 = Keywords
	5 = Creator
	6 = Producer
	7 = Creation date
	8 = Modification date
	*/

	DebenuPDFLibraryAX0912 QP;
	QP.UnlockKey(_T(QUICKPDF_LICENSE_KEY));
	if(QP.Unlocked() != 1)
	{
		return FALSE;
	}
	try
	{
		long rtn = QP.LoadFromFile(inFile.GetString(),L""); //load a local file//TODO with password
		if(rtn == 0)
		{
			int err = QP.LastErrorCode();
			AfxMessageBox(_T("can not read file, please close all of the files opened."));
		}
		CString strMetadata;
		try
		{
			strMetadata = QP.GetInformation(0).c_str();
			if (strMetadata)
			{
				int lDot = strMetadata.Find(_T("."));
				if (lDot != -1)
				{
					strMetadata.Delete(0, lDot+1);
				}
				outEntry->mPDFVersion = _wtoi(strMetadata);
			}
		}
		catch(...)
		{
		}

	// Get Author
		strMetadata = QP.GetInformation(1).c_str();
		if (strMetadata)
		{
			outEntry->mAuthor = strMetadata;
		}
	
	// Get Title
		strMetadata = QP.GetInformation(2).c_str();
		if (strMetadata)
		{
			outEntry->mTitle = strMetadata; 
		}

	// Get Subject
		strMetadata = QP.GetInformation(3).c_str();
		if (strMetadata)
		{
			outEntry->mSubject = strMetadata;
		}

	// Get Keywords
		strMetadata = QP.GetInformation(4).c_str();
		if (strMetadata)
		{
			outEntry->mKeywords = strMetadata;
		}
	}
	catch(...)
	{
		return FALSE;
	}

	return TRUE;
}
