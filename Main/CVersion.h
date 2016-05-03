//CVersion.h

#ifndef _CVersion_H_
#define _CVersion_H_

#include "stdafx.h"

class CVersion
{
public:
	CVersion();
	~CVersion();
	void GetVersion(int& num1, int& num2, int& num3, int &num4);
	void GetVersionString(char *outStrNum);
	CString GetVersionString(CString &outStr);
};

#endif
