#pragma once

#include "stdafx.h"
#include <afxtempl.h>

typedef struct
{
	bool bold;
	bool italic;
	UINT iconID;
	int indent;
}ITEMDATA;

#define ITEMDATA_Init(i)	\
	memset(i, 0, sizeof(ITEMDATA));	\
	(i)->iconID = -1;

class CIconComboBox : public CComboBox
{
public:
	CIconComboBox();
	virtual ~CIconComboBox();
// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIconComboBox)
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetImageList(CImageList * inImageList);
	void SetItemIcon(int iItem, UINT iconID);	
	void SetItemIndent(int iItem, int inIndent);	
	void SetItemBold(int iItem, bool inBold);
	void SetItemItalic(int iItem, bool inItalic);

protected:
	DECLARE_MESSAGE_MAP()

	CMap<int,int,ITEMDATA,ITEMDATA> m_IconMap;
	CImageList* m_ImageList;
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
};