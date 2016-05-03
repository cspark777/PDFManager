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
// PDFDoc.cpp : implementation of the CPDFDoc class
//

#include "stdafx.h"
#include "mainfrm.h"
#include "Benubird.h"
#include "DebenuPDFLibraryAX0912.h"
#include "PDFDoc.h"
#include "PDFView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPDFDoc

IMPLEMENT_DYNCREATE(CPDFDoc, CDocument)

BEGIN_MESSAGE_MAP(CPDFDoc, CDocument)
	//{{AFX_MSG_MAP(CPDFDoc)
	ON_COMMAND(ID_FILE_CLOSE, OnFileClose)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPDFDoc construction/destruction

CPDFDoc::CPDFDoc()
	: m_bPdfModified		(FALSE)
{
	TCHAR lpszFilePath [_MAX_PATH];
	if (::GetModuleFileName (NULL, lpszFilePath, _MAX_PATH) > 0)
	{
		TCHAR path_buffer[_MAX_PATH];   
		TCHAR drive[_MAX_DRIVE];   
		TCHAR dir[_MAX_DIR];

#if _MSC_VER < 1400
		_tsplitpath (lpszFilePath, drive, dir, NULL, NULL);
		_tmakepath (path_buffer, drive, dir, NULL, NULL);
#else
		_tsplitpath_s (lpszFilePath, drive, _MAX_DRIVE, dir, _MAX_DIR, NULL, 0, NULL, 0);
		_tmakepath_s (path_buffer, drive, dir, NULL, NULL);
#endif

		m_strPath = path_buffer;
	}

	m_nPdfCurrentPageNo = 1;
	m_nPdfTotalPage = 1;
}

CPDFDoc::~CPDFDoc()
{
}

BOOL CPDFDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	return TRUE;
}

BOOL CPDFDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	// pdf
	//pdfZoom = _T("75");
	pdfPath = lpszPathName;

	// not our file type
	//	if (((int)ShellExecute(NULL, _T("open") ,lpszFileName, NULL, NULL, SW_SHOWDEFAULT)) <= 32)
	//		return NULL;

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CPDFDoc serialization

void CPDFDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CPDFDoc diagnostics

#ifdef _DEBUG
void CPDFDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CPDFDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

void CPDFDoc::SetModifiedFlag(BOOL bModified/* = TRUE*/)
{

}

BOOL CPDFDoc::SaveModified()
{
	if (m_bPdfModified)
	{
	}

	return TRUE;
}

void CPDFDoc::OnFileClose() 
{
}
