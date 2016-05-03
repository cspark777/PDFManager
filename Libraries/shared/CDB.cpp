
#include <BCGCBProInc.h>			// BCGPro Control Bar
#include "CDB.h"
#include "CLog.h"
#include "Define.h"
//#include "COrganizer.h"
#include "CProgressDlg.h"
//#include "CUtils.h"

#include "COptionsDefs.h"
//#include "CBRepository.h"
//#include "CPDFPropertiesFrame.h"

//FILE PRoperties
#include <windows.h>
#include <ole2.h>

#include <Lm.h>

#include "DebenuPDFLibraryAX0912.h"

#ifdef _BNB_ASSISTANT
#include "strsafe.h"

//Benubird assistand definitions
extern void PeekMessages();
extern bool gTimersPaused;

#endif _BNB_ASSISTANT

// Implicitly link ole32.dll
#pragma comment( lib, "ole32.lib" )

#ifdef _BNB_ASSISTANT
CString GUIDgen()
{
 GUID guid;
 CoCreateGuid(&guid);

 WORD * str;
 UuidToString((UUID*)&guid, &str);

 CString unique((LPTSTR)str);

 RpcStringFree(&str);

 unique.Replace(_T("-"), _T("_"));

 return unique;
}

CString GetDefaultPath(CString key)
{
	CString result;
	result.Empty();

	TCHAR lDBPath[MAX_PATH + 10];
	memset(lDBPath, 0, (MAX_PATH + 10) * sizeof(TCHAR));

	if (::SHGetSpecialFolderPath(NULL, lDBPath, CSIDL_APPDATA, TRUE) == FALSE) 
	{
		return result;		
	}
	
	if (::PathAppend(lDBPath, _T("Benubird\\")) == FALSE) 
	{
		return result;
	}
	CreateDirectory(lDBPath, NULL);

	if(::PathAppend(lDBPath, key.GetBuffer()) == FALSE) 
	{
		return result;
	}
	CreateDirectory(lDBPath, NULL);
	result = lDBPath;
	return result;	
}
#endif 



#ifndef _BNB_ASSISTANT

extern CString GUIDgen();
extern CString GetDefaultPath(CString key);
extern int g_nMaximumFileCount;
#endif
//CWFolderManager	gWFolderManager;

//string list holding the names of common system folder

////////////////////////////////////////////////////////////////////////////////////////
// class CDBSQL
////////////////////////////////////////////////////////////////////////////////////////
CDBSQL::CDBSQL()
{

}

CDBSQL::~CDBSQL()
{
}

bool CDBSQL::CopyStringList(CStringList* destList, CStringList* sourceList)
{
	POSITION lPos;
	if (!sourceList->IsEmpty())
	{
		lPos = sourceList->GetHeadPosition();
		while (lPos != NULL)
		{
			CString lStr = sourceList->GetAt(lPos);
			destList->AddTail(lStr);
			sourceList->GetNext(lPos); 

		}
	}
	return true;
}

bool CDBSQL::Copy(CDBSQL* destDBSQL)
{
	destDBSQL->Clear();

	CopyStringList(&destDBSQL->mField, &mField);
	CopyStringList(&destDBSQL->mTable, &mTable);
	CopyStringList(&destDBSQL->mCondition, &mCondition);
	CopyStringList(&destDBSQL->mSpecCondition, &mSpecCondition);
	CopyStringList(&destDBSQL->mGroup, &mGroup);
	CopyStringList(&destDBSQL->mSort, &mSort);
	CopyStringList(&destDBSQL->mLimit, &mLimit);

	return true;
}

void CDBSQL::Clear()
{
	mField.RemoveAll();
	mTable.RemoveAll();
	mCondition.RemoveAll();
	mSpecCondition.RemoveAll();
	mGroup.RemoveAll();
	mSort.RemoveAll();
	mLimit.RemoveAll();
}

void CDBSQL::AddField(CString inStr)
{
	if (mField.Find(inStr) == NULL)
		mField.AddTail(inStr);
}

void CDBSQL::AddTable(CString inStr)
{
	if (mTable.Find(inStr) == NULL)
		mTable.AddTail(inStr);
}

void CDBSQL::AddCondition(CString inStr)
{
	if (mCondition.Find(inStr) == NULL)
		mCondition.AddTail(inStr);
}

void CDBSQL::AddSpecCondition(CString inStr)
{
	if (mSpecCondition.Find(inStr) == NULL)
		mSpecCondition.AddTail(inStr);
}

void CDBSQL::AddGroup(CString inStr)
{
	if (mGroup.Find(inStr) == NULL)
		mGroup.AddTail(inStr);
}

void CDBSQL::AddSort(CString inStr)
{
	if (mSort.Find(inStr) == NULL)
		mSort.AddTail(inStr);
}

void CDBSQL::AddLimit(CString inStr)
{
	if (mLimit.Find(inStr) == NULL)
		mLimit.AddTail(inStr);
}

CString CDBSQL::GetSelect()
{
	if (mTable.IsEmpty())
		return _T(""); 
	POSITION lPos;
	CString lResult = _T("");
	
	try
	{
		// add fields
		lResult += _T("select ");
		if (mField.IsEmpty())
			lResult += _T("* ");
		else
		{
			lPos = mField.GetHeadPosition();
			while (lPos != NULL)
			{
				CString lStr = mField.GetAt(lPos);
				mField.GetNext(lPos); 

				lResult += lStr; 
				if (lPos == NULL)
					lResult += _T(" "); 
				else
					lResult += _T(", "); 
				}
		}

		// add tables
		lResult += _T("from "); 
		lPos = mTable.GetHeadPosition();
		while (lPos != NULL)
		{
			CString lStr = mTable.GetAt(lPos);
			mTable.GetNext(lPos); 

			lResult += lStr; 
			if (lPos == NULL)
				lResult += _T(" "); 
			else
				lResult += _T(", "); 
		}

		// add conditions
		if (!mCondition.IsEmpty())
		{
			lResult += _T("where "); 
			lPos = mCondition.GetHeadPosition();
			while (lPos != NULL)
			{
				CString lStr = mCondition.GetAt(lPos);
				mCondition.GetNext(lPos); 

				lResult += lStr; 
				if (lPos == NULL)
					lResult += _T(" ");
				else
					lResult += _T(" and ");
			}
		}

		// add spec condition 
		if (!mSpecCondition.IsEmpty())
		{
			if (mCondition.IsEmpty())
				lResult += _T("where "); 
			else
				lResult += _T(" and ");
			lPos = mSpecCondition.GetHeadPosition();
			while (lPos != NULL)
			{
				CString lStr = mSpecCondition.GetAt(lPos);
				mSpecCondition.GetNext(lPos); 

				lResult += lStr; 
				if (lPos == NULL)
					lResult += _T(" ");
				else
					lResult += _T(" and ");
			}
		}

		// group by
		if (!mGroup.IsEmpty())
		{
			lResult += _T("group by "); 
			lPos = mGroup.GetHeadPosition();
			while (lPos != NULL)
			{
				CString lStr = mGroup.GetAt(lPos);
				mGroup.GetNext(lPos); 

				lResult += lStr; 
				if (lPos == NULL)
					lResult += _T(" "); 
				else
					lResult += _T(", "); 
			}
		}

		// sort
		if (!mSort.IsEmpty())
		{
			lResult += _T("order by ");  // 
			lPos = mSort.GetHeadPosition();
			while (lPos != NULL)
			{
				CString lStr = mSort.GetAt(lPos);
				mSort.GetNext(lPos); 

				lResult += lStr; 
				if (lPos == NULL)
					lResult += _T(" "); 
				else
					lResult += _T(", "); 
			}
		}

		// add sort with limit 
		if (!mLimit.IsEmpty())
		{
			lPos = mLimit.GetHeadPosition();
			while (lPos != NULL)
			{
				CString lStr = mLimit.GetAt(lPos);
				mLimit.GetNext(lPos); 

				lResult += lStr; 
				if (lPos == NULL)
					lResult += _T(" ");
				else
					lResult += _T(", ");
			}
		}

		lResult += _T(";");
	}
	catch(...)
	{
		return _T("");
	}
	return lResult;
}

LPCTSTR EncodeString(LPCTSTR inStr, CString &outEncodedStr)
{
	outEncodedStr = inStr;
	outEncodedStr.Replace(_T("'"), _T("''"));
	return (LPCTSTR)outEncodedStr;
}

////////////////////////////////////////////////////////////////////////////////////////
// class CIDData
////////////////////////////////////////////////////////////////////////////////////////
LPCTSTR Filetime2Timestamp(FILETIME inFiletime, CString &outTimeStamp)
{
	SYSTEMTIME lSysTime;
	FileTimeToSystemTime(&inFiletime, &lSysTime);
	outTimeStamp.Format(_T("%d-%02d-%02d %02d:%02d:%02d"),  lSysTime.wYear, lSysTime.wMonth, lSysTime.wDay, lSysTime.wHour, lSysTime.wMinute, lSysTime.wSecond);
	return outTimeStamp;
}

CIDData::CIDData(CIDData & inData)
{
	mIDs.Append(inData.mIDs);
	mStrs.AddTail(&inData.mStrs);
}

CIDData::CIDData(CIDData * inData)
{
	mIDs.Append(inData->mIDs);
	mStrs.AddTail(&inData->mStrs);
}

bool CIDData::GetDataByIdx(int inIdx, int &outID, CString &outStr)
{
	if(mIDs.GetCount() <= inIdx)
		return false;
	if(mStrs.GetCount() <= inIdx)
		return false;

	outID = mIDs.GetAt(inIdx);
	POSITION lPos = mStrs.FindIndex(inIdx);
	outStr = mStrs.GetAt(lPos);
	return true;
}

int CIDData::GetIdxByID(int inID)
{	
	int lCount = (int)mIDs.GetCount();
	int lID; 
	for(int i = 0; i < lCount; i++)
	{
		lID = mIDs.GetAt(i);
		if(lID == inID)
			return i;
	}

	return -1;
}
int CIDData::GetIdxByStr(LPCTSTR inStr)
{
	int lIdx = 0;
	CString lStr; 
	POSITION lPos;
	lPos = mStrs.GetHeadPosition();

	while(lPos)
	{
		lStr = mStrs.GetNext(lPos);
		if (inStr[0] == '.')
			lStr.Insert(0, _T("."));

		if(lStr == inStr)
			return lIdx;
		lIdx++;
	}
	return -1;
}

int CIDData::GetCount()
{
	return (int)mIDs.GetCount();
}

LPCTSTR CIDData::GetStringsFormatted(CString &outString, LPCTSTR inSeparator /*= _T("; ")*/)
{
	POSITION lPos = mStrs.GetHeadPosition();
	CString lStr;
	outString = _T("");
	while(lPos)
	{
		lStr = mStrs.GetNext(lPos);
		if(lPos != NULL)
			outString = outString + lStr + inSeparator;
		else
			outString = outString + lStr;
	}

	return outString;
}

int CIDData::AddData(int inID, LPCTSTR inStr)
{
	mIDs.Add(inID);
	mStrs.AddTail(inStr);

	return (int)mIDs.GetCount();
}
void CIDData::RemoveAt(int inIdx)
{
	mIDs.RemoveAt(inIdx);
	POSITION lPos = mStrs.FindIndex(inIdx);
	mStrs.RemoveAt(lPos);
}

void CIDData::RemoveAll()
{
	mIDs.RemoveAll();
	mStrs.RemoveAll();
}

bool  CIDData::HasEqualIDs(CIDData &inCIDData)
{
	int lMemberCount = GetCount();
	int lInCoutn = inCIDData.GetCount();

	if(lMemberCount != lInCoutn) //must have a same IDs
		return false;

	for(int i = 0; i < lInCoutn; i++)  //get the IDs from the input and find them in the memeber
	{
		if(GetIdxByID(inCIDData.mIDs.GetAt(i)) == -1)
			return false; //we can't find it in the memebers
	}
	return true;
}
/*
int CTagsIDData::GetFromFormattedString(CString inString, LPCTSTR inSeparator)
{
	CString lString = inString;
	CString lSep = inSeparator;

	int lCount = 0;
	while (true)
	{
		int lPos = lString.Find(lSep);
		CString lStr = lString;
		if (lPos >= 0)
		{
			lStr = lString.Left(lPos-1);
			lString.Delete(0, lPos + lSep.GetLength());
		}

		CTTags lTags;
		int lID = lTags.GetIDByName(lStr);
		if (lID != -1)
		{
			this->AddData(lID, lStr);
			lCount++;
		}
	}
	return lCount;
}
*/

////////////////////////////////////////////////////////////////////////////////////////
// class CDBTable
////////////////////////////////////////////////////////////////////////////////////////
CDBTable::CDBTable()
{
}
CDBTable::CDBTable(CDB *pDB)
{
	m_pDB = pDB;
}
CDBTable::~CDBTable()
{
}

bool CDBTable::LoadNextRow()
{
	try
	{
		mDataIsValid = false;
		mQ.nextRow();	
		LoadCurrentRow();
	}
	catch(...)
	{
		return false;
	}
	return true;
}

bool CDBTable::ExecSelect(CString inSQL)
{
	try
	{
		mQ = m_pDB->mDB.execQuery(inSQL);
	}
	catch(...)
	{
		return false;
	}

	LoadCurrentRow();
	return true;
}


////////////////////////////////////////////////////////////////////////////////////////
// class CTWatchedFolder
////////////////////////////////////////////////////////////////////////////////////////
CTWatchedFolder::CTWatchedFolder()
{
}
CTWatchedFolder::CTWatchedFolder(CDB*pDB):CDBTable(pDB)
{
	m_pDB = pDB;
}
CTWatchedFolder::~CTWatchedFolder()
{

}

bool CTWatchedFolder::GetWatchedFolders()
{
	try
	{
		mQ = m_pDB->mDB.execQuery(_T("select * from WFolders"));
	}
	catch(...)
	{
		return false;
	}

	LoadCurrentRow();
	return true;

}

bool CTWatchedFolder::GetWatchedFoldersByFreq(int inFrequency)
{
	try
	{
		CString lSQL;
		lSQL.Format(_T("select * from WFolders where Frequency = %d"), inFrequency);
		mQ = m_pDB->mDB.execQuery(lSQL);
	}
	catch(...)
	{
		return false;
	}

	LoadCurrentRow();
	return true;

}

bool CTWatchedFolder::GetWatchedFolderByID(DWORD_PTR inWFolderID)
{
	try
	{	CString lSQL;
		lSQL.Format(_T("select * from WFolders where idWFolder = %u"), inWFolderID);
		mQ = m_pDB->mDB.execQuery(lSQL);
	}
	catch(...)
	{
		return false;
	}

	LoadCurrentRow();
	return true;

}
bool CTWatchedFolder::AddNewWatchedFolder(CIntArray *inFExtArray, CIntArray *inTagsArray)
{
	try
	{
		CppSQLite3Query lQ;
		CString lSQL;

		if(mID <= 0) //new WFolder
		{
			lSQL = _T("select max(idWFolder) from WFolders;"); 
			lQ = m_pDB->mDB.execQuery(lSQL); 
			if(lQ.eof())
				mID = 1;
			else
				mID = lQ.getIntField(0) + 1;
		//		continue; //do not add files more time

			CString lEncPath;
			EncodeString(mPath, lEncPath);
			lSQL.Format(_T("insert into WFolders values (%u, '', '%s', %u, %d, %u, %d, %d, '%s', %d, '%s', %d, '%s', %d, '%s', %d, '%s', %d)"), 
				mID,
				lEncPath,
				mIncludeSubfolders,
				mAllFileType,
				mCollectionID,
				mCheckFrequency,
				mUseTitle,
				mTitle,
				mUseAuthor,
				mAuthor,
				mUseSubject,
				mSubject,
				mUseCategories,
				mCategories,
				mUseComments,
				mComments,
				mUseTags
				);
		}
		else //update an existing WFolder
		{
			CString lEncPath;
			EncodeString(mPath, lEncPath);
			lSQL.Format(_T("update WFolders set Path = '%s', IncludeSubfolders = %u, AllFileType = %d, Collections_idCollection = %u, Frequency = %d, UseTitle = %d, Title = '%s', UseAuthor = %d, Author = '%s', UseSubject = %d, Subject = '%s', UseCategories = %d, Categories = '%s', UseComments = %d, Comments = '%s', UseTags = %d where idWFolder = %u;"), 
				lEncPath,
				mIncludeSubfolders,
				mAllFileType,
				mCollectionID,
				mCheckFrequency,
				mUseTitle,
				mTitle,
				mUseAuthor,
				mAuthor,
				mUseSubject,
				mSubject,
				mUseCategories,
				mCategories,
				mUseComments,
				mComments,
				mUseTags,
				mID
				);

			CString lSQL2;
			//remove the existing extensions
			lSQL2.Format(_T("delete from WFolders_has_FExtensions where  WFolders_idWFolder = %d;"), mID);
			m_pDB->mDB.execDML(lSQL2);

			//remove the existing tags
			lSQL2.Format(_T("delete from WFolders_has_Tags where  WFolders_idWFolder = %d;"), mID);
			m_pDB->mDB.execDML(lSQL2);
		}

		m_pDB->mDB.execDML(lSQL);

		//add the specified extensions
		if(inFExtArray != NULL)
		{
			int lCount = (int)inFExtArray->GetCount();
			int lFExtID = 0;
			for(int i = 0; i < lCount; i++)
			{
				lFExtID = inFExtArray->GetAt(i);
				m_pDB->AddFExtToWFolder(lFExtID, mID); 
			}
		}

		//add the specified tags
		if(inTagsArray != NULL)
		{
			int lCount = (int)inTagsArray->GetCount();
			int lTagID = 0;
			for(int i = 0; i < lCount; i++)
			{
				lTagID = inTagsArray->GetAt(i);
				m_pDB->AddTagToWFolder(lTagID, mID); 
			}
		}
	}
	catch(...)
	{
		return false;
	}

	return true;
}


bool CTWatchedFolder::LoadCurrentRow()
{
	try
	{
		mDataIsValid = false;
		if(!mQ.eof())
		{
			mID = mQ.getIntField(0);
			mName = mQ.getStringField(1);
			mPath = mQ.getStringField(2);
			mIncludeSubfolders = (mQ.getIntField(3) != 0);
			mAllFileType = (mQ.getIntField(4) != 0);
			mCollectionID = mQ.getIntField(5);
			mCheckFrequency = mQ.getIntField(6);

			mUseTitle = mQ.getIntField(7);
			mTitle = mQ.getStringField(8);
			mUseAuthor = mQ.getIntField(9);
			mAuthor = mQ.getStringField(10);
			mUseSubject = mQ.getIntField(11);
			mSubject = mQ.getStringField(12);
			mUseCategories = mQ.getIntField(13);
			mCategories = mQ.getStringField(14);
			mUseComments = mQ.getIntField(15);
			mComments = mQ.getStringField(16);
			mUseTags = mQ.getIntField(17);
			mDataIsValid = true;
		}
	}
	catch(...)
	{
		return false;
	}
	return true;
}

