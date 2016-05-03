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
// BenubirdDoc.cpp : implementation of the CLibraryDoc class
//

#include "stdafx.h"
#include "Define.h"
#include "mainfrm.h"
#include "Benubird.h"
#include "LibraryDoc.h"

#include "LibraryView.h"
#include "PDFView.h"
#include "ShortcutsView.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern int nLibraryCount;
extern TCHAR  g_szDBPath_Backup[MAX_PATH];
extern bool gIsPro;

/////////////////////////////////////////////////////////////////////////////
// CLibraryDoc

IMPLEMENT_DYNCREATE(CLibraryDoc, CDocument)

BEGIN_MESSAGE_MAP(CLibraryDoc, CDocument)
	//{{AFX_MSG_MAP(CLibraryDoc)
	ON_COMMAND(ID_FILE_CLOSE, OnFileClose)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLibraryDoc construction/destruction

CLibraryDoc::CLibraryDoc()
{
	m_csDBPathName = g_szDBPath_Backup;
	m_csDBPath = m_csDBPathName;
	int nLen = m_csDBPathName.GetLength();

	for(int i=nLen-1;i>0;i--)
	{
		if(g_szDBPath_Backup[i] == _T('\\'))
		{
			m_csDBPath = m_csDBPathName.Left(i+1);
			m_csDBName = m_csDBPathName.Right(nLen-i-1);
			break;
		}
			
	}

	BOOL ret = m_db.Initialize(g_szDBPath_Backup);
	
	m_ParamCDB.pDB = &m_db;
	m_ParamCDB.pFilesSQL = &m_FilesSQL;
	m_ParamCDB.pSystemDirsList = &m_SystemDirsList;

	wcscpy(m_ParamCDB.szLibPathName,m_csDBPathName.GetBuffer());
	
	m_pParamCDB = &m_ParamCDB;

	m_ViewInfo.pRepository = NULL;
	m_ViewInfo.pWorkspace = NULL;
}

CLibraryDoc::~CLibraryDoc()
{
}

BOOL CLibraryDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	if(!gIsPro)
		CDocument::SetTitle(_T("benubird"));

	nLibraryCount++;
	return TRUE;
}

BOOL CLibraryDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	m_csDBPathName = lpszPathName;
	m_csDBPath = m_csDBPathName;
	int nLen = wcslen(lpszPathName);

	for(int i=nLen-1;i>0;i++)
	{
		if(lpszPathName == _T("\\"))
		{
			m_csDBPath = m_csDBPathName.Left(i);
			break;
		}
			
	}
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	
	nLibraryCount++;

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CLibraryDoc serialization

void CLibraryDoc::Serialize(CArchive& ar)
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
// CLibraryDoc diagnostics

#ifdef _DEBUG
void CLibraryDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CLibraryDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

void CLibraryDoc::SetModifiedFlag(BOOL bModified/* = TRUE*/)
{
}

BOOL CLibraryDoc::SaveModified()
{
	return TRUE;
}

void CLibraryDoc::UpdateLibrary()
{
    CMDIChildWnd * pChild = ((CMDIFrameWnd*)(AfxGetApp()->m_pMainWnd))->MDIGetActive();
	CLibraryView* pView = DYNAMIC_DOWNCAST(CLibraryView, pChild->GetActiveView ());
	if (pView != NULL)
	{
		ASSERT_VALID (pView);
		pView->Refresh();
	}
}

void CLibraryDoc::OnFileClose() 
{
	nLibraryCount--;
}


