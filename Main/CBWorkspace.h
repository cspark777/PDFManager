#pragma once

#include "stdafx.h"
#include "CDB.h"
#include "BCGPPopupWindow.h"

class CBWorkspace;

class CBWorkSpaceWnd : public CWnd
{
	DECLARE_DYNAMIC(CBWorkSpaceWnd)
public:
	CBWorkspace *m_pParent;

	CBWorkSpaceWnd(CBWorkspace *pWorkspace);
protected:
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

class CBWorkspace
{
	friend class CBWorkSpaceWnd;

private:
	static UINT __cdecl WorkspaceCheckControllingFunction( LPVOID pParam );

private:

	CWinThread * m_ControlThread;
	bool m_SuspendThtread;

	CBWorkSpaceWnd* m_Wnd;
	HICON m_hIcon;

	CIntArray m_ChangedFilesArr;

	bool m_ShowMessage;

public:
	CString m_strWorkspacePath;
	PARAM_FOR_CDB *m_pParamCDB;
	void Set_ParamFromParent(PARAM_FOR_CDB *pParamCDB);
	void SetWorkspacePath(LPCTSTR path);
public:

	CBWorkspace();
	~CBWorkspace();



	bool Initialize(LPCTSTR path);

	bool AddFile(int inFileID, bool inOpen = true);
	bool RemoveFile(int inFileID);
	int CheckFile(int inFileID);

	bool FileIsInWorkspace(int inFileID);
	bool FileIsModified(int inFileID);

	bool AskForSaveToRepository(int inFileID);
	bool SaveBackToRepository(int inFileID);

	bool SaveCompareFile(CString& inFileName);

	bool GetWorkspaceFileFolder(int inFileID, CString & outFolder, CString & outFileName);
	CString GetCompareFilePath(CString & inFileName);

	CString GetPropertiesFilePath(CString & inFileName);
	bool SaveFilePropsFile(CString & inFileName);
	bool UpdateFileProperties(int inFileID, CString& inFileName);

	int CompareFile(CString& inFileName);

	int GetNumChangedFiles() {return (int) m_ChangedFilesArr.GetCount();}
	int GetChangedFileID(int inPos) {return m_ChangedFilesArr.GetAt(inPos);}	

};