bool CTWatchedFolder::DeleteByID(int inWFolderID)
{
	try
	{
		CppSQLite3Query lQ;

		CString lSQL;
		lSQL.Format(_T("delete from WFolders where idWFolder = %u"), inWFolderID);
		m_pDB->mDB.execDML(lSQL);
	}
	catch(...)
	{
		return false;
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////
// class CTFiles
////////////////////////////////////////////////////////////////////////////////////////
CTFiles::CTFiles()
{ 
	m_ProgressDlg = NULL; 
}

CTFiles::CTFiles(PARAM_FOR_CDB *pParamCDB):CDBTable(pParamCDB->pDB)
{ 
	m_pDB = pParamCDB->pDB ; 
	m_pSystemDirsList = pParamCDB->pSystemDirsList;
	m_ProgressDlg = NULL; 
}

bool CTFiles::LoadDataFromFile()
{
	ASSERT(mPath != _T("")); //mPath must be initialized

	LoadSummaryInfo();
	LoadAutoSummaryInfo();

	HANDLE lHFind = FindFirstFile(mPath, &mFindData);	

	mFlags = 0;
	if (lHFind == INVALID_HANDLE_VALUE) 
	{
		mFlags = FILE_FLAG_FNF;
	}
	else
		FindClose(lHFind);

	TCHAR drv[_MAX_DRIVE];
	TCHAR fld[_MAX_DIR];
	TCHAR nam[_MAX_FNAME];
	TCHAR ext[_MAX_EXT];

	_tsplitpath_s(mPath, drv, fld, nam, ext);

	CString lExtStr = ext;
//	mName = nam + lExtStr;
/*
	CString lExtStr = mFindData.cFileName;
	lExtStr = lExtStr.Right(lExtStr.GetLength() - lExtStr.ReverseFind('.') - 1);
*/
	CTFExtensions lTFExt(m_pDB);
	mExtID = lTFExt.GetExtensionIDWithCreate(lExtStr.MakeLower());
	return true;
}

bool CTFiles::LoadCurrentRow()
{
	try
	{
		mDataIsValid = false;
		if(!mQ.eof())
		{
			mID = mQ.getIntField(0);
			mPath = mQ.getStringField(1);
			mName =  mQ.getStringField(2);
			mFindData.nFileSizeHigh = mQ.getIntField(3);
			mFindData.nFileSizeLow = mQ.getIntField(4);
			mFindData.ftCreationTime.dwHighDateTime = mQ.getIntField(5);
			mFindData.ftCreationTime.dwLowDateTime = mQ.getIntField(6);
			mFindData.ftLastWriteTime.dwHighDateTime = mQ.getIntField(7);
			mFindData.ftLastWriteTime.dwLowDateTime = mQ.getIntField(8);
			mFindData.ftLastAccessTime.dwHighDateTime = mQ.getIntField(9);
			mFindData.ftLastAccessTime.dwLowDateTime = mQ.getIntField(10);
			mFindData.dwFileAttributes = mQ.getIntField(11);
			mTitle = mQ.getStringField(12);
			mSubject = mQ.getStringField(13);
			mAuthor = mQ.getStringField(14);
			mCategory = mQ.getStringField(15);
			mComments = mQ.getStringField(16);
			mAddedDate.dwHighDateTime = mQ.getIntField(17); 
			mAddedDate.dwLowDateTime = mQ.getIntField(18);
			mFlags = mQ.getIntField(19);
			mExtID = mQ.getIntField(20);

			//CString mCreatedTmStmp = mQ.getStringField(21);
			//CString mModifiedTmStmp = mQ.getStringField(22);
			//CString mAccessedTmStmp = mQ.getStringField(23);
			//CString mAddedTmStmp = mQ.getStringField(24);

			mDataIsValid = true;
		}
	}
	catch(...)
	{
		return false;
	}
	return true;
}

bool CTFiles::GetFiles(LPCTSTR inStr)
{
	try
	{
		CString lSQL;
		CString lStr;

		lStr.Empty();
		if (inStr != NULL)
			lStr = inStr;
		if (lStr.IsEmpty())
			lSQL.Format(_T("select * from Files"));
		else
			lSQL.Format(_T("select Files.* from Files, Files_has_Tags where %s group by Files.idFile"), lStr);
		// lucia ! opytat sa TONA
		//if (!gIsTagStr) lSQL.Replace(_T(", Files_has_Tags"), _T("")); // SQLite bug, if this will be fixed, this line must be removed
		mQ = m_pDB->mDB.execQuery(lSQL);
	}
	catch(...)
	{
		return false;
	}

	LoadCurrentRow();
	return true;
}

bool CTFiles::GetFileByID(int inFileID)
{
	try
	{
		CString lSQL; 
		lSQL.Format(_T("select * from Files where idFile = %d"), inFileID);
		mQ = m_pDB->mDB.execQuery(lSQL);
	}
	catch(...)
	{
		return false;
	}

	LoadCurrentRow();
	return true;
}

int CTFiles::GetFileIDByPath(LPCTSTR inPath)
{
	int lRetVal = -1;
	try
	{
		CppSQLite3Query lQ;
		CString lSQL; 
		CString lEncPath;
		EncodeString(inPath, lEncPath);

		/* The LIKE operator is not case sensitive and will match upper case characters 
		on one side against lower case characters on the other. 
		(A bug: SQLite only understands upper/lower case for 7-bit Latin characters. 
		Hence the LIKE operator is case sensitive for 8-bit iso8859 characters or UTF-8 characters. 
		For example, the expression 'a' LIKE 'A' is TRUE but '?' LIKE '?' is FALSE.)

		The GLOB operator is similar to LIKE but uses the Unix file globbing syntax for its wildcards. 
		Also, GLOB is case sensitive, unlike LIKE. Both GLOB and LIKE may be preceded by the NOT keyword 
		to invert the sense of the test. The infix GLOB operator is implemented by calling 
		the function glob(Y,X) and can be modified by overriding that function.
		*/

		lSQL.Format(_T("select idFile from Files where lower(Path) = lower('%s')"), lEncPath);
		//lSQL.Format(_T("select idFile from Files where lower(Path) LIKE lower('%s')"), lEncPath);
		//lSQL.Format(_T("select idFile from Files where lower(Path) GLOB lower('%s')"), lEncPath);		
		lQ = m_pDB->mDB.execQuery(lSQL);
		if (lQ.eof()) lRetVal = -1;
		else lRetVal = lQ.getIntField(0);
	}
	catch(...)
	{
		return -1;
	}

	return lRetVal;
}

//! returns the number of succesfully added files
//! outAddedFilesIDs the array of the Added files ID
//! outExistingFilesIDs the array of the Existing files ID
/*
int CTFiles::AddFiles(CStringList &inFilesPathList, CIntArray *outAddedFilesIDs, CIntArray *outExistingFilesIDs, bool inUseProgMon)
{
	if (gOrganizer->IsActive())
	{
		gOrganizer->OrganizeFiles(inFilesPathList, outAddedFilesIDs, inUseProgMon);
		return 0;
	}

	int lRetVal = 0;
	try
	{
		int lNumFiles = (int)inFilesPathList.GetCount();
		CString lSQL_AddFile;
		POSITION lPos = inFilesPathList.GetHeadPosition();	
		CppSQLite3Query lQ;
		CString lEncodedPath, lEncodedName;
		CString lProgTex;
		int lCnt = 0;
		
		if(m_ProgressDlg != NULL)
			delete m_ProgressDlg;
		m_ProgressDlg = NULL;
		if ((lNumFiles > 1) && (inUseProgMon))
		{
			m_ProgressDlg = new CProgressDlg;
			m_ProgressDlg->SetMax(lNumFiles);
			m_ProgressDlg->SetCaption(_T("Adding files..."));
			m_ProgressDlg->CreateProgressDlg();
		}

		while(lPos)
		{
			CString lTmpStr = inFilesPathList.GetNext(lPos);
			mPath = ExpandShortcut(lTmpStr);
			if(mPath.IsEmpty())
				mPath = lTmpStr;

			CString lStr;
			lStr.Format(_T("Adding file %s"), mPath);
			if(m_ProgressDlg != NULL)
			{
				m_ProgressDlg->SetMessage(lStr);
				m_ProgressDlg->StepIt();
				if (m_ProgressDlg->IsCancelled())
					break;
			}

			//lucia!!
			//if (!CUtils::IsAddThisFileEnabled(mPath))
			//	continue;
			if (!IsAddThisFileEnabled(mPath, true, true))
				continue;

			CString lSQL;
			CString lEncPath;
			EncodeString(mPath, lEncPath);
			//lSQL.Format(_T("select idFile from Files where Path = '%s';"), lEncPath);	
			lSQL.Format(_T("select idFile from Files where Path LIKE '%s';"), lEncPath); // solves problem with adding the same file aaaa.txt and AAAA.txt two times
			lQ = m_pDB->mDB.execQuery(lSQL); 
			if(!lQ.eof())
			{
				int lID = lQ.getIntField(0, 0);
				if(outExistingFilesIDs != NULL && lID != 0)
					outExistingFilesIDs->Add(lID);
				continue; //do not add this file more time
			}

			// load all summary info from file
			mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_TITLE] = true;
			mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_SUBJECT] = true;
			mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_AUTHOR] = true;
			mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_CATEGORY] = true;	
			mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_KEYWORDS] = true;	
			mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_COMMENTS] = true;	
			LoadDataFromFile();

			m_pDB->GetCurrentFileTime(mAddedDate);
		
			CString lEncName, lEncTitle, lEncSubject, lEncAuthor, lEncCat, lEncComments;
//			EncodeString(mFindData.cFileName, lEncName);
			EncodeString(mName, lEncName);
			EncodeString(mTitle, lEncTitle);
			EncodeString(mSubject, lEncSubject);
			EncodeString(mAuthor, lEncAuthor);
			EncodeString(mCategory, lEncCat);
			EncodeString(mComments, lEncComments);
			
			CString lCreatedTmStmp, lModifiedTmStmp, lAccessedTmStmp, lAddedTmStmp;
			Filetime2Timestamp(mFindData.ftCreationTime, lCreatedTmStmp);
			Filetime2Timestamp(mFindData.ftLastWriteTime, lModifiedTmStmp);
			Filetime2Timestamp(mFindData.ftLastAccessTime, lAccessedTmStmp);
			Filetime2Timestamp(mAddedDate, lAddedTmStmp);

			lSQL_AddFile.Format(_T("insert into Files values (NULL, '%s', '%s', %u, %u, %u, %u, %u, %u, %u, %u, %u, '%s', '%s', '%s', '%s', '%s', %u, %u, %u, %u, '%s', '%s', '%s', '%s')"), 
				lEncPath,
				lEncName,
				mFindData.nFileSizeHigh,
				mFindData.nFileSizeLow,
				mFindData.ftCreationTime.dwHighDateTime, //cr time
				mFindData.ftCreationTime.dwLowDateTime,
				mFindData.ftLastWriteTime.dwHighDateTime, //wr time
				mFindData.ftLastWriteTime.dwLowDateTime,
				mFindData.ftLastAccessTime.dwHighDateTime,
				mFindData.ftLastAccessTime.dwLowDateTime,
				mFindData.dwFileAttributes,
				lEncTitle,
				lEncSubject,
				lEncAuthor,
				lEncCat,
				lEncComments,
				mAddedDate.dwHighDateTime,
				mAddedDate.dwLowDateTime,
				mFlags,
				mExtID,
				lCreatedTmStmp,
				lModifiedTmStmp,
				lAccessedTmStmp,
				lAddedTmStmp
				);

			int lRet = m_pDB->mDB.execDML(lSQL_AddFile);
			//while (lRet == SQLITE_BUSY)
			//	lRet = m_pDB->mDB.execDML(lSQL_AddFile);

			CString lLogMsg;
			lLogMsg.Format(_T("Add file: %s"), lEncPath);
			CLog::StoreUserMessage(lLogMsg);

			int lID = GetFileIDByPath(mPath);
			if(outAddedFilesIDs != NULL)
			{
				outAddedFilesIDs->Add(lID);
			}
			AfxGetApp()->AddToRecentFileList(mPath);
			AddTagsFromKeywords(lID, mKeywords);

			lRetVal++;
		}
	}
	catch(...)
	{
		CLog::StoreUserMessage(_T("Add files error"));
	}

	if (m_ProgressDlg) 
	{
		delete m_ProgressDlg;
		m_ProgressDlg = NULL;
	}
	return lRetVal;
}
*/
int CTFiles::AddFiles(CStringList &inFilesPathList, CFileProperties_Entry * inFolderEntry, CIntArray *outAddedFilesIDs, CIntArray *outExistingFilesIDs, bool inUseProgMon)
{
	DebenuPDFLibraryAX0912 qpdf;
	qpdf.UnlockKey(_T(QUICKPDF_LICENSE_KEY));

	CFileProperties_Entry lFolderEntry;
	if (inFolderEntry)
	{
		lFolderEntry = *inFolderEntry;
	}

	int lRetVal = 0;
	try
	{
		int lNumFiles = (int)inFilesPathList.GetCount();

#ifndef _BNB_ASSISTANT
		if(m_ProgressDlg != NULL)
			delete m_ProgressDlg;
		m_ProgressDlg = NULL;
		if ((lNumFiles > 1) && (inUseProgMon))
		{
			m_ProgressDlg = new CProgressDlg;
			m_ProgressDlg->SetMax(lNumFiles);
			m_ProgressDlg->SetCaption(_T("Importing files..."));
			m_ProgressDlg->CreateProgressDlg();
		}
#endif //_BNB_ASSISTANT

		CString lSQL_AddFile;
		POSITION lPos = inFilesPathList.GetHeadPosition();	
		CppSQLite3Query lQ;
		CString lEncodedPath, lEncodedName;
		CString lProgTex;
		int lCnt = 0;
		
		while(lPos)
		{
#ifdef _BNB_ASSISTANT
			PeekMessages();
			if (gTimersPaused) 
			{
				return false;
			}
#endif

			CString lTmpStr = inFilesPathList.GetNext(lPos);
			
#ifndef _BNB_ASSISTANT
			TCHAR lFName[_MAX_FNAME];
			TCHAR lFExt[_MAX_EXT];

			_tsplitpath(lTmpStr.GetBuffer(), NULL, NULL, lFName, lFExt);

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
#endif

			mPath = ExpandShortcut(lTmpStr);
			if(mPath.IsEmpty())
			{
				mPath = lTmpStr;
			}

#ifndef _BNB_ASSISTANT
			CString lStr;
			lStr.Format(_T("Importing file %s"), mPath);
			if(m_ProgressDlg != NULL)
			{
				m_ProgressDlg->SetMessage(lStr);
				m_ProgressDlg->StepIt();
				if (m_ProgressDlg->IsCancelled())
					break;
			}
#endif _BNB_ASSISTANT

			//toto sa uz vola v CBRepository::AddFiles
//			if (!IsAddThisFileEnabled(mPath))
//				continue;

			CString lSQL;
			CString lEncPath;
			EncodeString(mPath, lEncPath);
			lSQL.Format(_T("select idFile from Files where Path LIKE '%s';"), lEncPath);	
			lQ = m_pDB->mDB.execQuery(lSQL); 
			if(!lQ.eof())
			{
#ifndef _BNB_ASSISTANT		
		
				int lID = lQ.getIntField(0, 0);
				if(outExistingFilesIDs != NULL && lID != 0)
				{
					outExistingFilesIDs->Add(lID);
				}
					
				continue; //do not add this file again
#endif
#ifdef _BNB_ASSISTANT
				//delete old entry from db and add.
				lSQL.Format(_T("delete from Files where Path LIKE '%s';"), lEncPath);
				m_pDB->mDB.execDML(lSQL);
#endif
			}
#ifdef _BNB_ASSISTANT
			//change file property
			qpdf.LoadFromFile(lEncPath.GetBuffer(), _T(""));
			
			long rtn = 0;//general return values
			if(inFolderEntry->mUseAuthor == 1)
			{
				rtn = qpdf.SetInformation(1, inFolderEntry->mAuthor.GetBuffer());
			}
			if(inFolderEntry->mUseCategories == 1)
			{

			}
			if(inFolderEntry->mUseComments == 1)
			{

			}
			if(inFolderEntry->mUseSubject == 1)
			{
				rtn = qpdf.SetInformation(3, inFolderEntry->mSubject.GetBuffer());
			}
			if(inFolderEntry->mUseTags == 1)
			{
				rtn = qpdf.SetInformation(4, inFolderEntry->mTags.GetBuffer());
			}
			if(inFolderEntry->mUseTitle == 1)
			{
				rtn = qpdf.SetInformation(2, inFolderEntry->mTitle.GetBuffer());
			}
			int nRet = qpdf.SaveToFile(lEncPath.GetBuffer());
#endif			
			
#ifdef _BNB_ASSISTANT

			/* If Benubird is running - send the message with the data to the Benubird and
			add the file in there. 

			Another possible communications:
			- sockets
			- named pipes
			- remoting
			- memory mapped files
			- windows messages ( e.g. WM_COPYDATA )
			*/

			// Determine if another window with our class name exists...
 			HWND lHwnd = FindWindow(UniqueBenubirdClassName, NULL);
			//if (pWndPrev = CWnd::FindWindow(_T("MyNewClass"), NULL))
			/*if( lHwnd != NULL )
			{
			// Prepare the data transfer package
				BenubirdData_Transfer BenubirdData = {0};
				BenubirdData.mMessageType = Watched_Folder_Message;

			// Prepare the assistant data
				BenubirdData.mAssistantData.mWFolderID = ((CTWatchedFolder_Entry*) inFolderEntry)->mID;

			// Copy the path
				LPTSTR pAssistantData = (LPTSTR) &BenubirdData.mAssistantData.mPath;
				wcscpy(pAssistantData, mPath.GetBufferSetLength(MAX_PATH));
				mPath.ReleaseBuffer();
				
			// Prepare the transfer copy structure
				COPYDATASTRUCT* pCOPYDATASTRUCT = new COPYDATASTRUCT;
				pCOPYDATASTRUCT->dwData = 1;							// function identifier
				pCOPYDATASTRUCT->cbData = sizeof(BenubirdData_Transfer)+1;
				pCOPYDATASTRUCT->lpData = &BenubirdData;

				// Call function, passing data in lCDS
				SendMessage(lHwnd, WM_COPYDATA, (WPARAM) NULL, (LPARAM) (LPVOID) pCOPYDATASTRUCT);

// _______________________________________________________
				
				// It appears if watched folders are running and Benubird is running, that the entries are sent to Benubird via a Windows message
				// and it adds them itself.

				if(outAddedFilesIDs != NULL)
				{
					outAddedFilesIDs->Add(lID);
				}

// _______________________________________________________


				continue; 
				
				//-----------------
				//PostMessage(WM_MYMSG, reinterpret_cast<WPARAM> (ptr) , static_cast<LPARAM> 0);
			}*/
#endif //_BNB_ASSISTANT

			// fill the private members in CTFiles class
			//mFlags; //0x1 - missing,
			mExtID = -1;
			mName = _T("");

			//processing folder entry

			// load summary from file (from disc!)
			mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_TITLE] = !lFolderEntry.mUseTitle;
			mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_SUBJECT] = !lFolderEntry.mUseSubject;
			mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_AUTHOR] = !lFolderEntry.mUseAuthor;
			mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_CATEGORY] = !lFolderEntry.mUseCategories;	
			mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_KEYWORDS] = !lFolderEntry.mUseTags;	
			mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_COMMENTS] = !lFolderEntry.mUseComments;	
			LoadDataFromFile(); // loads also dates

			// update summary info from watched folder settings

			if(lFolderEntry.mUseTitle != 0)
			{
				mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_TITLE] = true;
				mTitle = lFolderEntry.mTitle;
			}
			else
			{
				mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_TITLE] = false;
			}


			if(lFolderEntry.mUseAuthor != 0)
			{
				mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_AUTHOR] = true;
				mAuthor = lFolderEntry.mAuthor;
			}
			else
			{
				mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_AUTHOR] = false;
			}

			if(lFolderEntry.mUseSubject != 0)
			{
				mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_SUBJECT] = true;
				mSubject = lFolderEntry.mSubject;
			}
			else
			{
				mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_SUBJECT] = false;
			}

			if(lFolderEntry.mUseCategories != 0)
			{
				mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_CATEGORY] = true;
				mCategory = lFolderEntry.mCategories;
			}
			else
			{
				mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_CATEGORY] = false;
			}

			if(lFolderEntry.mUseComments)
			{
				mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_COMMENTS] = true;
				mComments = lFolderEntry.mComments;
			}
			else
			{
				mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_COMMENTS] = false;
			}

			if(lFolderEntry.mUseTags)
			{
				mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_KEYWORDS] = true;
				// read tags from the watched folder to mKeywords
				
/*
				if (lFolderEntry.mTags.GetCount() > 0)
				{
					//sem to pride ked sa vola Import files a pre dany CTWatchedEntry nie je prisluny Watched folder
					lFolderEntry.mTags.GetStringsFormatted(mKeywords, _T("; "));
				}
				else
				{
					//pre w-folder bude lFolderEntry.mTags urcite prazdny...

					CIDData lTagsIDData;
					CTTags lTTags;
					if(lTTags.GetCIDDataByWFolderID(lTagsIDData, lFolderEntry.mID))
					{
						lTagsIDData.GetStringsFormatted(mKeywords, _T("; "));
					}
				}
*/
			}
			else
			{
				mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_KEYWORDS] = false;
			}

			CString lEncName, lEncTitle, lEncSubject, lEncAuthor, lEncCat, lEncComments;
			EncodeString(mFindData.cFileName, lEncName);
			EncodeString(mTitle, lEncTitle);
			EncodeString(mSubject, lEncSubject);
			EncodeString(mAuthor, lEncAuthor);
			EncodeString(mCategory, lEncCat);
			EncodeString(mComments, lEncComments);
		
			m_pDB->GetCurrentFileTime(mAddedDate);
			CString lCreatedTmStmp, lModifiedTmStmp, lAccessedTmStmp, lAddedTmStmp;
			Filetime2Timestamp(mFindData.ftCreationTime, lCreatedTmStmp);
			Filetime2Timestamp(mFindData.ftLastWriteTime, lModifiedTmStmp);
			Filetime2Timestamp(mFindData.ftLastAccessTime, lAccessedTmStmp);
			Filetime2Timestamp(mAddedDate, lAddedTmStmp);

			// I need to update the summary info of the file on the disc at first - due to
			// avoid conflicts with the UpgradeFiles function from Benubird! 
			UpdateFileOnDiskFromMembers();

			lSQL_AddFile.Format(_T("insert into Files values (NULL, '%s', '%s', %u, %u, %u, %u, %u, %u, %u, %u, %u, '%s', '%s', '%s', '%s', '%s', %u, %u, %u, %u, '%s', '%s', '%s', '%s')"), 
				lEncPath,
				lEncName,
				mFindData.nFileSizeHigh,
				mFindData.nFileSizeLow,
				mFindData.ftCreationTime.dwHighDateTime, //cr time
				mFindData.ftCreationTime.dwLowDateTime,
				mFindData.ftLastWriteTime.dwHighDateTime, //wr time
				mFindData.ftLastWriteTime.dwLowDateTime,
				mFindData.ftLastAccessTime.dwHighDateTime,
				mFindData.ftLastAccessTime.dwLowDateTime,
				mFindData.dwFileAttributes,
				lEncTitle,
				lEncSubject,
				lEncAuthor,
				lEncCat,
				lEncComments,
				mAddedDate.dwHighDateTime,
				mAddedDate.dwLowDateTime,
				mFlags,
				mExtID,
				lCreatedTmStmp,
				lModifiedTmStmp,
				lAccessedTmStmp,
				lAddedTmStmp
				);
		
			m_pDB->mDB.execDML(lSQL_AddFile);

#ifndef _BNB_ASSISTANT
			CString lLogMsg;
			lLogMsg.Format(_T("Add file: %s"), lEncPath);
			CLog::StoreUserMessage(lLogMsg);
#endif //_BNB_ASSISTANT

			// get the identifier of the currently inserted file
			int lID = GetFileIDByPath(mPath);

			///////////////////////////////////////////////////////////////////////////////
			// additional tables can be updated only after the insert into the CTFile table 
			// update keywords table (tags)
			AddTagsFromKeywords(lID, mKeywords);

			// update collections
			if(lFolderEntry.mCollectionID > 0)
			{
				m_pDB->AddFileToCollection(lFolderEntry.mCollectionID - lFolderEntry.mCollectionIDGroup, lID);
			}

			//CString lLogMsg;
			//lLogMsg.Format(_T("Add file: %s"), lEncPath);
			//CLog::StoreUserMessage(lLogMsg);

			if(outAddedFilesIDs != NULL)
			{
				outAddedFilesIDs->Add(lID);
			}
			lRetVal++;
		}
	}
	catch(...)
	{
#ifndef _BNB_ASSISTANT
		CLog::StoreUserMessage(_T("Add files error"));
#endif //_BNB_ASSISTANT
	}

#ifndef _BNB_ASSISTANT
	if (m_ProgressDlg) 
	{
		delete m_ProgressDlg;
		m_ProgressDlg = NULL;
	}
#endif //_BNB_ASSISTANT

	return lRetVal;
}

int CTFiles::AddFiles_OverWrite(CStringList &inFilesPathList, CFileProperties_Entry * inFolderEntry, CIntArray *outAddedFilesIDs, CIntArray *outExistingFilesIDs, bool inUseProgMon)
{
	CFileProperties_Entry lFolderEntry;
	if (inFolderEntry)
	{
		lFolderEntry = *inFolderEntry;
	}

	int lRetVal = 0;
	try
	{
		int lNumFiles = (int)inFilesPathList.GetCount();

		if(m_ProgressDlg != NULL)
			delete m_ProgressDlg;

		m_ProgressDlg = NULL;
		if ((lNumFiles > 1) && (inUseProgMon))
		{
			m_ProgressDlg = new CProgressDlg;
			m_ProgressDlg->SetMax(lNumFiles);
			m_ProgressDlg->SetCaption(_T("Importing files..."));
			m_ProgressDlg->CreateProgressDlg();
		}

		CString lSQL_AddFile;
		POSITION lPos = inFilesPathList.GetHeadPosition();	
		CppSQLite3Query lQ;
		CString lEncodedPath, lEncodedName;
		CString lProgTex;
		int lCnt = 0;
		
		while(lPos)
		{
			CString lTmpStr = inFilesPathList.GetNext(lPos);

			TCHAR lFName[_MAX_FNAME];
			TCHAR lFExt[_MAX_EXT];

			_tsplitpath(lTmpStr.GetBuffer(), NULL, NULL, lFName, lFExt);

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

			mPath = ExpandShortcut(lTmpStr);
			if(mPath.IsEmpty())
			{
				mPath = lTmpStr;
			}

			CString lStr;
			lStr.Format(_T("Importing file %s"), mPath);
			if(m_ProgressDlg != NULL)
			{
				m_ProgressDlg->SetMessage(lStr);
				m_ProgressDlg->StepIt();
				if (m_ProgressDlg->IsCancelled())
					break;
			}

			CString lSQL;
			CString lEncPath;
			EncodeString(mPath, lEncPath);
			lSQL.Format(_T("select idFile from Files where Path LIKE '%s';"), lEncPath);	
			lQ = m_pDB->mDB.execQuery(lSQL); 
			if(!lQ.eof())
			{
				int lID = lQ.getIntField(0, 0);

				lSQL.Format(_T("delete from Files where idFile=%d;"), lID);	
				m_pDB->mDB.execDML(lSQL); 
			}

 			//HWND lHwnd = FindWindow(UniqueBenubirdClassName, NULL);

			mExtID = -1;
			mName = _T("");

			//processing folder entry

			// load summary from file (from disc!)
			mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_TITLE] = !lFolderEntry.mUseTitle;
			mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_SUBJECT] = !lFolderEntry.mUseSubject;
			mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_AUTHOR] = !lFolderEntry.mUseAuthor;
			mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_CATEGORY] = !lFolderEntry.mUseCategories;	
			mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_KEYWORDS] = !lFolderEntry.mUseTags;	
			mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_COMMENTS] = !lFolderEntry.mUseComments;	
			LoadDataFromFile(); // loads also dates

			// update summary info from watched folder settings

			if(lFolderEntry.mUseTitle != 0)
			{
				mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_TITLE] = true;
				mTitle = lFolderEntry.mTitle;
			}
			else
			{
				mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_TITLE] = false;
			}


			if(lFolderEntry.mUseAuthor != 0)
			{
				mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_AUTHOR] = true;
				mAuthor = lFolderEntry.mAuthor;
			}
			else
			{
				mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_AUTHOR] = false;
			}

			if(lFolderEntry.mUseSubject != 0)
			{
				mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_SUBJECT] = true;
				mSubject = lFolderEntry.mSubject;
			}
			else
			{
				mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_SUBJECT] = false;
			}

			if(lFolderEntry.mUseCategories != 0)
			{
				mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_CATEGORY] = true;
				mCategory = lFolderEntry.mCategories;
			}
			else
			{
				mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_CATEGORY] = false;
			}

			if(lFolderEntry.mUseComments)
			{
				mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_COMMENTS] = true;
				mComments = lFolderEntry.mComments;
			}
			else
			{
				mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_COMMENTS] = false;
			}

			if(lFolderEntry.mUseTags)
			{
				mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_KEYWORDS] = true;
			}
			else
			{
				mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_KEYWORDS] = false;
			}

			CString lEncName, lEncTitle, lEncSubject, lEncAuthor, lEncCat, lEncComments;
			EncodeString(mFindData.cFileName, lEncName);
			EncodeString(mTitle, lEncTitle);
			EncodeString(mSubject, lEncSubject);
			EncodeString(mAuthor, lEncAuthor);
			EncodeString(mCategory, lEncCat);
			EncodeString(mComments, lEncComments);
		
			m_pDB->GetCurrentFileTime(mAddedDate);
			CString lCreatedTmStmp, lModifiedTmStmp, lAccessedTmStmp, lAddedTmStmp;
			Filetime2Timestamp(mFindData.ftCreationTime, lCreatedTmStmp);
			Filetime2Timestamp(mFindData.ftLastWriteTime, lModifiedTmStmp);
			Filetime2Timestamp(mFindData.ftLastAccessTime, lAccessedTmStmp);
			Filetime2Timestamp(mAddedDate, lAddedTmStmp);

			// I need to update the summary info of the file on the disc at first - due to
			// avoid conflicts with the UpgradeFiles function from Benubird! 
			UpdateFileOnDiskFromMembers();

			lSQL_AddFile.Format(_T("insert into Files values (NULL, '%s', '%s', %u, %u, %u, %u, %u, %u, %u, %u, %u, '%s', '%s', '%s', '%s', '%s', %u, %u, %u, %u, '%s', '%s', '%s', '%s')"), 
				lEncPath,
				lEncName,
				mFindData.nFileSizeHigh,
				mFindData.nFileSizeLow,
				mFindData.ftCreationTime.dwHighDateTime, //cr time
				mFindData.ftCreationTime.dwLowDateTime,
				mFindData.ftLastWriteTime.dwHighDateTime, //wr time
				mFindData.ftLastWriteTime.dwLowDateTime,
				mFindData.ftLastAccessTime.dwHighDateTime,
				mFindData.ftLastAccessTime.dwLowDateTime,
				mFindData.dwFileAttributes,
				lEncTitle,
				lEncSubject,
				lEncAuthor,
				lEncCat,
				lEncComments,
				mAddedDate.dwHighDateTime,
				mAddedDate.dwLowDateTime,
				mFlags,
				mExtID,
				lCreatedTmStmp,
				lModifiedTmStmp,
				lAccessedTmStmp,
				lAddedTmStmp
				);
		
			m_pDB->mDB.execDML(lSQL_AddFile);

			CString lLogMsg;
			lLogMsg.Format(_T("Add file: %s"), lEncPath);
			CLog::StoreUserMessage(lLogMsg);

			// get the identifier of the currently inserted file
			int lID = GetFileIDByPath(mPath);

			///////////////////////////////////////////////////////////////////////////////
			// additional tables can be updated only after the insert into the CTFile table 
			// update keywords table (tags)
			AddTagsFromKeywords(lID, mKeywords);

			// update collections
			if(lFolderEntry.mCollectionID > 0)
			{
				m_pDB->AddFileToCollection(lFolderEntry.mCollectionID - lFolderEntry.mCollectionIDGroup, lID);
			}

			//CString lLogMsg;
			//lLogMsg.Format(_T("Add file: %s"), lEncPath);
			//CLog::StoreUserMessage(lLogMsg);

			if(outAddedFilesIDs != NULL)
			{
				outAddedFilesIDs->Add(lID);
			}
			lRetVal++;
		}
	}
	catch(...)
	{
		CLog::StoreUserMessage(_T("Add files error"));
	}

	if (m_ProgressDlg) 
	{
		delete m_ProgressDlg;
		m_ProgressDlg = NULL;
	}

	return lRetVal;
}


bool CTFiles::AddFile(CString &inFilesPath, CString & inFileName, CFileProperties_Entry * inFolderEntry)
{
#ifndef _BNB_ASSISTANT
	TCHAR lFName[_MAX_FNAME];
	TCHAR lFExt[_MAX_EXT];

	_tsplitpath(inFilesPath.GetBuffer(), NULL, NULL, lFName, lFExt);

	CString strMsg;
	strMsg.Format(_T("try to add library file called %s.bb , are you sure?"),lFName);
	if(wcscmp(lFExt,_T(".bb")) == 0)
	{
		int nRet = MessageBox(NULL,strMsg.GetBuffer() ,_T("Warning"),MB_OKCANCEL);
		if(nRet == IDCANCEL)
		{
			return true;
		}
	}
#endif

	CFileProperties_Entry lFolderEntry;
	if (inFolderEntry)
	{
		lFolderEntry = *inFolderEntry;
	}

	int lRetVal = 0;
	try
	{

		CString lSQL_AddFile;
		CppSQLite3Query lQ;
		CString lEncodedPath, lEncodedName;
		CString lProgTex;
		int lCnt = 0;
		

			CString lTmpStr = inFilesPath;
			mPath = ExpandShortcut(lTmpStr);
			if(mPath.IsEmpty())
			{
				mPath = lTmpStr;
			}

			//toto sa uz vola v CBRepository::AddFiles
//			if (!IsAddThisFileEnabled(mPath))
//				return false;

			CString lSQL;
			CString lEncPath;
			EncodeString(mPath, lEncPath);
			lSQL.Format(_T("select idFile from Files where Path LIKE '%s';"), lEncPath);	
			lQ = m_pDB->mDB.execQuery(lSQL); 
			if(!lQ.eof())
			{
				int lID = lQ.getIntField(0, 0);
					
				return false; //do not add this file more time
			}

			// fill the private members in CTFiles class
			//mFlags; //0x1 - missing,
			mExtID = -1;
			mName = inFileName;

			//processing folder entry

			// load summary from file (from disc!)
			mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_TITLE] = !lFolderEntry.mUseTitle;
			mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_SUBJECT] = !lFolderEntry.mUseSubject;
			mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_AUTHOR] = !lFolderEntry.mUseAuthor;
			mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_CATEGORY] = !lFolderEntry.mUseCategories;	
			mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_KEYWORDS] = !lFolderEntry.mUseTags;	
			mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_COMMENTS] = !lFolderEntry.mUseComments;	
			LoadDataFromFile(); // loads also dates

			// update summary info from watched folder settings

			if(lFolderEntry.mUseTitle != 0)
			{
				mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_TITLE] = true;
				mTitle = lFolderEntry.mTitle;
			}
			else
			{
				mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_TITLE] = false;
			}


			if(lFolderEntry.mUseAuthor != 0)
			{
				mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_AUTHOR] = true;
				mAuthor = lFolderEntry.mAuthor;
			}
			else
			{
				mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_AUTHOR] = false;
			}

			if(lFolderEntry.mUseSubject != 0)
			{
				mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_SUBJECT] = true;
				mSubject = lFolderEntry.mSubject;
			}
			else
			{
				mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_SUBJECT] = false;
			}

			if(lFolderEntry.mUseCategories != 0)
			{
				mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_CATEGORY] = true;
				mCategory = lFolderEntry.mCategories;
			}
			else
			{
				mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_CATEGORY] = false;
			}

			if(lFolderEntry.mUseComments)
			{
				mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_COMMENTS] = true;
				mComments = lFolderEntry.mComments;
			}
			else
			{
				mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_COMMENTS] = false;
			}

			if(lFolderEntry.mUseTags)
			{
				mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_KEYWORDS] = true;
				mKeywords = lFolderEntry.mTags;
				// read tags from the watched folder to mKeywords
/*
				if (lFolderEntry.mTags.GetCount() > 0)
				{
					//sem to pride ked sa vola Import files a pre dany CTWatchedEntry nie je prisluny Watched folder
					lFolderEntry.mTags.GetStringsFormatted(mKeywords, _T("; "));
				}
				else
				{
					//pre w-folder bude lFolderEntry.mTags urcite prazdny...

					CIDData lTagsIDData;
					CTTags lTTags;
					if(lTTags.GetCIDDataByWFolderID(lTagsIDData, lFolderEntry.mID))
					{
						lTagsIDData.GetStringsFormatted(mKeywords, _T("; "));
					}
				}
*/
			}
			else
			{
				mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_KEYWORDS] = false;
			}

			CString lEncName, lEncTitle, lEncSubject, lEncAuthor, lEncCat, lEncComments;
			EncodeString(/*mFindData.cFileName*/mName, lEncName);
			EncodeString(mTitle, lEncTitle);
			EncodeString(mSubject, lEncSubject);
			EncodeString(mAuthor, lEncAuthor);
			EncodeString(mCategory, lEncCat);
			EncodeString(mComments, lEncComments);
		
			m_pDB->GetCurrentFileTime(mAddedDate);
			CString lCreatedTmStmp, lModifiedTmStmp, lAccessedTmStmp, lAddedTmStmp;
			Filetime2Timestamp(mFindData.ftCreationTime, lCreatedTmStmp);
			Filetime2Timestamp(mFindData.ftLastWriteTime, lModifiedTmStmp);
			Filetime2Timestamp(mFindData.ftLastAccessTime, lAccessedTmStmp);
			Filetime2Timestamp(mAddedDate, lAddedTmStmp);

			// I need to update the summary info of the file on the disc at first - due to
			// avoid conflicts with the UpgadeFiles function from Benubird! 
			UpdateFileOnDiskFromMembers();

			lSQL_AddFile.Format(_T("insert into Files values (NULL, '%s', '%s', %u, %u, %u, %u, %u, %u, %u, %u, %u, '%s', '%s', '%s', '%s', '%s', %u, %u, %u, %u, '%s', '%s', '%s', '%s')"), 
				lEncPath,
				lEncName,
				mFindData.nFileSizeHigh,
				mFindData.nFileSizeLow,
				mFindData.ftCreationTime.dwHighDateTime, //cr time
				mFindData.ftCreationTime.dwLowDateTime,
				mFindData.ftLastWriteTime.dwHighDateTime, //wr time
				mFindData.ftLastWriteTime.dwLowDateTime,
				mFindData.ftLastAccessTime.dwHighDateTime,
				mFindData.ftLastAccessTime.dwLowDateTime,
				mFindData.dwFileAttributes,
				lEncTitle,
				lEncSubject,
				lEncAuthor,
				lEncCat,
				lEncComments,
				mAddedDate.dwHighDateTime,
				mAddedDate.dwLowDateTime,
				mFlags,
				mExtID,
				lCreatedTmStmp,
				lModifiedTmStmp,
				lAccessedTmStmp,
				lAddedTmStmp
				);
		
			m_pDB->mDB.execDML(lSQL_AddFile);