//*****************************************************************************
// methods for DBSQL 
/* Pomocou tychto metod je mozne vyskladat select pre tabulku TFiles.
Poradie: collection -> tags -> browser -> find */
bool CLibraryDoc::UpdateSQLFromCollection(int inTreeFilterID, CDBSQL* inDBSQL)
{
//	CMainFrame* lMainFrame = (CMainFrame*)AfxGetMainWnd();
//	int inTreeFilterID = lMainFrame->GetSelectedCollectionGuiID();
	CString lCondition;
	int iFilterTreeCollectionMin = FILTERTREE_COLLECTION_MIN;
	int iFilterTreeCollectionMax = FILTERTREE_COLLECTION_MAX;
	
	try
	{
		CString lSQL;
		if (inTreeFilterID == FILTERTREE_MAJOR_ALL)
		{ // don't need to add anything - no condition
		}
		else if((inTreeFilterID == FILTERTREE_MAJOR_DOC) ||
				(inTreeFilterID == FILTERTREE_MAJOR_PRES) ||
				(inTreeFilterID == FILTERTREE_MAJOR_SPREAD) ||
				(inTreeFilterID == FILTERTREE_MAJOR_IMAGES) ||
				(inTreeFilterID == FILTERTREE_MAJOR_VIDEOS) ||
				(inTreeFilterID == FILTERTREE_MAJOR_OTHER))
		{
			inDBSQL->AddTable(_T("FExtensions"));
			lCondition.Format(_T("(FExtensions.MFilters_idMfilter = %u and Files.FExtensions_idFExtension = FExtensions.idFExtension)"), inTreeFilterID - FILTERTREE_MAJOR_BASE);
			inDBSQL->AddCondition(lCondition);
		}
		else if ( (inTreeFilterID > FILTERTREE_COLLECTION_MIN) &&
			(inTreeFilterID < FILTERTREE_COLLECTION_MAX))
		{	//a valid collection
			inDBSQL->AddTable(_T("Collections_has_Files"));
			lCondition.Format(_T("(Collections_has_Files.Collections_idCollection = %d and Collections_has_Files.Files_idFile = Files.idFile)"), inTreeFilterID - FILTERTREE_COLLECTION_MIN);
			inDBSQL->AddCondition(lCondition);
		}
		else if ( (inTreeFilterID > FILTERTREE_SMART_COLLECTION_MIN) &&
			(inTreeFilterID < FILTERTREE_SMART_COLLECTION_MAX))
		{	
			CTSmartColl lSmartColl(m_pParamCDB);
			lSmartColl.UpdateSQL(inTreeFilterID - FILTERTREE_SMART_COLLECTION_MIN, inDBSQL);
		}
		else if (inTreeFilterID > FILTERTREE_PREDEF_COLL_BASE)
		{	//a valid predefined value (unsorted, recently added, ...)
			//if(inTreeFilterID == FILTERTREE_COLLECTION_UNSORTED)
			//lSQL.Format(_T("select Files.* from Files, Collections_has_Files where Collections_has_Files.Collections_idCollection = %d and Collections_has_Files.Files_idFile = Files.idFile"), (inCollectionID - COLLECTION_TYPE_CUSTOM));
			if (inTreeFilterID == FILTERTREE_COLLECTION_UNSORTED)
			{
				CString lSubStr, lTmpStr;
				CBenubirdApp *lApp =  (CBenubirdApp*)AfxGetApp();
				if( lApp->GetProfileInt(_T("Options"), _T("UFTitle"), BST_CHECKED) == BST_CHECKED)
				{	
					lSubStr.Format(_T(" Files.Title LIKE '' "));
					lTmpStr = lSubStr;
				}
				if( lApp->GetProfileInt(_T("Options"), _T("UFAuthor"), BST_CHECKED) == BST_CHECKED)
				{
					if(lSubStr.GetLength()>0)
						lSubStr.Format(_T(" %s or Files.Author LIKE '' "), lTmpStr);
					else
						lSubStr.Format(_T(" Files.Author LIKE '' "));
					lTmpStr = lSubStr;
						
				}
				if( lApp->GetProfileInt(_T("Options"), _T("UFSubject"), BST_CHECKED) == BST_CHECKED)
				{
					if(lSubStr.GetLength()>0)
						lSubStr.Format(_T(" %s or Files.Subject LIKE '' "), lTmpStr);
					else
						lSubStr.Format(_T(" Files.Subject LIKE '' "));
					lTmpStr = lSubStr;
						
				}
				if( lApp->GetProfileInt(_T("Options"), _T("UFCategories"), BST_CHECKED) == BST_CHECKED)
				{
					if(lSubStr.GetLength()>0)
						lSubStr.Format(_T("%s or Files.Category LIKE '' "), lTmpStr);
					else
						lSubStr.Format(_T(" Files.Category LIKE ''"));
					lTmpStr = lSubStr;
				}
				if (lApp->GetProfileInt(_T("Options"), _T("UFTags"), BST_CHECKED) == BST_CHECKED)
				{
					if (lSubStr.GetLength() > 0)
						lSubStr.Format(_T("(idFile not in (select Files_idFile from Files_has_Tags) or (%s))"), lTmpStr);
					else
						lSubStr.Format(_T("(idFile not in (select Files_idFile from Files_has_Tags))"));
					lTmpStr = lSubStr;
				}

				if( lApp->GetProfileInt(_T("Options"), _T("UFCollections"), BST_CHECKED) == BST_CHECKED)
				{
					if(lSubStr.GetLength() > 0)
						lSubStr.Format(_T("(idFile not in (select Files_idFile from Collections_has_Files) or (%s))"), lTmpStr);
					else
						lSubStr.Format(_T("(idFile not in (select Files_idFile from Collections_has_Files))"));
					//inDBSQL->AddCondition(lCondition);
				}

				//inDBSQL->AddCondition(lCondition);
				//else // add nothing;
				if (lSubStr.IsEmpty())
					lSubStr.Format(_T("idFile = -1"));
				inDBSQL->AddCondition(lSubStr);



/* old approach by collection (not assigned to collection == UNSORTED)
				if (lStrWithAnd.IsEmpty())
					lSQL.Format(_T("select Files.* from Files where idFile not in (select Files_idFile from Collections_has_Files);"));
				else
					lSQL.Format(_T("select Files.* from Files, Files_has_Tags where (idFile not in (select Files_idFile from Collections_has_Files))%s group by Files.idFile;"), lStrWithAnd);
*/
			}
			else if (inTreeFilterID == FILTERTREE_COLLECTION_REC_ADDED)
			{
				inDBSQL->AddSort(_T("Files.AddedHi desc"));
				inDBSQL->AddSort(_T("Files.AddedLo desc"));
				lCondition.Format(_T("limit %u"), RECENTLY_ROWS_SHOW_NUM);
				inDBSQL->AddLimit(lCondition);
			}
			else if(inTreeFilterID == FILTERTREE_COLLECTION_REC_MODIF)
			{
				inDBSQL->AddSort(_T("Files.ModifiedHi desc"));
				inDBSQL->AddSort(_T("Files.ModifiedLo desc"));
				lCondition.Format(_T("limit %u"), RECENTLY_ROWS_SHOW_NUM);
				inDBSQL->AddLimit(lCondition);
			}
			else
			{
				ASSERT(0);
				return false;
			}
		}
		else
		{
			return false;
		}

		//if (!gIsTagStr) lSQL.Replace(_T(", Files_has_Tags"), _T("")); // SQLite bug, if this will be fixed, this line must be removed
	}
	catch(...)
	{
		return false;
	}
	return true;
}

