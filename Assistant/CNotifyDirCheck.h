#if !defined(__CNotifyDirCheck_H__)
#define __CNotifyDirCheck_H__

#include "stdafx.h"
#include "Define.h"
#include "CDB.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define NOTIFICATION_TIMEOUT 1000

class CNotifyDirCheck;
typedef CArray<CNotifyDirCheck*,CNotifyDirCheck*> CNotifyDirCheckArray;

UINT NotifyDirThread( LPVOID pParam );

///////////////////////////////////////////////////////////////////////////////////
// class CNotifyDirCheck
///////////////////////////////////////////////////////////////////////////////////
class CNotifyDirCheck : public CObject  
{
protected:
	CWinThread*               m_pThread;
	CString                   m_csDir;
	BOOL                      m_isRun;
	LPVOID					  m_lpData;

	void	SetRun()  { m_isRun = TRUE; }
	void	SetStop() { m_isRun = FALSE; }

public:
	CTWatchedFolder_Entry mWFolderEntry;
	bool mProcessingEntry;

	CNotifyDirCheck(CTWatchedFolder_Entry& inWFolderEntry);
	virtual ~CNotifyDirCheck();

	BOOL	IsRun() const { return m_isRun; }
	BOOL	Run();
	void	Stop();
};

#endif // !defined(__CNotifyDirCheck_H__)