#ifndef _BNB_ASSISTANT
			CString lLogMsg;
			lLogMsg.Format(_T("Add file: %s"), lEncPath);
			CLog::StoreUserMessage(lLogMsg);
#endif //_BNB_ASSISTANT

			// get the identifier of the currently inserted file
			int lID = GetFileIDByPath(mPath);

			///////////////////////////////////////////////////////////////////////////////
			// additional tables can be updated only after the insert into the CTFile table 
			// update keywords table (tags)
			AddTagsFromKeywords(lID, mKeywords);

			// update collections
			if(lFolderEntry.mCollectionID > 0)
			{
				m_pDB->AddFileToCollection(lFolderEntry.mCollectionID - lFolderEntry.mCollectionIDGroup, lID);
			}

			//CString lLogMsg;
			//lLogMsg.Format(_T("Add file: %s"), lEncPath);
			//CLog::StoreUserMessage(lLogMsg);

			lRetVal = true;
	}
	catch(...)
	{
#ifndef _BNB_ASSISTANT
		CLog::StoreUserMessage(_T("Add files error"));
#endif //_BNB_ASSISTANT
	}

	return lRetVal;
}


int CTFiles::GetNextFileID()
{
	int lRet = 1;
	CppSQLite3Query lQ;
	CString lSQL = _T("select seq from sqlite_sequence where name='Files'");
	lQ = m_pDB->mDB.execQuery(lSQL); 

	if(lQ.eof())
	{
		lRet = 1;
	}
	else
	{
		lRet = lQ.getIntField(0) + 1;
	}

	return lRet;
}

void CTFiles::AddTagsFromKeywords(int inFileID, LPCTSTR inKeywords)
{
	CString lKeywords = inKeywords;
	int lPos;
	CString lTag;
	CTTags lTTags(m_pDB);

	//a tricky solution: if #part1 is true the #part2 is not executed
	while (((lPos = lKeywords.Find(_T(";"))) != -1 /*#Part1*/) || ((lPos = lKeywords.GetLength()) /*#Part2*/)) 
	{
		lTag = lKeywords.Left(lPos);
//		lTag.Replace(_T(" "), _T(""));
		if (!lTag.IsEmpty())
		{
			int lTagID = lTTags.AddNewTag(lTag);
			m_pDB->AddTagToFile(lTagID, inFileID);
		}
		lKeywords = lKeywords.Right(lKeywords.GetLength() - lPos - 1);
	}
}

void FileFinderProc(CFileFinder *pFinder, DWORD dwCode, void *pCustomParam)
{
	CString			sText, sNewFile;
	MSG				msg;
	CStringList		*pStrList = (CStringList *)pCustomParam;

	switch (dwCode)
	{
	case FF_FOUND:
		// Update list
		sNewFile = pFinder->GetFilePath(pFinder->GetFileCount() - 1);
		pStrList->AddTail(sNewFile);

//	case FF_FOLDER:
//		;
	}

	// Process all process messages
	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}
/*
int CTFiles::AddFolders(LPCTSTR inFolderPath, bool inRecursive, CIntArray *outAddedFilesIDs = NULL)
{
	CStringList lFilesPath;
	CFileFinder lFFinder;

	lFFinder.SetCallback(FileFinderProc, &lFilesPath);

//	_finder.StopSearch();
	CFileFinder::CFindOpts	lFOpts;
	// Set CFindOpts object
	lFOpts.sBaseFolder = inFolderPath;
//	lFOpts.sFileMask.Format("*%s*", m_sFileMask);
	lFOpts.bSubfolders = inRecursive;
	lFOpts.FindAllFiles();

	CProgressDlg lDlg;
	lDlg.SetCaption(_T("Collecting files..."));
	//lDlg.SetMessage(_T("Line1 Line2"));
	lDlg.SetNeverEndingMode();
	lDlg.ShowCancel(FALSE);
	lDlg.CreateProgressDlg();
	
	lFFinder.RemoveAll();
	lFFinder.Find(lFOpts);

	//if (lDlg.IsCancelled())
	//	return 0;

	lDlg.DestroyWindow();

	return AddFiles(lFilesPath, NULL, outAddedFilesIDs);
}
*/
bool CTFiles::UpdateFileFromDisk(bool inRefresh)
{
	try
	{
		if(mDataIsValid)
		{
			WIN32_FIND_DATA lFindData;		
			HANDLE lHFind = FindFirstFile(mPath, &lFindData);
			if(lHFind != INVALID_HANDLE_VALUE)
			{
				if(	((mFindData.ftLastWriteTime.dwHighDateTime != lFindData.ftLastWriteTime.dwHighDateTime) ||	//LastWrite
					(mFindData.ftLastWriteTime.dwLowDateTime != lFindData.ftLastWriteTime.dwLowDateTime)) ||
					((mFindData.ftCreationTime.dwHighDateTime != lFindData.ftCreationTime.dwHighDateTime) ||	//Create
					(mFindData.ftCreationTime.dwLowDateTime != lFindData.ftCreationTime.dwLowDateTime))
					)
				{	//the file was modified on the disk, so update its data in DB
					memcpy(&mFindData, &lFindData, sizeof(mFindData));
					BOOL lRet = TRUE;
					
					// set that all summary informations must be updated!
					mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_TITLE] = true;
					mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_SUBJECT] = true;
					mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_AUTHOR] = true;
					mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_CATEGORY] = true;	
					mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_KEYWORDS] = true;	
					mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_COMMENTS] = true;	

					// lucia bug 12 ! 
					// if (!globalData.bIsWindowsVista)
					// lucia end bug 12 ! 
					lRet = LoadSummaryInfo();

					if (lRet && inRefresh)
					{
						UpdateTablebyID();

						//refresh
//						if (inRefresh)
							::PostMessage(HWND_BROADCAST, BENUBIRD_ASSISTANT_REFRESH_FILE, (WPARAM)false, (LPARAM) mID);
					}
				}
				if( (mFlags & FILE_FLAG_FNF) == FILE_FLAG_FNF)
				{
					UpdateFileFlag(0);
					if (inRefresh)
					{
						//CUtils::RefreshFile(mID, false /*dont need to refresh navigator/tags*/);
						::PostMessage(HWND_BROADCAST, BENUBIRD_ASSISTANT_REFRESH_FILE, (WPARAM)true, (LPARAM) mID);
					}
				}
				FindClose(lHFind);
			}
			else if ((mFlags & FILE_FLAG_FNF) != FILE_FLAG_FNF)
			{
				UpdateFileFlag(FILE_FLAG_FNF);
				if (inRefresh)
				{
					::PostMessage(HWND_BROADCAST, BENUBIRD_ASSISTANT_REFRESH_FILE, (WPARAM)true, (LPARAM) mID);
				}
			}
		}
	}
	catch(...)
	{
		return false;
	}
	return true;
}

bool CTFiles::UpdateTablebyID()
{
	try
	{
//		ASSERT(0);

		CString lSQL;
		CString lEncPath, lEncName, lEncTitle, lEncSubject, lEncAuthor, lEncCat, lEncComments;\
		EncodeString(mPath, lEncPath);
		EncodeString(mFindData.cFileName, lEncName);
		EncodeString(mTitle, lEncTitle);
		EncodeString(mSubject, lEncSubject);
		EncodeString(mAuthor, lEncAuthor);
		EncodeString(mCategory, lEncCat);
		EncodeString(mComments, lEncComments);

		CString lCreatedTmStmp, lModifiedTmStmp, lAccessedTmStmp, lAddedTmStmp;
		Filetime2Timestamp(mFindData.ftCreationTime, lCreatedTmStmp);
		Filetime2Timestamp(mFindData.ftLastWriteTime, lModifiedTmStmp);
		Filetime2Timestamp(mFindData.ftLastAccessTime, lAccessedTmStmp);
		Filetime2Timestamp(mAddedDate, lAddedTmStmp);

		//JOZO nechcem updatnut title...
//		lSQL.Format(_T("update Files set Path = '%s', Name = '%s', SizeHi = %u, SizeLo = %u, CreatedHi = %u, CreatedLo = %u, ModifiedHi = %u, ModifiedLo = %u, AccessedHi = %u, AccessedLo = %u, Attributes = %u, Title = '%s', Subject = '%s', Author = '%s', Category = '%s', Comments = '%s', AddedHi  = %u, AddedLo = %u, BenuFlags = %u, Created_TmStmp = '%s', Modified_TmStmp = '%s', Accessed_TmStmp = '%s', Added_TmStmp = '%s' where idFile = %u;"),
//		lEncPath, lEncName,	mFindData.nFileSizeHigh, mFindData.nFileSizeLow,

		lSQL.Format(_T("update Files set Path = '%s', SizeHi = %u, SizeLo = %u, CreatedHi = %u, CreatedLo = %u, ModifiedHi = %u, ModifiedLo = %u, AccessedHi = %u, AccessedLo = %u, Attributes = %u, Title = '%s', Subject = '%s', Author = '%s', Category = '%s', Comments = '%s', AddedHi  = %u, AddedLo = %u, BenuFlags = %u, Created_TmStmp = '%s', Modified_TmStmp = '%s', Accessed_TmStmp = '%s', Added_TmStmp = '%s' where idFile = %u;"),
		lEncPath, mFindData.nFileSizeHigh, mFindData.nFileSizeLow,
		mFindData.ftCreationTime.dwHighDateTime, mFindData.ftCreationTime.dwLowDateTime,
		mFindData.ftLastWriteTime.dwHighDateTime, mFindData.ftLastWriteTime.dwLowDateTime,
		mFindData.ftLastAccessTime.dwHighDateTime, mFindData.ftLastAccessTime.dwLowDateTime,
		mFindData.dwFileAttributes,
		lEncTitle, lEncSubject, lEncAuthor, lEncCat, lEncComments,
		mAddedDate.dwHighDateTime, mAddedDate.dwLowDateTime, mFlags,
		lCreatedTmStmp,	lModifiedTmStmp, lAccessedTmStmp, lAddedTmStmp,
		mID);
		m_pDB->mDB.execDML(lSQL);

		/* --------------------------------- KDA -- 27th Jan 2009 -- removal of this update means that when editing multiple files, that tags are not deleted. 
		// This has been tested to work in both single and multiple edit operations

		// delete the old tags join to file
		//m_pDB->DeAssociateTagsFromFile(mID);

		// add a new tags
		//AddTagsFromKeywords(mID, mKeywords);

		 --------------------------------- KDA -- 27th Jan 2009 -- removal of this update means that when editing multiple files, that tags are not deleted. */

	}
	catch(...)
	{
		return false;
	}
	return true;
}

bool CTFiles::UpdateFileFlag(int inNewFlag)
{
	try
	{
		CString lSQL;
		lSQL.Format(_T("update Files set BenuFlags = %u where idFile = %u;"), inNewFlag, mID);
		m_pDB->mDB.execDML(lSQL);
	}
	catch(...)
	{
		return false;
	}
	return true;
}

bool CTFiles::UpdateFilePath(int inID, LPCTSTR inNewPath)
{
	try
	{
		CString lSQL;
		CString lEncStr;
		EncodeString(inNewPath, lEncStr);
		lSQL.Format(_T("update Files set Path = '%s' where idFile = %u;"), lEncStr, inID);
		m_pDB->mDB.execDML(lSQL);

		CString lLogMsg;
		lLogMsg.Format(_T("Update file path %s"), inNewPath);
		CLog::StoreUserMessage(lLogMsg);
	}
	catch(...)
	{
		CLog::StoreUserMessage(_T("Update file path error."));
		return false;
	}
	return true;
}
bool CTFiles::UpdateFilePath(LPCTSTR inOldPath, LPCTSTR inNewPath)
{
	try
	{
		CString lSQL;
		CString lEncStr;
		EncodeString(inNewPath, lEncStr);
		lSQL.Format(_T("update Files set Path = '%s' where Path = '%s';"), lEncStr, inOldPath);
		m_pDB->mDB.execDML(lSQL);

		CString lLogMsg;
		lLogMsg.Format(_T("Update file path %s"), inNewPath);
		CLog::StoreUserMessage(lLogMsg);
	}
	catch(...)
	{
		CLog::StoreUserMessage(_T("Update file path error."));
		return false;
	}
	return true;
}

bool CTFiles::UpdateFilePath(LPCTSTR inAddPath,int flag)
{
	try
	{
		CString lSQL;
		CString lEncStr;
		lSQL.Format(_T("update Files set Path = '%s'||Path;"),  inAddPath);
		m_pDB->mDB.execDML(lSQL);

		CString lLogMsg;
		lLogMsg.Format(_T("Update file add path %s"), inAddPath);
		CLog::StoreUserMessage(lLogMsg);
	}
	catch(...)
	{
		CLog::StoreUserMessage(_T("Update file path error."));
		return false;
	}
	return true;
}

//#include <shobjidl.h>
bool CTFiles::UpdateFileOnDiskFromMembers()
{
	bool lRetVal = false;
	
	/*if(globalData.bIsWindowsVista)
	{
		mFindData.ftLastWriteTime.dwHighDateTime = 0;
		mFindData.ftLastWriteTime.dwLowDateTime = 0;
		lRetVal = true;
		return lRetVal;
	}*/

	/*check if this is an image file*/
	CPath lExtPath(mPath);
	CString lExt = lExtPath.GetExtension();
	lExt.MakeLower();
	bool lIsImage = (lExt == ".bmp") || 
		(lExt == ".dib") || 
		(lExt == ".emf") || 
		(lExt == ".gif") || 
		(lExt == ".ico") || 
		(lExt == ".jfif") || 
		(lExt == ".jpe") || 
		(lExt == ".jpeg") || 
		(lExt == ".jpg") || 
		(lExt == ".png") || 
		(lExt == ".rle") || 
		(lExt == ".tiff") || 
		(lExt == ".wmf");

	
	IPropertySetStorage *pPropSetStg = NULL;
	IPropertyStorage *pPropStg = NULL;
	try
	{
		PROPSPEC propspec; 
		PROPVARIANT propWrite;

		HRESULT hr = S_OK;

		CString lPath = mPath;
		lPath.Replace(_T("\\"), _T("\\\\"));
		// Open a file and a property set within it.
		
//	VISTA
/*		hr = StgOpenStorageEx(lPath,
						STGM_DIRECT|STGM_SHARE_EXCLUSIVE|STGM_READWRITE,
						STGFMT_ANY,
						// STGFMT_STORAGE //Structured Storage property sets
						// STGFMT_FILE  //NTFS file system property sets
						0,
						NULL,
						NULL,
						IID_IPropertySetStorage,
						reinterpret_cast<void**>(&pPropSetStg) );
//b version

IShellItem2 *psi;
if (SUCCEEDED(SHCreateItemFromParsingName(lPath, NULL, IID_IShellItem2, (void**)&psi)))
{
   IPropertyStore *pps;
   if (SUCCEEDED(psi->GetPropertyStore(GPS_DEFAULT, IID_IPropertyStore, (void**)&pps))) 
   {
      PROPVARIANT propvar = {0};
      if (SUCCEEDED(pps->GetValue(PKEY_Title, &propvar))) 
      {
         wprintf(L"Title: %s\n", PropVariantToStringWithDefault(propvar, L"<none>"));
         PropVariantClear(&propvar);
      }
      pps->Release();
   }
   psi->Release();
}
*/
//VISTA	

		IShellFolder * psfDesktop = NULL;

		// get IShellFolder interface of Desktop (root of shell namespace)
		SHGetDesktopFolder (&psfDesktop);	// needed to obtain full qualified pidl
		if(FAILED(hr)) throw 0;

		// ParseDisplayName creates a PIDL from a file system path relative to the IShellFolder interface
		// but since we use the Desktop as our interface and the Desktop is the namespace root
		// that means that it's a fully qualified PIDL, which is what we need
		LPITEMIDLIST pidlFile = NULL, pidlWorkDir = NULL;
	
		TCHAR lDir[1000]; //path to dir
		TCHAR lFile[1000]; //file name
		_tcscpy_s(lDir, mPath);
		::PathRemoveFileSpecW(lDir);
		CString lFileName = ::PathFindFileName(mPath);
		_tcscpy_s(lFile, lFileName);

		hr = psfDesktop->ParseDisplayName (NULL, 0, lDir, NULL, &pidlWorkDir, NULL);
		if(FAILED(hr)) throw 0;

		IShellFolder* psfWorkDir = NULL;
		hr = psfDesktop->BindToObject(pidlWorkDir, NULL, IID_IShellFolder, (LPVOID*)&psfWorkDir);
		if(FAILED(hr)) throw 0;

		hr = psfWorkDir->ParseDisplayName(NULL, NULL, lFile, NULL, &pidlFile, NULL);
		if(FAILED(hr)) throw 0;
	
		//hr = psfWorkDir->BindToObject(pidlFile, NULL, IID_IPropertySetStorage, (LPVOID*)&pPropSetStg);
		//lucia bug 12 - metadata

		// get a R/W bind context
		LPBC pbc = 0;
		hr = CreateBindCtx(0, &pbc);
		BIND_OPTS bindopts = {sizeof(BIND_OPTS)};
		hr = pbc->GetBindOptions(&bindopts);
		bindopts.grfMode = STGM_READWRITE | STGM_SHARE_EXCLUSIVE;
		hr = pbc->SetBindOptions(&bindopts);

		hr = psfWorkDir->BindToStorage(pidlFile, pbc, IID_IPropertySetStorage, (void**)&pPropSetStg);
		
		/*
		Actually, it does work if you call IPropertyStorage::Commit. The reason
		is that on Vista, BindToStorage gives you an object that implements an
		OLE property set on top of a property store implemented on top of a OLE
		property set. Unfortunately it doesn't automatically commit the changes
		when the object is released. Oh what a tangled web!
		*/
		
		//end lucia

		if(FAILED(hr)) 
		{
			throw 0;
		}
		psfDesktop->Release();
		psfDesktop = NULL;
		LPMALLOC lpMalloc = NULL;
		SHGetMalloc (&lpMalloc);
		lpMalloc->Free(pidlWorkDir);
		pidlWorkDir = NULL;

		int lNumRuns = -1;
		do
		{
			//open the property set
			hr = -1;
			if(pPropSetStg)
			{
				hr = pPropSetStg->Open( FMTID_SummaryInformation, STGM_READWRITE//|STGM_SHARE_DENY_NONE,
									|STGM_SHARE_EXCLUSIVE, &pPropStg );
			}

			if(!SUCCEEDED(hr))
			{ //create it!!!
				hr = -1;
				if(pPropSetStg)
				{
					hr = pPropSetStg->Create(FMTID_SummaryInformation, 
											NULL,
											PROPSETFLAG_DEFAULT,
											STGM_CREATE|STGM_READWRITE|STGM_SHARE_EXCLUSIVE,
											&pPropStg );
				}
				if(!SUCCEEDED(hr))
				{
					hr = -1;
					if(pPropSetStg)
					{
						hr = pPropSetStg->Release();
					}
					hr = StgOpenStorageEx(
							lPath,
							STGM_DIRECT|STGM_SHARE_EXCLUSIVE|STGM_READWRITE,
							STGFMT_ANY,
							// STGFMT_STORAGE //Structured Storage property sets
							// STGFMT_FILE  //NTFS file system property sets
							0,
							NULL,
							NULL,
							IID_IPropertySetStorage,
							reinterpret_cast<void**>(&pPropSetStg) );
					if(lNumRuns == -1)
					{
						lNumRuns = 2;
					}
				}
			}
			lNumRuns--;
		}while(lNumRuns>0);

		if(FAILED(hr)) throw 0;

		//TITLE
		if(mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_TITLE])
		{
			propspec.ulKind = PRSPEC_PROPID;
			propspec.propid  = PIDSI_TITLE;

			//specify the value of property
			propWrite.vt = VT_LPWSTR;
			propWrite.pwszVal = (LPWSTR)(LPCTSTR)mTitle;
			hr = pPropStg->WriteMultiple( 1, &propspec, &propWrite, PID_FIRST_USABLE );

			//	if( FAILED(hr) ) 
			//		throw L"Failed IPropertyStorage::WriteMultiple";
			
			/*
			//read back
			PROPSPEC propspec; 
			PROPVARIANT propRead;
			propspec.ulKind = PRSPEC_PROPID;
			propspec.propid  = PIDSI_TITLE;
			hr = pPropStg->ReadMultiple( 1, &propspec, &propRead );
			//end read back
			*/			

			lRetVal = true;
		}

		//SUBJECT
		if(mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_SUBJECT])
		{
			propspec.ulKind = PRSPEC_PROPID;
			propspec.propid  = 0x00000003; //PIDSI_SUBJECT
			propWrite.vt = VT_LPWSTR;
			propWrite.pwszVal = (LPWSTR)(LPCTSTR)mSubject;	
			hr = pPropStg->WriteMultiple( 1, &propspec, &propWrite, PID_FIRST_USABLE );
			lRetVal = true;
		}

		//AUTHOR
		CStringList lAuthorList;
		if(mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_AUTHOR])
		{
			propWrite.calpwstr.pElems = NULL;
			if(globalData.bIsWindowsVista && !mAuthor.IsEmpty())
			{ 
				CString lAuthor = mAuthor;
				int lStrPos;
				CString lStr;
				//a tricky solution: if #part1 is true the #part2 is not executed
				while (((lStrPos = lAuthor.Find(_T(";"))) != -1 /*#Part1*/) || ((lStrPos = lAuthor.GetLength()) /*#Part2*/)) 
				{
					lStr = lAuthor.Left(lStrPos);
					lStr.Trim();
					if (!lStr.IsEmpty())
					{
						lAuthorList.AddTail(lStr);
					}
					lAuthor = lAuthor.Right(lAuthor.GetLength() - lStrPos - 1);
				}

				propWrite.vt = VT_VECTOR | VT_LPWSTR;
				propWrite.calpwstr.cElems = lAuthorList.GetCount();
				propWrite.calpwstr.pElems = new LPWSTR[propWrite.calpwstr.cElems];

				POSITION lPos = lAuthorList.GetHeadPosition();
				long lIndex = 0;
				while(lPos && (lIndex < lAuthorList.GetCount()))
				{
					lStr = lAuthorList.GetNext(lPos);
					propWrite.calpwstr.pElems[lIndex] = (LPWSTR)(LPCTSTR)lStr;
				}

				/*propWrite.vt = VT_SAFEARRAY;
				propWrite.pwszVal = (LPWSTR)(LPCTSTR)mAuthor;	

				SAFEARRAYBOUND rgsabound[1];
				rgsabound[0].lLbound = 0;
				rgsabound[0].cElements = lAuthorList.GetCount();

				propWrite.vt = VT_SAFEARRAY;
				propWrite.parray = ::SafeArrayCreate(VT_VARIANT, 1, rgsabound);
				if (propWrite.parray == NULL)
					throw(0);

				hr  = ::SafeArrayPutElement(propWrite.parray, &lIndex, (LPWSTR)(LPCTSTR)lStr);
				//if(FAILED(hr))
				}*/
			}
			else
			{
				propWrite.vt = VT_LPWSTR;
				propWrite.pwszVal = (LPWSTR)(LPCTSTR)mAuthor;	
			}
			propspec.ulKind = PRSPEC_PROPID;
			propspec.propid  = 0x00000004; //PIDSI_AUTHOR
			hr = pPropStg->WriteMultiple( 1, &propspec, &propWrite, PID_FIRST_USABLE );

			if (propWrite.calpwstr.pElems != NULL)
			{
				delete(propWrite.calpwstr.pElems);
			}
			lRetVal = true;
		}

		//KEYWORDS
		if(mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_KEYWORDS])
		{
			CTTags lTTags(m_pDB);
			lTTags.GetTagsByFileID_Formatted(mKeywords, mID);
			CStringList lKeywordsList;
			propWrite.calpwstr.pElems = NULL;
			if(globalData.bIsWindowsVista && !mKeywords.IsEmpty())
			{ 
				CString lKeywords = mKeywords;
				int lStrPos;
				CString lStr;
				//a tricky solution: if #part1 is true the #part2 is not executed
				while (((lStrPos = lKeywords.Find(_T(";"))) != -1 /*#Part1*/) || ((lStrPos = lKeywords.GetLength()) /*#Part2*/)) 
				{
					lStr = lKeywords.Left(lStrPos);
					lStr.Trim();
					if (!lStr.IsEmpty())
					{
						lKeywordsList.AddTail(lStr);
					}
					lKeywords = lKeywords.Right(lKeywords.GetLength() - lStrPos - 1);
				}

				propWrite.vt = VT_VECTOR | VT_LPWSTR;
				propWrite.calpwstr.cElems = lKeywordsList.GetCount();
				propWrite.calpwstr.pElems = new LPWSTR[propWrite.calpwstr.cElems];

				POSITION lPos = lKeywordsList.GetHeadPosition();
				long lIndex = 0;
				while(lPos && (lIndex < lKeywordsList.GetCount()))
				{
					lStr = lKeywordsList.GetNext(lPos);
					propWrite.calpwstr.pElems[lIndex] = (LPWSTR)(LPCTSTR)lStr;
					lIndex++;
				}
			}
			else
			{
				propWrite.vt = VT_LPWSTR;
				propWrite.pwszVal = (LPWSTR)(LPCTSTR)mKeywords;	
			}

			propspec.ulKind = PRSPEC_PROPID;
			propspec.propid  = PIDSI_KEYWORDS;
			hr = pPropStg->WriteMultiple( 1, &propspec, &propWrite, PID_FIRST_USABLE );

			if (propWrite.calpwstr.pElems != NULL)
			{
				delete(propWrite.calpwstr.pElems);
			}
			lRetVal = true;
		}
		
		if(mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_COMMENTS])
		{
			//PROPSPEC rgpspec = {PRSPEC_PROPID, PIDSI_COMMENTS};
			//PROPVARIANT rgvar;
			//PropVariantInit( &rgvar );
			//rgvar.vt = VT_LPWSTR;
			//rgvar.pwszVal = L"test";
			//hr = pPropStg->WriteMultiple(1, &rgpspec, &rgvar, 2/*irrelevant*/);

			propspec.ulKind = PRSPEC_PROPID;
			propspec.propid  = PIDSI_COMMENTS;
			propWrite.vt = VT_LPWSTR;
			propWrite.pwszVal = (LPWSTR)(LPCTSTR)mComments;	
			hr = pPropStg->WriteMultiple( 1, &propspec, &propWrite, PID_FIRST_USABLE );
			lRetVal = true;
		}

		// FROM NOW pPropStg is going into FMTID_DocSummaryInformation
		// CATEGORY, it is in DOC property set!!!
		if(globalData.bIsWindowsVista || !lIsImage)
		{
			if(pPropStg) pPropStg->Commit(STGC_DEFAULT/*STGC_ONLYIFCURRENT*/); //do not use for XP for image files
		}

		hr = pPropStg->Release();
		hr = pPropSetStg->Open( FMTID_DocSummaryInformation, STGM_READWRITE//|STGM_SHARE_DENY_NONE,
								|STGM_SHARE_EXCLUSIVE, &pPropStg );
		if(!SUCCEEDED(hr))
		{
			CLog::StoreUserMessage(_T("CTFiles::UpdateFileOnDiskFromMembers - 11"));
			hr = pPropSetStg->Create( FMTID_DocSummaryInformation, NULL, 
                            PROPSETFLAG_DEFAULT,
                            STGM_CREATE|STGM_READWRITE|
                            STGM_SHARE_EXCLUSIVE,
                            &pPropStg );
			CLog::StoreUserMessage(_T("CTFiles::UpdateFileOnDiskFromMembers - 12"));
		}

		if(mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_CATEGORY])
		{
			CLog::StoreUserMessage(_T("CTFiles::UpdateFileOnDiskFromMembers - 13"));
			propspec.ulKind = PRSPEC_PROPID;
			propspec.propid  = PIDDSI_CATEGORY;
			propWrite.vt = VT_LPWSTR;
			propWrite.pwszVal = (LPWSTR)(LPCTSTR)mCategory;	
			hr = pPropStg->WriteMultiple( 1, &propspec, &propWrite, PID_FIRST_USABLE );
			lRetVal = true;
		}
	}
	catch(...)
	{
		//gGeneralLog.StoreMessage(_T("CTFiles::UpdateFileOnDiskFromMembers - 111"));
		lRetVal = false;
	}

	/* IPropertyStorage::Commit Method
	http://msdn.microsoft.com/en-us/library/aa379969(VS.85).aspx

	Note  Using IPropertyStorage::Commit to write properties to image files on Windows XP does not work. 
	Affected image file formats include:
    * .bmp
    * .dib
    * .emf
    * .gif
    * .ico
    * .jfif
    * .jpe
    * .jpeg
    * .jpg
    * .png
    * .rle
    * .tiff
    * .wmf

	Due to a bug in the image file property handler on Windows XP, calling IPropertyStorage::Commit 
	actually discards any changes made rather than persisting them. A workaround is to omit the call to 
	IPropertyStorage::Commit. Calling IUnknown::Release on the XP image file property handler without calling 
	IPropertyStorage::Commit first implicitly commits the changes to the file. Note that in general, 
	calling IUnknown::Release without first calling IPropertyStorage::Commit will discard any changes made; 
	this workaround is specific to the image file property handler on Windows XP. 
	Also note that on later versions of Windows, this component functions properly 
	(that is, calling IPropertyStorage::Commit persists changes and calling IUnknown::Release without 
	calling IPropertyStorage::Commit discards them).
	*/

	if(globalData.bIsWindowsVista || !lIsImage)
	{
		if(pPropStg) 
		{
			pPropStg->Commit(STGC_DEFAULT);	//do not use for XP for image files
		}
	}
	if( pPropStg ) 
	{
		pPropStg->Release();
	}
	if( pPropSetStg ) 
	{
		pPropSetStg->Release();
	}
	return lRetVal;
}

