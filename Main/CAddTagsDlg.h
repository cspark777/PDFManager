#ifndef _CAddTagsDlg_H_
#define _CAddTagsDlg_H_

#pragma once

#include "stdafx.h"
#include "afxwin.h"

class CAddTagsDlg : public CDialog
{
	DECLARE_DYNAMIC(CAddTagsDlg)

public:
	// Dialog Data
	enum { IDD = IDD_ADD_TAGS_DLG };

private:
	CString mText;
	CString mDefaultEditText;
	BOOL	mShowAddMultipleTagsDlg;
	
	CEdit	m_AddTagsEdit;
	
public:
	CAddTagsDlg(CWnd* pParent = NULL);
	virtual ~CAddTagsDlg();
	LPCTSTR GetText(){ return mText.IsEmpty() ? _T("") : mText; };
	BOOL NeedShowAddMultipleTagsDlg(){ return mShowAddMultipleTagsDlg; }

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);

protected:
	afx_msg void OnBnClickedAddMultipleBtn();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnBnClickedOk();
	DECLARE_MESSAGE_MAP()
};

#endif
