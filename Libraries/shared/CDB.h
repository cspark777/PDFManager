#pragma once

/* INTERNAL KNOWLEDGEBASE  TODO

// --------  Foreign key
// Foreign key is ignored in SQLite, but there is a way to use them when insert/delete/update queries are executed
// http://www.sqlite.org/cvstrac/wiki?p=ForeignKeyTriggers
// we are not using this approach, so the references must be deleted manually!

*/

#include "CppSQLite3U.h"
#include "FileFinder.h"

struct PARAM_FOR_CDB;
class CProgressDlg;
class CDB;

#define TagField _T("(select group_concat(Tags.Name,'; ') from Tags, Files_has_Tags where Files_has_Tags.Files_idFile = Files.idFile and Files_has_Tags.Tags_idTag = Tags.idTag) as TagsStr")
#define CollectionField _T("(select group_concat(Collections.Name,'; ') from Collections, Collections_has_Files where Collections_has_Files.Files_idFile = Files.idFile and Collections_has_Files.Collections_idCollection = Collections.idCollection) as CollectionsStr")

#define BENUBIRD_ASSISTANT_HWND							0x77777700
#define BENUBIRD_ASSISTANT_LIBRARY_SETTING				0x77777701
#define BENUBIRD_ASSISTANT_REFRESH						0x77777702
#define BENUBIRD_ASSISTANT_GET_PREVOPTION_ASK			0x77777703
#define BENUBIRD_ASSISTANT_GET_PREVOPTION_RESPONSE		0x77777704

static const UINT BENUBIRD_ASSISTANT_INIT = RegisterWindowMessage(_T("BENUBIRD_ASSISTANT_INIT")); // Typo (?) - KARL 25/05/2009
static const UINT BENUBIRD_ASSISTANT_STOP = RegisterWindowMessage(_T("BENUBIRD_ASSISTANT_STOP")); // Typo (?) - KARL 25/05/2009
static const UINT BENUBIRD_ASSISTANT_START = RegisterWindowMessage(_T("BENUBIRD_ASSISTANT_START")); // Typo (?) - KARL 25/05/2009
static const UINT BENUBIRD_ASSISTANT_REFRESH_FILE = RegisterWindowMessage(_T("BENUBIRD_ASSISTANT_REFRESH_FILE"));  // Typo (?) - KARL 25/05/2009
static const UINT BENUBIRD_ASSISTANT_REFRESH_ALLFILES = RegisterWindowMessage(_T("BENUBIRD_ASSISTANT_REFRESH_ALLFILES"));

#ifndef _FREE
#define UniqueBenubirdClassName _T("Benubird PDF Pro Class Name")
#else
#define UniqueBenubirdClassName _T("Benubird PDF Class Name")
#endif

#define FILE_FLAG_FNF 0x1 //! File Not found on the disk

#define CTWFOLDER_FREQ_BASE		1001
#define CTWFOLDER_FREQ_ALW		(CTWFOLDER_FREQ_BASE + 0)
#define CTWFOLDER_FREQ_10MIN	(CTWFOLDER_FREQ_BASE + 1)
#define CTWFOLDER_FREQ_1HOUR	(CTWFOLDER_FREQ_BASE + 2)
#define CTWFOLDER_FREQ_1DAY		(CTWFOLDER_FREQ_BASE + 3) 

enum
{
	F1_Author = 0,		
	F1_Category,
	F1_Description,
	F1_DateAdded,
	F1_DateCreated,
	F1_DateModified,
	F1_FileType,
	F1_LastOpened,
	F1_Filename,
	F1_Path,
	F1_Size,
	F1_Subject,
	F1_Tags,
	F1_Title
};

enum
{
	T1_Contains = 0,		//type1
	T1_DoesNotContains,
	T1_Is,
	T1_IsNot,
	T1_StartsWith,
	T1_EndsWith,
	T2_IsGreaterThan = 20,	//type2
	T2_IsLessThan,
	T2_IsInTheRange,
	T3_Is = 40,
	T3_IsNot,
	T3_IsAfter,
	T3_IsBefore
};