//http://www.codeproject.com/KB/files/SummInfoPropSetFile.aspx
//http://msdn2.microsoft.com/en-us/library/aa380072.aspx
//http://blogs.msdn.com/benkaras/archive/2007/01/05/choosing-your-property-api.aspx


/*
Choosing your property API
It is time that I talk a little about what to do if you want your application to run on XP.  
There are three sets of APIs, each with subtle differences and caveats, 
and ultimately your choice requires deciding what platform your application must run on:

Windows Vista:
Call IShellItem2::GetPropertyStore to get an IPropertyStore interface.  
The output property store includes property handler properties (e.g. Photo dimensions) and 
innate properties (e.g. Size, Name).  This is your one-stop-shop for properties.

Caveat: This technique only works on Vista and later OSes.  

Windows XP with Windows Desktop Search 3.x:
You can pass an IShellItem interface to PSGetItemPropertyHandler to get an IPropertyStore interface.  
Much of the property system works with Windows Desktop Search 3.x, so you can ask for property descriptions and 
everything.  One upshot is that this API works identically on Vista so if you are able to choose to use it, 
you don't have to detect versions or anything.

Unfortunately, innate properties are only available through methods on IShellFolder and IShellFolder2.  
So you're on your own for those.

Caveat: Properties written to this API may or may not readable on Vista. 
[Aside: The full story is more complicated, but I'd like to move on for now]  
Innate properties like PKEY_Size are not exposed. 

Windows XP; Windows XP with Windows Desktop Search 2.x:
Properties on XP are exposed through IPropertySetStorage.  
The supported technique is to call IShellFolder::BindToStorage(IPropertySetStorage) for the item. 

Innate properties such as size and name are exposed through various methods on IShellFolder and IShellFolder2.

Caveats: Properties written with this API may or may not be readable on Vista.  
Furthermore, on Vista, this technique only works for some namespaces and not others. 

Gotcha:  Many people mistakenly call StgOpenStorageEx.  
Don't do that!  
StgOpenStorageEx is only supported for specific formats like OLE Compound Documents or NTFS secondary stream 
storage.  StgOpenStorageEx doesn't know how to read the EXIF header from a .JPG image. 
IShellFolder::BindToStorage knows how to do such things.

Best of all worlds:
If you really enjoy coding, you can detect if a given API is present by calling GetProcAddress or 
QueryInterface [Aside: please don't detect versions... just directly test if the API is present or not], 
select the best available API, and deliver an application that works better on Vista.  
Hey, "Works better on Vista" sounds like a marketing slogan -- you could have fun coding and 
sell more software too!
*/

bool CTFiles::LoadSummaryInfo()
{	
	mTitle = mSubject = mAuthor = mCategory = mKeywords = mComments = _T("");

#ifndef _BNB_ASSISTANT
	// Check to see if the file is a PDF file -- if so, then import its properties -- KDA 11 Feb, 2009 (Kludge)
	// Overwrite those that have been loaded from the file
	TCHAR lDrv[_MAX_DRIVE];
	TCHAR lFld[_MAX_DIR];
	TCHAR lNam[_MAX_FNAME];
	TCHAR lExt[_MAX_EXT];
	_tsplitpath_s(mPath, lDrv, lFld, lNam, lExt);
	CString lExtStr = lExt;
	if ((lExtStr == _T(".pdf")))
	{
		PDF_Entry inEntry;
		LoadPDFMetadata(mPath, &inEntry);
		mTitle = inEntry.mTitle;
		mAuthor = inEntry.mAuthor;
		mSubject = inEntry.mSubject;
		mKeywords = inEntry.mKeywords;
		return true;
	}
#endif

	LPMALLOC lpMalloc = NULL;
	SHGetMalloc (&lpMalloc);
	IShellFolder* psfDesktop = NULL;
	
	try
	{		
		IPropertySetStorage *pPropSetStg = NULL;
		IPropertyStorage *pPropStg = NULL;
		PROPSPEC propspec; 
		PROPVARIANT propRead;
		HRESULT hr = S_OK;

		CString lPath = mPath;
		lPath.Replace(_T("\\"), _T("\\\\"));

		// get IShellFolder interface of Desktop (root of shell namespace)

		SHGetDesktopFolder (&psfDesktop);	// needed to obtain full qualified pidl
		if(FAILED(hr))
		{
			throw 0;
		}

		// ParseDisplayName creates a PIDL from a file system path relative to the IShellFolder interface
		// but since we use the Desktop as our interface and the Desktop is the namespace root
		// that means that it's a fully qualified PIDL, which is what we need
		LPITEMIDLIST pidlFile = NULL, pidlWorkDir = NULL;
	
		TCHAR lDir[1000]; //path to dir
		TCHAR lFile[1000]; //file name
		_tcscpy_s(lDir, mPath);
		::PathRemoveFileSpecW(lDir);
		CString lFileName = ::PathFindFileName(mPath);
		_tcscpy_s(lFile, lFileName);

		hr = psfDesktop->ParseDisplayName (NULL, 0, lDir, NULL, &pidlWorkDir, NULL);
		if(FAILED(hr)) 
		{
			throw 0;
		}

		IShellFolder* psfWorkDir = NULL;
		hr = psfDesktop->BindToObject(pidlWorkDir, NULL, IID_IShellFolder, (LPVOID*)&psfWorkDir);
		if(FAILED(hr)) 
		{
			throw 0;
		}

		hr = psfWorkDir->ParseDisplayName(NULL, NULL, lFile, NULL, &pidlFile, NULL);
		if(FAILED(hr)) 
		{
			throw 0;
		}
	
		//hr = psfWorkDir->BindToObject(pidlFile, NULL, IID_IPropertySetStorage, (LPVOID*)&pPropSetStg);
		//lucia bug 12 - metadata
		hr = psfWorkDir->BindToStorage(pidlFile, NULL, IID_IPropertySetStorage, (void**)&pPropSetStg);
		//end lucia

		if(FAILED(hr)) 
		{
			throw 0;
		}
		psfDesktop->Release();
		psfDesktop = NULL;
		lpMalloc->Free(pidlWorkDir);
		pidlWorkDir = NULL;


		//////////////////////////////////////////////////////////////////////////////////
		// FMTID_DocSummaryInformation
		//start Category is located in doc summ. inf
		if(mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_CATEGORY])
		{
			hr = pPropSetStg->Open( FMTID_DocSummaryInformation, STGM_READ//|STGM_SHARE_DENY_NONE,
									|STGM_SHARE_EXCLUSIVE, &pPropStg );
			if(SUCCEEDED(hr)) 
			{
				propspec.ulKind = PRSPEC_PROPID;
				propspec.propid  = PIDDSI_CATEGORY;
				hr = pPropStg->ReadMultiple( 1, &propspec, &propRead );
				if(hr == SEVERITY_SUCCESS) 
				{	
					if(propRead.vt == VT_LPWSTR)
					{
						mCategory.Format(_T("%s"), propRead.pwszVal);
					}
					else if(propRead.vt == VT_LPSTR)
					{
						mCategory.Format(_T("%s"), CA2CT(propRead.pszVal));
					}
				}
			}

			if(pPropStg)
			{
				pPropStg->Release();
			}
			pPropStg = NULL;
		}
		//end category

		//////////////////////////////////////////////////////////////////////////////////
		// FMTID_SummaryInformation
		hr = pPropSetStg->Open( FMTID_SummaryInformation, STGM_READ//|STGM_SHARE_DENY_NONE,
								|STGM_SHARE_EXCLUSIVE, &pPropStg );
		//if(FAILED(hr))
		if(hr!= S_OK )
		{
			pPropSetStg->Release();
			return true;// no kind of property steven 8/4
		}
/*		if( FAILED(hr) ) 
		{
			hr = pPropSetStg->Open( FMTID_ImageSummaryInformation, STGM_READ//|STGM_SHARE_DENY_NONE,
								|STGM_SHARE_EXCLUSIVE, &pPropStg );
			if(FAILED(hr))
			{
				hr = pPropSetStg->Open( FMTID_MediaFileSummaryInformation, STGM_READ//|STGM_SHARE_DENY_NONE,
								|STGM_SHARE_EXCLUSIVE, &pPropStg );
				hr = pPropSetStg->Open( FMTID_UserDefinedProperties, STGM_READ//|STGM_SHARE_DENY_NONE,
								|STGM_SHARE_EXCLUSIVE, &pPropStg );
				hr = pPropSetStg->Open( FMTID_DiscardableInformation, STGM_READ//|STGM_SHARE_DENY_NONE,
								|STGM_SHARE_EXCLUSIVE, &pPropStg );
				hr = pPropSetStg->Open( FMTID_AudioSummaryInformation, STGM_READ//|STGM_SHARE_DENY_NONE,
								|STGM_SHARE_EXCLUSIVE, &pPropStg );
				hr = pPropSetStg->Open( FMTID_VideoSummaryInformation, STGM_READ//|STGM_SHARE_DENY_NONE,
								|STGM_SHARE_EXCLUSIVE, &pPropStg );
				if(FAILED(hr))
				{
					pPropSetStg->Release();
					return true;// no kind of property
				}
			}
		}
*/
		// Read the properties 

		//TITLE
		propspec.ulKind = PRSPEC_PROPID;
		
		if(mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_TITLE])
		{
			propspec.propid  = 0x00000002; //PIDSI_TITLE
			hr = pPropStg->ReadMultiple( 1, &propspec, &propRead );
			if(hr == SEVERITY_SUCCESS) 
			{	
				if(propRead.vt == VT_LPWSTR)
				{
					mTitle.Format(_T("%s"), propRead.pwszVal);
				}
				else if(propRead.vt == VT_LPSTR)
				{
					mTitle.Format(_T("%s"), CA2CT(propRead.pszVal));
				}
			}
		}

		//SUBJECT
		if(mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_SUBJECT])
		{
			propspec.propid  = 0x00000003; //PIDSI_SUBJECT
			hr = pPropStg->ReadMultiple( 1, &propspec, &propRead );
			if(hr == SEVERITY_SUCCESS) 
			{	
				if(propRead.vt == VT_LPWSTR)
				{
					mSubject.Format(_T("%s"), propRead.pwszVal);
				}
				else if(propRead.vt == VT_LPSTR)
				{
					mSubject.Format(_T("%s"), CA2CT(propRead.pszVal));
				}
			}
		}

		// AUTHOR
		if(mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_AUTHOR])
		{
			propspec.propid  = 0x00000004; //PIDSI_AUTHOR
			hr = pPropStg->ReadMultiple( 1, &propspec, &propRead );
			if(hr == SEVERITY_SUCCESS) 
			{	
				// VT_VECTOR | VT_LPWSTR
				if (propRead.vt == (VT_VECTOR | VT_LPWSTR))
				{
					mAuthor = _T("");
					for(ULONG lIndex = 0; lIndex < propRead.calpwstr.cElems; lIndex++)
					{
						if (!mAuthor.IsEmpty())
						{
							mAuthor += _T(";");
						}
						WCHAR *tmpArr;
						long rg = 0;
						CString lStr; 
						lStr.Format(_T("%s"), propRead.calpwstr.pElems[lIndex]);
						mAuthor += lStr;
					}
				}
				// VT_LPWSTR
				else if(propRead.vt == VT_LPWSTR)
				{
					mAuthor.Format(_T("%s"), propRead.pwszVal);
				}
				// VT_LPSTR
				else if(propRead.vt == VT_LPSTR)
				{
					mAuthor.Format(_T("%s"), CA2CT(propRead.pszVal));
				}
				/*else if ((propRead.vt & VT_SAFEARRAY) == VT_SAFEARRAY)
				{ 
					for(long lIndex = 0; lIndex < propRead.parray->cbElements; lIndex++)
					{
						if (!mAuthor.IsEmpty())
							mAuthor += _T(";");
						WCHAR *tmpArr;
						long rg = 0;
						hr = SafeArrayGetElement(propRead.parray, &rg, (void*)&tmpArr);
						CString lStr; 
						mAuthor.Format(_T("%s"), tmpArr);
						mAuthor += lStr;
					}
				}*/
			}
		}

		//PIDSI_KEYWORDS
		if(mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_KEYWORDS])
		{
			propspec.propid  = 0x00000005;  //PIDSI_KEYWORDS;?
			hr = pPropStg->ReadMultiple( 1, &propspec, &propRead );
			if(hr == SEVERITY_SUCCESS) 
			{	
				// VT_VECTOR | VT_LPWSTR
				if (propRead.vt == (VT_VECTOR | VT_LPWSTR))
				{
					mKeywords = _T("");
					for(ULONG lIndex = 0; lIndex < propRead.calpwstr.cElems; lIndex++)
					{
						if (!mKeywords.IsEmpty())
						{
							mKeywords += _T(";");
						}
						WCHAR *tmpArr;
						long rg = 0;
						CString lStr; 
						lStr.Format(_T("%s"), propRead.calpwstr.pElems[lIndex]);
						mKeywords += lStr;
					}
				}
				else if(propRead.vt == VT_LPWSTR)
				{
					mKeywords.Format(_T("%s"), propRead.pwszVal);
				}
				else if(propRead.vt == VT_LPSTR)
				{
					mKeywords.Format(_T("%s"), CA2CT(propRead.pszVal));
				}
			}
		}

		//PIDSI_COMMENTS
		if(mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_COMMENTS])
		{
			propspec.propid  = 0x00000006;
			hr = pPropStg->ReadMultiple( 1, &propspec, &propRead );
			if(hr == SEVERITY_SUCCESS) 
			{	
				if(propRead.vt == VT_LPWSTR)
				{
					mComments.Format(_T("%s"), propRead.pwszVal);
				}
				else if(propRead.vt == VT_LPSTR)
				{
					mComments.Format(_T("%s"), CA2CT(propRead.pszVal));
				}
			} 
		}

		if(pPropStg != NULL) pPropStg->Release();
		if(pPropSetStg != NULL) pPropSetStg->Release();
		if(pidlFile != NULL) lpMalloc->Free(pidlFile);
		if(pidlWorkDir != NULL) lpMalloc->Free(pidlWorkDir);
		if(psfDesktop != NULL) psfDesktop->Release();
		if(psfWorkDir != NULL) psfWorkDir->Release();

#ifdef _BNB_ASSISTANT
	// Check to see if the file is a PDF file -- if so, then import its properties -- KDA 11 Feb, 2009 (Kludge)
	// Overwrite those that have been loaded from the file
		TCHAR lDrv[_MAX_DRIVE];
		TCHAR lFld[_MAX_DIR];
		TCHAR lNam[_MAX_FNAME];
		TCHAR lExt[_MAX_EXT];
		_tsplitpath_s(mPath, lDrv, lFld, lNam, lExt);
		CString lExtStr = lExt;
		if ((lExtStr == _T(".pdf")))
		{
			PDF_Entry inEntry;
			LoadPDFMetadata(mPath, &inEntry);
			mTitle = inEntry.mTitle;
			mAuthor = inEntry.mAuthor;
			mSubject = inEntry.mSubject;
			mKeywords = inEntry.mKeywords;
		}
#endif
//////////////////////////////////////////////////

		return true;
	}
	catch(...)
	{
	}

/*	if(pidlFile != NULL) m_pMalloc->Free(pidlFile);
	if(pidlWorkDir != NULL) m_pMalloc->Free(pidlWorkDir);
	if(psfDesktop != NULL) psfDesktop->Release();
	if(psfWorkDir != NULL) psfWorkDir->Release();
*/	return false;
}

bool CTFiles::LoadAutoSummaryInfo()
{
	CString lPath = mPath;

	TCHAR lDrv[_MAX_DRIVE];
	TCHAR lDir[_MAX_DIR];
	TCHAR lNam[_MAX_FNAME];
	TCHAR lExt[_MAX_EXT];

	_tsplitpath_s(lPath, lDrv, lDir, lNam, lExt);

	CWinApp* lApp = AfxGetApp();
	bool lChanged = false;

	if (lApp->GetProfileInt(kOptions, kAutoGenerateTitle, BST_UNCHECKED) && mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_TITLE] &&
		(lApp->GetProfileInt(kOptions, kAutoGenerateDontOverwrite, BST_CHECKED) == BST_UNCHECKED || mTitle.IsEmpty()))
	{
		mTitle = lNam;
		lChanged = true;
	}

	if (lApp->GetProfileInt(kOptions, kAutoGenerateAuthor, BST_UNCHECKED) && mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_AUTHOR] &&
		(lApp->GetProfileInt(kOptions, kAutoGenerateDontOverwrite, BST_CHECKED) == BST_UNCHECKED || mAuthor.IsEmpty()))
	{
		LPWKSTA_USER_INFO_1 lBuf = NULL;
		DWORD lLevel = 1;

		NET_API_STATUS lStatus;

		lStatus = NetWkstaUserGetInfo(NULL, lLevel, (LPBYTE *)&lBuf);

		mAuthor = lBuf->wkui1_username;

		if (lBuf != NULL)
			NetApiBufferFree(lBuf);	

		lChanged = true;
	}
	
	if (lApp->GetProfileInt(kOptions, kAutoGenerateTags, BST_UNCHECKED) && mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_KEYWORDS] &&
		(lApp->GetProfileInt(kOptions, kAutoGenerateDontOverwrite, BST_CHECKED) == BST_UNCHECKED || mKeywords.IsEmpty()))
	{
		CString lFilePath = lDrv;
		lFilePath += lDir;
		CString lTmpPath(lFilePath);
		lTmpPath.MakeLower();

		//ignore specific paths like C:\\Documents and settings\\My documents etc.

		LoadSystemDirList();

		POSITION lPos = m_pSystemDirsList->GetHeadPosition();
		while (lPos)
		{
			CString lSysPath = m_pSystemDirsList->GetNext(lPos);

			if (lTmpPath.Find(lSysPath) == 0)
			{
				lFilePath.Delete(0, lSysPath.GetLength());
			}
			else if (lSysPath.Find(lTmpPath) == 0)
			{
				lFilePath.Empty();
			}
		}

		//remove the drive if still there...
		int i = lFilePath.Find(_T(":"));
		if (i > 0)
		{
			lFilePath.Delete(0, i+1);
		}

		if (!lFilePath.IsEmpty())
		{
			while (!lFilePath.IsEmpty())
			{
				//process the path of current file
				int lLen = 0;
				int i = lFilePath.Find(_T("\\"));
				if (i == -1)
				{
					i = lFilePath.GetLength();
				}

				if (i > 0)
				{
					CString lKW = lFilePath.Left(i);
					if (!mKeywords.IsEmpty())
					{
						mKeywords += _T("; ");
					}
					mKeywords += lKW;
				}
				lFilePath.Delete(0, i+1);
			}
		}
		lChanged = true;
	}

	if (lApp->GetProfileInt(kOptions, kAutoGenerateSubject, BST_UNCHECKED) && mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_SUBJECT] &&
		(lApp->GetProfileInt(kOptions, kAutoGenerateDontOverwrite, BST_CHECKED) == BST_UNCHECKED || mSubject.IsEmpty()))
	{
		CString lFilePath = lDrv;
		lFilePath += lDir;
		CString lTmpPath(lFilePath);
		lTmpPath.MakeLower();

		//ignore specific paths like C:\\Documents and settings\\My documents etc.

		LoadSystemDirList();

		POSITION lPos = m_pSystemDirsList->GetHeadPosition();
		while (lPos)
		{
			CString lSysPath = m_pSystemDirsList->GetNext(lPos);

			if (lTmpPath.Find(lSysPath) == 0)
			{
				lFilePath.Delete(0, lSysPath.GetLength());
			}
			else if (lSysPath.Find(lTmpPath) == 0)
			{
				lFilePath.Empty();
			}
		}

		if (!lFilePath.IsEmpty())
		{
			//remove the drive if still there...
			int i = lFilePath.Find(_T(":"));
			if (i > 0)
			{
				lFilePath.Delete(0, i+1);
			}

			if (lFilePath.Right(1) == _T("\\"))
			{
				lFilePath.Delete(lFilePath.GetLength()-1);
			}

			
			i = lFilePath.ReverseFind('\\');

			mSubject = lFilePath.Right(lFilePath.GetLength()-i-1);
		}

		lChanged = true;

	}

	if (lChanged)
	{
		UpdateFileOnDiskFromMembers();
	}

	return true;
}

void CTFiles::LoadSystemDirList()
{
	if (m_pSystemDirsList->IsEmpty())
	{
		//initialize the common directory list

		for (int i = CSIDL_DESKTOP; i <= CSIDL_COMPUTERSNEARME; i++)
		{
			TCHAR lPath[MAX_PATH+1];
			memset(lPath, 0, sizeof(lPath));

			if (::SHGetFolderPath(NULL, i, NULL, SHGFP_TYPE_CURRENT, lPath) != S_OK)
			{
				continue;
			}

			CString lSysPath = lPath;
			lSysPath.MakeLower();
			m_pSystemDirsList->AddTail(lPath);
		}
	}
}

CTFExtensions::CTFExtensions()
{

}
CTFExtensions::CTFExtensions(CDB *pDB):CDBTable(pDB)
{
	m_pDB = pDB;
}
CTFExtensions::~CTFExtensions()
{

}
	
bool CTFExtensions::LoadCurrentRow()
{
	try
	{
		mDataIsValid = false;
		if(!mQ.eof())
		{
			mID = mQ.getIntField(0);
			mName = mQ.getStringField(1);
			mDescription = mQ.getStringField(2);
			mMajorFilterID = mQ.getIntField(3);
			mDataIsValid = true;
		}
	}
	catch(...)
	{
		return false;
	}
	return true;

}

int CTFExtensions::GetExtensionIDWithCreate(LPCTSTR inExtName)
{
	try
	{
		CString lSQL; 
		CString lEncExtName;
		
		EncodeString(inExtName, lEncExtName);

		// Szabi, Case 263
		if (lEncExtName.Find('.') != -1)
		{
			lEncExtName = lEncExtName.Right(lEncExtName.GetLength() - lEncExtName.ReverseFind('.') - 1);
		}

		lSQL.Format(_T("select * from FExtensions where Name = '%s';"), lEncExtName);
		mQ = m_pDB->mDB.execQuery(lSQL);

		if(mQ.eof())
		{
			CString lSQLInsert;
			lSQLInsert.Format(_T("insert into FExtensions values (NULL, '%s', '', NULL, %u)"), lEncExtName, FILTERTREE_MAJOR_OTHER - FILTERTREE_MAJOR_BASE);
			m_pDB->mDB.execDML(lSQLInsert);
		}
		mQ = m_pDB->mDB.execQuery(lSQL);
		
	}
	catch(...)
	{
		return -1;
	}

	LoadCurrentRow();
	return mID;
}

bool CTFExtensions::GetExtensionsByID(int inID)
{
	try
	{
		CString lStr;
		lStr.Format(_T("select * from FExtensions where FExtensions.idFExtension = %d"), inID);
		mQ = m_pDB->mDB.execQuery(lStr);
	}
	catch(...)
	{
		return false;
	}

	LoadCurrentRow();
	return true;
}

bool CTFExtensions::GetExtensions()
{
	try
	{
		mQ = m_pDB->mDB.execQuery(_T("select * from FExtensions"));
	}
	catch(...)
	{
		return false;
	}

	LoadCurrentRow();
	return true;
}

bool CTFExtensions::GetExtensionsByWFolderID(int inWFolderID)
{
	try
	{
		CString lSQL;
		lSQL.Format(_T("select FExtensions.* from FExtensions, WFolders_has_FExtensions where  WFolders_has_FExtensions.WFolders_idWFolder = %d and WFolders_has_FExtensions.FExtensions_idFExtension = FExtensions.idFExtension;"), inWFolderID);
		mQ = m_pDB->mDB.execQuery(lSQL); 
	}
	catch(...)
	{
		return false;
	}

	LoadCurrentRow();
	return true;
}

bool CTFExtensions::GetCIDDataByWFolderID(CIDData &outIDData, int inWFolderID)
{
	bool lRetVal = false;
	outIDData.RemoveAll();
	if(GetExtensionsByWFolderID(inWFolderID))
	{
		while(mDataIsValid)
		{
			outIDData.AddData(mID, mName);
			LoadNextRow();
			lRetVal = true;
		}
	}
	return lRetVal;
}


CTCollections::CTCollections(CDB *pDB):CDBTable(pDB)
{
	m_pDB = pDB;
};
bool CTCollections::LoadCurrentRow()
{
	try
	{
		mDataIsValid = false;
		if(!mQ.eof())
		{
			mID = mQ.getIntField(0);
			mName = mQ.getStringField(1);
			mParentID = mQ.getIntField(3);
			mDataIsValid = true;
		}
	}
	catch(...)
	{
		return false;
	}
	return true;

}

bool CTCollections::GetCollectionsByNames(int inParentID)
{
	try
	{
		CString lStr;
		if (inParentID == -2)
		{
			lStr = _T("select * from Collections order by Name;");
		}
		else
		{
			lStr.Format(_T("select * from Collections where idParentCollection = %d order by Name;"), inParentID);
		}
		mQ = m_pDB->mDB.execQuery(lStr);
	}
	catch(...)
	{
		return false;
	}

	LoadCurrentRow();
	return true;
}

//!returns false if the collection already existing
bool CTCollections::AddNewCollection(LPCTSTR inCollectionName, int inParentID)
{	
	try
	{
		CString lSQL;
		CString lEncCollName;
		EncodeString(inCollectionName, lEncCollName);
		lSQL.Format(_T("select * from Collections where Name = '%s';"), lEncCollName);
		 
		CppSQLite3Query lQ = m_pDB->mDB.execQuery(lSQL); 
		if(!lQ.eof())
		{
			return false; //do not add twice the same Collection
		}

		lSQL.Format(_T("insert into Collections values (NULL, '%s', NULL, %d)"), lEncCollName, inParentID);
		m_pDB->mDB.execDML(lSQL);
	}
	catch(...)
	{
		return false;
	}
	return true;
}

bool CTCollections::GetCollectionsByName(LPCTSTR inCollName)
{
	try
	{
		CString lSQL;
		CString lEncCollName;
		EncodeString(inCollName, lEncCollName);
		lSQL.Format(_T("select * from Collections where Name = '%s';"), lEncCollName);
		mQ = m_pDB->mDB.execQuery(lSQL); 
	}
	catch(...)
	{
		return false;
	}

	LoadCurrentRow();
	return true;
}

bool CTCollections::HasCollectionChildItems(int inParentID)
{
	int lRetVal = -1;
	try
	{
		CString lSQL;
		lSQL.Format(_T("SELECT COUNT (idParentCollection) FROM collections where idParentCollection = %d;"), inParentID);
		CppSQLite3Query lQ;
		lQ = m_pDB->mDB.execQuery(lSQL);
		lRetVal = lQ.getIntField(0);
	}
	catch(...)
	{
	}
	return lRetVal;
}

bool CTCollections::UpdateTablebyID()
{
	try
	{
//		ASSERT(0);

		CString lSQL;
		CString lEncName;
		EncodeString(mName, lEncName);
		lSQL.Format(_T("update Collections set Name = '%s' where idCollection = %u;"), lEncName , mID);
		m_pDB->mDB.execDML(lSQL);
	}
	catch(...)
	{
		return false;
	}
	return true;
}

//case 118
bool CTCollections::GetAllCollectionsAssignedToFileIDs(CIntArray *inFileIDs, CIDData &outCollIDData)
{
	try
	{
		if(inFileIDs == NULL)
		{
			return false;
		}

		int lNumFileIDs = (int)inFileIDs->GetCount();

		CString lSubSql;
		CString lSQL;
		int lFileID;
		for(int i=0; i < lNumFileIDs; i++)
		{
			lFileID = inFileIDs->GetAt(i);
			CString lTmp;
			if(lSubSql.GetLength() == 0)
			{
				lTmp.Format(_T("(Collections_has_Files.Files_idFile = %d)"), lFileID);
			}
			else
			{
				lTmp.Format(_T(" OR (Collections_has_Files.Files_idFile = %d)"), lFileID);
			}
			lSubSql += lTmp;
		}
		lSQL.Format(_T("select distinct * from Collections, Collections_has_Files where Collections.idCollection = Collections_has_Files.Collections_idCollection AND (%s) group by Collections_has_Files.Collections_idCollection;"), lSubSql);
		CppSQLite3Query lQ = m_pDB->mDB.execQuery(lSQL); 

		int lID;
		CString lName;
		while(!lQ.eof())
		{
			lID = lQ.getIntField(0);
			lName = lQ.getStringField(1);
			outCollIDData.AddData(lID, lName);
			lQ.nextRow();
		}
	}
	catch(...)
	{
		return false;
	}
	return true;
}