bool CLibraryDoc::UpdateSQLFromTag(CString lCondition, CDBSQL* inDBSQL)
{
	try
	{
		if (!lCondition.IsEmpty())
		{
			/* dovod tohto specialneho pridania je triedenie podla tagov*/
			//inDBSQL->AddGroup(_T("Files.idFile having ") + lCondition);

			inDBSQL->AddTable(_T("Files_has_Tags"));
			inDBSQL->AddCondition(lCondition);
			inDBSQL->AddGroup(_T("Files.idFile"));
		}
	}
	catch(...)
	{
		return false;
	}
	return true;
}

bool CLibraryDoc::UpdateSQLFromBrowser(CString lCondition, CDBSQL* inDBSQL)
{
	try
	{
		if (!lCondition.IsEmpty())
			inDBSQL->AddCondition(lCondition);
	}
	catch(...)
	{
		return false;
	}
	return true;
}

bool CLibraryDoc::UpdateSQLFromFind(bool bTagsPanelEmpty, int lSearchMode, CString lFindStr, CDBSQL* inDBSQL)
{
	if (lFindStr.IsEmpty())
		return true;

	CString lCondition;
	try
	{
		switch (lSearchMode)
		{
			case kSearchAll:
				if (bTagsPanelEmpty)
				{
					inDBSQL->AddTable(_T("Files_has_Tags"));
					lCondition.Format(_T("((Files.Name LIKE '%%%s%%') or (Files.Title LIKE '%%%s%%') or (Files.Author LIKE '%%%s%%') or (Files.Subject LIKE '%%%s%%') or (Files.Comments LIKE '%%%s%%') or (Files_has_Tags.Files_idFile = Files.idFile and Files_has_Tags.Tags_idTag = (select Tags.idTag from Tags where Tags.Name LIKE '%%%s%%')))"), 
						lFindStr, lFindStr, lFindStr, lFindStr, lFindStr, lFindStr);
					inDBSQL->AddCondition(lCondition);
					inDBSQL->AddGroup(_T("Files.idFile"));
				}
				else
				{
					lCondition.Format(_T("((Files.Name LIKE '%%%s%%') or (Files.Title LIKE '%%%s%%') or (Files.Author LIKE '%%%s%%') or (Files.Subject LIKE '%%%s%%') or (Files.Comments LIKE '%%%s%%'))"), 
						lFindStr, lFindStr, lFindStr, lFindStr, lFindStr);
					inDBSQL->AddCondition(lCondition);
					inDBSQL->AddGroup(_T("Files.idFile"));
				}
				break;
			case kSearchTitle:
				lCondition.Format(_T("Files.Title LIKE '%%%s%%'"), lFindStr);
				inDBSQL->AddCondition(lCondition);
				break;
			case kSearchAuthor:
				lCondition.Format(_T("Files.Author LIKE '%%%s%%';"), lFindStr);
				inDBSQL->AddCondition(lCondition);
				break;
			case kSearchSubject:
				lCondition.Format(_T("Files.Subject LIKE '%%%s%%';"), lFindStr);
				inDBSQL->AddCondition(lCondition);
				break;
			case kSearchTags:
				inDBSQL->AddTable(_T("Files_has_Tags"));
				lCondition.Format(_T("Files_has_Tags.Files_idFile = Files.idFile and Files_has_Tags.Tags_idTag in (select Tags.idTag from Tags where Tags.Name LIKE '%%%s%%')"), lFindStr);
				inDBSQL->AddCondition(lCondition);
				break;
		}
	}
	catch(...)
	{
		return false;
	}
	return true;
}

