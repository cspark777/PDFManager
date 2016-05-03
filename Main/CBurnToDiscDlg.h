#pragma once
#include "stdafx.h"

// CBurnToDiscDlg dialog

class CBurnToDiscDlg : public CDialog
{
	DECLARE_DYNAMIC(CBurnToDiscDlg)

public:
	CBurnToDiscDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CBurnToDiscDlg();

// Dialog Data
	enum { IDD = IDD_BURN_TO_DISC_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
};
