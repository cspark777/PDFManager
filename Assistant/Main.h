// BenubirdAssistant.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include <windows.h>
#include "resource.h"		// main symbols
#include "CDB.h"

#define CTWFOLDER_FREQ_BASE		1001
#define CTWFOLDER_FREQ_ALW		(CTWFOLDER_FREQ_BASE + 0) 
#define CTWFOLDER_FREQ_10MIN	(CTWFOLDER_FREQ_BASE + 1)
#define CTWFOLDER_FREQ_1HOUR	(CTWFOLDER_FREQ_BASE + 2)
#define CTWFOLDER_FREQ_1DAY		(CTWFOLDER_FREQ_BASE + 3)
/*
typedef struct _CTWatchedFolder_Entry
{
	int mID;

	CString mName;
	CString mPath;
	bool mIncludeSubfolders;
	bool mAllFileType;
	int mCollectionID;
	int mCheckFrequency;

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

}CTWatchedFolder_Entry, FAR * LPCTWatchedFolder_Entry;
*/

typedef CArray<CTWatchedFolder_Entry,CTWatchedFolder_Entry> CTWatchedFolderArray;

extern bool gTimersPaused;
extern bool ProcessWatchedFolderEntry(CTWatchedFolder_Entry& inFolderEntry);

///////////////////////////////////////////////////////////////////////////////////
// CBenubirdAssistantApp class
///////////////////////////////////////////////////////////////////////////////////
class CBenubirdAssistantApp : public CWinApp, public CBCGPWorkspace
{
public:
	CBenubirdAssistantApp();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	DECLARE_MESSAGE_MAP()

};

extern CBenubirdAssistantApp theApp;