enum
{
	L1_Items = 0,		//limits
	L1_MB,
	L1_GB
};

enum
{
	Command_Line_Message = 1,
	Watched_Folder_Message
};

typedef CArray<int,int> CIntArray;

// helps to send information between two applications (Benubird <=> Assistant)
// ------------------------------
typedef struct _AssistantData_Entry
{
	int mWFolderID;
	TCHAR mPath[MAX_PATH];
}AssistantData_Entry, FAR * LPAssistantData_Entry;

typedef struct _CmdLineData_Entry
{
  TCHAR mPath[MAX_PATH];
} CmdLineData_Entry, FAR * LPCmdLineData_Entry;

typedef struct _BenubirdData_Transfer
{
	int mMessageType;
	AssistantData_Entry mAssistantData;
	CmdLineData_Entry mCmdLineData;
} BenubirdData_Transfer, FAR * LPBenubirdData_Transfer;
// ------------------------------

// SQL command for select
class CDBSQL
{
private:
public:
	CStringList mField;
	CStringList mTable;
	CStringList mCondition;
	CStringList mSpecCondition;
	CStringList mGroup;
	CStringList mSort;
	CStringList mLimit;

	CDBSQL();
	~CDBSQL();

	bool CopyStringList(CStringList* destList, CStringList* sourceList);
	bool Copy(CDBSQL* destDBSQL);

	void Clear();

	void AddField(CString inStr);
	void AddTable(CString inStr);
	void AddCondition(CString inStr);
	void AddSpecCondition(CString inStr);
	void AddGroup(CString inStr);
	void AddSort(CString inStr);
	void AddLimit(CString inStr);

	CString GetSelect();
};

//int the sql queries we can't have apostroph inside a srings.
//each ' must be doubled to ''
LPCTSTR EncodeString(LPCTSTR inStr, CString &outEncodedStr);


LPCTSTR Filetime2Timestamp(FILETIME inFiletime, CString &outTimeStamp);

//this class stores the database IDs assigned to strings
//Exaple: storing some Tag IDs and their Names
class CIDData
{
public:	
	CIntArray mIDs;
	CStringList mStrs;

	CIDData(){};
	CIDData(CIDData & inData);
	CIDData(CIDData * inData);
	~CIDData(){};

	bool GetDataByIdx(int inIdx, int &outID, CString &outStr);
	int GetIdxByID(int inID);
	int GetIdxByStr(LPCTSTR inStr);

	int GetCount();
	LPCTSTR GetStringsFormatted(CString &outString, LPCTSTR inSeparator = _T("; ")); 
	virtual int GetFromFormattedString(CString inString, LPCTSTR inSeparator = _T("; "))
	{
		return 0;
	}

	int AddData(int inID, LPCTSTR inStr);

	//compare
	bool HasEqualIDs(CIDData &inCIDData);

	//remove, cleanup
	void RemoveAt(int inIdx);
	void RemoveAll();
};
/*
class CTagsIDData : public CIDData
{
	virtual int GetFromFormattedString(CString inString, LPCTSTR inSeparator = _T("; ")); 
};*/

class CDBTransaction
{	
public:
	PARAM_FOR_CDB *m_pParamCDB;
	CDB *m_pDB;
public:
	CDBTransaction();
	CDBTransaction(PARAM_FOR_CDB *pParamCDB);
	~CDBTransaction();

	bool mCommit;
};

class CDBTable //query manager
{
public:
	CDB * m_pDB;

public:
	CppSQLite3Query mQ; //this holds the result set

	bool mDataIsValid;
	int mID;
	CDBTable();
	CDBTable(CDB * pDB);
	~CDBTable();

	virtual bool LoadCurrentRow() = 0;
	bool LoadNextRow();
	bool ExecSelect(CString inSQL);
};

