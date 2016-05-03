#include "stdafx.h"
#include "CCollectionsComboBox.h"
#include "resource.h"
#include "CDB.h"
#include "LibraryView.h"


BEGIN_MESSAGE_MAP(CCollectionsComboBox, CComboBox)
	//{{AFX_MSG_MAP(CCollectionsComboBox)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CCollectionsComboBox::Initialize(long selectCollection,  UINT include)
{
	if(m_nIsForLibrary == 1)
	{
		return;
	}
	CBCGPToolBarImages lImages;
	m_ImageList = new CImageList;	
	lImages.SetImageSize(CSize(16, 16));
	lImages.Load(IDB_COLLECTIONS_BMP);
	m_ImageList->Create(16, 16, ILC_COLOR32, lImages.GetCount(), 1);
	for (int i = 0; i < lImages.GetCount(); i++)
		m_ImageList->Add(lImages.ExtractIcon(i));

	SetImageList(m_ImageList);

	mSelectedCollection = selectCollection;

	if (include & includeLibrary)
		InitializeLibrary();
	if (include & includeCollections)
		InitializeCollections();

//#ifndef _FREE // Smart collections only for professional edition
	if (include & includeSmartCollections)
		InitializeSmartCollections();
//#endif
	int i = -1;
	if (include & includeCurrentSelection)
	{
		i = AddCollectionItem(_T("Current selection"), -1, 0, FILTERTREE_CURRENT_SELECTION);
		SetItemItalic(i, true);
	}

	if (GetCurSel() == -1)
		SetCurSel(0);
}

void CCollectionsComboBox::InitializeLibrary()
{
	AddCollectionItem(_T("Library"), 0, 0, 0);
	
	if(m_nIsForLibrary == 0)
	{
		AddCollectionItem(_T("All"), 3, 1, FILTERTREE_MAJOR_ALL);
		AddCollectionItem(_T("Documents"), 5, 1, FILTERTREE_MAJOR_DOC);
		AddCollectionItem(_T("Spreadsheets"), 6, 1, FILTERTREE_MAJOR_SPREAD);
		AddCollectionItem(_T("Presentations"), 7, 1, FILTERTREE_MAJOR_PRES);
		AddCollectionItem(_T("Images"), 8, 1, FILTERTREE_MAJOR_IMAGES);
		AddCollectionItem(_T("Videos"), 9, 1, FILTERTREE_MAJOR_VIDEOS);
		AddCollectionItem(_T("Other"), 10, 1, FILTERTREE_MAJOR_OTHER);
	}
}

void CCollectionsComboBox::InitializeCollections(int parentID, int indent)
{
	CTCollections lCol(m_pParamCDB->pDB);
	if(	lCol.GetCollectionsByNames(parentID))
	{
		 while (lCol.mDataIsValid)
		 {
			AddCollectionItem(lCol.mName, 14, indent+1, FILTERTREE_COLLECTION_MIN + lCol.mID);  
			//AddCollectionItem(lCol.mName, 14, indent+1, FILTERTREE_COLLECTION_MIN + lCol.mID); 
			InitializeCollections(lCol.mID, indent+2);
			lCol.LoadNextRow();
		 }
	}
}

void CCollectionsComboBox::InitializeSmartCollections(int parentID, int indent)
{
	if (parentID == -1 && indent == 0)
	{
		AddCollectionItem(_T("Collections"), 1, 0, 0);
		AddCollectionItem(_T("Incomplete Properties"), 11, 1, FILTERTREE_COLLECTION_UNSORTED);
		AddCollectionItem(_T("Last modified"), 12, 1, FILTERTREE_COLLECTION_REC_MODIF);
		AddCollectionItem(_T("Last added"), 13, 1, FILTERTREE_COLLECTION_REC_ADDED);
	}

	CTSmartColl lSmartColl(m_pParamCDB);
	if (lSmartColl.GetSmartCollections())
	{
		while (lSmartColl.mDataIsValid)
		{
			AddCollectionItem(lSmartColl.mName, 4, 1, FILTERTREE_SMART_COLLECTION_MIN + lSmartColl.mID);
			lSmartColl.LoadNextRow();
		}
	}
}

int CCollectionsComboBox::AddCollectionItem(CString inName, int iconID, long indent, long inData)
{
	int i = AddString(inName);
	if (iconID != -1)
		SetItemIcon(i, iconID);
	SetItemIndent(i, indent);
	if (inData != 0)
	{
		SetItemData(i, inData);
		if (mSelectedCollection == inData) SetCurSel(i);
	}

	return i;
}


long CCollectionsComboBox::GetCollectionID()
{
	int i = GetCurSel();
	return (int)GetItemData(i);
}
void CCollectionsComboBox::SetData_FromParent(PARAM_FOR_CDB *pParamCDB,int nForLibrary)
{
	m_nIsForLibrary = nForLibrary;
	m_pParamCDB = pParamCDB;
}

CCollectionsComboBox::CCollectionsComboBox()
{
	m_nIsForLibrary = 0;
}