bool CTCollections::GetAllCollectionsNotAssignedToFileIDs(CIntArray *inFileIDs, CIDData &outCollIDData)
{
	try
	{
		if(inFileIDs == NULL)
		{
			return false;
		}

		int lNumFileIDs = (int)inFileIDs->GetCount();

		CString lSubSql;
		CString lSQL;
		int lFileID;
		for(int i = 0; i < lNumFileIDs; i++)
		{
			lFileID = inFileIDs->GetAt(i);
			CString lTmp;
			if(lSubSql.GetLength() == 0)
			{
				lTmp.Format(_T("(Collections_has_Files.Files_idFile = %d)"), lFileID);
			}
			else
			{
				lTmp.Format(_T(" OR (Collections_has_Files.Files_idFile = %d)"), lFileID);
			}
			lSubSql += lTmp;
		}
		lSQL.Format(_T("select * from Collections where idCollection NOT IN (select distinct Collections_idCollection from Collections_has_Files where %s);"), lSubSql);
		CppSQLite3Query lQ = m_pDB->mDB.execQuery(lSQL); 

		int lID;
		CString lName;
		while(!lQ.eof())
		{
			lID = lQ.getIntField(0);
			lName = lQ.getStringField(1);
			outCollIDData.AddData(lID, lName);
			lQ.nextRow();
		}
	}
	catch(...)
	{
		return false;
	}
	return true;
}

bool CTCollections::DeleteByID(int inCollID)
{
	try
	{
		CppSQLite3Query lQ;
		CString lSQL;
		
		//delete the references
		lSQL.Format(_T("delete from Collections_has_Files where Collections_idCollection = %d"), inCollID);
		m_pDB->mDB.execDML(lSQL);
		lSQL.Format(_T("update WFolders set Collections_idCollection = 0 where Collections_idCollection = %d"), inCollID);
		m_pDB->mDB.execDML(lSQL);

		lSQL.Format(_T("delete from Collections where idCollection = %d"), inCollID);
		m_pDB->mDB.execDML(lSQL);
	}
	catch(...)
	{
		return false;
	}
	return true;
}

//----------------------------------------------------------
//! CTSmartColl 
//! represents the Smart collection table and the operations over them
//----------------------------------------------------------

CTSmartColl::CTSmartColl()
{
	for(int j = 0; j < SMART_COLL_MAXFILTERS; j++)
	{
		mCombo1[j] = 0;
		mCombo2[j] = 0;
		mValue[j] = _T("");
	}
}
CTSmartColl::CTSmartColl(PARAM_FOR_CDB *pParamCDB):CDBTable(pParamCDB->pDB)
{
	m_pParamCDB = pParamCDB;
	m_pDB = pParamCDB->pDB;
	for(int j = 0; j < SMART_COLL_MAXFILTERS; j++)
	{
		mCombo1[j] = 0;
		mCombo2[j] = 0;
		mValue[j] = _T("");
	}
}
bool CTSmartColl::LoadCurrentRow()
{	
	/* Smart collections su urobene trochu netypicky oproti ostatnym z historickych dovodov:) 
	LoadCurrentRow nacitava este dodatocne udaje z tabulky condition. */
	try
	{
		int lNumFields = mQ.numFields();
		if (lNumFields < 5) return false;

		mDataIsValid = false;
		int i = 0, j = 0;
		if(!mQ.eof())
		{
			mID = mQ.getIntField(i++);
			mName = mQ.getStringField(i++);
			mLimitToBool = mQ.getIntField(i++);
			mLimitToStr = mQ.getStringField(i++);
			mLimitToCombo = mQ.getIntField(i++);
			mNumActiveFilters = 0;
			
			// select conditions
			CppSQLite3Query lQ;
			CString lSQL;
			lSQL.Format(_T("select distinct Conditions.* from Conditions, SmartCollections_has_Conditions where Conditions.idCondition = SmartCollections_has_Conditions.Conditions_idCondition AND SmartCollections_has_Conditions.SmartCollections_idSmartColl = %u;"), mID);
			lQ = m_pDB->mDB.execQuery(lSQL); 
			while (!lQ.eof())
			{
				mCombo1[mNumActiveFilters] = lQ.getIntField(1);
				mCombo2[mNumActiveFilters] = lQ.getIntField(2);
				mValue[mNumActiveFilters] = lQ.getStringField(3);
				mNumActiveFilters++;
				lQ.nextRow();
			}

			mDataIsValid = true;
		}
	}
	catch(...)
	{
		return false;
	}
	return true;

}

bool CTSmartColl::AddNewSmartColl(LPCTSTR inCollectionName)
{
	try
	{
		CppSQLite3Query lQ;
		CString lSQL;

		CString lEncName;
		EncodeString(inCollectionName, lEncName);
		lSQL.Format(_T("select idSmartColl from SmartCollections where Name = '%s';"), lEncName);		 
		lQ = m_pDB->mDB.execQuery(lSQL); 
		if(!lQ.eof())
		{
			return false; //do not add twice the same Collection
		}

		if(mID <= 0) //new Collection
		{
			lSQL = _T("select max(idSmartColl) from SmartCollections;"); 
			lQ = m_pDB->mDB.execQuery(lSQL); 
			if(lQ.eof())
			{
				mID = 1;
			}
			else
			{
				mID = lQ.getIntField(0) + 1;
			}
			
			mName = inCollectionName;

			CString lEncLimitToStr, lEncVal[SMART_COLL_MAXFILTERS];
			EncodeString(mLimitToStr, lEncLimitToStr);
			for(int i = 0; i < SMART_COLL_MAXFILTERS; i++)
			{
				EncodeString(mValue[i], lEncVal[i]);
			}

			lSQL.Format(_T("insert into SmartCollections values (%u, '%s', %d, '%s', %d);"), mID, lEncName, mLimitToBool, lEncLimitToStr, mLimitToCombo);
			m_pDB->mDB.execDML(lSQL);

			// insert conditions
			int lMaxCondition = 1;
			lSQL = _T("select max(idCondition) from Conditions;"); 
			lQ = m_pDB->mDB.execQuery(lSQL); 
			if(!lQ.eof())
			{
				lMaxCondition = lQ.getIntField(0) + 1;
			}
		
			for (int i=0; i<mNumActiveFilters; i++)
			{
				lSQL.Format(_T("insert into Conditions values (%u, %d, %d, '%s');"), lMaxCondition+i, mCombo1[i], mCombo2[i], lEncVal[i]);
				m_pDB->mDB.execDML(lSQL);
				lSQL.Format(_T("insert into SmartCollections_has_Conditions values (%u, %u);"), mID, lMaxCondition+i);			 
				m_pDB->mDB.execDML(lSQL);
			}
		}
		else //update an existing WFolder
		{
	;/*		lSQL.Format(_T("update SmartCollections set (%u, '%s', %d, '%s', %d, %d, %d, %d, '%s', %d, %d, '%s', %d, %d, '%s', %d, %d, '%s', %d, %d, '%s', %d, %d, '%s', %d, %d, '%s', %d, %d, '%s', %d, %d, '%s', %d, %d, '%s';"), 
				mID,
				mPath,
				mIncludeSubfolders,
				mAllFileType,
				mCollectionID,
				mCheckFrequency
				);	
			*/
		}
	}
	catch(...)
	{
		return false;
	}

	return true;
}

bool CTSmartColl::GetSmartCollections()
{
	try
	{
		CString lSQL;
		lSQL.Format(_T("select * from SmartCollections"));
		mQ = m_pDB->mDB.execQuery(lSQL);
	}
	catch(...)
	{
		return false;
	}

	LoadCurrentRow();
	return true;
}

bool CTSmartColl::GetSmartCollByName(LPCTSTR inSmartCollName)
{
	try
	{
		CString lSQL;
		CString lEncSmCollName;
		EncodeString(inSmartCollName, lEncSmCollName);
		lSQL.Format(_T("select * from SmartCollections where Name = '%s';"), lEncSmCollName);
		mQ = m_pDB->mDB.execQuery(lSQL); 
	}
	catch(...)
	{
		return false;
	}

	LoadCurrentRow();
	return true;

}

bool CTSmartColl::GetSmartCollByID(DWORD_PTR inSmartCollID)
{
	try
	{	CString lSQL;
		lSQL.Format(_T("select * from SmartCollections where idSmartColl = %u"), inSmartCollID);
		mQ = m_pDB->mDB.execQuery(lSQL);
	}
	catch(...)
	{
		return false;
	}

	LoadCurrentRow();
	return true;
}

bool CTSmartColl::UpdateNameByID()
{
	try
	{
		CString lSQL;
		CString lEncName;
		EncodeString(mName, lEncName);
		lSQL.Format(_T("update SmartCollections set Name = '%s' where idSmartColl = %u;"), lEncName , mID);
		m_pDB->mDB.execDML(lSQL);
	}
	catch(...)
	{
		return false;
	}
	return true;
}

bool CTSmartColl::UpdateTableByID()
{
	try
	{
		CString lSQL;
		CString lEncLimitToStr, lEncVal[SMART_COLL_MAXFILTERS], lEncName;

		EncodeString(mLimitToStr, lEncLimitToStr);
		EncodeString(mName, lEncName);
		for(int i = 0; i < SMART_COLL_MAXFILTERS; i++)
		{
			EncodeString(mValue[i], lEncVal[i]);
		}

		CppSQLite3Query lQ;
		//delete the references
		//lSQL.Format(_T("delete from Conditions where Conditions.idCondition = SmartCollections_has_Conditions.Conditions_idCondition AND SmartCollections_has_Conditions.SmartCollections_idSmartColl = %u"), mID);
		//m_pDB->mDB.execDML(lSQL);
		lSQL.Format(_T("delete from SmartCollections_has_Conditions where SmartCollections_idSmartColl = %u"), mID);
		m_pDB->mDB.execDML(lSQL);

		// update the table
		lSQL.Format(_T("update SmartCollections set Name = '%s', LimitToBool = %d, LimitToStr = '%s', LimitToCombo = %d where idSmartColl = %u;"),
			lEncName, mLimitToBool, lEncLimitToStr, mLimitToCombo, mID);
		m_pDB->mDB.execDML(lSQL);
	
		// insert all conditions again
		int lMaxCondition = 1;
		lSQL = _T("select max(idCondition) from Conditions;"); 
		lQ = m_pDB->mDB.execQuery(lSQL); 
		if(!lQ.eof())
		{
			lMaxCondition = lQ.getIntField(0) + 1;
		}

		for (int i=0; i<mNumActiveFilters; i++)
		{
			lSQL.Format(_T("insert into Conditions values (%u, %d, %d, '%s');"), lMaxCondition+i, mCombo1[i], mCombo2[i], lEncVal[i]);
			m_pDB->mDB.execDML(lSQL);
			lSQL.Format(_T("insert into SmartCollections_has_Conditions values (%u, %u);"), mID, lMaxCondition+i);			 
			m_pDB->mDB.execDML(lSQL);
		}
	}
	catch(...)
	{
		return false;
	}
	return true;
}

bool CTSmartColl::UpdateSQL(int inSmartCollID, CDBSQL* inDBSQL)
{
	CString outStr;
	outStr = _T("");

	if (!GetSmartCollByID(inSmartCollID) || !mDataIsValid)
	{
		return false;
	}
	
	try
	{
		//if(!mMatchTheFollowingRuleBool)
		//{
		//	return false;
		//}
		CString lEncVal[SMART_COLL_MAXFILTERS];
		for(int i = 0; i < SMART_COLL_MAXFILTERS; i++)
		{
			EncodeString(mValue[i], lEncVal[i]);
		}

		CString lLine, lCollector, lTmp;
		for(int i=0; i<mNumActiveFilters; i++)
		{
			switch(mCombo1[i])
			{
				case F1_Author:
					lLine = _T("Files.Author");
					break;
				case F1_Category:
					lLine = _T("Files.Category");
					break;
				case F1_DateAdded:
					lLine = _T("Files.Added_TmStmp");
					//AddedHi
					//AddedLo
					break;
				case F1_DateCreated:
					lLine = _T("Files.Created_TmStmp");
					//CreatedHi
					//CreatedLo
					break;
				case F1_DateModified:
					lLine = _T("Files.Modified_TmStmp");
					//ModifiedHi
					//ModifiedLo
					break;
				case F1_Description:
					lLine = _T("Files.Comments");					
					//Comments
					break;
				case F1_FileType:
					//FExtensions_idFExtension
					lLine = _T("FExtensions.Name");
					inDBSQL->AddTable(_T("FExtensions"));
					inDBSQL->AddCondition(_T("(Files.FExtensions_idFExtension = FExtensions.idFExtension)"));
					break;
				case F1_LastOpened:
					lLine = _T("Files.Accessed_TmStmp");

					/*COleDateTime lDT;
					if (lDT.ParseDateTime(mTSmartColl.mValue[i]) == false)
						lDT = COleDateTime::GetCurrentTime();
					m_ValueButton[i].SetDate(DATE(lDT));*/
					//AccessedHi
					//AccessedLo
					break;
				case F1_Filename:
					lLine = _T("Files.Name");
					break;
				case F1_Path:
					lLine = _T("Files.Path");
					break;
				case F1_Size:
					//SizeHi
					lLine = _T("Files.SizeLo");
					break;
				case F1_Subject:
					lLine = _T("Files.Subject");
					break;
				case F1_Tags:
					lLine = _T("Tags.Name");
					inDBSQL->AddTable(_T("Tags"));
					inDBSQL->AddTable(_T("Files_has_Tags"));
					inDBSQL->AddCondition(_T("(Files_has_Tags.Files_idFile = Files.idFile and Files_has_Tags.Tags_idTag = Tags.idTag)"));
					break;
				case F1_Title:
					lLine = _T("Files.Title");
					break;
				default:
					continue; //not supported, skip the line
					break;
			}

			if (lEncVal[i].IsEmpty())
			{
				switch(mCombo2[i])
				{
				case T1_Contains: //contains
				case T1_Is: //is
				case T1_StartsWith: //starts with
				case T1_EndsWith: //ends with
					lLine = lLine + _T(" is NULL");
					break;
				case T1_DoesNotContains: //does not contains
				case T1_IsNot: //is not
					lLine = lLine + _T(" is not NULL");
					break;

				/*case T2_IsGreaterThan:
					break;
				case T2_IsLessThan:
					break;
				case T2_IsInTheRange:
					break;				
				case T3_Is:
					break;
				case T3_IsNot:
					break;
				case T3_IsAfter:
					break;
				case T3_IsBefore:
					break;*/
				default:
					continue; //not supported, skip the line
					break;
				}
			}
			else switch(mCombo2[i])
			{
				case T1_Contains: //contains
					lTmp.Format(_T(" LIKE '%%%s%%'"), lEncVal[i]);
					lLine = lLine + lTmp;
					break;
				case T1_DoesNotContains: //does not contains
					lTmp.Format(_T(" NOT LIKE '%%%s%%'"), lEncVal[i]);
					lLine = lLine + lTmp;
					break;
				case T1_Is: //is
					lTmp.Format(_T(" LIKE '%s'"), lEncVal[i]);
					lLine = lLine + lTmp;
					break;
				case T1_IsNot: //is not
					lTmp.Format(_T(" NOT LIKE '%s'"), lEncVal[i]);
					lLine = lLine + lTmp;
					break;
				case T1_StartsWith: //starts with
					lTmp.Format(_T(" LIKE '%s%%'"), lEncVal[i]);
					lLine = lLine + lTmp;
					break;
				case T1_EndsWith: //ends with
					lTmp.Format(_T(" LIKE '%%%s'"), lEncVal[i]);
					lLine = lLine + lTmp;
					break;
				case T2_IsGreaterThan:
					lTmp.Format(_T(" > %s"), lEncVal[i]);
					lLine = lLine + lTmp;
					break;
				case T2_IsLessThan:
					lTmp.Format(_T(" < %s"), lEncVal[i]);
					lLine = lLine + lTmp;
					break;
				case T2_IsInTheRange:
					lTmp.Format(_T(" = %s"), lEncVal[i]);
					lLine = lLine + lTmp;
					break;				
				case T3_Is:
					lTmp.Format(_T(" = '%s'"), lEncVal[i]);
					lLine = lLine + lTmp;
					break;
				case T3_IsNot:
					lTmp.Format(_T(" <> '%s'"), lEncVal[i]);
					lLine = lLine + lTmp;
					break;
				case T3_IsAfter:
					lTmp.Format(_T(" > '%s'"), lEncVal[i]);
					lLine = lLine + lTmp;
					break;
				case T3_IsBefore:
					lTmp.Format(_T(" < '%s'"), lEncVal[i]);
					lLine = lLine + lTmp;
					break;
				default:
					continue; //not supported, skip the line
					break;
			}
			
			if(i == 0)
			{
				outStr = _T("(") + lLine + _T(")");
			}
			else
			{
				outStr = outStr + _T(" and ") + _T("(") + lLine + _T(")");
			}
		}

		inDBSQL->AddCondition(outStr);

		//////////////////////////////
		// apply limits
		int lLimitNumber = 0;
		try
		{
			lLimitNumber = _wtol(mLimitToStr);
		}
		catch(...)
		{
			mLimitToBool = false;
		}

		if (mLimitToBool)
		{
			switch(mLimitToCombo)
			{
			case L1_MB:
				outStr.Format(_T("sum(Files.SizeLo) <= %u"), lLimitNumber);
				inDBSQL->AddCondition(outStr);
				break;
			case L1_GB:
				outStr.Format(_T("sum(Files.SizeLo) <= %u"), lLimitNumber);
				inDBSQL->AddCondition(outStr);
				break;
			case L1_Items:
			default:
				outStr.Format(_T("limit %u"), lLimitNumber);
				inDBSQL->AddLimit(outStr);
				break;
			}
		}
	}
	catch(...)
	{
		return false;
	}
	
	return true;
}

