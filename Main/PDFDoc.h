//*******************************************************************************
// COPYRIGHT NOTES
// ---------------
// This is a sample for BCGControlBar Library Professional Edition
// Copyright (C) 1998-2012 BCGSoft Ltd.
// All rights reserved.
//
// This source code can be used, distributed or modified
// only under terms and conditions 
// of the accompanying license agreement.
//*******************************************************************************
//
// PDFDoc.h : interface of the CPDFDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_BenubirdDoc_H__A48CA9BC_BEDC_499A_9148_D1861ABCF3B9__INCLUDED_)
#define AFX_BenubirdDoc_H__A48CA9BC_BEDC_499A_9148_D1861ABCF3B9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CPlannerOptions;

enum PasswordType
{
  PT_NONE,
  PT_USER,
  PT_OWNER
};

class CPDFDoc : public CDocument
{
public: // create from serialization only
	CPDFDoc();
	DECLARE_DYNCREATE(CPDFDoc)

// Attributes
public:
	CString m_csDBPathName;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPDFDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual void Serialize(CArchive& ar);
	virtual void SetModifiedFlag(BOOL bModified = TRUE);
	virtual BOOL SaveModified();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPDFDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CPDFDoc)
	afx_msg void OnFileClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	BOOL                    m_bPdfModified;
	CString					m_strPath;

// for PDF Viewer
public:
	CImageList imageList;
	String     pdfPath;
	String	   pdfPage;

	int		   m_nPdfCurrentPageNo;
	int		   m_nPdfTotalPage;

	//String		pdfZoom;
	String     password;

	PasswordType passwordType;
	bool         hasUserPassword;
	bool         hasOwnerPassword;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BenubirdDoc_H__A48CA9BC_BEDC_499A_9148_D1861ABCF3B9__INCLUDED_)
