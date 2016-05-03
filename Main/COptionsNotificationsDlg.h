// COptionsNotificationsDlg.h
//*****************************************************************************

#ifndef _COptionsNotificationsDlg_H_
#define _COptionsNotificationsDlg_H_

#include "stdafx.h"

#if _MSC_VER > 1000
#pragma once
#endif

//*****************************************************************************
// COptionsNotificationsDlg class
class COptionsNotificationsDlg : public CDialog
{
	DECLARE_DYNAMIC(COptionsNotificationsDlg)

// Dialog data
public:
	enum { IDD = IDD_OPTIONS_NOTIFICATIONS_DLG };

// Member variables
private:
	CButton m_AddSilentlyCheck;
	CButton m_AddDirectlyCheck;
	CButton m_UpdateAvailableCheck;

// Public methods
public:
	COptionsNotificationsDlg(CWnd* pParent = NULL);
	virtual ~COptionsNotificationsDlg();
	BOOL ValidateAndProcess();

// Override
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

// Messages
protected:
	DECLARE_MESSAGE_MAP()
};

#endif