bool CLibraryDoc::UpdateSQLSort(int iColumn, bool bAscending, CDBSQL* inDBSQL)
{
	try
	{
		// change m_FilesSQL
		CString lFieldName;
		switch(iColumn){
			case 1: lFieldName = _T("lower(Files.Title)"); break;
			case 2: lFieldName = _T("lower(Files.Author)"); break;
			case 3: lFieldName = _T("lower(Files.Subject)"); break;
			case 4: //tags (special case, we need to add a new coulumn into the select)
				lFieldName = _T("lower(TagsStr)");
				break;
			case 5:
				lFieldName = _T("lower(CollectionsStr)");
				break;
			case 6:
				inDBSQL->AddField(_T("FExtensions.Name"));
				inDBSQL->AddTable(_T("FExtensions"));
				inDBSQL->AddCondition(_T("Files.FExtensions_idFExtension = FExtensions.idFExtension"));
				lFieldName = _T("lower(FExtensions.Name)"); 
				break;
			case 7: lFieldName = _T("Files.SizeHi, Files.SizeLo"); break; 
			case 8: lFieldName = _T("lower(Files.Category)"); break;
			case 9: lFieldName = _T("lower(Files.Path)"); break;
			case 10: lFieldName = _T("Files.CreatedHi, Files.CreatedLo"); break;
			case 11: lFieldName = _T("Files.ModifiedHi, Files.ModifiedLo"); break;
			case 12: lFieldName = _T("Files.AddedHi, Files.AddedLo"); break;
			case 13: lFieldName = _T("Files.AccessedHi, Files.AccessedLo"); break;
			case 14: lFieldName = _T("Files.Attributes"); break;
			case 15: lFieldName = _T("lower(Files.Comments)"); break;
			default: lFieldName = _T("lower(Files.Name)"); break;
		}
		if (bAscending)
			lFieldName += _T(" asc");
		else 
			lFieldName += _T(" desc");
		inDBSQL->AddSort(lFieldName);
	}
	catch(...)
	{
		return false;
	}

	return true;
}