bool CTSmartColl::DeleteByID(int inSmartCollID)
{
	try
	{
		CppSQLite3Query lQ;

		//delete the references
		CString lSQL;
		//lSQL.Format(_T("delete from Conditions where Conditions.idCondition = SmartCollections_has_Conditions.Conditions_idCondition AND SmartCollections_has_Conditions.SmartCollections_idSmartColl = %u"), inSmartCollID);
		//m_pDB->mDB.execDML(lSQL);
		lSQL.Format(_T("delete from SmartCollections_has_Conditions where SmartCollections_idSmartColl = %u"), inSmartCollID);
		m_pDB->mDB.execDML(lSQL);
		lSQL.Format(_T("delete from SmartCollections where idSmartColl = %u"), inSmartCollID);
		m_pDB->mDB.execDML(lSQL);
	}
	catch(...)
	{
		return false;
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////
// class CTTags
// represents the Tags table and the operations over them
////////////////////////////////////////////////////////////////////////////////////////
CTTags::CTTags(CDB *pDB):CDBTable(pDB)
{
	m_pDB = pDB;
}
bool CTTags::LoadCurrentRow()
{
	try
	{
		mDataIsValid = false;
		if(!mQ.eof())
		{
			mID = mQ.getIntField(0);
			mName = mQ.getStringField(1);
			mDataIsValid = true;
		}
	}
	catch(...)
	{
		return false;
	}
	return true;
}


//returns the added tags id
int CTTags::AddNewTag(LPCTSTR inTagName)
{
	int lRetVal = 0;
	try
	{
		CString lSQL;
		CString lEncTagName;
		EncodeString(inTagName, lEncTagName);
		lSQL.Format(_T("select idTag from Tags where Name = '%s';"), lEncTagName);
		 
		CppSQLite3Query lQ = m_pDB->mDB.execQuery(lSQL); 
		if(!lQ.eof())
		{
			return lQ.getIntField(0, lRetVal);
		}
	
		lSQL.Format(_T("insert into Tags values (NULL, '%s', NULL)"), lEncTagName);
		m_pDB->mDB.execDML(lSQL);
		
		lSQL.Format(_T("select idTag from Tags where Name = '%s';"), lEncTagName);
		lQ = m_pDB->mDB.execQuery(lSQL); 
		if(!lQ.eof())
		{
			return lQ.getIntField(0, lRetVal);
		}
	}
	catch(...)
	{
	}
	return lRetVal;
}

bool CTTags::DeleteByName(LPCTSTR inTagName)
{
	try
	{
		CString lSQL;
		CString lEncTagName;
		EncodeString(inTagName, lEncTagName);
		lSQL.Format(_T("delete from Tags where Name = '%s';"), lEncTagName);
		m_pDB->mDB.execDML(lSQL);
	}
	catch(...)
	{
		return false;
	}
	return true;
}

int CTTags::GetIDByName(LPCTSTR inTagName)
{
	int lID = -1;

	try
	{
		CString lSQL;
		CString lEncTagName;
		EncodeString(inTagName, lEncTagName);
		lSQL.Format(_T("select from Tags where Name = '%s';"), lEncTagName);
		m_pDB->mDB.execDML(lSQL);

		if (LoadCurrentRow())
		{
			lID = mID;		
		}
	}
	catch(...)
	{
		return false;
	}
	return lID;
}


int CTTags::AddNewTags(CStringList &inTagNames)
{
	ASSERT (0);
	return 0;
}

bool CTTags::GetTags()
{
	try
	{
		CString lSQL;
		lSQL.Format(_T("select * from Tags;"));
		mQ = m_pDB->mDB.execQuery(lSQL); 
	}
	catch(...)
	{
		return false;
	}

	LoadCurrentRow();
	return true;
}

bool CTTags::GetTagsByFileID(int inFileID)
{
	try
	{
		CString lSQL;
		lSQL.Format(_T("select Tags.* from Tags, Files_has_Tags where Files_has_Tags.Files_idFile = %d and Files_has_Tags.Tags_idTag = Tags.idTag;"), inFileID);
		mQ = m_pDB->mDB.execQuery(lSQL); 
	}
	catch(...)
	{
		return false;
	}

	LoadCurrentRow();
	return true;
}

bool CTTags::GetTagsByWFolderID(int inWFolderID)
{
	try
	{
		CString lSQL;
		lSQL.Format(_T("select Tags.* from Tags, WFolders_has_Tags where WFolders_has_Tags.WFolders_idWFolder = %d and WFolders_has_Tags.Tags_idTag = Tags.idTag;"), inWFolderID);
		mQ = m_pDB->mDB.execQuery(lSQL); 
	}
	catch(...)
	{
		return false;
	}

	LoadCurrentRow();
	return true;
}

bool CTTags::GetCIDDataByWFolderID(CIDData &outIDData, int inWFolderID)
{
	bool lRetVal = false;
	outIDData.RemoveAll();
	if(GetTagsByWFolderID(inWFolderID))
	{
		while(mDataIsValid)
		{
			outIDData.AddData(mID, mName);
			LoadNextRow();
			lRetVal = true;
		}
	}
	return lRetVal;
}

bool CTTags::GetCIDDataByFileID(CIDData &outIDData, int inFileID)
{
	bool lRetVal = false;
	outIDData.RemoveAll();
	if(GetTagsByFileID(inFileID))
	{
		while(mDataIsValid)
		{
			outIDData.AddData(mID, mName);
			LoadNextRow();
			lRetVal = true;
		}
	}
	return lRetVal;
}

int CTTags::GetTagsByFileID_Formatted(CString &outStr, int inFileID)
{
	int lTagsCount = 0;
	CIDData lIDData;
	if(GetCIDDataByFileID(lIDData, inFileID))
	{
		lTagsCount = lIDData.GetCount();
		lIDData.GetStringsFormatted(outStr);
	}
	return lTagsCount;

/*

	try
	{
		if(GetTagsByFileID(inFileID))
		{
			outStr = _T("");

			while(mDataIsValid)
			{
				if(lTagsCount == 0)
					outStr = mName;
				else
					outStr = outStr + _T("; ") + mName ;
				LoadNextRow();
				lTagsCount++;
			}
		}
	}
	catch(...)
	{
		return -1;
	}
	return lTagsCount;
*/
}

bool CTTags::GetTagsNotAssignedToFileID(int inFileID)
{
	ASSERT(0);  	return false; //not working

	try
	{
		CString lSQL;
		lSQL.Format(_T("select * from Tags where idTag = (select idTag from Tags except select Tags_idTag from Files_has_Tags where Files_idFile <> %d);"), inFileID);
		mQ = m_pDB->mDB.execQuery(lSQL); 
	}
	catch(...)
	{
		return false;
	}

	LoadCurrentRow();
	return true;
}

bool CTTags::UpdateByID()
{
	try
	{
		CString lSQL;
		CString lEncTagName;
		EncodeString(mName, lEncTagName);
		lSQL.Format(_T("update Tags set Name = '%s' where idTag = %d;"), lEncTagName, mID);
		m_pDB->mDB.execDML(lSQL);
	}
	catch(...)
	{
		return false;
	}
	return true;
}

int CTTags::GetPopularity()
{
	int lRetVal = -1;
	try
	{
		CString lSQL;
		CString lEncTagName;
		EncodeString(mName, lEncTagName);
		lSQL.Format(_T("select count(Tags_idTag) from Files_has_Tags where Tags_idTag = %d;"), mID);
		CppSQLite3Query lQ;
		lQ = m_pDB->mDB.execQuery(lSQL);
		lRetVal = lQ.getIntField(0);
	}
	catch(...)
	{
	}
	return lRetVal;
}

//case 118
bool CTTags::GetAllTagsAssignedToFileIDs(CIntArray *inFileIDs, CIDData &outTagsIDData)
{
	try
	{
		if(inFileIDs == NULL)
		{
			return false;
		}

		int lNumFileIDs = (int)inFileIDs->GetCount();

		CString lSubSql;
		CString lSQL;
		int lFileID;
		for (int i = 0; i < lNumFileIDs; i++)
		{
			lFileID = inFileIDs->GetAt(i);
			CString lTmp;
			if (lSubSql.GetLength() == 0)
			{
				lTmp.Format(_T("(Files_has_Tags.Files_idFile = %d)"), lFileID);
			}
			else
			{
				lTmp.Format(_T(" OR (Files_has_Tags.Files_idFile = %d)"), lFileID);
			}
			lSubSql += lTmp;
		}
		lSQL.Format(_T("select distinct * from Tags, Files_has_Tags where Tags.idTag = Files_has_Tags.Tags_idTag AND (%s) group by Files_has_Tags.Tags_idTag order by Name;"), lSubSql);
		CppSQLite3Query lQ = m_pDB->mDB.execQuery(lSQL); 

		int lID;
		CString lName;
		while(!lQ.eof())
		{
			lID = lQ.getIntField(0);
			lName = lQ.getStringField(1);
			outTagsIDData.AddData(lID, lName);
			lQ.nextRow();
		}
	}
	catch(...)
	{
		return false;
	}
	return true;
}

bool CTTags::GetAllTagsNotAssignedToFileIDs(CIntArray *inFileIDs, CIDData &outTagsIDData)
{
	try
	{
		if(inFileIDs == NULL)
			return false;


		int lNumFileIDs = (int)inFileIDs->GetCount();

		CString lSubSql;
		CString lSQL;

		//JOZO
		//tento SQL nie je moc dobry, pretoze vracia iba tie tag-id, ktore nie su pridelene pre ani jeden file id
		//zatial budem vracat vzdy vsetky tagy
/*
		int lFileID;
		for(int i = 0; i < lNumFileIDs; i++)
		{
			lFileID = inFileIDs->GetAt(i);
			CString lTmp;
			if(lSubSql.GetLength() == 0)
				lTmp.Format(_T("(Files_has_Tags.Files_idFile = %d)"), lFileID);
			else
				lTmp.Format(_T(" OR (Files_has_Tags.Files_idFile = %d)"), lFileID);
			lSubSql += lTmp;
		}
		lSQL.Format(_T("select * from Tags where Tags.idTag NOT IN (select distinct Tags_idTag from Files_has_Tags where %s);"), lSubSql);
		CppSQLite3Query lQ = m_pDB->mDB.execQuery(lSQL); 
*/
		lSQL.Format(_T("select * from Tags order by Name;"));
		CppSQLite3Query lQ = m_pDB->mDB.execQuery(lSQL); 

		int lID;
		CString lName;
		while(!lQ.eof())
		{
			lID = lQ.getIntField(0);
			lName = lQ.getStringField(1);
			outTagsIDData.AddData(lID, lName);
			lQ.nextRow();
		}
	}
	catch(...)
	{
		return false;
	}
	return true;
}

//----------------------------------------------------------
//! CDBTransaction
//!
//----------------------------------------------------------


CDBTransaction::CDBTransaction()
{
	/*
	mCommit = false;

	CString lSQL = _T("begin transaction;");
	try
	{
		m_pDB->mDB.execDML(lSQL);
	}
	catch(CppSQLite3Exception e)
	{
		ASSERT(0); //transaction ENTER failed
	}
	*/
}
CDBTransaction::CDBTransaction(PARAM_FOR_CDB *pParamCDB)
{
	m_pParamCDB = pParamCDB;
	m_pDB = pParamCDB->pDB;
	mCommit = false;

	CString lSQL = _T("begin transaction;");
	try
	{
		m_pDB->mDB.execDML(lSQL);
	}
	catch(CppSQLite3Exception e)
	{
		ASSERT(0); //transaction ENTER failed
	}
}

CDBTransaction::~CDBTransaction()
{
	CString lSQL;
	if(mCommit == true)
	{
		lSQL = _T("commit;");
	}
	else
	{
		lSQL = _T("rollback;");
	}

	try
	{
		m_pDB->mDB.execDML(lSQL);
	}
	catch(CppSQLite3Exception e)
	{
		ASSERT(0); //transaction END failed
	}
}



bool CTFiles::DeleteByID(int inFileID)
{
	try
	{
		CppSQLite3Query lQ;

		CString lSQL;

		lSQL.Format(_T("delete from Collections_has_Files where Files_idFile = %d"), inFileID);
		m_pDB->mDB.execDML(lSQL);
		
		//int lRet = m_pDB->mDB.execDML(lSQL);
		//while (lRet == SQLITE_BUSY)
		//	lRet = m_pDB->mDB.execDML(lSQL);

		lSQL.Format(_T("delete from Files_has_Tags where Files_idFile = %d"), inFileID);
		m_pDB->mDB.execDML(lSQL);
		//lRet = m_pDB->mDB.execDML(lSQL);
		//while (lRet == SQLITE_BUSY)
		//	lRet = m_pDB->mDB.execDML(lSQL);

		lSQL.Format(_T("delete from Files where idFile = %d"), inFileID);
		m_pDB->mDB.execDML(lSQL);
		//lRet = m_pDB->mDB.execDML(lSQL);
		//while (lRet == SQLITE_BUSY)
		//	lRet = m_pDB->mDB.execDML(lSQL);

		lSQL.Format(_T("delete from AddedFiles where FileID = %d"), inFileID);
		m_pDB->mDB.execDML(lSQL);

		CString lLogMsg;
		lLogMsg.Format(_T("Delete file by ID: %d"), inFileID);
		CLog::StoreUserMessage(lLogMsg);



	}
	catch(...)
	{
		CString lLogMsg;
		lLogMsg.Format(_T("Delete file by ID error: %d"), inFileID);
		CLog::StoreUserMessage(lLogMsg);
		return false;
	}

	return true;
}
//----------------------------------------------------------
//! CTFilesUpdateOnIdle
//! 
//----------------------------------------------------------

CTFilesUpdateOnIdle::CTFilesUpdateOnIdle()
{
	mNumFiles = -1;
	mFileIndex = -1;
}
CTFilesUpdateOnIdle::~CTFilesUpdateOnIdle()
{
}

bool CTFilesUpdateOnIdle::UpdateFiles()
{
	bool lResult = true;
	//CIntArray lFilesIDs;
	try
	{
		if(mFileIndex == -1)
		{
			if(GetFiles())
			{
				mFileIndex = 0;
			}
		}

		if(mFileIndex >= 0)
		{
			while(mDataIsValid)
			{
				UpdateFileFromDisk(true);
				//lFilesIDs.Add(mID);
			
				LoadNextRow();
				mFileIndex++;
				if((mFileIndex % 20) == 0)
				{
					break;
				}
			}
			if(! mDataIsValid)
			{
				mFileIndex = -1;
			}
		}
	}
	catch(...)
	{
		lResult = false;
	}

	// refresh files
	//CUtils::RefreshFiles(&lFilesIDs);
	return lResult;
}

////////////////////////////////////////////////////////////////////////////////////////
// class CDB
// interface to our internal SQLite database

//for watch
BOOL CDB::Initalize_For_Watch(LPCTSTR path)
{
	TCHAR lDrv[_MAX_DRIVE];
	TCHAR lFld[_MAX_DIR];
	TCHAR lNam[_MAX_FNAME];
	TCHAR lExt[_MAX_EXT];

	_tsplitpath(path, lDrv, lFld, lNam, lExt);

	m_strLibPath = path;
	m_strLibName = lNam;

	if (!Open(path)) return false;
	if (!CreateTables_For_Watch()) return false;

	return true;
}


BOOL CDB::CreateTables_For_Watch()
{
	CString lSQL = _T("CREATE TABLE librarys (id INTEGER PRIMARY KEY AUTOINCREMENT, Lib VARCHAR NULL ,Mode INTEGER ,State INTEGER );");
	try
	{
		mDB.execDML(lSQL);
	}
	catch(CppSQLite3Exception e)
	{
		;
	}

	return true;
}


//for default
BOOL CDB::CreateTables_For_Default()
{
	CString lSQL = _T("CREATE TABLE defaultlibrary (id INTEGER PRIMARY KEY AUTOINCREMENT, Lib VARCHAR NULL );");
	CString strTmp;

	try
	{
		mDB.execDML(lSQL);
	}
	catch(CppSQLite3Exception e)
	{
		;
	}

	lSQL = _T("CREATE TABLE globalsetting (id INTEGER PRIMARY KEY AUTOINCREMENT, InfoName VARCHAR NULL,Value VARCHAR NULL );");
	try
	{
		mDB.execDML(lSQL);

		strTmp = GetDefaultPath(_T(""));

		if(strTmp.IsEmpty()) 
		{
			AfxMessageBox(_T("to get Workspace path is error1"));
			ExitProcess(0);
		}
		
		lSQL.Format(_T("insert into globalsetting values (1, 'defaultPathForCreateLibrary','%s')"),strTmp);
		mDB.execDML(lSQL);

	}
	catch(CppSQLite3Exception e)
	{
		;
	}

	return true;
}

BOOL CDB::Initalize_For_Default(LPCTSTR path)
{
	TCHAR lDrv[_MAX_DRIVE];
	TCHAR lFld[_MAX_DIR];
	TCHAR lNam[_MAX_FNAME];
	TCHAR lExt[_MAX_EXT];

	_tsplitpath(path, lDrv, lFld, lNam, lExt);

	m_strLibPath = path;
	m_strLibName = lNam;

	if (!Open(path)) return false;
	if (!CreateTables_For_Default()) return false;

	return true;
}
BOOL CDB::SetDefaultLibrary_For_Default(LPCTSTR lib)
{
	CString lSQL; 
	CppSQLite3Query mQ;

	lSQL.Format(_T("delete from defaultlibrary"));
	try
	{
		mDB.execDML(lSQL);
	}
	catch(CppSQLite3Exception e)
	{
		return false;
	}

	lSQL.Format(_T("insert into defaultlibrary values(%d,'%s');"),1,lib);
	try
	{
		mDB.execDML(lSQL);
	}
	catch(CppSQLite3Exception e)
	{
		return false;
	}
	return true;
}
BOOL CDB::GetDefaultLibrary_For_Default(TCHAR* lib)
{
	CString lSQL; 
	CppSQLite3Query mQ;
	LPCTSTR psz;

	lSQL.Format(_T("select * from defaultlibrary"));
	try
	{
		mQ = mDB.execQuery(lSQL);
		if(!mQ.eof())
		{
			psz = mQ.getStringField(1);
			wcscpy(lib,psz);
		}
	}
	catch(CppSQLite3Exception e)
	{
		return false;
	}

	return true;
}

int CDB::SetGlobalSettingInfo_For_Default(TCHAR *infoname , TCHAR *infovalue)
{
	CString lSQL;
	lSQL.Format(_T("update globalsetting set Value ='%s' where InfoName = '%s';"), infovalue, infoname);
	mDB.execDML(lSQL);
	return 1;
}

int CDB::GetGlobalSettingInfo_For_Default(TCHAR *infoname , TCHAR *infovalue)
{
	CString lSQL;
	lSQL.Format(_T("select Value from globalsetting where InfoName = '%s';"),  infoname);
	CppSQLite3Query lq = mDB.execQuery(lSQL);

	LPCTSTR pszValue;

	if(!lq.eof())
	{
			pszValue = lq.getStringField(0);
			wcscpy(infovalue,pszValue);
			return 1;
	}

	return 0;
}





BOOL CDB::SetLibrarySetting_For_Watch(LPCTSTR lib, int mode ,int state)
{
	CString lSQL; 
	CppSQLite3Query mQ;
	int nId;

	lSQL.Format(_T("select * from librarys where Lib='%s';"),lib);
	mQ = mDB.execQuery(lSQL);

	if(!mQ.eof()) //exist
	{
		lSQL.Format(_T("update librarys set Mode=%d , State=%d where Lib='%s';"),mode,state,lib);
		try
		{
			mDB.execDML(lSQL);
		}
		catch(CppSQLite3Exception e)
		{
			return false;
		}
	}
	else //new
	{
		lSQL.Format(_T("select max(id) from librarys;"));
		mQ =  mDB.execQuery(lSQL); 
		nId = mQ.getIntField(0);
		nId = nId + 1;

		lSQL.Format(_T("insert into librarys values(%d,'%s',%d,%d);"),nId,lib,mode,state);
		try
		{
			mDB.execDML(lSQL);
		}
		catch(CppSQLite3Exception e)
		{
			return false;
		}
	}
	return true;
}


////////////////////////////////////////////////////////////////////////////////////////
CDB::CDB(void)
{
	mCollTWasCreated = FALSE;
}

CDB::~CDB(void)
{

}

bool CDB::Initialize(LPCTSTR path)
{
	TCHAR lDrv[_MAX_DRIVE];
	TCHAR lFld[_MAX_DIR];
	TCHAR lNam[_MAX_FNAME];
	TCHAR lExt[_MAX_EXT];

	_tsplitpath(path, lDrv, lFld, lNam, lExt);

	m_strLibPath = path;
	m_strLibName = lNam;

	if (!Open(path)) return false;
	if (!CreateTables()) return false;
	if (!Patches())  return false;

	return true;
}


bool CDB::Open(LPCTSTR inPath)
{
	try
	{
		mDB.open(inPath);
	}
	catch (...)
	{
		return false;
	}
	return true;
}

//
bool CDB::Close()
{
	mDB.close();
	return true;
}

void CDB::CreateDefaultDBEntries()
{
	try
	{
		CString lSQL;
		int i = 1; 
//TODO: apostroph ' is not allowed! if you wana use it, you have to use the EncodeString funtion to encode the string

		// Default Extensions
		lSQL.Format(_T("insert into FExtensions values (NULL, '%s', '%s', NULL, %u)"), _T("doc"), _T("Microsoft Word 1997-2003 Document"), FILTERTREE_MAJOR_DOC - FILTERTREE_MAJOR_BASE);
		mDB.execDML(lSQL);
		lSQL.Format(_T("insert into FExtensions values (NULL, '%s', '%s', NULL, %u)"), _T("xls"), _T("Microsoft Excel 1997-2003 Worksheet"), FILTERTREE_MAJOR_SPREAD - FILTERTREE_MAJOR_BASE);
		mDB.execDML(lSQL);
		lSQL.Format(_T("insert into FExtensions values (NULL, '%s', '%s', NULL, %u)"), _T("ppt"), _T("Microsoft PowerPoint 1997-2003 Presentation"), FILTERTREE_MAJOR_PRES - FILTERTREE_MAJOR_BASE);
		mDB.execDML(lSQL);
		lSQL.Format(_T("insert into FExtensions values (NULL, '%s', '%s', NULL, %u)"), _T("pdf"), _T("PDF Document"), FILTERTREE_MAJOR_DOC - FILTERTREE_MAJOR_BASE);
		mDB.execDML(lSQL);
		lSQL.Format(_T("insert into FExtensions values (NULL, '%s', '%s', NULL, %u)"), _T("rtf"), _T("Rich Text Format"), FILTERTREE_MAJOR_DOC - FILTERTREE_MAJOR_BASE);
		mDB.execDML(lSQL);
		lSQL.Format(_T("insert into FExtensions values (NULL, '%s', '%s', NULL, %u)"), _T("docm"), _T("Microsoft Word Macro-Enabled Document"), FILTERTREE_MAJOR_DOC - FILTERTREE_MAJOR_BASE);
		mDB.execDML(lSQL);
	
		lSQL.Format(_T("insert into FExtensions values (NULL, '%s', '%s', NULL, %u)"), _T("xps"), _T("XPS Document"), FILTERTREE_MAJOR_DOC - FILTERTREE_MAJOR_BASE);
		mDB.execDML(lSQL);
		lSQL.Format(_T("insert into FExtensions values (NULL, '%s', '%s', NULL, %u)"), _T("jpg"), _T("JPG Image"), FILTERTREE_MAJOR_IMAGES - FILTERTREE_MAJOR_BASE);
		mDB.execDML(lSQL);
		lSQL.Format(_T("insert into FExtensions values (NULL, '%s', '%s', NULL, %u)"), _T("gif"), _T("GIF Image"), FILTERTREE_MAJOR_IMAGES - FILTERTREE_MAJOR_BASE);
		mDB.execDML(lSQL);
		lSQL.Format(_T("insert into FExtensions values (NULL, '%s', '%s', NULL, %u)"), _T("tif"), _T("TIF Image"), FILTERTREE_MAJOR_IMAGES - FILTERTREE_MAJOR_BASE);
		mDB.execDML(lSQL);
		lSQL.Format(_T("insert into FExtensions values (NULL, '%s', '%s', NULL, %u)"), _T("bmp"), _T("BMP Image"), FILTERTREE_MAJOR_IMAGES - FILTERTREE_MAJOR_BASE);
		mDB.execDML(lSQL);
		lSQL.Format(_T("insert into FExtensions values (NULL, '%s', '%s', NULL, %u)"), _T("png"), _T("PNG Image"), FILTERTREE_MAJOR_IMAGES - FILTERTREE_MAJOR_BASE);
		mDB.execDML(lSQL);
		lSQL.Format(_T("insert into FExtensions values (NULL, '%s', '%s', NULL, %u)"), _T("ico"), _T("Icon File"), FILTERTREE_MAJOR_IMAGES - FILTERTREE_MAJOR_BASE);
		mDB.execDML(lSQL);
		lSQL.Format(_T("insert into FExtensions values (NULL, '%s', '%s', NULL, %u)"), _T("html"), _T("HTML Document"), FILTERTREE_MAJOR_DOC - FILTERTREE_MAJOR_BASE);
		mDB.execDML(lSQL);
		lSQL.Format(_T("insert into FExtensions values (NULL, '%s', '%s', NULL, %u)"), _T("js"), _T("JavaScript File"), FILTERTREE_MAJOR_OTHER - FILTERTREE_MAJOR_BASE);
		mDB.execDML(lSQL);
		lSQL.Format(_T("insert into FExtensions values (NULL, '%s', '%s', NULL, %u)"), _T("css"), _T("CSS File"), FILTERTREE_MAJOR_OTHER - FILTERTREE_MAJOR_BASE);
		mDB.execDML(lSQL);
		lSQL.Format(_T("insert into FExtensions values (NULL, '%s', '%s', NULL, %u)"), _T("txt"), _T("Plain Text File"), FILTERTREE_MAJOR_DOC - FILTERTREE_MAJOR_BASE);
		mDB.execDML(lSQL);
		// Next extension descriptions are from http://www.fileinfo.net/
		lSQL.Format(_T("insert into FExtensions values (NULL, '%s', '%s', NULL, %u)"), _T("zip"), _T("Zipped File"), FILTERTREE_MAJOR_OTHER - FILTERTREE_MAJOR_BASE);
		mDB.execDML(lSQL);
		lSQL.Format(_T("insert into FExtensions values (NULL, '%s', '%s', NULL, %u)"), _T("rar"), _T("WinRAR Compressed Archive"), FILTERTREE_MAJOR_OTHER - FILTERTREE_MAJOR_BASE);
		mDB.execDML(lSQL);
		lSQL.Format(_T("insert into FExtensions values (NULL, '%s', '%s', NULL, %u)"), _T("iso"), _T("Disc Image File"), FILTERTREE_MAJOR_OTHER - FILTERTREE_MAJOR_BASE);
		mDB.execDML(lSQL);
		lSQL.Format(_T("insert into FExtensions values (NULL, '%s', '%s', NULL, %u)"), _T("csv"), _T("Comma Separated Values File"), FILTERTREE_MAJOR_SPREAD - FILTERTREE_MAJOR_BASE);
		mDB.execDML(lSQL);
		lSQL.Format(_T("insert into FExtensions values (NULL, '%s', '%s', NULL, %u)"), _T("xml"), _T("XML File"), FILTERTREE_MAJOR_OTHER - FILTERTREE_MAJOR_BASE);
		mDB.execDML(lSQL);
		lSQL.Format(_T("insert into FExtensions values (NULL, '%s', '%s', NULL, %u)"), _T("docx"), _T("Microsoft Word Open XML Document"), FILTERTREE_MAJOR_DOC - FILTERTREE_MAJOR_BASE);
		mDB.execDML(lSQL);
		lSQL.Format(_T("insert into FExtensions values (NULL, '%s', '%s', NULL, %u)"), _T("dot"), _T("Microsoft Word Template"), FILTERTREE_MAJOR_DOC - FILTERTREE_MAJOR_BASE);
		mDB.execDML(lSQL);
		lSQL.Format(_T("insert into FExtensions values (NULL, '%s', '%s', NULL, %u)"), _T("dotx"), _T("Word 2007 Document Template"), FILTERTREE_MAJOR_DOC - FILTERTREE_MAJOR_BASE);
		mDB.execDML(lSQL);
		lSQL.Format(_T("insert into FExtensions values (NULL, '%s', '%s', NULL, %u)"), _T("egt"), _T("Universal Document File"), FILTERTREE_MAJOR_DOC - FILTERTREE_MAJOR_BASE);
		mDB.execDML(lSQL);
		lSQL.Format(_T("insert into FExtensions values (NULL, '%s', '%s', NULL, %u)"), _T("odt"), _T("OpenDocument Text Document"), FILTERTREE_MAJOR_DOC - FILTERTREE_MAJOR_BASE); // ?
		mDB.execDML(lSQL);
		lSQL.Format(_T("insert into FExtensions values (NULL, '%s', '%s', NULL, %u)"), _T("ott"), _T("OpenDocument Text Document Template"), FILTERTREE_MAJOR_DOC - FILTERTREE_MAJOR_BASE); // ?
		mDB.execDML(lSQL);
		lSQL.Format(_T("insert into FExtensions values (NULL, '%s', '%s', NULL, %u)"), _T("wpd"), _T("WordPerfect Document"), FILTERTREE_MAJOR_DOC - FILTERTREE_MAJOR_BASE); // ?
		mDB.execDML(lSQL);
		lSQL.Format(_T("insert into FExtensions values (NULL, '%s', '%s', NULL, %u)"), _T("wps"), _T("Microsoft Works Word Processor Document"), FILTERTREE_MAJOR_DOC - FILTERTREE_MAJOR_BASE); // ?
		mDB.execDML(lSQL);
		lSQL.Format(_T("insert into FExtensions values (NULL, '%s', '%s', NULL, %u)"), _T("wpt"), _T("WordPerfect Template"), FILTERTREE_MAJOR_DOC - FILTERTREE_MAJOR_BASE); // ?
		mDB.execDML(lSQL);
		lSQL.Format(_T("insert into FExtensions values (NULL, '%s', '%s', NULL, %u)"), _T("wri"), _T("Windows Write Document"), FILTERTREE_MAJOR_DOC - FILTERTREE_MAJOR_BASE); // ?
		mDB.execDML(lSQL);
		lSQL.Format(_T("insert into FExtensions values (NULL, '%s', '%s', NULL, %u)"), _T("xhtml"), _T("Extensible Hypertext Markup Language File"), FILTERTREE_MAJOR_DOC - FILTERTREE_MAJOR_BASE); // ?
		mDB.execDML(lSQL);
		lSQL.Format(_T("insert into FExtensions values (NULL, '%s', '%s', NULL, %u)"), _T("ps"), _T("PostScript File"), FILTERTREE_MAJOR_DOC - FILTERTREE_MAJOR_BASE); // ?
		mDB.execDML(lSQL);
		lSQL.Format(_T("insert into FExtensions values (NULL, '%s', '%s', NULL, %u)"), _T("odp"), _T("OpenDocument Presentation"), FILTERTREE_MAJOR_PRES - FILTERTREE_MAJOR_BASE); // ?
		mDB.execDML(lSQL);
		lSQL.Format(_T("insert into FExtensions values (NULL, '%s', '%s', NULL, %u)"), _T("otp"), _T("OpenDocument Presentation Template"), FILTERTREE_MAJOR_PRES - FILTERTREE_MAJOR_BASE); // ?
		mDB.execDML(lSQL);
		lSQL.Format(_T("insert into FExtensions values (NULL, '%s', '%s', NULL, %u)"), _T("pps"), _T("PowerPoint Slide Show"), FILTERTREE_MAJOR_PRES - FILTERTREE_MAJOR_BASE);
		mDB.execDML(lSQL);
		lSQL.Format(_T("insert into FExtensions values (NULL, '%s', '%s', NULL, %u)"), _T("sti"), _T("StarOffice Presentation Template"), FILTERTREE_MAJOR_PRES - FILTERTREE_MAJOR_BASE); // ?
		mDB.execDML(lSQL);
		lSQL.Format(_T("insert into FExtensions values (NULL, '%s', '%s', NULL, %u)"), _T("sxi"), _T("StarOffice Impress Presentation"), FILTERTREE_MAJOR_PRES - FILTERTREE_MAJOR_BASE); // ?
		mDB.execDML(lSQL);
		lSQL.Format(_T("insert into FExtensions values (NULL, '%s', '%s', NULL, %u)"), _T("ods"), _T("OpenDocument Spreadsheet"), FILTERTREE_MAJOR_SPREAD - FILTERTREE_MAJOR_BASE);
		mDB.execDML(lSQL);
		lSQL.Format(_T("insert into FExtensions values (NULL, '%s', '%s', NULL, %u)"), _T("ots"), _T("OpenDocument Spreadsheet Template"), FILTERTREE_MAJOR_SPREAD - FILTERTREE_MAJOR_BASE); // ?
		mDB.execDML(lSQL);
		lSQL.Format(_T("insert into FExtensions values (NULL, '%s', '%s', NULL, %u)"), _T("stc"), _T("StarOffice Calc Spreadsheet Template"), FILTERTREE_MAJOR_SPREAD - FILTERTREE_MAJOR_BASE); // ?
		mDB.execDML(lSQL);
		lSQL.Format(_T("insert into FExtensions values (NULL, '%s', '%s', NULL, %u)"), _T("sxc"), _T("StarOffice Calc Spreadsheet"), FILTERTREE_MAJOR_SPREAD - FILTERTREE_MAJOR_BASE); // ?
		mDB.execDML(lSQL);
		lSQL.Format(_T("insert into FExtensions values (NULL, '%s', '%s', NULL, %u)"), _T("xlk"), _T("Excel Backup File"), FILTERTREE_MAJOR_SPREAD - FILTERTREE_MAJOR_BASE); // ?
		mDB.execDML(lSQL);
		lSQL.Format(_T("insert into FExtensions values (NULL, '%s', '%s', NULL, %u)"), _T("xlsx"), _T("Microsoft Excel Open XML Document"), FILTERTREE_MAJOR_SPREAD - FILTERTREE_MAJOR_BASE); // ?
		mDB.execDML(lSQL);
		lSQL.Format(_T("insert into FExtensions values (NULL, '%s', '%s', NULL, %u)"), _T("exe"), _T("Executable File"), FILTERTREE_MAJOR_OTHER - FILTERTREE_MAJOR_BASE);
		mDB.execDML(lSQL);
		lSQL.Format(_T("insert into FExtensions values (NULL, '%s', '%s', NULL, %u)"), _T("msi"), _T("Windows Installer File"), FILTERTREE_MAJOR_OTHER - FILTERTREE_MAJOR_BASE); // ?
		mDB.execDML(lSQL);
		lSQL.Format(_T("insert into FExtensions values (NULL, '%s', '%s', NULL, %u)"), _T("psd"), _T("Photoshop Document"), FILTERTREE_MAJOR_IMAGES - FILTERTREE_MAJOR_BASE);
		mDB.execDML(lSQL);
		lSQL.Format(_T("insert into FExtensions values (NULL, '%s', '%s', NULL, %u)"), _T("tiff"), _T("TIFF Image"), FILTERTREE_MAJOR_IMAGES - FILTERTREE_MAJOR_BASE);
		mDB.execDML(lSQL);
		lSQL.Format(_T("insert into FExtensions values (NULL, '%s', '%s', NULL, %u)"), _T("sxw"), _T("StarOffice Writer Text Document"), FILTERTREE_MAJOR_DOC - FILTERTREE_MAJOR_BASE);
		mDB.execDML(lSQL);
		lSQL.Format(_T("insert into FExtensions values (NULL, '%s', '%s', NULL, %u)"), _T("htm"), _T("Hypertext Markup Language File"), FILTERTREE_MAJOR_DOC - FILTERTREE_MAJOR_BASE);
		mDB.execDML(lSQL);
		lSQL.Format(_T("insert into FExtensions values (NULL, '%s', '%s', NULL, %u)"), _T("jpeg"), _T("JPEG Image File"), FILTERTREE_MAJOR_IMAGES - FILTERTREE_MAJOR_BASE);
		mDB.execDML(lSQL);	
		lSQL.Format(_T("insert into FExtensions values (NULL, '%s', '%s', NULL, %u)"), _T("3gp"), _T("3GPP Multimedia File"), FILTERTREE_MAJOR_VIDEOS - FILTERTREE_MAJOR_BASE);
		mDB.execDML(lSQL);
		lSQL.Format(_T("insert into FExtensions values (NULL, '%s', '%s', NULL, %u)"), _T("asf"), _T("Advanced Systems Format File"), FILTERTREE_MAJOR_VIDEOS - FILTERTREE_MAJOR_BASE);
		mDB.execDML(lSQL);
		lSQL.Format(_T("insert into FExtensions values (NULL, '%s', '%s', NULL, %u)"), _T("aif"), _T("Audio Interchange File Format"), FILTERTREE_MAJOR_VIDEOS - FILTERTREE_MAJOR_BASE);
		mDB.execDML(lSQL);
		lSQL.Format(_T("insert into FExtensions values (NULL, '%s', '%s', NULL, %u)"), _T("asx"), _T("Microsoft ASF Redirector File"), FILTERTREE_MAJOR_VIDEOS - FILTERTREE_MAJOR_BASE);
		mDB.execDML(lSQL);
		lSQL.Format(_T("insert into FExtensions values (NULL, '%s', '%s', NULL, %u)"), _T("avi"), _T("Audio Video Interleave File"), FILTERTREE_MAJOR_VIDEOS - FILTERTREE_MAJOR_BASE);
		mDB.execDML(lSQL);
		lSQL.Format(_T("insert into FExtensions values (NULL, '%s', '%s', NULL, %u)"), _T("mp4"), _T("MPEG-4 Video File"), FILTERTREE_MAJOR_VIDEOS - FILTERTREE_MAJOR_BASE);
		mDB.execDML(lSQL);
		lSQL.Format(_T("insert into FExtensions values (NULL, '%s', '%s', NULL, %u)"), _T("mpg"), _T("MPEG Video File"), FILTERTREE_MAJOR_VIDEOS - FILTERTREE_MAJOR_BASE);
		mDB.execDML(lSQL);
		lSQL.Format(_T("insert into FExtensions values (NULL, '%s', '%s', NULL, %u)"), _T("mpeg"), _T("MPEG Video File"), FILTERTREE_MAJOR_VIDEOS - FILTERTREE_MAJOR_BASE);
		mDB.execDML(lSQL);
		lSQL.Format(_T("insert into FExtensions values (NULL, '%s', '%s', NULL, %u)"), _T("rm"), _T("Real Media File"), FILTERTREE_MAJOR_VIDEOS - FILTERTREE_MAJOR_BASE);
		mDB.execDML(lSQL);
		lSQL.Format(_T("insert into FExtensions values (NULL, '%s', '%s', NULL, %u)"), _T("swf"), _T("Macromedia Flash Movie"), FILTERTREE_MAJOR_VIDEOS - FILTERTREE_MAJOR_BASE);
		mDB.execDML(lSQL);
		lSQL.Format(_T("insert into FExtensions values (NULL, '%s', '%s', NULL, %u)"), _T("wmv"), _T("Windows Media Video File"), FILTERTREE_MAJOR_VIDEOS - FILTERTREE_MAJOR_BASE);
		mDB.execDML(lSQL);
		lSQL.Format(_T("insert into FExtensions values (NULL, '%s', '%s', NULL, %u)"), _T("chm"), _T("HTML Help"), FILTERTREE_MAJOR_OTHER - FILTERTREE_MAJOR_BASE);
		mDB.execDML(lSQL);
		lSQL.Format(_T("insert into FExtensions values (NULL, '%s', '%s', NULL, %u)"), _T("djvu"), _T("DjVu File"), FILTERTREE_MAJOR_OTHER - FILTERTREE_MAJOR_BASE);
		mDB.execDML(lSQL);
							
		// Default Filters
		lSQL.Format(_T("insert into MFilters values (%u, 'All', NULL)"), FILTERTREE_MAJOR_ALL - FILTERTREE_MAJOR_BASE);
		mDB.execDML(lSQL);
		lSQL.Format(_T("insert into MFilters values (%u, 'Documents', NULL)"), FILTERTREE_MAJOR_DOC - FILTERTREE_MAJOR_BASE);
		mDB.execDML(lSQL);
		lSQL.Format(_T("insert into MFilters values (%u, 'Presentations', NULL)"), FILTERTREE_MAJOR_PRES - FILTERTREE_MAJOR_BASE);
		mDB.execDML(lSQL);
		lSQL.Format(_T("insert into MFilters values (%u, 'Spreadsheats', NULL)"), FILTERTREE_MAJOR_SPREAD - FILTERTREE_MAJOR_BASE);
		mDB.execDML(lSQL);		
		lSQL.Format(_T("insert into MFilters values (%u, 'Images', NULL)"), FILTERTREE_MAJOR_IMAGES - FILTERTREE_MAJOR_BASE);
		mDB.execDML(lSQL);
		lSQL.Format(_T("insert into MFilters values (%u, 'Videos', NULL)"), FILTERTREE_MAJOR_VIDEOS - FILTERTREE_MAJOR_BASE);
		mDB.execDML(lSQL);
		lSQL.Format(_T("insert into MFilters values (%u, 'Other', NULL)"), FILTERTREE_MAJOR_OTHER - FILTERTREE_MAJOR_BASE);
		mDB.execDML(lSQL);
		
		// Default Tags
		lSQL.Format(_T("insert into Tags values (NULL, '2008', NULL)"));
		mDB.execDML(lSQL);
		lSQL.Format(_T("insert into Tags values (NULL, '2009', NULL)"));
		mDB.execDML(lSQL);
		lSQL.Format(_T("insert into Tags values (NULL, '2010', NULL)"));
		mDB.execDML(lSQL);
		lSQL.Format(_T("insert into Tags values (NULL, 'art', NULL)"));
		mDB.execDML(lSQL);
		lSQL.Format(_T("insert into Tags values (NULL, 'beach', NULL)"));
		mDB.execDML(lSQL);
		lSQL.Format(_T("insert into Tags values (NULL, 'career', NULL)"));
		mDB.execDML(lSQL);
		lSQL.Format(_T("insert into Tags values (NULL, 'computer', NULL)"));
		mDB.execDML(lSQL);
		lSQL.Format(_T("insert into Tags values (NULL, 'design', NULL)"));
		mDB.execDML(lSQL);
		lSQL.Format(_T("insert into Tags values (NULL, 'entertainment', NULL)"));
		mDB.execDML(lSQL);
		lSQL.Format(_T("insert into Tags values (NULL, 'exercise', NULL)"));
		mDB.execDML(lSQL);
		lSQL.Format(_T("insert into Tags values (NULL, 'family', NULL)"));
		mDB.execDML(lSQL);
		lSQL.Format(_T("insert into Tags values (NULL, 'finance', NULL)"));
		mDB.execDML(lSQL);
		lSQL.Format(_T("insert into Tags values (NULL, 'fashion', NULL)"));
		mDB.execDML(lSQL);
		lSQL.Format(_T("insert into Tags values (NULL, 'education', NULL)"));
		mDB.execDML(lSQL);
		lSQL.Format(_T("insert into Tags values (NULL, 'friends', NULL)"));
		mDB.execDML(lSQL);
		lSQL.Format(_T("insert into Tags values (NULL, 'fun', NULL)"));
		mDB.execDML(lSQL);
		lSQL.Format(_T("insert into Tags values (NULL, 'health', NULL)"));
		mDB.execDML(lSQL);
		lSQL.Format(_T("insert into Tags values (NULL, 'life', NULL)"));
		mDB.execDML(lSQL);
		lSQL.Format(_T("insert into Tags values (NULL, 'music', NULL)"));
		mDB.execDML(lSQL);
		lSQL.Format(_T("insert into Tags values (NULL, 'personal', NULL)"));
		mDB.execDML(lSQL);
		lSQL.Format(_T("insert into Tags values (NULL, 'purchase', NULL)"));
		mDB.execDML(lSQL);
		lSQL.Format(_T("insert into Tags values (NULL, 'travel', NULL)"));
		mDB.execDML(lSQL);
		lSQL.Format(_T("insert into Tags values (NULL, 'receipt', NULL)"));
		mDB.execDML(lSQL);
		lSQL.Format(_T("insert into Tags values (NULL, 'windows', NULL)"));
		mDB.execDML(lSQL);
		lSQL.Format(_T("insert into Tags values (NULL, 'writing', NULL)"));
		mDB.execDML(lSQL);
		lSQL.Format(_T("insert into Tags values (NULL, 'business', NULL)"));
		mDB.execDML(lSQL);
		lSQL.Format(_T("insert into Tags values (NULL, 'holidays', NULL)"));
		mDB.execDML(lSQL);
	}
	catch(...)
	{
	}
}

bool CDB::CreateTables()
{
	CProgressDlg* lProgressDlg = NULL;
#ifndef DEBUG
#ifndef _BNB_ASSISTANT
	lProgressDlg = new CProgressDlg;
	if (lProgressDlg != NULL)
	{
		lProgressDlg->SetMax(12);
#ifndef _FREE
		lProgressDlg->SetCaption(_T("Benubird PDF Pro"));
#else
		lProgressDlg->SetCaption(_T("Benubird PDF"));
#endif
		lProgressDlg->ShowCancel(FALSE);
		lProgressDlg->CreateProgressDlg();
		lProgressDlg->SetMessage(_T("Creating/loading database for Benubird."));
	}
#endif
#endif

	//CString lStr;
	//clock_t ticks = clock();
	//clock_t ticksStart = ticks;
	//ticks = clock() - ticks;
	//lStr.Format(_T("CREATE TABLE Collections %d"), ticks);
	//gGeneralLog.StoreMessage(lStr);
 
	CString lSQL = _T("CREATE TABLE Collections (idCollection INTEGER PRIMARY KEY AUTOINCREMENT, Name VARCHAR NULL, Created DATE NULL);");
	try
	{
		mDB.execDML(lSQL);
		mCollTWasCreated = TRUE;
		
	}
	catch(CppSQLite3Exception e)
	{
		;
	}
	if(lProgressDlg != NULL)
	{
		lProgressDlg->StepIt();
	}

	lSQL.Format(_T("CREATE TABLE Files (idFile INTEGER PRIMARY KEY AUTOINCREMENT, Path VARCHAR NOT NULL, Name VARCHAR NOT NULL, SizeHi INTEGER, SizeLo INTEGER, CreatedHi INTEGER, CreatedLo INTEGER, ModifiedHi INTEGER, ModifiedLo INTEGER, AccessedHi INTEGER, AccessedLo INTEGER, Attributes INTEGER, Title VARCHAR NULL, Subject VARCHAR NULL, Author VARCHAR NULL, Category VARCHAR NULL, Comments VARCHAR NULL, AddedHi INTEGER, AddedLo INTEGER, BenuFlags INTEGER, FExtensions_idFExtension INTEGER, Created_TmStmp VARCHAR NULL, Modified_TmStmp VARCHAR NULL, Accessed_TmStmp VARCHAR NULL, Added_TmStmp VARCHAR NULL );"));
	try
	{
		mDB.execDML(lSQL);
	}
	catch(CppSQLite3Exception e)
	{
		;
	}
	if(lProgressDlg != NULL)
	{
		lProgressDlg->StepIt();
	}

	lSQL = _T("CREATE TABLE Tags (idTag INTEGER PRIMARY KEY AUTOINCREMENT, Name VARCHAR NULL, Created DATE NULL);");
	try
	{
		mDB.execDML(lSQL);
	}
	catch(CppSQLite3Exception e)
	{
		;
	}
	if(lProgressDlg != NULL)
	{
		lProgressDlg->StepIt();
	}

	lSQL.Format(_T("CREATE TABLE Collections_has_Files (Collections_idCollection INTEGER NOT NULL, Files_idFile INTEGER NOT NULL, PRIMARY KEY(Collections_idCollection, Files_idFile));"));
	try
	{
		mDB.execDML(lSQL);
	}
	catch(CppSQLite3Exception e)
	{
		;
	}
	if(lProgressDlg != NULL)
	{
		lProgressDlg->StepIt();
	}

	lSQL.Format(_T("CREATE TABLE Files_has_Tags (Files_idFile INTEGER NOT NULL, Tags_idTag INTEGER NOT NULL, PRIMARY KEY(Files_idFile, Tags_idTag));"));
	try
	{
		mDB.execDML(lSQL);
	}
	catch(CppSQLite3Exception e)
	{
		;
	}
	if(lProgressDlg != NULL)
	{
		lProgressDlg->StepIt();
	}

	//File extensions table
	lSQL.Format(_T("CREATE TABLE FExtensions (idFExtension INTEGER PRIMARY KEY AUTOINCREMENT, Name VARCHAR NULL, Desc VARCHAR NULL, Created DATE NULL, MFilters_idMfilter INTEGER NULL);"));
	try
	{
		mDB.execDML(lSQL);
	}
	catch(CppSQLite3Exception e)
	{
		;
	}
	if(lProgressDlg != NULL)
	{
		lProgressDlg->StepIt();
	}

	//-------------- major filters
	lSQL.Format(_T("CREATE TABLE MFilters (idMFilter INTEGER PRIMARY KEY AUTOINCREMENT, Name VARCHAR NULL, Created DATE NULL);"));
	try
	{
		mDB.execDML(lSQL);
	}
	catch(CppSQLite3Exception e)
	{
		;
	}
	if(lProgressDlg != NULL)
	{
		lProgressDlg->StepIt();
	}


	//-------------- watched folders
	lSQL = _T("CREATE TABLE WFolders (idWFolder INTEGER PRIMARY KEY AUTOINCREMENT, Name VARCHAR NULL, Path VARCHAR NOT NULL, IncludeSubfolders INTEGER, AllFileType INTEGER, Collections_idCollection INTEGER, Frequency INTEGER);");
	try
	{
		mDB.execDML(lSQL);
	}
	catch(CppSQLite3Exception e)
	{
		;
	}
	if(lProgressDlg != NULL)
	{
		lProgressDlg->StepIt();
	}


	lSQL = _T("CREATE TABLE WFolders_has_FExtensions (WFolders_idWFolder INTEGER NOT NULL, FExtensions_idFExtension INTEGER NOT NULL, PRIMARY KEY(WFolders_idWFolder, FExtensions_idFExtension));");
	try
	{
		mDB.execDML(lSQL);
	}
	catch(CppSQLite3Exception e)
	{
		;
	}
	if(lProgressDlg != NULL)
	{
		lProgressDlg->StepIt();
	}


	lSQL = _T("CREATE TABLE WFolders_has_Tags (WFolders_idWFolder INTEGER NOT NULL, Tags_idTag INTEGER NOT NULL, PRIMARY KEY(WFolders_idWFolder, Tags_idTag));");
	try
	{
		mDB.execDML(lSQL);
		//fill in the default db values
		CreateDefaultDBEntries();

	}
	catch(CppSQLite3Exception e)
	{
		;
	}
	if(lProgressDlg != NULL)
	{
		lProgressDlg->StepIt();
	}



	//////////////////////////////
	//Smart collections
	//lSQL = _T("CREATE TABLE SmartCollections (idSmartColl INTEGER PRIMARY KEY AUTOINCREMENT, Name VARCHAR NOT NULL, LimitToBool INTEGER, LimitToStr VARCHAR NULL, LimitToCombo INTEGER, NumActiveFilters INTEGER, Comb1_0 INTEGER, Comb2_0 INTEGER, Value_0 VARCHAR, Comb1_1 INTEGER, Comb2_1 INTEGER, Value_1 VARCHAR,  Comb1_2 INTEGER, Comb2_2 INTEGER, Value_2 VARCHAR,  Comb1_3 INTEGER, Comb2_3 INTEGER, Value_3 VARCHAR, Comb1_4 INTEGER, Comb2_4 INTEGER, Value_4 VARCHAR,  Comb1_5 INTEGER, Comb2_5 INTEGER, Value_5 VARCHAR,  Comb1_6 INTEGER, Comb2_6 INTEGER, Value_6 VARCHAR,  Comb1_7 INTEGER, Comb2_7 INTEGER, Value_7 VARCHAR,  Comb1_8 INTEGER, Comb2_8 INTEGER, Value_8 VARCHAR,  Comb1_9 INTEGER, Comb2_9 INTEGER, Value_9 VARCHAR);");
	lSQL = _T("CREATE TABLE SmartCollections (idSmartColl INTEGER PRIMARY KEY AUTOINCREMENT, Name VARCHAR NOT NULL, LimitToBool INTEGER, LimitToStr VARCHAR NULL, LimitToCombo INTEGER);");
	try
	{
		mDB.execDML(lSQL);
	}
	catch(CppSQLite3Exception e)
	{
		;
	}
	if(lProgressDlg != NULL)
	{
		lProgressDlg->StepIt();
	}
	
	//Smart collection conditions
	lSQL.Format(_T("CREATE TABLE Conditions (idCondition INTEGER PRIMARY KEY AUTOINCREMENT, Field INTEGER, Condition INTEGER, Value VARCHAR);"));
	try
	{
		mDB.execDML(lSQL);
	}
	catch(CppSQLite3Exception e)
	{
		;
	}
	if(lProgressDlg != NULL)
	{
		lProgressDlg->StepIt();
	}


	lSQL.Format(_T("CREATE TABLE SmartCollections_has_Conditions (SmartCollections_idSmartColl INTEGER NOT NULL, Conditions_idCondition INTEGER NOT NULL, PRIMARY KEY(SmartCollections_idSmartColl, Conditions_idCondition));"));
	try
	{
		mDB.execDML(lSQL);
	}
	catch(CppSQLite3Exception e)
	{
		;
	}
	if(lProgressDlg != NULL)
	{
		lProgressDlg->StepIt();
	}

	// for SetInfo table
	lSQL.Format(_T("CREATE TABLE SetInfo (id INTEGER PRIMARY KEY AUTOINCREMENT, InfoName VARCHAR NOT NULL, Value VARCHAR NOT NULL);"));
	CString strTmp;
	CString strGUID = GUIDgen();
	try
	{
		mDB.execDML(lSQL);
		lSQL.Format(_T("insert into SetInfo values (1, 'isRepository', 'NO')"));
		mDB.execDML(lSQL);
		strTmp = GetDefaultPath(_T("Workspace\\"));

		if(strTmp.IsEmpty()) 
		{
			AfxMessageBox(_T("to get Workspace path is error1"));
			ExitProcess(0);
		}
		strTmp = strTmp + strGUID + _T("\\");

		lSQL.Format(_T("insert into SetInfo values (2, 'Workspace', '%s')"),strTmp);
		mDB.execDML(lSQL);

		strTmp = GetDefaultPath(_T("Repository\\"));
		if(strTmp.IsEmpty()) 
		{
			AfxMessageBox(_T("to get Workspace path is error1"));
			ExitProcess(0);
		}
		strTmp = strTmp + m_strLibName + _T("\\");
		lSQL.Format(_T("insert into SetInfo values (3, 'Repository', '%s')"),strTmp);
		mDB.execDML(lSQL);
#ifndef _BNB_ASSISTANT
		lSQL.Format(_T("insert into SetInfo values (4, 'WorkspaceType', '2')"));
		mDB.execDML(lSQL);
#endif
	}
	catch(CppSQLite3Exception e)
	{
		;
	}
	if(lProgressDlg != NULL)
	{
		lProgressDlg->StepIt();
	}

/* Not used, deleted from DB version 3
	// the properties table will be used by watched folders 
	lSQL.Format(_T("CREATE TABLE Properties (idProperty INTEGER PRIMARY KEY AUTOINCREMENT, CustomID INTEGER, Condition INTEGER, Name VARCHAR, Value VARCHAR);"));
	try
	{
		mDB.execDML(lSQL);
	}
	catch(CppSQLite3Exception e)
	{
		;
	}

	lSQL = _T("CREATE TABLE WFolders_has_Properties (WFolders_idWFolder INTEGER NOT NULL, Properties_idProperty INTEGER NOT NULL, PRIMARY KEY(WFolders_idWFolder, Properties_idProperty));");
	try
	{
		mDB.execDML(lSQL);
	}
	catch(CppSQLite3Exception e)
	{
		;
	}
*/

	if(lProgressDlg != NULL)
	{
		lProgressDlg->StepIt();
	}


	if(lProgressDlg != NULL)
	{
		lProgressDlg->StepIt();
	}

	//////////////////////////////
	if (lProgressDlg != NULL)
	{
		delete lProgressDlg;
		lProgressDlg = NULL;
	}

	return true;
}

bool CDB::GetCurrentFileTime(FILETIME &outCurrFileTime)
{
	CTime lTime = CTime::GetCurrentTime();
	SYSTEMTIME lSysTime;
	lTime.GetAsSystemTime(lSysTime);
	return (SystemTimeToFileTime(&lSysTime, &outCurrFileTime) == TRUE);
}

int CDB::SetInfoValue(TCHAR *infoname , TCHAR *infovalue)
{
	CString lSQL;
	lSQL.Format(_T("update Setinfo set Value ='%s' where InfoName = '%s';"), infovalue, infoname);
	mDB.execDML(lSQL);
	return 1;
}

int CDB::GetInfoValue(TCHAR *infoname , TCHAR *infovalue)
{
	CString lSQL;
	lSQL.Format(_T("select Value from Setinfo where InfoName = '%s';"),  infoname);
	CppSQLite3Query lq = mDB.execQuery(lSQL);

	LPCTSTR pszValue;

	if(!lq.eof())
	{
			pszValue = lq.getStringField(0);
			wcscpy(infovalue,pszValue);
			return 1;
	}

	return 0;
}

//return values: 1 = relation created, 2 = relation exists, 0 = error
int CDB::AddFileToCollection(int inCollectionID, int inFileID)
{
	try
	{
		//check for existence of this relation
		CString lSQL;
		lSQL.Format(_T("select * from Collections_has_Files where (Collections_idCollection = %d and Files_idFile = %d);"), inCollectionID, inFileID);			 
		CppSQLite3Query lQ = mDB.execQuery(lSQL); 
		if(!lQ.eof())
		{
			return 2; //already existing, nothing changed now
		}

		lSQL.Format(_T("insert into Collections_has_Files values (%u, %u);"), inCollectionID, inFileID);			 
		mDB.execDML(lSQL);

		return 1; //the only succes ret value
	}
	catch(...)
	{
	}

	return 0;
}

//return values: 1 = relation deleted, 2 = relation not exists, 0 = error
int CDB::RemoveFileFromCollection(int inCollectionID, int inFileID)
{
	try
	{
		//check for existence of this relation
		CString lSQL;
		lSQL.Format(_T("select * from Collections_has_Files where (Collections_idCollection = %d and Files_idFile = %d);"), inCollectionID, inFileID);			 
		CppSQLite3Query lQ = mDB.execQuery(lSQL); 
		if(lQ.eof())
		{
			return 2; //not existing, nothing changed now
		}

		lSQL.Format(_T("delete from Collections_has_Files where (Collections_idCollection = %d and Files_idFile = %d);"), inCollectionID, inFileID);			 
		mDB.execDML(lSQL);

		return 1; //the only succes ret value
	}
	catch(...)
	{
	}
	return 0;
}
//return values: 1 = relation created, 2 = relation exists, 0 = error
int CDB::AddTagToFile(int inTagID, int inFileID)
{
	try
	{
		//check for existence of this relation
		CString lSQL;
		lSQL.Format(_T("select * from Files_Has_Tags where (Files_idFile = %d and Tags_idTag = %d);"), inFileID, inTagID);			 
		CppSQLite3Query lQ = mDB.execQuery(lSQL); 
		if(!lQ.eof())
		{
			return 2; //already existing, nothing changed now
		}

		lSQL.Format(_T("insert into Files_Has_Tags values (%u, %u);"), inFileID, inTagID);			 
		mDB.execDML(lSQL);

		return 1; //the only succes ret value
	}
	catch(...)
	{
	}

	return 0;
}

int CDB::RemoveTagFromFile(int inTagID, int inFileID)
{
	try
	{
		//CppSQLite3Query iQ = mDB.execQuery(_T("BEGIN TRANSACTION"));

		// Check for existence of this relation
		CString lSQL;
		lSQL.Format(_T("select * from Files_Has_Tags where (Files_idFile = %d and Tags_idTag = %d);"), inFileID, inTagID);			 
		CppSQLite3Query lQ = mDB.execQuery(lSQL); 
		if (lQ.eof())
		{
			return 0; // Relation not exist
		}

		lSQL.Format(_T("delete from Files_Has_Tags where (Files_idFile = %d and Tags_idTag = %d);"), inFileID, inTagID);			 
		mDB.execDML(lSQL);
		
		//iQ = mDB.execQuery(_T("COMMIT"));

		return 1; //the only succes ret value
	}
	catch(...)
	{
	}
	return 0;
}

//return values: 1 = relation created, 2 = relation exists, 0 = error
int CDB::AddTagToWFolder(int inTagID, int inWFolderID)
{
	try
	{
		//check for existence of this relation
		CString lSQL;
		lSQL.Format(_T("select * from WFolders_Has_Tags where (WFolders_idWFolder = %d and Tags_idTag = %d);"), inWFolderID, inTagID);			 
		CppSQLite3Query lQ = mDB.execQuery(lSQL); 
		if(!lQ.eof())
		{
			return 2; //already existing, nothing changed now
		}

		lSQL.Format(_T("insert into WFolders_Has_Tags values (%u, %u);"), inWFolderID, inTagID);			 
		mDB.execDML(lSQL);

		return 1; //the only succes ret value
	}
	catch(...)
	{
	}

	return 0;
}

int CDB::AddFExtToWFolder(int inFExtID, int inWFolderID)
{
	try
	{
		//check for existence of this relation
		CString lSQL;
		lSQL.Format(_T("select * from WFolders_has_FExtensions where (WFolders_idWFolder = %d and FExtensions_idFExtension = %d);"), inWFolderID, inFExtID);			 
		CppSQLite3Query lQ = mDB.execQuery(lSQL); 
		if(!lQ.eof())
		{
			return 2; //already existing, nothing changed now
		}

		lSQL.Format(_T("insert into WFolders_has_FExtensions values (%u, %u);"), inWFolderID, inFExtID);			 
		mDB.execDML(lSQL);

		return 1; //the only succes ret value
	}
	catch(...)
	{
	}

	return 0;

}

/*
bool CDB::UpdateFiles()
{
	try
	{
		CTFiles lTFiles;
		if(lTFiles.GetFiles())
		{
			int lNumModifiedFiles = 0;
			int lNumNOTModifFiles = 0;
			WIN32_FIND_DATA lFindData;
			while(lTFiles.mDataIsValid)
			{
				HANDLE lHFind = FindFirstFile(lTFiles.mPath, &lFindData);	
				if(lHFind != INVALID_HANDLE_VALUE)
				{
					if(	(lTFiles.mFindData.ftLastWriteTime.dwHighDateTime != lFindData.ftLastWriteTime.dwHighDateTime) ||
						(lTFiles.mFindData.ftLastWriteTime.dwLowDateTime != lFindData.ftLastWriteTime.dwLowDateTime))
					{	//the file was modified on the disk, so update its data in DB
						memcpy(&lTFiles.mFindData, &lFindData, sizeof(lTFiles.mFindData));
						lTFiles.LoadSummaryInfo();
						lTFiles.UpdateTablebyID();
					}
					else 
						lNumNOTModifFiles++;
					
					if(lTFiles.mFlags != 0)
						lTFiles.UpdateFileFlag(0);
					
					FindClose(lHFind);
				}
				else
					lTFiles.UpdateFileFlag(FILE_FLAG_FNF);

				lTFiles.LoadNextRow();
			}
		}
		return true;
	}
	catch(...)
	{
	}
	
	return false;
}
*/

int CDB::DeAssociateTagsFromFile(int inFileID)
{
	try
	{
		CString lSQL2;
		lSQL2.Format(_T("delete from Files_Has_Tags where Files_idFile = %d;"), inFileID);
		mDB.execDML(lSQL2);
	}
	catch(...)
	{
		return false;
	}
	return true;
}

int CDB::GetDBVersion()
{
	int lVersion;
	try
	{
		CString lSQL;
		lSQL.Format(_T("select * from DBVersion"));
		lVersion = mDB.execScalar(lSQL);
	}
	catch(...)
	{
		lVersion = 0;
	}
	return lVersion;
}

bool CDB::Patches()
{
	try
	{
		int lVersion = GetDBVersion();
		switch (lVersion)
		{
			case 0:
				if (!Patch0To1()) return false;
			case 1:
				if (!Patch1To2()) return false;
			case 2:
				if (!Patch2To3()) return false;
			case 3:
				if (!Patch3To4()) return false;
			case 4:
				if (!Patch4To5()) return false;
			case 5:
				if (!Patch5To6()) return false;
			case 6:
				if (!Patch6To7()) return false;
			case 7:
				if (!Patch7To8()) return false;
			case 8:
				if (!Patch8To9()) return false;
			case 9:
				if (!Patch9To10()) return false;

			// Please add another patches here without break ...
		}
	}
	catch(...)
	{
		return false;
	}
	return true;
}

bool CDB::Patch0To1()
{
	try
	{
		CString lSQL;
		lSQL = _T("CREATE TABLE DBVersion (version INTEGER);");
		mDB.execDML(lSQL);
		lSQL = _T("insert into DBVersion values (1);");
		mDB.execDML(lSQL);
	}
	catch(...)
	{
		return false;
	}
	return true;
}


bool CDB::Patch1To2()
{
//	return true;

	CString lSQL;
	try
	{
		lSQL = _T("ALTER TABLE WFolders ADD UseTitle INTEGER NULL;");
		mDB.execDML(lSQL);
		lSQL = _T("ALTER TABLE WFolders ADD Title VARCHAR NULL;");
		mDB.execDML(lSQL);
		lSQL = _T("ALTER TABLE WFolders ADD  UseAuthor INTEGER NULL;");
		mDB.execDML(lSQL);
		lSQL = _T("ALTER TABLE WFolders ADD Author VARCHAR NULL");
		mDB.execDML(lSQL);
		lSQL = _T("ALTER TABLE WFolders ADD UseSubject INTEGER NULL;");
		mDB.execDML(lSQL);
		lSQL = _T("ALTER TABLE WFolders ADD Subject VARCHAR NULL;");
		mDB.execDML(lSQL);
		lSQL = _T("ALTER TABLE WFolders ADD UseCategories INTEGER NULL;");
		mDB.execDML(lSQL);
		lSQL = _T("ALTER TABLE WFolders ADD Categories VARCHAR NULL;");
		mDB.execDML(lSQL);
		lSQL = _T("ALTER TABLE WFolders ADD UseComments INTEGER NULL;");
		mDB.execDML(lSQL);
		lSQL = _T("ALTER TABLE WFolders ADD Comments VARCHAR NULL;");
		mDB.execDML(lSQL);

		lSQL = _T("update DBVersion set version = 2 where version = 1;");
		mDB.execDML(lSQL);
	}
	catch(CppSQLite3Exception e)
	{
		;
	}
	return true;
}

bool CDB::Patch2To3()
{
	CString lSQL;
	try
	{
		lSQL = _T("ALTER TABLE WFolders ADD UseTags INTEGER NULL;");
		mDB.execDML(lSQL);
		
		lSQL = _T("update DBVersion set version = 3 where version = 2;");
		mDB.execDML(lSQL);
	}
	catch(CppSQLite3Exception e)
	{
		;
	}
	return true;
}

bool CDB::Patch3To4()
{
	CString lSQL;
	try
	{
		lSQL = _T("ALTER TABLE Collections ADD idParentCollection INTEGER NULL;");
		mDB.execDML(lSQL);
		
		lSQL = _T("update DBVersion set version = 4 where version = 3;");
		mDB.execDML(lSQL);
	}
	catch(CppSQLite3Exception e)
	{
		;
	}
	return true;
}

bool CDB::Patch4To5()
{
	//case 218, inserting predefined collections
	CTCollections lCTCol(this);
	CString lSQL;
	try
	{
		if (mCollTWasCreated)
		{
			lCTCol.AddNewCollection(_T("General"), -1);
			lCTCol.AddNewCollection(_T("Work"), -1);
			lCTCol.AddNewCollection(_T("Finances"), -1);
			lCTCol.AddNewCollection(_T("Personal"), -1);
		}
		
		lSQL = _T("update DBVersion set version = 5 where version = 4;");
		mDB.execDML(lSQL);
	}
	catch(CppSQLite3Exception e)
	{
		;
	}
	return true;
}

// Szabi, Case 263
bool CDB::Patch5To6()
{
	int lGoodID;
	CString lSQL;
	CString lExtWithDotStr;
	CString lExtWithoutDotStr;
	CTFExtensions lTFExtensions(this);

	try
	{
		lTFExtensions.GetExtensions();
		while (lTFExtensions.mDataIsValid)
		{
			if (lTFExtensions.mName.Find('.') != -1)
			{
				lExtWithDotStr = lTFExtensions.mName;
				lExtWithoutDotStr = lExtWithDotStr.Right(lExtWithDotStr.GetLength() - lExtWithDotStr.ReverseFind('.') - 1);
				lSQL.Format(_T("SELECT idFExtension FROM FExtensions WHERE Name = '%s';"), lExtWithoutDotStr);
				lGoodID = mDB.execScalar(lSQL);
				lSQL.Format(_T("UPDATE Files SET FExtensions_idFExtension = %d WHERE FExtensions_idFExtension = %d;"), lGoodID, lTFExtensions.mID);
				mDB.execDML(lSQL);
			}
			lTFExtensions.LoadNextRow();
		}

		lSQL.Format(_T("DELETE FROM FExtensions WHERE Name LIKE '.%%'"));		
		mDB.execDML(lSQL);

		lSQL = _T("update DBVersion set version = 6 where version = 5;");
		mDB.execDML(lSQL);
	}
	catch(CppSQLite3Exception e)
	{
		;
	}
	return true;
}

//Tono, Case 
bool CDB::Patch6To7()
{
	CString lSQL;
	try
	{
		lSQL.Format(_T("insert into FExtensions values (NULL, '%s', '%s', NULL, %u)"), _T("xlsm"), _T("Microsoft Excel 2007 Macro-Enabled Spreadsheet"), FILTERTREE_MAJOR_SPREAD - FILTERTREE_MAJOR_BASE);
		mDB.execDML(lSQL);
		lSQL.Format(_T("insert into FExtensions values (NULL, '%s', '%s', NULL, %u)"), _T("xltx"), _T("Microsoft Excel 2007 Spreadsheet Template"), FILTERTREE_MAJOR_SPREAD - FILTERTREE_MAJOR_BASE);
		mDB.execDML(lSQL);
		lSQL.Format(_T("insert into FExtensions values (NULL, '%s', '%s', NULL, %u)"), _T("xltm"), _T("Microsoft Excel 2007 Macro-Enabled Spreadsheet Template"), FILTERTREE_MAJOR_SPREAD - FILTERTREE_MAJOR_BASE);
		mDB.execDML(lSQL);
		lSQL.Format(_T("insert into FExtensions values (NULL, '%s', '%s', NULL, %u)"), _T("xltx"), _T("Microsoft Excel 2007 Spreadsheet Template"), FILTERTREE_MAJOR_SPREAD - FILTERTREE_MAJOR_BASE);
		mDB.execDML(lSQL);
		lSQL.Format(_T("insert into FExtensions values (NULL, '%s', '%s', NULL, %u)"), _T("xlam"), _T("Microsoft Excel 2007 Add-In"), FILTERTREE_MAJOR_SPREAD - FILTERTREE_MAJOR_BASE);
		mDB.execDML(lSQL);
		lSQL.Format(_T("insert into FExtensions values (NULL, '%s', '%s', NULL, %u)"), _T("prn"), _T("Printable File"), FILTERTREE_MAJOR_SPREAD - FILTERTREE_MAJOR_BASE);
		mDB.execDML(lSQL);
		lSQL.Format(_T("insert into FExtensions values (NULL, '%s', '%s', NULL, %u)"), _T("pptx"), _T("PowerPoint Open XML Document"), FILTERTREE_MAJOR_PRES - FILTERTREE_MAJOR_BASE);
		mDB.execDML(lSQL);
		lSQL.Format(_T("insert into FExtensions values (NULL, '%s', '%s', NULL, %u)"), _T("pptm"), _T("PowerPoint 2007 Macro-Enabled Presentation"), FILTERTREE_MAJOR_PRES - FILTERTREE_MAJOR_BASE);
		mDB.execDML(lSQL);
		lSQL.Format(_T("insert into FExtensions values (NULL, '%s', '%s', NULL, %u)"), _T("potx"), _T("PowerPoint Open XML Presentation Template"), FILTERTREE_MAJOR_PRES - FILTERTREE_MAJOR_BASE);
		mDB.execDML(lSQL);
		lSQL.Format(_T("insert into FExtensions values (NULL, '%s', '%s', NULL, %u)"), _T("potm"), _T("PowerPoint 2007 Macro-Enabled Presentation Template"), FILTERTREE_MAJOR_PRES - FILTERTREE_MAJOR_BASE);
		mDB.execDML(lSQL);
		lSQL.Format(_T("insert into FExtensions values (NULL, '%s', '%s', NULL, %u)"), _T("ppam"), _T("PowerPoint 2007 Add-In"), FILTERTREE_MAJOR_PRES - FILTERTREE_MAJOR_BASE);
		mDB.execDML(lSQL);
		lSQL.Format(_T("insert into FExtensions values (NULL, '%s', '%s', NULL, %u)"), _T("ppsx"), _T("PowerPoint Open XML Slide Show"), FILTERTREE_MAJOR_PRES - FILTERTREE_MAJOR_BASE);
		mDB.execDML(lSQL);
		lSQL.Format(_T("insert into FExtensions values (NULL, '%s', '%s', NULL, %u)"), _T("ppsm"), _T("PowerPoint 2007 Macro-Enabled Slide Show"), FILTERTREE_MAJOR_PRES - FILTERTREE_MAJOR_BASE);
		mDB.execDML(lSQL);
		lSQL.Format(_T("insert into FExtensions values (NULL, '%s', '%s', NULL, %u)"), _T("pot"), _T("PowerPoint Template"), FILTERTREE_MAJOR_PRES - FILTERTREE_MAJOR_BASE);
		mDB.execDML(lSQL);

		lSQL.Format(_T("insert into FExtensions values (NULL, '%s', '%s', NULL, %u)"), _T("ppa"), _T("PowerPoint Add-in"), FILTERTREE_MAJOR_PRES - FILTERTREE_MAJOR_BASE);
		mDB.execDML(lSQL);

		lSQL = _T("update DBVersion set version = 7 where version = 6;");
		mDB.execDML(lSQL);
	}
	catch(CppSQLite3Exception e)
	{
		;
	}
	return true;
}

bool CDB::Patch7To8()
{

	//copy all files into the repository
	try
	{
		CString lSQL;

/*		CTFiles lFiles;

		lSQL = _T("SELECT * FROM Files");
		lFiles.ExecSelect(lSQL);


		CProgressDlg* lProgressDlg = NULL;
		lProgressDlg = new CProgressDlg;
		lProgressDlg->CreateProgressDlg();
		lProgressDlg->SetCaption(_T("Updating file repository"));
		lProgressDlg->SetMax((long)lFiles.GetCount());


		lFiles.LoadCurrentRow();
		int i = 0;
		while ((lFiles.mDataIsValid) && (i < lFiles.GetCount()))
		{
			CString lFilePath = lFiles.mPath;
			int lFileID = lFiles.mID;

			CPath lPath(lFiles.mPath);
			CString lFileName = lPath.GetFileName();

			CString lStrMsg;
			lStrMsg.Format(_T("Processing file... %s"), lFileName);
			lProgressDlg->SetMessage(lStrMsg);
			lProgressDlg->StepIt();

			CString lRepPath = gRepository.GetFilePath(lFileName, lFileID);

			::CopyFile(lPath, lRepPath, false);

			lSQL.Format(_T("UPDATE Files SET Path = '%s' WHERE idFile = %d"), lRepPath, lFileID);
			CTFiles lUpdateFiles;
			lUpdateFiles.ExecSelect(lSQL);

			lFiles.LoadNextRow();
			i++;			
		}

		lProgressDlg->DestroyWindow();
		delete lProgressDlg; */


		lSQL = _T("update DBVersion set version = 8 where version = 7;");
		mDB.execDML(lSQL);
	}
	catch(CppSQLite3Exception e)
	{
		;
	}

	return true;
}

bool CDB::Patch8To9()
{
	try
	{
		CString lSQL;

		lSQL.Format(_T("SELECT COUNT(*) FROM FExtensions WHERE Name = 'bat';"));
		if (mDB.execScalar(lSQL))
		{
			lSQL.Format(_T("UPDATE FExtensions SET Desc = 'Windows Batch File' where Name = 'bat';"));
		}
		else
		{
			lSQL.Format(_T("INSERT INTO FExtensions VALUES (NULL, '%s', '%s', NULL, %u)"), _T("bat"), _T("Windows Batch File"), FILTERTREE_MAJOR_OTHER - FILTERTREE_MAJOR_BASE);
		}
		mDB.execDML(lSQL);

		lSQL.Format(_T("UPDATE FExtensions SET Desc = 'Zip Archive' where Name = 'zip';"));		
		mDB.execDML(lSQL);

		lSQL = _T("update DBVersion set version = 9 where version = 8;");
		mDB.execDML(lSQL);
	}
	catch(CppSQLite3Exception e)
	{
		;
	}

	return true;
}

bool CDB::Patch9To10()
{
	try
	{
		CString lSQL;

		lSQL.Format(_T("Create Table AddedFiles (idAddedFile INTEGER PRIMARY KEY AUTOINCREMENT, FileID INTEGER NULL, FileName VARCHAR NULL, CreatedHi INTEGER NULL, CreatedLo INTEGER NULL)"));
		mDB.execDML(lSQL);

		lSQL = _T("update DBVersion set version = 10 where version = 9;");
		mDB.execDML(lSQL);
	}
	catch(CppSQLite3Exception e)
	{
		;
	}

	return true;
}


bool CDB::AddPDFFile(CString szFile)
{
/*
		PDF_Entry inEntry;
		LoadPDFMetadata(szFile, &inEntry);

		CString lEncName, lEncTitle, lEncSubject, lEncAuthor, lEncCat, lEncComments;

		TCHAR lDrv[_MAX_DRIVE];
		TCHAR lFld[_MAX_DIR];
		TCHAR lNam[_MAX_FNAME];
		TCHAR lExt[_MAX_EXT];

		_tsplitpath(szFile.GetBuffer(), lDrv, lFld, lNam, lExt);

		CString szFileName,szTitle,szSubject,szAuthor,szCategory,szComments;

		EncodeString(inEntry.mName, lEncName);
		EncodeString(inEntry.mTitle, lEncTitle);
		EncodeString(inEntry.mSubject, lEncSubject);
		EncodeString(inEntry.mAuthor, lEncAuthor);
		EncodeString(inEntry.mKeywords, lEncCat);
		EncodeString(inEntry.mComments, lEncComments);
		
		FILETIME addedDate;
		GetCurrentFileTime(addedDate);
		CString lCreatedTmStmp, lModifiedTmStmp, lAccessedTmStmp, lAddedTmStmp;
		Filetime2Timestamp(inEntry.ftCreationTime, lCreatedTmStmp);
		Filetime2Timestamp(mFindData.ftLastWriteTime, lModifiedTmStmp);
		Filetime2Timestamp(mFindData.ftLastAccessTime, lAccessedTmStmp);
		Filetime2Timestamp(addedDate, lAddedTmStmp);

		// I need to update the summary info of the file on the disc at first - due to
		// avoid conflicts with the UpgradeFiles function from Benubird! 
		UpdateFileOnDiskFromMembers();

		lSQL_AddFile.Format(_T("insert into Files values (NULL, '%s', '%s', %u, %u, %u, %u, %u, %u, %u, %u, %u, '%s', '%s', '%s', '%s', '%s', %u, %u, %u, %u, '%s', '%s', '%s', '%s')"), 
			lEncPath,
			lEncName,
			mFindData.nFileSizeHigh,
			mFindData.nFileSizeLow,
			mFindData.ftCreationTime.dwHighDateTime, //cr time
			mFindData.ftCreationTime.dwLowDateTime,
			mFindData.ftLastWriteTime.dwHighDateTime, //wr time
			mFindData.ftLastWriteTime.dwLowDateTime,
			mFindData.ftLastAccessTime.dwHighDateTime,
			mFindData.ftLastAccessTime.dwLowDateTime,
			mFindData.dwFileAttributes,
			lEncTitle,
			lEncSubject,
			lEncAuthor,
			lEncCat,
			lEncComments,
			mAddedDate.dwHighDateTime,
			mAddedDate.dwLowDateTime,
			mFlags,
			mExtID,
			lCreatedTmStmp,
			lModifiedTmStmp,
			lAccessedTmStmp,
			lAddedTmStmp
			);
		
		m_pDB->mDB.execDML(lSQL_AddFile);
*/
	return true;
}















bool gNowStarted = true;

/*
bool CWFolderManager::MarkWFoldersToProcess(UINT_PTR nIDEvent)
{
	try{
		if( (CTWFOLDER_FREQ_ALW == nIDEvent) &&			//if there are WFolders marked for processing the "Always" kind of WFolders are not added
			(mMarkedWFoldersToProcess.GetCount() > 0))
			return true; 

		GetWatchedFoldersByFreq((CTWFOLDER_FREQ_BASE + mCheckFrequency) != nIDEvent);
		while(mDataIsValid) //proces each WFolder now
		{
			mMarkedWFoldersToProcess.Add(mID);

/*			CStringList lFilePaths;
			LoadFilesFromDisk(mPath, mIncludeSubfolders, &lFilePaths);

			if(mAllFileType == false) //do not add all files!!!
			{ 
				CTFExtensions lFExt;
				CIDData lWFolderExtenesions;
				lFExt.GetCIDDataByWFolderID(lWFolderExtenesions, mID);
				KeepPathsByExtension(lFilePaths, lWFolderExtenesions.mStrs);
			}
			mFilePaths.AddTail(&lFilePaths);
*//*			LoadNextRow();
		}
	}
	catch(...)
	{
		return false;
	}
	return true;
}

*/

bool LoadFilesFromDisk(LPCTSTR inFolderPath, bool inWithSubfolders, CStringList *outFilePaths)
{
	CFileFinder lFFinder;
	lFFinder.SetCallback(FileFinderProc, outFilePaths);

	CFileFinder::CFindOpts	lFOpts;

	// Set CFindOpts object
	lFOpts.sBaseFolder = inFolderPath;
	lFOpts.bSubfolders = inWithSubfolders;

	CWinApp* lApp;
	lApp = AfxGetApp();
	if(lApp->GetProfileInt(kOptions, kIgnoreSVNFiles, BST_UNCHECKED) == BST_CHECKED)
	{
		lFOpts.AddExcludedDirs(".svn");
	}

	lFOpts.FindNormalFiles();
	if(lApp->GetProfileInt(kOptions, kIgnoreTmpFiles, BST_UNCHECKED) == BST_UNCHECKED)
	{
		lFOpts.FindTempFiles();
	}
	if(lApp->GetProfileInt(kOptions, kIgnoreWinFiles, BST_UNCHECKED) == BST_UNCHECKED)
	{
		lFOpts.FindSystemFiles();
	}

	CProgressDlg lDlg;
	lDlg.SetCaption(_T("Collecting files..."));
	lDlg.SetNeverEndingMode();
	lDlg.ShowCancel(FALSE);
	lDlg.CreateProgressDlg();

	lFFinder.RemoveAll();
	lFFinder.Find(lFOpts);
	
#ifndef _BNB_ASSISTANT
	if(lFFinder.m_nTotalFileCount>=g_nMaximumFileCount)
	{
		CString str;
		str.Format(_T("Can not add over %d files"),g_nMaximumFileCount);
		AfxMessageBox(str);
	}
#endif
	return true;
}


void KeepPathsByExtension(CStringList &outFilePaths, CStringList &inFileExtFilters)
{
	if(outFilePaths.GetCount()<=0)
		return;

	POSITION lPos = outFilePaths.GetTailPosition();
	while(lPos && (outFilePaths.GetCount()>0))
	{
		CString lPath = outFilePaths.GetAt(lPos);
		CString lExt = lPath.Right(lPath.GetLength() - lPath.ReverseFind('.') - 1);
		POSITION lFound = inFileExtFilters.Find(lExt);
//here is a bug: the .lnk extensions are not expanded before the validation

		if(lFound == NULL)
		{
			outFilePaths.RemoveAt(lPos);
		}
		outFilePaths.GetPrev(lPos);
	}
}

//*****************************************************************************
CString ExpandShortcut(CString& csFilename)
{
	USES_CONVERSION;		// For T2COLE() below
	CString csExpandedFile;

	// Make sure we have a path
	if(csFilename.IsEmpty())
	{
		ASSERT(FALSE);
		return csExpandedFile;
	}

	// Get a pointer to the IShellLink interface
	HRESULT hr;
	IShellLink* pIShellLink;

	hr = ::CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (LPVOID*) &pIShellLink);
	if (SUCCEEDED(hr))
	{

		// Get a pointer to the persist file interface
		IPersistFile* pIPersistFile;
		hr = pIShellLink->QueryInterface(IID_IPersistFile, (LPVOID*) &pIPersistFile);
		if (SUCCEEDED(hr))
		{
			// Load the shortcut and resolve the path
			// IPersistFile::Load() expects a UNICODE string
			// so we're using the T2COLE macro for the conversion.
			// For more info, check out MFC Technical note TN059
			// (these macros are also supported in ATL and are
			// so much better than the ::MultiByteToWideChar() family)
			hr = pIPersistFile->Load(T2COLE(csFilename), STGM_READ);
			if (SUCCEEDED(hr))
			{
				WIN32_FIND_DATA wfd;
				hr = pIShellLink->GetPath(csExpandedFile.GetBuffer(MAX_PATH), MAX_PATH, &wfd, SLGP_UNCPRIORITY);
				csExpandedFile.ReleaseBuffer(-1);
			}
			pIPersistFile->Release();
		}
		pIShellLink->Release();
	}
	return csExpandedFile;
}



CGetInfoData::CGetInfoData(CIntArray *inFileIDs)
{
	/*
	mFileIDs = inFileIDs;
	mLastLoadedIdx = -1;
	mNumFiles = (int)mFileIDs->GetCount();

	ASSERT(mNumFiles < 100);
	
	mNumFiles = 100;
//	mTFiles = new CTFiles[mNumFiles];
//	mIDDataTags = new CIDData[mNumFiles];
	for(int i = 0; i<mNumFiles; i++)
	{
		
		mTFiles[i].mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_TITLE] = false;
		mTFiles[i].mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_SUBJECT] = false;
		mTFiles[i].mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_AUTHOR] = false;
		mTFiles[i].mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_CATEGORY] = false;	
		mTFiles[i].mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_KEYWORDS] = false;	
		mTFiles[i].mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_COMMENTS] = false;	
	}

//	mTFiles = new CTFiles mFileIDs];
//	mIDDataTags = new CIDData[mFileIDs];
*/
}
CGetInfoData::CGetInfoData(CIntArray * inFileIDs, PARAM_FOR_CDB *pParamCDB)
{
	m_pParamCDB = pParamCDB;
	m_pDB = pParamCDB->pDB;

	mFileIDs = inFileIDs;
	mLastLoadedIdx = -1;
	mNumFiles = (int)mFileIDs->GetCount();

	ASSERT(mNumFiles < 100);
	
	mNumFiles = 100;
//	mTFiles = new CTFiles[mNumFiles];
//	mIDDataTags = new CIDData[mNumFiles];
	for(int i = 0; i<mNumFiles; i++)
	{
		mTFiles[i] = new CTFiles(m_pParamCDB);

		mTFiles[i]->mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_TITLE] = false;
		mTFiles[i]->mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_SUBJECT] = false;
		mTFiles[i]->mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_AUTHOR] = false;
		mTFiles[i]->mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_CATEGORY] = false;	
		mTFiles[i]->mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_KEYWORDS] = false;	
		mTFiles[i]->mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_COMMENTS] = false;	
	}

