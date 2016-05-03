#pragma once
#include "stdafx.h"
#include "afxwin.h"
#include "Define.h"
#include "CUtils.h"

#include "common.h"
//#import "..\Libraries\iSedQuickPDF\QuickPDFAX0712.dll" //named_guids raw_interfaces_only

/////////////////////////////////////////////////////////////////////
// CConvertToPDFDlg dialog
class CConvertToPDFDlg : public CDialog
{
	DECLARE_DYNAMIC(CConvertToPDFDlg)
public:
	BOOL ConvertImageToPDF(CString inFile, PDF_Entry* inEntry, int inCollection);

private:
	CStringList* mFileList;

protected:
	BOOL InitFiles();
	BOOL InitFiles(CStringList* inFileList);

	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

public:
	CListBox m_FileList;
	CComboBox m_CollectionCombo;
	CButton m_OpenPDFCheck;

	int m_nRepository;
	LIBRARY_VIEW_INFO *m_pViewInfo;
	PARAM_FOR_CDB *m_pParamCDB;
	void Set_ParamForCDB(PARAM_FOR_CDB *pParamCDB,LIBRARY_VIEW_INFO *pViewInfo);

	CConvertToPDFDlg(CStringList* inFileList = NULL);
	
	virtual ~CConvertToPDFDlg();
	enum { IDD = IDD_CONVERT2PDF_DLG };

	void EnableControls();

	afx_msg void OnBnClickedPropertiesButton();
	afx_msg void OnBnClickedDeleteButton();
	afx_msg void OnBnClickedOk();

};