//*****************************************************************************
// Files grid/list methods 
int CLibraryDoc::GetRowID(int inFileID)
{
	int lCount = (int)sFilesArray.GetCount();
	for (int i = 0; i < lCount; i++)
	{
		if (sFilesArray[i].mID == inFileID)
			return i;
	}
	return -1;
}

bool CLibraryDoc::RefreshFile(int inFileID, bool inRefreshAll)
{
	CIntArray lFilesIDs;
	lFilesIDs.Add(inFileID);
	return true;
}

//*************************************************************************************************
CMenu* CLibraryDoc::FillAddToCollMenuRecursively(CMenu* inParentMenu, int inParentID, LPCTSTR inParentName, CIntArray* inFileIDs)
{
	CTCollections lCol(m_pParamCDB->pDB);
	CMenu* lMenu = NULL;
	CIDData lAddToCollIDData;

	if (!lCol.GetAllCollectionsNotAssignedToFileIDs(inFileIDs, lAddToCollIDData))
		return NULL;
	
	if (lCol.GetCollectionsByNames(inParentID))
	{
		lMenu = new CMenu;
		lMenu->CreateMenu();
		while (lCol.mDataIsValid)
		{
			if (lCol.HasCollectionChildItems(lCol.mID))
			{
				FillAddToCollMenuRecursively(lMenu, lCol.mID, lCol.mName, inFileIDs); 
			}
			else
			{
				lMenu->AppendMenu(MF_BYPOSITION | MF_STRING, ID_COLLECTIONID_MENUROOT + lCol.mID, lCol.mName);
				if (lAddToCollIDData.GetIdxByID(lCol.mID) == -1)
					lMenu->EnableMenuItem(ID_COLLECTIONID_MENUROOT + lCol.mID, MF_BYCOMMAND | MF_GRAYED | MF_DISABLED);
			}
			lCol.LoadNextRow();
		}
	}
	if (lMenu->m_hMenu)
	{
		if (lMenu->GetMenuItemCount() > 0)
		{
			if (inParentID != -1)
			{
				lMenu->InsertMenu(0, MF_BYPOSITION | MF_STRING, ID_COLLECTIONID_MENUROOT + inParentID, _T("-this-"));
				if (lAddToCollIDData.GetIdxByID(inParentID) == -1)
					lMenu->EnableMenuItem(0, MF_BYPOSITION | MF_GRAYED | MF_DISABLED);
				lMenu->InsertMenu(1, MF_BYPOSITION | MF_SEPARATOR, 0, _T(""));
				inParentMenu->AppendMenu(MF_BYCOMMAND | MF_POPUP, reinterpret_cast<UINT_PTR>(lMenu->GetSafeHmenu()), inParentName);
			}
			else
			{
				for (int i = 0; i < inParentMenu->GetMenuItemCount(); i++)
				{
					UINT lID = inParentMenu->GetMenuItemID(i);
					if (lID == ID_ADD_TO_COLLECTION)
					{
						inParentMenu->RemoveMenu(i, MF_BYPOSITION);
						inParentMenu->InsertMenu(i, MF_BYPOSITION | MF_POPUP, reinterpret_cast<UINT_PTR>(lMenu->GetSafeHmenu()), inParentName);
						return lMenu;
					}
				}
			}
		}
		else if (lMenu != NULL) delete lMenu;
	}
	return NULL;
}
//*************************************************************************************************
CMenu* CLibraryDoc::FillRemoveFromCollMenuRecursively(CMenu* inParentMenu, int inParentID, LPCTSTR inParentName, CIntArray* inFileIDs)
{
	CTCollections lCol(m_pParamCDB->pDB);
	CMenu* lMenu;
	CIDData lRemoveFromCollIDData;

	if (!lCol.GetAllCollectionsAssignedToFileIDs(inFileIDs, lRemoveFromCollIDData))
		return NULL;

	if (lCol.GetCollectionsByNames(inParentID))
	{
		lMenu = new CMenu;
		lMenu->CreateMenu();
		while (lCol.mDataIsValid)
		{
			if (lCol.HasCollectionChildItems(lCol.mID))
			{
				FillRemoveFromCollMenuRecursively(lMenu, lCol.mID, lCol.mName, inFileIDs); 
			}
			else
			{
				lMenu->AppendMenu(MF_BYPOSITION | MF_STRING, ID_REMOVE_FROM_COLLECTION_ID_MENUROOT + lCol.mID, lCol.mName);
				if (lRemoveFromCollIDData.GetIdxByID(lCol.mID) == -1)
					lMenu->EnableMenuItem(ID_REMOVE_FROM_COLLECTION_ID_MENUROOT + lCol.mID, MF_BYCOMMAND | MF_GRAYED | MF_DISABLED);
			}
			lCol.LoadNextRow();
		}
	}
	if (lMenu->m_hMenu)
	{
		if (lMenu->GetMenuItemCount() > 0)
		{
			if (inParentID != -1)
			{
				lMenu->InsertMenu(0, MF_BYPOSITION | MF_STRING, ID_REMOVE_FROM_COLLECTION_ID_MENUROOT + inParentID, _T("-this-"));
				if (lRemoveFromCollIDData.GetIdxByID(inParentID) == -1)
					lMenu->EnableMenuItem(0, MF_BYPOSITION | MF_GRAYED | MF_DISABLED);
				lMenu->InsertMenu(1, MF_BYPOSITION | MF_SEPARATOR, 0, _T(""));
				inParentMenu->AppendMenu(MF_BYCOMMAND | MF_POPUP, reinterpret_cast<UINT_PTR>(lMenu->GetSafeHmenu()), inParentName);
			}
			else
			{
				for (int i = 0; i < inParentMenu->GetMenuItemCount(); i++)
				{
					UINT lID = inParentMenu->GetMenuItemID(i);
					if (lID == ID_REMOVE_FROM_COLLECTION)
					{
						inParentMenu->RemoveMenu(i, MF_BYPOSITION);
						inParentMenu->InsertMenu(i, MF_BYPOSITION | MF_POPUP, reinterpret_cast<UINT_PTR>(lMenu->GetSafeHmenu()), inParentName);
						return lMenu;
					}
				}
			}
		}
		else if (lMenu != NULL) delete lMenu;
	}
	return NULL;
}
//*************************************************************************************************
CMenu* CLibraryDoc::FillAddToTagsMenu(CMenu* inParentMenu, int inParentID, LPCTSTR inParentName, CIntArray* inFileIDs)
{
	CTTags lTags(m_pParamCDB->pDB);
	CMenu* lMenu;
	CIDData lAddToTagsIDData;

	if (!lTags.GetAllTagsNotAssignedToFileIDs(inFileIDs, lAddToTagsIDData))
		return NULL;
	
	lMenu = new CMenu;
	lMenu->CreateMenu();
	for (int i = 0; i < lAddToTagsIDData.GetCount(); i++)
	{
		int lID;
		CString lStr;
		lAddToTagsIDData.GetDataByIdx(i, lID, lStr);
		lMenu->AppendMenu(MF_BYPOSITION | MF_STRING, ID_ADD_TO_TAGS_ID_MENUROOT + lID, lStr);
	}
	if (lMenu->GetMenuItemCount() > 0)
	{
		for (int i = 0; i < inParentMenu->GetMenuItemCount(); i++)
		{
			UINT lID = inParentMenu->GetMenuItemID(i);
			if (lID == ID_ADD_TO_TAGS)
			{
				inParentMenu->RemoveMenu(i, MF_BYPOSITION);
				inParentMenu->InsertMenu(i, MF_BYPOSITION | MF_POPUP, reinterpret_cast<UINT_PTR>(lMenu->GetSafeHmenu()), inParentName);
				return lMenu;
			}
		}
	}
	else if (lMenu != NULL) delete lMenu;
	
	return NULL;
}
//*************************************************************************************************
CMenu* CLibraryDoc::FillRemoveFromTagsMenu(CMenu* inParentMenu, int inParentID, LPCTSTR inParentName, CIntArray* inFileIDs)
{
	CTTags lTags(m_pParamCDB->pDB);
	CMenu* lMenu;
	CIDData lRemoveFromTagsIDData;

	if (!lTags.GetAllTagsAssignedToFileIDs(inFileIDs, lRemoveFromTagsIDData))
		return NULL;

	lMenu = new CMenu;
	lMenu->CreateMenu();
	for (int i = 0; i < lRemoveFromTagsIDData.GetCount(); i++)
	{
		int lID;
		CString lStr;
		lRemoveFromTagsIDData.GetDataByIdx(i, lID, lStr);
		lMenu->AppendMenu(MF_BYPOSITION | MF_STRING, ID_REMOVE_FROM_TAGS_ID_MENUROOT + lID, lStr);
	}
	if (lMenu->GetMenuItemCount() > 0)
	{
		for (int i = 0; i < inParentMenu->GetMenuItemCount(); i++)
		{
			UINT lID = inParentMenu->GetMenuItemID(i);
			if (lID == ID_REMOVE_FROM_TAGS)
			{
				inParentMenu->RemoveMenu(i, MF_BYPOSITION);
				inParentMenu->InsertMenu(i, MF_BYPOSITION | MF_POPUP, reinterpret_cast<UINT_PTR>(lMenu->GetSafeHmenu()), inParentName);
				return lMenu;
			}
		}
	}
	else if (lMenu != NULL) delete lMenu;

	return NULL;
}
//*************************************************************************************************
void CLibraryDoc::FreeMenuRecursively(CMenu* inMenu)
{
	if (inMenu != NULL)
	{
		int lCount = inMenu->GetMenuItemCount();
		for (int i = 0; i < lCount; i++)
		{
			CMenu* lSubMenu = inMenu->GetSubMenu(0);
			if (lSubMenu != NULL)
				FreeMenuRecursively(lSubMenu);
			inMenu->RemoveMenu(0, MF_BYPOSITION);
		}
		delete inMenu;
	}
}

