#ifndef _CPDFPropertiesDlg_H_
#define _CPDFPropertiesDlg_H_

#if _MSC_VER > 1000
#pragma once
#endif

#include "stdafx.h"
#include "Define.h"
#include "resource.h"
#include "CPDFPropertiesFrame.h"
#include "afxcmn.h"

/////////////////////////////////////////////////////////////////////////
// CPDFPropertiesDlg dialog class
class CPDFPropertiesDlg : public CDialog
{
	DECLARE_DYNAMIC(CPDFPropertiesDlg)

private:
	PDF_Entry* mEntry;
	CPDFPropertiesFrame* mFrame;
	CRect mRect;

public:
	enum { IDD = IDD_PDF_PROPERTIES_DLG };
	CPDFPropertiesDlg(PDF_Entry* inEntry);
	virtual ~CPDFPropertiesDlg();

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual void OnCancel();
    virtual void OnOK();
	
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};

#endif