// COptionsPDFDlg.h
//*****************************************************************************

#ifndef _COptionsPDFDlg_H_
#define _COptionsPDFDlg_H_

#include "stdafx.h"
#include "afxwin.h"

#if _MSC_VER > 1000
#pragma once
#endif

//*****************************************************************************
// COptionsPDFDlg class
class COptionsPDFDlg : public CDialog
{
	DECLARE_DYNAMIC(COptionsPDFDlg)

private:
	CButton m_HyperlinkCheck;
	CButton m_BookmarksCheck;
	CButton m_EmbedSubsetCheck;
	CButton m_DisplayInfoTabCheck;
	CComboBox m_GSettingsCombo;
	CBCGPEdit m_PDFOutputEdit;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

public:
	enum { IDD = IDD_OPTIONS_PDF_DLG };
	COptionsPDFDlg(CWnd* pParent = NULL);
	virtual ~COptionsPDFDlg();
	BOOL ValidateAndProcess();

	afx_msg void OnBnClickedOptGeneralShowIconInSystemTrayCheck();
	CButton m_RenderWithQuickPDFCheck;
};

#endif