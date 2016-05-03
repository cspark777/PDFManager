#ifndef _CPDFPropertiesFrame_H_
#define _CPDFPropertiesFrame_H_

#include "stdafx.h"

#include "Define.h"
#include "afxwin.h"
#include "CUtils.h"

#if _MSC_VER > 1000
#pragma once
#endif

/////////////////////////////////////////////////////////////////////////
// CPDFPropertiesFrame class
class CPDFPropertiesFrame : public CDialog
{
	DECLARE_DYNAMIC(CPDFPropertiesFrame)

public:
	static BOOL LoadPDFMetadata(CString inFile, PDF_Entry* outEntry);
	static BOOL SavePDFMetadata(CString inFile, PDF_Entry* inEntry);

private:
	PDF_Entry* mEntry;
	CComboBox m_VersionCombo;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

public:
	PDF_Entry* GetPDFEntry(){return mEntry;}
	enum { IDD = IDD_PDF_PROPERTIES_FRAME };
	CPDFPropertiesFrame(PDF_Entry* inEntry);
	virtual ~CPDFPropertiesFrame();

	BOOL ValidateAndProcess();
	void Update();

	afx_msg void OnBnClickedWfEditTagsBtn();
	afx_msg void OnCbnSelchangeVersionCombo();
};

#endif