//	mTFiles = new CTFiles mFileIDs];
//	mIDDataTags = new CIDData[mFileIDs];	
}
CGetInfoData::~CGetInfoData()
{
/*	if(	mTFiles != NULL)
		delete mTFiles; 
	
	if(mIDDataTags != NULL)
		delete mIDDataTags;
*/
	for(int i = 0; i<mNumFiles; i++)
	{
		delete mTFiles[i];
	}
}

void CGetInfoData::LoadFromDB(int inIdx)
{
	if(mLastLoadedIdx < inIdx)
	{ //we have to read the data from the DB to the member variables
		int i;
		for(i = mLastLoadedIdx+1; i <= inIdx; i++)
		{
			int lFileID = mFileIDs->GetAt(i);
			mTFiles[i]->GetFileByID(lFileID);
			CTTags lTTags(m_pDB);
			lTTags.GetCIDDataByFileID(mIDDataTags[i], lFileID);
		}
		mLastLoadedIdx = i-1;
	}
}

CTFiles *CGetInfoData::GetDataTFiles(int inIdx)
{
	LoadFromDB(inIdx);
	return mTFiles[inIdx];
}

CIDData *CGetInfoData::GetDataIDDataTags(int inIdx)
{
	LoadFromDB(inIdx);
	return &mIDDataTags[inIdx];
}