class CTWatchedFolder : public CDBTable
{
public:
	CDB *m_pDB;
public:
	CTWatchedFolder();
	CTWatchedFolder(CDB*pDB);
//	CTWatchedFolder(LPCTSTR inPath, bool inWithSubfolders, int inAddToCollectionID);
	~CTWatchedFolder();

	CString mName;
	CString mPath;
	bool mIncludeSubfolders;
	bool mAllFileType;
	int mCollectionID;
	int mCheckFrequency;

	int mUseTitle, mUseAuthor, mUseSubject, mUseCategories, mUseComments, mUseTags;
	CString mTitle, mAuthor, mSubject, mCategories, mComments;

	bool GetWatchedFolders();
	bool GetWatchedFoldersByFreq(int inFrequency);
	bool GetWatchedFolderByID(DWORD_PTR inWFolderID);

	bool LoadCurrentRow();
	bool AddNewWatchedFolder(CIntArray *inFExtArray, CIntArray *inTagsArray);

	bool DeleteByID(int inWFolderID);
};

#define FINFO_UPDATE_TITLE		0
#define FINFO_UPDATE_SUBJECT	1
#define FINFO_UPDATE_AUTHOR		2
#define FINFO_UPDATE_CATEGORY	3
#define FINFO_UPDATE_KEYWORDS	4
#define FINFO_UPDATE_COMMENTS	5

//the last must be updated to be a last
#define FINFO_UPDATE_LAST		FINFO_UPDATE_COMMENTS + 1

typedef struct _CTFiles_Entry
{
	bool mWasRead;
	bool mInWorkspace;
	int mPreviewID;
	
	int mID;
	CString mPath;
	CString mName;
	CString mTitle;
	CString mSubject;
	CString mAuthor;
	CString mCategory;
	CString mKeywords;
	CString mComments;
	CString mDescription;
	WIN32_FIND_DATA mFindData;
	FILETIME mAddedDate;
	int mFlags; //0x1 - missing,
	int mExtID; //foreign key to FExtension table

	// variables for grid view
	CString mTags;
	CString mSize;
	CString mExt;
	CString mAttr;
	CString mCreatedTmStmp;
	CString mModifiedTmStmp;
	CString mAccessedTmStmp;
	CString mAddedTmStmp;
	CString mCollections;
}CTFiles_Entry, FAR * LPCTFiles_Entry;

class CFileProperties_Entry;

class CTFiles : public CDBTable
{
	friend class CBRepository;
public:
	CDB *m_pDB;
	CStringList *m_pSystemDirsList;
public:
	CString mPath;
	CString mName;
	CString mTitle, mSubject, mAuthor, mCategory, mKeywords, mComments, mDescription; //the summary info
	WIN32_FIND_DATA mFindData;
	int mFlags; //0x1 - missing,
	int mExtID; //foreign key to FExtension table
	CProgressDlg* m_ProgressDlg;
	
	bool mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_LAST];

	FILETIME mAddedDate;

	CTFiles();
	CTFiles(PARAM_FOR_CDB *pParamCDB);
	virtual ~CTFiles(){};
	
	//these methods fills the members
	bool LoadDataFromFile();
	bool LoadSummaryInfo();
	bool LoadAutoSummaryInfo();
	bool LoadCurrentRow();

	//these metohods makes a select over the db and changes the 'mQ'
	bool GetFiles(LPCTSTR inStr = NULL);
	bool GetFileByID(int inFileID);
	int  GetFileIDByPath(LPCTSTR inPath);
	
	bool UpdateFileFromDisk(bool inRefresh);
	bool UpdateTablebyID();
	bool UpdateFileFlag(int inNewFlag);
	bool UpdateFileOnDiskFromMembers();
	bool UpdateFilePath(int inID, LPCTSTR inNewPath);

	bool UpdateFilePath(LPCTSTR inOldPath, LPCTSTR inNewPath);
	bool UpdateFilePath(LPCTSTR inAddPath,int flag);

	//wrapper for inserting multiple files into DB
	//! returns the number of succesfully added files
	//! outAddedFilesIDs the added files ID
	//! outExistingFilesIDs the files ID already exists in Benubird - ignored in this function for this reason
