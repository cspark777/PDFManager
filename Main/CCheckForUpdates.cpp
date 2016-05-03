#include "stdafx.h"
#include "CCheckForUpdates.h"

#include "w3c.h"
#include "CVersion.h"
#include "CAlertDlg.h"

void CCheckForUpdates::CheckForUpdates(bool silent)
{
	AfxBeginThread(&CCheckForUpdates::CheckForUpdatesControllingFunction, (void*)(silent == false));
}

UINT __cdecl CCheckForUpdates::CheckForUpdatesControllingFunction( LPVOID pParam )
{
	bool lSilent = (pParam == NULL);
	int err = 0;

/*
	errors:
	0 = no error
	1 = general error
	2 = connection error
*/

	CW3Client w3c;

	int szRead = 0;
	char buf [1024];
	memset(buf, 0, sizeof(buf));

	if (w3c.Connect("http://www.debenu.com/"))
	{
#ifndef _FREE
		if (w3c.Request("benubird/benubird_pro_version_status_check.txt"))
		{
			szRead = w3c.Response((unsigned char*)buf, 1024);
			if (szRead == 0)
				err = 1;
		}
		else 
			w3c.Close();
#else
		if (w3c.Request("benubird/benubird_free_version_status_check.txt"))
		{
			szRead = w3c.Response((unsigned char*)buf, 1024);
			if (szRead == 0)
				err = 1;
		}
		else 
			w3c.Close();
#endif
	}
	else
		err = 2;

	CString lStrTitle;
	lStrTitle.LoadString(IDS_CHECK_FOR_UPDATES);

	HWND hWnd = AfxGetApp()->GetMainWnd()->GetSafeHwnd();

	if (!err)
	{
		int u1 = 0, u2 = 0, u3 = 0, u4 = 0;
		int v1 = 0, v2 = 0, v3 = 0, v4 = 0;

		if (sscanf(buf, "%d.%d.%d.%d", &u1, &u2, &u3, &u4) == 0)
			err = 1;

		CString lVerStr;
		CVersion lVersion;
		lVersion.GetVersionString(lVerStr);

		if (sscanf(CT2CA(lVerStr), "%d.%d.%d.%d", &v1, &v2, &v3, &v4) == 0)
			err = 1;

		if (!err)
		{
			CString lSvrNum;
			lSvrNum.Format(_T("%d%d%d%d"), u1, u2, u3, u4);
			CString lCurNum;
			lCurNum.Format(_T("%d%d%d%d"), v1, v2, v3, v4);

			int svr = _ttoi(lSvrNum);
			int cur = _ttoi(lCurNum);

			if (svr > cur)
			{
				CString lStrMsg, lStrTmp;
				lStrTmp.LoadString(IDS_UPDATE_AVAILABLE);
				lStrMsg.Format(lStrTmp, CA2CT(buf));

				if (MessageBox(hWnd, lStrMsg, lStrTitle, MB_YESNO | MB_ICONINFORMATION) == IDYES)
				{
					CString lWebPage = _T("http://www.debenu.com/benubird/updates.html");
					::ShellExecute(NULL, NULL, lWebPage, NULL, NULL, NULL);
				}
			}
			else if (!lSilent)
			{
				CString lStrMsg;
				lStrMsg.LoadString(IDS_UPDATE_NONE);
				MessageBox(hWnd, lStrMsg, lStrTitle, MB_OK | MB_ICONINFORMATION);
			}
		}
	}

	if (err && !lSilent)
	{
		CString lStrMsg;
		UINT lStrID = IDS_UPDATE_ERROR_GENERAL;
		if (err == 2)
			lStrID = IDS_UPDATE_ERROR_CONNECTION;

		lStrMsg.LoadString(lStrID);
		MessageBox(hWnd, lStrMsg, lStrTitle, MB_OK | MB_ICONSTOP);
	}
	
	AfxEndThread(0);
	return 0;
}
