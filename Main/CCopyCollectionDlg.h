#pragma once
#include "stdafx.h"

// CCopyCollectionDlg dialog

class CCopyCollectionDlg : public CDialog
{
	DECLARE_DYNAMIC(CCopyCollectionDlg)

public:
	CCopyCollectionDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCopyCollectionDlg();

// Dialog Data
	enum { IDD = IDD_COPY_COLLECTION_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
