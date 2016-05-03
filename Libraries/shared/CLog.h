#pragma once

#include <windows.h>

class CLog
{
public:
	CLog(void);
	~CLog(void);

	static void StoreUserMessage(LPCTSTR inText);

private:
	TCHAR m_LogFilePath[_MAX_PATH];
};
