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
// LibraryDoc.h : interface of the CLibraryDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_BenubirdDoc_H__A48CA9BC_BEDC_499A_9148_D1861ABCF3A9__INCLUDED_)
#define AFX_BenubirdDoc_H__A48CA9BC_BEDC_499A_9148_D1861ABCF3A9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "common.h"
#include "CPPSQLite3u.h"
#include "../libraries/shared/CBRepository.h"

typedef CArray<CTFiles_Entry,CTFiles_Entry> CTFilesArray;

class CLibraryDoc : public CDocument
{
public: // create from serialization only
	CLibraryDoc();
	DECLARE_DYNCREATE(CLibraryDoc)

// Attributes
public:

	LIBRARY_VIEW_INFO		m_ViewInfo; //for view
	
	PARAM_FOR_CDB m_ParamCDB;
	PARAM_FOR_CDB *m_pParamCDB;
	CDB m_db;
	CDBSQL m_FilesSQL;
	CStringList m_SystemDirsList;

	CString m_csDBPathName;
	CString m_csDBPath;
	CString m_csDBName;
// Operations
public:
	void UpdateLibrary();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLibraryDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual void Serialize(CArchive& ar);
	virtual void SetModifiedFlag(BOOL bModified = TRUE);
	virtual BOOL SaveModified();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CLibraryDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CLibraryDoc)
	afx_msg void OnFileClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	CTFilesArray sFilesArray;

	// SQL
	bool UpdateSQLFromCollection(int inTreeFilterID, CDBSQL* inDBSQL);
	bool UpdateSQLFromTag(CString lCondition, CDBSQL* inDBSQL);
	bool UpdateSQLFromBrowser(CString lCondition, CDBSQL* inDBSQL);
	bool UpdateSQLFromFind(bool bTagsPanelEmpty, int lSearchMode, CString lFindStr, CDBSQL* inDBSQL);
	bool UpdateSQLSort(int iColumn, bool bAscending, CDBSQL* inDBSQL);

	// Files grid/list methods 
	int GetRowID(int inFileID);
	bool RefreshFile(int inFileID, bool inRefreshAll = true);

	CMenu* FillAddToCollMenuRecursively(CMenu* inParentMenu, int inParentID, LPCTSTR inParentName, CIntArray* inFileIDs);
	CMenu* FillRemoveFromCollMenuRecursively(CMenu* inParentMenu, int inParentID, LPCTSTR inParentName, CIntArray* inFileIDs);
	CMenu* FillAddToTagsMenu(CMenu* inParentMenu, int inParentID, LPCTSTR inParentName, CIntArray* inFileIDs);
	CMenu* FillRemoveFromTagsMenu(CMenu* inParentMenu, int inParentID, LPCTSTR inParentName, CIntArray* inFileIDs);
	void FreeMenuRecursively(CMenu* inMenu);

	BOOL CanConvertToPDF(CString inFilePath);
	CString GetAssociatedApplicationFromFileExtension(CString strFileExtensions);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BenubirdDoc_H__A48CA9BC_BEDC_499A_9148_D1861ABCF3A9__INCLUDED_)