bool CGetInfoData::WriteChangesToDiskAndUpdateBenu()
{
	bool lRetVal = false;
	for(int i = 0; i <= mLastLoadedIdx; i++)
	{
		m_pDB->DeAssociateTagsFromFile(mFileIDs->GetAt(i));
		int lCount =  mIDDataTags[i].GetCount();
		for(int j = 0; j<lCount; j++)
		{
			m_pDB->AddTagToFile(mIDDataTags[i].mIDs.GetAt(j),mFileIDs->GetAt(i));
			lRetVal = true;
		}

		lRetVal = mTFiles[i]->UpdateFileOnDiskFromMembers() || lRetVal;
		mTFiles[i]->UpdateFileFromDisk(true); // refresh is called from the parent method
	}
	return lRetVal;
}

bool CGetInfoData::WriteChangesToDB()
{
	bool lRetVal = false;
	for(int i = 0; i <= mLastLoadedIdx; i++)
	{
		m_pDB->DeAssociateTagsFromFile(mFileIDs->GetAt(i));
		int lCount =  mIDDataTags[i].GetCount();
		for(int j = 0; j<lCount; j++)
		{
			m_pDB->AddTagToFile(mIDDataTags[i].mIDs.GetAt(j),mFileIDs->GetAt(i));
			lRetVal = true;
		}

		mTFiles[i]->UpdateTablebyID();
	}
	return lRetVal;
}

int CTFiles::GetCount()
{
	try
	{
		// return number of items
		//  mQ.
		return 100;
	}
	catch(...)
	{
		return -1;
	}
}

bool IsAddThisFileEnabled(CString inPath)
{
	if (inPath.IsEmpty()) return false;

	inPath.MakeLower();

	CWinApp* lApp;
	lApp = AfxGetApp();
	bool lIgnoreTmpFiles = lApp->GetProfileInt(_T("Options"), _T("IgnoreTmpFiles"), BST_CHECKED);
	bool lIgnoreWinFiles = lApp->GetProfileInt(_T("Options"), _T("IgnoreWinFiles"), BST_CHECKED);
	bool lIgnoreLnkFiles = lApp->GetProfileInt(_T("Options"), _T("IgnoreLnkFiles"), BST_CHECKED);
	bool lIgnoreNoExtFiles = lApp->GetProfileInt(_T("Options"), _T("IgnoreNoExtFiles"), BST_CHECKED);

	int lDotPos;
	int lFilenameStartPos;

	CString lExt;
	CString lFilename;

	lExt.Empty();
	lFilename.Empty();

	lFilenameStartPos = inPath.ReverseFind('\\');
	if (lFilenameStartPos == -1)
		return false;

	lFilename = inPath.Right(inPath.GetLength() - lFilenameStartPos - 1);
	lFilename.MakeLower();
	
	lDotPos = lFilename.ReverseFind('.');
	if (lDotPos != -1)
	{
		lExt = lFilename.Right(lFilename.GetLength() - lDotPos - 1);
	}
	
	if (lIgnoreTmpFiles)
	{
		if (lFilename.Find('~') == 0)
		{
			return false;	
		}
		if (lExt == _T("tmp"))
		{
			return false;
		}
	}

	if (lIgnoreLnkFiles)
	{
		if (lExt == _T("lnk"))
		{
			return false;
		}
	}

	if (lIgnoreNoExtFiles)
	{
		if (lExt.IsEmpty())
		{
			return false;
		}
	}

	if (lIgnoreWinFiles)
	{
		TCHAR lPath[MAX_PATH];

		// XP:	  C:\Windows
		// Vista: C:\Windows
		SHGetSpecialFolderPath(0, lPath, CSIDL_WINDOWS, FALSE);
		_tcslwr(lPath);
		if (inPath.Find(lPath) > -1) return false;

		// XP:	  C:\Program Files
		// Vista: C:\Program Files
		SHGetSpecialFolderPath(0, lPath, CSIDL_PROGRAM_FILES, FALSE);
		_tcslwr(lPath);
		if (inPath.Find(lPath) > -1) return false;

		// XP:	  C:\Documents and Settings\<user>\Application Data
		// Vista: C:\Users\<user>\AppData\Roaming
//		SHGetSpecialFolderPath(0, lPath, CSIDL_APPDATA, FALSE);
//		if (inPath.Find(lPath) > -1) return false;

		// XP:	  C:\Documents and Settings\All Users\Application Data
		// Vista: C:\ProgramData
//		SHGetSpecialFolderPath(0, lPath, CSIDL_COMMON_APPDATA, FALSE);
//		if (inPath.Find(lPath) > -1) return false;
	}
	return true;
}

///////////////////////////////////////////////////////
CAddedFiles::CAddedFiles()
{
	
}
CAddedFiles::CAddedFiles(PARAM_FOR_CDB *pParamCDB)
{
	m_pParamCDB = pParamCDB;
	m_pDB = pParamCDB->pDB;
}
bool CAddedFiles::WasFileAdded(CString& inFilePath)
{
	try
	{
		CFileFind lFinder;
		if (lFinder.FindFile(inFilePath))
		{
			lFinder.FindNextFile();

			FILETIME lTime;
			lFinder.GetCreationTime(&lTime);

			CString lEncPath;
			EncodeString(inFilePath, lEncPath);

			CppSQLite3Query lQ;
			CString lSQL;
			lSQL.Format(_T("select FileID from AddedFiles where FileName='%s' and CreatedLo=%u and CreatedHi=%u"), lEncPath, lTime.dwLowDateTime, lTime.dwHighDateTime);
			lQ = m_pDB->mDB.execQuery(lSQL); 
			if(lQ.eof())
			{
				return false;
			}

			int lFileID = lQ.getIntField(0);

			lSQL.Format(_T("select idFile from Files where idFile=%u"), lFileID);
			lQ = m_pDB->mDB.execQuery(lSQL); 
			if(lQ.eof())
			{
				lSQL.Format(_T("delete from AddedFiles where FileID = %d"), lFileID);
				m_pDB->mDB.execDML(lSQL);

				return false;
			}
		}
	}
	catch(CppSQLite3Exception e)
	{}
	return true;
}

bool CAddedFiles::AddFile(int inFileID, CString& inFilePath)
{
	try
	{
		CFileFind lFinder;
		if (lFinder.FindFile(inFilePath))
		{
			lFinder.FindNextFile();

			FILETIME lTime;
			lFinder.GetCreationTime(&lTime);

			CString lEncPath;
			EncodeString(inFilePath, lEncPath);

			CppSQLite3Query lQ;
			CString lSQL;

			int lID = 1;
			lSQL = _T("select max(idAddedFile) from AddedFiles;"); 
			lQ = m_pDB->mDB.execQuery(lSQL); 
			if(!lQ.eof())
			{
				lID = lQ.getIntField(0) + 1;
			}

			lSQL.Format(_T("insert into AddedFiles values (NULL, %u, '%s', %u, %u)"), inFileID, lEncPath, lTime.dwHighDateTime, lTime.dwLowDateTime);
			lQ = m_pDB->mDB.execQuery(lSQL); 
			return true;
		}
	}
	catch(CppSQLite3Exception e)
	{}
	return false;
}


CFileProperties_Entry::CFileProperties_Entry()
{
	mUseTitle = false;
	mUseAuthor = false;
	mUseSubject = false;
	mUseCategories = false;
	mUseComments = false;
	mUseTags = false;

	mCollectionID = -1;
	mCollectionIDGroup = 0;
}

CFileProperties_Entry & CFileProperties_Entry::operator =(CFileProperties_Entry &inEntry)
{
	mCollectionID = inEntry.mCollectionID;
	mCollectionIDGroup = inEntry.mCollectionIDGroup;

	mUseTitle = inEntry.mUseTitle;
	mUseAuthor = inEntry.mUseAuthor;
	mUseSubject = inEntry.mUseSubject;
	mUseCategories = inEntry.mUseCategories;
	mUseComments = inEntry.mUseComments;
	mUseTags = inEntry.mUseTags;

	mTitle = inEntry.mTitle;
	mAuthor = inEntry.mAuthor;
	mSubject = inEntry.mSubject;
	mCategories = inEntry.mCategories;
	mComments = inEntry.mComments;
	mTags = inEntry.mTags;

	mFileTypes.mIDs.Append(inEntry.mFileTypes.mIDs);
	mFileTypes.mStrs.AddHead(&inEntry.mFileTypes.mStrs);

	return *this;
}
