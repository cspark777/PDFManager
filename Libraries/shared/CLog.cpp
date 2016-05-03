
#include "CLog.h"

CLog gGeneralLog;

CLog::CLog(void)
{
	/*TCHAR lFileName[_MAX_PATH];
	memset(lFileName, 0, sizeof(lFileName));
	GetModuleFileName(NULL, lFileName, _MAX_PATH);

	TCHAR drv[_MAX_DRIVE];
	TCHAR fld[_MAX_DIR];
	TCHAR nam[_MAX_FNAME];
	TCHAR ext[_MAX_EXT];

	_tsplitpath_s(lFileName, drv, fld, nam, ext);
	_tmakepath(m_LogFilePath, drv, fld, nam, _T(".log"));*/

}
CLog::~CLog(void)
{
}

void CLog::StoreUserMessage(LPCTSTR inText)
{
	/*FILE* f = NULL;
	try
	{
		_tfopen_s(&f, gGeneralLog.m_LogFilePath, _T("ab"));
		fwrite( CT2CA(inText), sizeof(char), _tcslen(inText), f);
		fwrite( "\n", sizeof(char), 1, f);
		fclose(f);
		f = NULL;
	}
	catch (...)
	{
		if (f != NULL)
			fclose(f);
	}*/
}