//	int AddFiles(CStringList &inFilesPathList, CIntArray *outAddedFilesIDs = NULL, CIntArray *outExistingFilesIDs = NULL, bool inUseProgMon = true);

//#ifndef _DIRECT // Make public for direct

	int AddFiles(CStringList &inFilesPathList, CFileProperties_Entry * inFolderEntry, CIntArray *outAddedFilesIDs = NULL, CIntArray *outExistingFilesIDs = NULL, bool inUseProgMon = true);

private:
	bool AddFile(CString &inFilesPath, CString & inFileName, CFileProperties_Entry * inFolderEntry = NULL);
//	int AddFolders(LPCTSTR inFolderPath, bool inRecursive, CIntArray *outAddedFilesIDs = NULL);
public:
	void AddTagsFromKeywords(int inFileID, LPCTSTR inKeywords);

	//delete
	bool DeleteByID(int inFileID);

	int GetCount();

	int GetNextFileID();

	
	int AddFiles_OverWrite(CStringList &inFilesPathList, CFileProperties_Entry * inFolderEntry, CIntArray *outAddedFilesIDs = NULL, CIntArray *outExistingFilesIDs = NULL, bool inUseProgMon = true);
	


private:
	void LoadSystemDirList();
};

class CTCollections : public CDBTable
{
public:
	
	CDB *m_pDB;
public:
	CTCollections(){};
	CTCollections(CDB *pDB);
	~CTCollections(){};
	
	bool LoadCurrentRow();

	CString mName;
	int mParentID;

	bool AddNewCollection(LPCTSTR inCollectionName, int inParentID);
	bool UpdateTablebyID();
	bool GetCollectionsByNames(int inParentID);
	bool GetCollectionsByName(LPCTSTR inCollName);
	bool HasCollectionChildItems(int inParentID);
	
	bool GetAllCollectionsAssignedToFileIDs(CIntArray *inFileIDs, CIDData &outCollIDData);
	bool GetAllCollectionsNotAssignedToFileIDs(CIntArray* inFileID, CIDData &outCollIDData);

	bool DeleteByID(int inCollID);
};

#define SMART_COLL_MAXFILTERS 10
class CTSmartColl : public CDBTable
{
public:
	PARAM_FOR_CDB *m_pParamCDB;
	CDB *m_pDB;
public:
	CTSmartColl();
	CTSmartColl(PARAM_FOR_CDB *pParamCDB);
	~CTSmartColl(){};
	
	bool LoadCurrentRow();

	CString mName;
	int mMatchTheFollowingRuleBool;
	int mLimitToBool;
	CString mLimitToStr;
	int mLimitToCombo;
	int mNumActiveFilters;
	int mCombo1[SMART_COLL_MAXFILTERS];
	int mCombo2[SMART_COLL_MAXFILTERS];
	CString mValue[SMART_COLL_MAXFILTERS];


	bool AddNewSmartColl(LPCTSTR inCollectionName);
	bool GetSmartCollections();
	bool GetSmartCollByName(LPCTSTR inCollName);
	bool GetSmartCollByID(DWORD_PTR inSmartCollID);
	bool UpdateNameByID();
	bool UpdateTableByID();
	bool UpdateSQL(int inSmartCollID, CDBSQL* inDBSQL);
	
	bool DeleteByID(int inSmartCollID);
};

class CTTags : public CDBTable
{
	CDB *m_pDB;
public:
	CTTags(){};
	CTTags(CDB *pDB);
	~CTTags(){};
	
	CString mName;


	bool LoadCurrentRow();

