
#include "afxwin.h"
#include "sn.h"

const CString sn::snchr = _T("CTIRO2ZVA3Q8U1J5NB7FWMES6HT9KYPLG4XD");
const int sn::snlen = 16;

// Szabi: case 193, 'Benubird' changed to 'Benubird Pro'
#ifndef _FREE
#define BENUBIRD_REG_KEY _T("Software\\Debenu\\Benubird PDF")
#else
#define BENUBIRD_REG_KEY _T("Software\\Debenu\\Benubird PDF")
#endif

#define max_days 14


int sn::getPos(int maxPos)
{
	int len =  snchr.GetLength();
	double x = (double)rand()/MAXSHORT;
	x *= maxPos;
	return (int)(x + 0.5);
}

bool sn::read(CString & serial)
{
	// !!!IMPORTANT!!!
	// I don't known why, but this function doesn't work in release version. 
	// Jozef: if you will be a time, please check it
	// ***************************************************************
	/*bool lReturn = false;

	CRegKey lRegKey;
	LONG lRet;
	lRet = lRegKey.Open(HKEY_CURRENT_USER, BENUBIRD_REG_KEY);
	if (ERROR_SUCCESS != lRet)
		return false;


	TCHAR lBuf[256];
	ULONG lLen;

	lRet = lRegKey.QueryStringValue(_T("serialNumber"), lBuf, &lLen);
	if (ERROR_SUCCESS == lRet)
	{
		serial = lBuf;
		lReturn = true;
	}

	lRegKey.Close();
	return lReturn;*/

	// Substitution
	CWinApp* lApp;
	lApp = AfxGetApp();
	serial.Empty();
	serial = lApp->GetProfileString(_T(""), _T("serialNumber"), serial);
	if (serial.IsEmpty())
		return true;
	return true;
}

bool sn::write(CString serial)
{
/*	bool lReturn = false;

	CRegKey lRegKey;
	LONG lRet;
	lRet = lRegKey.Open(HKEY_CURRENT_USER, BENUBIRD_REG_KEY);
	if (ERROR_SUCCESS != lRet)
		return false;

	lRet = lRegKey.SetStringValue(_T("serialNumber"), serial);
	if (ERROR_SUCCESS == lRet)
	{
		lReturn = true;
	}
	lRegKey.Close();

	return lReturn;
*/
	//by Tono, case 227
	// Substitution
	CWinApp* lApp;
	lApp = AfxGetApp();
	if (serial.IsEmpty())
		return false;
	lApp->WriteProfileString(_T(""), _T("serialNumber"), serial);
	return true;
}

bool sn::read2(AppDatesRec* dates)
{
	bool lReturn = false;

	CRegKey lRegKey;
	LONG lRet;
	lRet = lRegKey.Open(HKEY_CURRENT_USER, BENUBIRD_REG_KEY);
	if (ERROR_SUCCESS != lRet)
		return false;

	ULONG lSz;

	lRet = lRegKey.QueryBinaryValue(_T("appData"), dates, &lSz);
	if (ERROR_SUCCESS == lRet)
	{
		lReturn = true;
	}
	lRegKey.Close();

	return lReturn;	
}

bool sn::write2(AppDatesRec* dates)
{
	bool lReturn = false;

	CRegKey lRegKey;
	LONG lRet;
	lRet = lRegKey.Open(HKEY_CURRENT_USER, BENUBIRD_REG_KEY);
	if (ERROR_SUCCESS != lRet)
		return false;

	lRet = lRegKey.SetBinaryValue(_T("appData"), dates, sizeof(AppDatesRec));
	if (ERROR_SUCCESS == lRet)
	{
		lReturn = true;
	}
	lRegKey.Close();

	return lReturn;
}
