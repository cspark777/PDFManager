#pragma once
#include "stdafx.h"
#include "afxwin.h"

const CString kValidChars = (_T("0123456789abcdefghijklmnopqrstuxyvwzABCDEFGHIJKLMNOPQRSTUXYVWZ\r\n "));


/////////////////////////////////////////////////////////////////////////////
// CBCGPTagMaskEdit class
class BCGCBPRODLLEXPORT CBCGPTagMaskEdit : public CBCGPMaskEdit
{
	DECLARE_DYNAMIC(CBCGPTagMaskEdit)

protected:
	// Generated message map functions
	//{{AFX_MSG(CBCGPMaskEdit)
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////
// CAddMultipleTagsDlg dialog
class CAddMultipleTagsDlg : public CDialog
{
	DECLARE_DYNAMIC(CAddMultipleTagsDlg)	
	CString mText;
		
public:
	CAddMultipleTagsDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAddMultipleTagsDlg();
	LPCTSTR GetText(){ return mText.IsEmpty() ? _T("") : mText; };

// Dialog Data
	enum { IDD = IDD_ADD_MULTIPLE_TAGS_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	virtual void OnCancel();

public:
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	CBCGPTagMaskEdit m_AddMultipleTagsEdit;
	afx_msg void OnBnClickedOk();

	virtual BOOL OnInitDialog();
};