	bool DeleteByName(LPCTSTR inTagName);
	int AddNewTag(LPCTSTR inTagName);
	int AddNewTags(CStringList &inTagNames);

//	bool GetTagByName(LPCTSTR inTagName);
	bool GetTags();
	int GetIDByName(LPCTSTR inTagName);
	
	bool GetTagsByFileID(int inFileID);
	bool GetTagsByWFolderID(int inWFolderID);
	bool GetCIDDataByWFolderID(CIDData &outIDData, int inWFolderID);
	bool GetCIDDataByFileID(CIDData &outIDData, int inFileID);
	//! encapsulation of GetTagsByFileID, returns the number of tags in outStr
	int GetTagsByFileID_Formatted(CString &outStr, int inFileID);

	bool GetTagsNotAssignedToFileID(int inFileID);
	bool UpdateByID();

	int GetPopularity();

	bool GetAllTagsAssignedToFileIDs(CIntArray *inFileIDs, CIDData &outTagsIDData);
	bool GetAllTagsNotAssignedToFileIDs(CIntArray *inFileIDs, CIDData &outTagsIDData);
};

class CTFExtensions : public CDBTable
{
public:
	
	CDB *m_pDB;
public:
	CTFExtensions();
	CTFExtensions(CDB *pDB);
	~CTFExtensions();
	
	bool LoadCurrentRow();


	CString mName;
	CString mDescription;
	int mMajorFilterID;

//	bool AddNewExtension(LPCTSTR inCollectionName);
	bool GetExtensions();
	bool GetExtensionsByID(int inID);
	bool GetExtensionsByWFolderID(int inWFolderID);
	bool GetCIDDataByWFolderID(CIDData &outIDData, int inWFolderID);
//	bool GetExtensionByName();
	int	 GetExtensionIDWithCreate(LPCTSTR inExtName);

//	bool GetCollectionsByName(LPCTSTR inCollName);
};

//
class CTFilesUpdateOnIdle : public CTFiles
{
public:
	CTFilesUpdateOnIdle();
	~CTFilesUpdateOnIdle();
	
	int mNumFiles;
	int mFileIndex;

	bool UpdateFiles();
};

class CDB
{
private:
	BOOL mCollTWasCreated;

public:
	CDB(void);
	~CDB(void);

	CString m_strLibPath;

	CString m_strLibName;

	int SetInfoValue(TCHAR *infoname , TCHAR *infovlaue);
	int GetInfoValue(TCHAR *infoname , TCHAR *infovlaue);

	void CreateDefaultDBEntries();
	bool Initialize(LPCTSTR path);

	bool Open(LPCTSTR inPath); //! opens the database
	bool Close(); //!closes the database

	bool CreateTables();

	bool GetCurrentFileTime(FILETIME &outCurrFileTime);

	//!Checks each file from the DB and update the DB entries if it is necessary
	//bool UpdateFiles();

	int AddFileToCollection(int inCollectionID, int inFileID);
	int RemoveFileFromCollection(int inCollectionID, int inFileID);
	int AddTagToFile(int inTagID, int inFileID);
	int RemoveTagFromFile(int inTagID, int inFileID);
	int AddTagToWFolder(int inTagID, int inWFolderID);
	int AddFExtToWFolder(int inFExtID, int inWFolderID);

	int DeAssociateTagsFromFile(int inFileID);

	int GetDBVersion();
	bool Patches();
	bool Patch0To1();
	bool Patch1To2();
	bool Patch2To3();
	bool Patch3To4();
	bool Patch4To5();
	bool Patch5To6();
	bool Patch6To7();
	bool Patch7To8();
	bool Patch8To9();
	bool Patch9To10();

	//for watch
	BOOL Initalize_For_Watch(LPCTSTR path);
	BOOL CreateTables_For_Watch();
	BOOL SetLibrarySetting_For_Watch(LPCTSTR lib, int mode ,int state);

