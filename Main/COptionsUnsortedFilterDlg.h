// COptionsUnsortedFilterDlg.h
//*****************************************************************************

#ifndef _COptionsUnsortedFilterDlg_H_
#define _COptionsUnsortedFilterDlg_H_

#include "stdafx.h"
#include "afxwin.h"

#if _MSC_VER > 1000
#pragma once
#endif

//*****************************************************************************
// COptionsUnsortedFilterDlg class
class COptionsUnsortedFilterDlg : public CDialog
{
	DECLARE_DYNAMIC(COptionsUnsortedFilterDlg)

// Dialog data
public:
	enum { IDD = IDD_OPTIONS_UNSORTED_FILTER_DLG };

// Member variables
private:
	CButton m_TitleCheck;
	CButton m_AuthorCheck;
	CButton m_SubjectCheck;
	CButton m_TagsCheck;
	CButton m_CategoriesCheck;
	CButton m_CollectionsCheck;
	
// Public methods
public:
	COptionsUnsortedFilterDlg(CWnd* pParent = NULL);
	virtual ~COptionsUnsortedFilterDlg();
	BOOL ValidateAndProcess();

// Private methods
private:
	BOOL AddToStartup();
	BOOL RemoveFromStartup();
	BOOL IsInStartup();

// Override
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

// Messages
protected:
	DECLARE_MESSAGE_MAP()
};

#endif