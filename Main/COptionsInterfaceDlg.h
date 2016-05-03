// COptionsInterfaceDlg.h
//*****************************************************************************

#ifndef _COptionsInterfaceDlg_H_
#define _COptionsInterfaceDlg_H_

#include "stdafx.h"

#if _MSC_VER > 1000
#pragma once
#endif

//*****************************************************************************
// COptionsInterfaceDlg class
class COptionsInterfaceDlg : public CDialog
{
	DECLARE_DYNAMIC(COptionsInterfaceDlg)

// Dialog data
public:
	enum { IDD = IDD_OPTIONS_INTERFACE_DLG };

// Member variables
private:
	CComboBox m_ColorSchemeCombo;

// Public methods
public:
	COptionsInterfaceDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~COptionsInterfaceDlg();
	BOOL ValidateAndProcess();

// Override
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

// Message
protected:
	DECLARE_MESSAGE_MAP()
};

#endif