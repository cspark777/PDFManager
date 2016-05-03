#pragma once

#include "stdafx.h"
#include "Benubird.h"
#include "Define.h"
#include "CIconComboBox.h"


class CCollectionsComboBox : public CIconComboBox
{
public:
	enum {
		includeLibrary = 0x01,
		includeCollections = 0x02,
//#ifndef _FREE // Smart collections only for professional editions
		includeSmartCollections = 0x14,
//#endif
		includeCurrentSelection = 0x08,
		includeAll = 0xFF
	};

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCollectionsComboBox)
	//}}AFX_VIRTUAL

// Implementation
public:
	PARAM_FOR_CDB *m_pParamCDB;
	int m_nIsForLibrary;
	CCollectionsComboBox();
	void SetData_FromParent( PARAM_FOR_CDB *pParamCDB,int nForLibrary=0);
public:
	void Initialize(long selectCollection = FILTERTREE_MAJOR_BASE, UINT include = includeAll);
	long GetCollectionID();

protected:
	void InitializeLibrary();
	void InitializeCollections(int parentID = -1, int indent = 0);
	void InitializeSmartCollections(int parentID = -1, int indent = 0);

	int AddCollectionItem(CString inName, int iconID, long indent, long inData);

protected:
	DECLARE_MESSAGE_MAP()

	CImageList* m_ImageList;
	long mSelectedCollection;
};