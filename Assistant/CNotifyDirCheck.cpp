
#include "stdafx.h"
#include "CNotifyDirCheck.h"

extern bool gTimersPaused;
extern bool ProcessWatchedFolderEntry(CTWatchedFolder_Entry& inFolderEntry);

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
	
//////////////////////////////////////////////////////////////////////
// Work Thread 
//////////////////////////////////////////////////////////////////////
UINT NotifyDirThread( LPVOID pParam )
{
	BOOL             bStop = FALSE;
	HANDLE           hDir  = NULL; 
	CNotifyDirCheck* pNDC  = (CNotifyDirCheck*)pParam;

	if( pNDC == NULL )
		return 0;

	hDir = FindFirstChangeNotification( pNDC->mWFolderEntry.mPath,
										pNDC->mWFolderEntry.mIncludeSubfolders,
										FILE_NOTIFY_CHANGE_FILE_NAME  |
										FILE_NOTIFY_CHANGE_DIR_NAME   |
										FILE_NOTIFY_CHANGE_SIZE       |
										FILE_NOTIFY_CHANGE_LAST_WRITE |
										FILE_NOTIFY_CHANGE_ATTRIBUTES );

	if( hDir == INVALID_HANDLE_VALUE )
	{
		return 0;
	}
	
 	while( pNDC->IsRun() )
	{
		bStop = FALSE;

		while( WaitForSingleObject( hDir, /*INFINITE*/WAIT_TIMEOUT ) != WAIT_OBJECT_0 )
		{	if( !pNDC->IsRun() )
			{
				bStop = TRUE;//to end
				break;
			}
		}
		if( bStop )
			break;//to end
		
		Sleep( WAIT_TIMEOUT );

		if (pNDC->IsRun())
		{
			if (!gTimersPaused)
				ProcessWatchedFolderEntry(pNDC->mWFolderEntry);
		}

		//faAction = CFileInformation::CompareFiles( &oldFIL, &newFIL, fi );

		if( FindNextChangeNotification( hDir ) == 0 )
		{
			return 0;
		}
	}

	return FindCloseChangeNotification( hDir );
}

///////////////////////////////////////////////////////////////////////////////////
// class CNotifyDirCheck
///////////////////////////////////////////////////////////////////////////////////
CNotifyDirCheck::CNotifyDirCheck(CTWatchedFolder_Entry& inWFolderEntry)
{
	SetStop();
	m_pThread = NULL;
	mProcessingEntry = false;

	mWFolderEntry.mID = inWFolderEntry.mID;
	mWFolderEntry.mName = inWFolderEntry.mName;
	mWFolderEntry.mPath = inWFolderEntry.mPath;
	mWFolderEntry.mIncludeSubfolders = inWFolderEntry.mIncludeSubfolders;
	mWFolderEntry.mAllFileType = inWFolderEntry.mAllFileType;
	mWFolderEntry.mCollectionID = inWFolderEntry.mCollectionID;
	mWFolderEntry.mCheckFrequency = inWFolderEntry.mCheckFrequency;
	mWFolderEntry.mUseTitle = inWFolderEntry.mUseTitle;
	mWFolderEntry.mTitle = inWFolderEntry.mTitle;
	mWFolderEntry.mUseAuthor = inWFolderEntry.mUseAuthor;
	mWFolderEntry.mAuthor = inWFolderEntry.mAuthor;
	mWFolderEntry.mUseSubject = inWFolderEntry.mUseSubject;
	mWFolderEntry.mSubject = inWFolderEntry.mSubject;
	mWFolderEntry.mUseCategories = inWFolderEntry.mUseCategories;
	mWFolderEntry.mCategories = inWFolderEntry.mCategories;
	mWFolderEntry.mUseComments = inWFolderEntry.mUseComments;
	mWFolderEntry.mComments = inWFolderEntry.mComments;
	mWFolderEntry.mUseTags = inWFolderEntry.mUseTags;
}

CNotifyDirCheck::~CNotifyDirCheck()
{
	Stop();
}

BOOL CNotifyDirCheck::Run()
{
	if( IsRun() || m_pThread != NULL)
		return FALSE;

	SetRun();
	m_pThread = AfxBeginThread( NotifyDirThread, this );
	
	if( m_pThread == NULL )
		SetStop();

	return IsRun();
}

void CNotifyDirCheck::Stop()
{
	if( !IsRun() || m_pThread == NULL )
		return;
	
	SetStop();

	WaitForSingleObject( m_pThread->m_hThread, 2 * NOTIFICATION_TIMEOUT );
	m_pThread = NULL;
}