	//for default
	BOOL Initalize_For_Default(LPCTSTR path);
	BOOL CreateTables_For_Default();
	BOOL SetDefaultLibrary_For_Default(LPCTSTR lib);
	BOOL GetDefaultLibrary_For_Default(TCHAR* lib);
	BOOL SetGlobalSettingInfo_For_Default(TCHAR *infoname , TCHAR *infovalue);
	BOOL GetGlobalSettingInfo_For_Default(TCHAR *infoname , TCHAR *infovalue);

	//
	bool AddPDFFile(CString szFileName);
	
//protected:
	CppSQLite3DB mDB;
};


/*class CWFolderManager : public CTWatchedFolder
{
public:
	static bool LoadFilesFromDisk(LPCTSTR inFolderPath, bool inWithSubfolders, CStringList *outFilePaths);
public:		
	CWFolderManager()
	{
		mWFolderIndex = -1;
	};
	~CWFolderManager(){};
	
	int mWFolderIndex;
	int mTotalAddedFiles;	
	//CIntArray	mMarkedWFoldersToProcess;
	CStringList mFilePaths;

	//bool MarkWFoldersToProcess(UINT_PTR nIDEvent);
};*/

class CGetInfoData
{
	PARAM_FOR_CDB *m_pParamCDB;
	CDB *m_pDB;
public:	
	CIntArray	*mFileIDs;
	int			mLastLoadedIdx;
	int			mNumFiles;
	//CTFiles		mTFiles[100];//*mTFiles; //by pyj
	CTFiles		*mTFiles[100];

	CIDData		mIDDataTags[100];//*mIDDataTags;
	CGetInfoData();
	CGetInfoData(CIntArray *inFileIDs);
	CGetInfoData(CIntArray *inFileIDs, PARAM_FOR_CDB *pParamCDB);
	~CGetInfoData();

	void LoadFromDB(int inIdx);
	CTFiles *GetDataTFiles(int inIdx);
	CIDData *GetDataIDDataTags(int inIdx);
	bool WriteChangesToDiskAndUpdateBenu();
	bool WriteChangesToDB();

};

class CAddedFiles
{
public:
	PARAM_FOR_CDB *m_pParamCDB;
	CDB *m_pDB;

public:
	CAddedFiles(PARAM_FOR_CDB *pParamCDB);
	CAddedFiles();

	bool WasFileAdded(CString& inFilePath);
	bool AddFile(int inFileID, CString& inFilePath);
};


class CFileProperties_Entry
{
public:
	CFileProperties_Entry();

	long mCollectionID;
	long mCollectionIDGroup;
	CIDData mFileTypes;

	int mUseTitle;
	int mUseAuthor;
	int mUseSubject;
	int mUseCategories;
	int mUseComments;
	int mUseTags;
	CString mTitle;
	CString mAuthor;
	CString mSubject;
	CString mCategories;
	CString mComments;
	CString mTags;
	CString mPath;

	CFileProperties_Entry &operator =(CFileProperties_Entry &inEntry);
};

class CTWatchedFolder_Entry : public CFileProperties_Entry
{
public:
	int mID;
	int nMode;
	CString mLibraryPathName;
	CString mName;
	CString mPath;
	bool mIncludeSubfolders;
	int mCheckFrequency;
	bool mAllFileType;
};

//extern CWFolderManager	gWFolderManager;

struct PARAM_FOR_CDB
{
	CDB * pDB;
	CDBSQL* pFilesSQL;
	CStringList *pSystemDirsList;
	int				nIsUseRepository;
	int				nWorkspaceType;
	TCHAR szLibPathName[0x100];
};

bool IsAddThisFileEnabled(CString inPath);
void KeepPathsByExtension(CStringList &outFilePaths, CStringList &inFileExtFilters);
CString ExpandShortcut(CString& csFilename);
void FileFinderProc(CFileFinder *pFinder, DWORD dwCode, void *pCustomParam);
bool LoadFilesFromDisk(LPCTSTR inFolderPath, bool inWithSubfolders, CStringList *outFilePaths);