BOOL CLibraryDoc::CanConvertToPDF(CString inFilePath)
{
	CTFExtensions lTableExt(m_pParamCDB->pDB);
	try
	{
		CString lStr;
		lStr.Format(_T("select * from FExtensions where (MFilters_idMfilter = %u) OR (MFilters_idMfilter = %u) OR (MFilters_idMfilter = %u) OR (MFilters_idMfilter = %u)"), 
			FILTERTREE_MAJOR_DOC - FILTERTREE_MAJOR_BASE,
			FILTERTREE_MAJOR_PRES - FILTERTREE_MAJOR_BASE,
			FILTERTREE_MAJOR_SPREAD - FILTERTREE_MAJOR_BASE,
			FILTERTREE_MAJOR_IMAGES - FILTERTREE_MAJOR_BASE);
		lTableExt.mQ = m_db.mDB.execQuery(lStr);
		lTableExt.LoadCurrentRow();
	}
	catch(...)
	{
		return false;
	}

	CPath lPath(inFilePath);
	CString lFileExt = lPath.GetExtName();
	lFileExt.MakeLower();
	if (lFileExt == _T("pdf"))
		return FALSE;

	while (lTableExt.mDataIsValid)
	{
		if (lTableExt.mName == lFileExt)
			return TRUE;
		lTableExt.LoadNextRow();
	}

	return FALSE;
}
