#include "stdafx.h"
#include "Benubird.h"
#include "CTagsPanel.h"
#include "LibraryView.h"
#include "MainFrm.h"

#define MAX_CLASS_NAME	255
#define STATIC_CLASS	_T("Static")
#define BUTTON_CLASS	_T("Button")

//*****************************************************************************
// CTagsListCtrl
//*****************************************************************************
IMPLEMENT_DYNAMIC(CTagsListCtrl, CBCGPListCtrl)

CTagsListCtrl::CTagsListCtrl()
{
	m_iSortedColumn = 0;
	m_bAscending = TRUE;
}

CTagsListCtrl::~CTagsListCtrl()
{
}

BEGIN_MESSAGE_MAP(CTagsListCtrl, CBCGPListCtrl)
	ON_WM_KEYDOWN()
	ON_NOTIFY_REFLECT(LVN_GETDISPINFO, &CTagsListCtrl::OnGetdispinfo)
END_MESSAGE_MAP()

void CTagsListCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CBCGPListCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
	switch (nChar)
	{
		case VK_UP:
		case VK_DOWN:
		{
//			((CLibraryView*)(GetParent()->GetParent()))->RefreshQuickBrowser();
			CMDIChildWnd * pChild = ((CMDIFrameWnd*)(AfxGetApp()->m_pMainWnd))->MDIGetActive();
			CLibraryDoc* pDoc = DYNAMIC_DOWNCAST(CLibraryDoc, pChild->GetActiveDocument ());
			if (pDoc != NULL)
			{
				pDoc->UpdateLibrary ();
			}

			//?CMainFrame* lMainFrame;
			//lMainFrame = (CMainFrame*)AfxGetMainWnd();
			//lMainFrame->InitFindText();
			break;
		}
	}
}

void CTagsListCtrl::OnGetdispinfo(NMHDR *pNMHDR, LRESULT *pResult)
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	LV_ITEM* pItem= &(pDispInfo)->item;

	int lItemIndx = pItem->iItem;

	// Caching and Virtual List Controls
	if (lItemIndx >= mTagsArray.GetCount()) return;

	CString lTimeStr;
	if (pItem->mask & LVIF_TEXT) //valid text buffer?
	{
		switch(pItem->iSubItem){

			case 0: //fill in tag text
				lstrcpy(pItem->pszText, mTagsArray[lItemIndx].mName);
				break;
			
			case 1:{ //fill in sub item 1 text
				CString lStr;
				lStr.Format(_T("%d"), mTagsArray[lItemIndx].mCount);
				lstrcpyn(pItem->pszText, lStr.GetBuffer(pItem->cchTextMax), pItem->cchTextMax);
				}
				break;
		}
	}

	*pResult = 0;
}

void CTagsListCtrl::Sort(int iColumn, BOOL bAscending, BOOL bAdd)
{
	GetHeaderCtrl().SetSortColumn (iColumn, bAscending, bAdd);
	m_iSortedColumn = iColumn;
	m_bAscending = bAscending;
	
	// Redraw the list.
	((CTagsPanel*)GetParent())->Refresh();
}

//*****************************************************************************
// CTagsPanel
//*****************************************************************************
IMPLEMENT_SERIAL(CTagsPanel, CBBDialogBar, VERSIONABLE_SCHEMA | 1)

CTagsPanel::CTagsPanel(CWnd* pParent /*=NULL*/)
{

}

CTagsPanel::~CTagsPanel()
{
}

void CTagsPanel::DoDataExchange(CDataExchange* pDX)
{
	CBBDialogBar::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTagsPanel, CBBDialogBar)
	ON_WM_ERASEBKGND()
	ON_WM_GETMINMAXINFO()

	ON_BN_CLICKED(IDC_MATCH_ALL_RADIO, OnBnClickedMatchAllRadio)
	ON_BN_CLICKED(IDC_MATCH_ANY_RADIO, OnBnClickedMatchAnyRadio)
	ON_NOTIFY(NM_CLICK, IDC_TAGS_LIST, OnNMClickTagsList)
END_MESSAGE_MAP()

BOOL CTagsPanel::OnEraseBkgnd(CDC* pDC) 
{
	CRect lClientRect;
	GetClientRect(lClientRect);
	CBCGPVisualManager::GetInstance()->OnFillBarBackground(pDC, this, lClientRect, lClientRect);
	return TRUE;
}

LRESULT CTagsPanel::HandleInitDialog(WPARAM wParam, LPARAM lParam)
{
	if (!CBBDialogBar::HandleInitDialog (wParam, lParam))
	{
		return FALSE;
	}

	OnInitDialog ();

	return TRUE;
}

BOOL CTagsPanel::OnInitDialog ()
{
	mTagsListBox.SubclassDlgItem(IDC_TAGS_LIST, this);
	mTagsListBox.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	mMatchAnyRadio.SubclassDlgItem(IDC_MATCH_ANY_RADIO, this);
	mMatchAllRadio.SubclassDlgItem(IDC_MATCH_ALL_RADIO, this);
	mSelectedTagsStatic.SubclassDlgItem(IDC_SELECTED_TAGS_ST, this);

	CRect lWindowRect;
	mTagsListBox.GetWindowRect(lWindowRect);
	ScreenToClient(lWindowRect);

	mTagsListBox.InsertColumn(0, _T("Tag"), LVCFMT_LEFT, ((lWindowRect.Width() / 3 ) * 2) - 1);
	mTagsListBox.InsertColumn(1, _T("#"), LVCFMT_LEFT, (lWindowRect.Width() / 3) - 1);

	mMatchAnyRadio.SetCheck(TRUE);

	Refresh();
	return TRUE;
}

void CTagsPanel::Resize (BOOL bFull)
{
	CRect rectClient;
	GetClientRect (rectClient);
	
	CRect rectWindow;
	CPoint pt;
	
	CSize lMatchAnyRadioSize;
	mMatchAnyRadio.GetWindowRect(rectWindow);
	ScreenToClient(rectWindow);
	lMatchAnyRadioSize = rectWindow.Size();
	pt.x = HORIZ_MARGIN;
	pt.y = rectClient.bottom - 2 * VERT_MARGIN;
	mMatchAnyRadio.SetWindowPos(NULL, pt.x, pt.y, lMatchAnyRadioSize.cx, lMatchAnyRadioSize.cy, SWP_NOZORDER | SWP_NOACTIVATE);

	CSize lMatchAllRadioSize;
	mMatchAllRadio.GetWindowRect(rectWindow);
	ScreenToClient(rectWindow);
	lMatchAllRadioSize = rectWindow.Size();
	pt.x = HORIZ_MARGIN;
	pt.y -= 2 * VERT_MARGIN;
	mMatchAllRadio.SetWindowPos(NULL, pt.x, pt.y, lMatchAllRadioSize.cx, lMatchAllRadioSize.cy, SWP_NOZORDER | SWP_NOACTIVATE);

	/*CSize lSelectedTagsStaticSize;
	mSelectedTagsStatic.GetWindowRect(rectWindow);
	ScreenToClient(rectWindow);
	lSelectedTagsStaticSize = rectWindow.Size();
	lSelectedTagsStaticSize.cx = rectClient.Width() - (2 * HORIZ_MARGIN); 
	pt.x = HORIZ_MARGIN;
	pt.y = pt.y - 6 * VERT_MARGIN;
	mSelectedTagsStatic.SetWindowPos(NULL, pt.x, pt.y, lSelectedTagsStaticSize.cx, lSelectedTagsStaticSize.cy, SWP_NOZORDER | SWP_NOACTIVATE);*/

	CSize lTagsListBoxSize;
	mTagsListBox.GetWindowRect(rectWindow);
	ScreenToClient(rectWindow);
	lTagsListBoxSize.cx = rectClient.Width() - (2 * HORIZ_MARGIN);
	lTagsListBoxSize.cy = pt.y - (rectClient.top) - 15;
	pt.x = HORIZ_MARGIN;
	pt.y = rectClient.top;
	mTagsListBox.SetWindowPos(NULL, pt.x, pt.y, lTagsListBoxSize.cx, lTagsListBoxSize.cy, SWP_NOZORDER | SWP_NOACTIVATE);

	int col1, col2;
	col1 = 2 * lTagsListBoxSize.cx / 3;
	col2 = lTagsListBoxSize.cx - col1 - 4;

	mTagsListBox.SetColumnWidth(0, col1);
	mTagsListBox.SetColumnWidth(1, col2);
}

void CTagsPanel::Refresh()
{
	CMDIChildWnd * pChild = ((CMDIFrameWnd*)(AfxGetApp()->m_pMainWnd))->MDIGetActive();
	if(pChild == NULL)
		return;
	CLibraryView* pView = DYNAMIC_DOWNCAST(CLibraryView, pChild->GetActiveView ());
	CLibraryDoc* pDoc = DYNAMIC_DOWNCAST(CLibraryDoc, pChild->GetActiveDocument ());

	int lTreeFilterID = 0;
	if (pView != NULL)
	{
		lTreeFilterID = pView->GetSelectedCollectionGuiID();
	}

	// Save selection
	/*
	CList <UINT, UINT> lSelectedItems;
	POSITION lPos = mTagsListBox.GetFirstSelectedItemPosition();
	int lIndex;
	while (lPos)
	{
		lIndex = mTagsListBox.GetNextSelectedItem(lPos);
		CTTags_Entry lEntry = mTagsListBox.mTagsArray.GetAt(lIndex);
		lSelectedItems.AddTail(lEntry.mID);
	}
	*/
	// clear all
//	mTagsListBox.DeleteAllItems();
//	mTagsListBox.SetItemCountEx(0);
	mTagsListBox.mTagsArray.RemoveAll();

	// Prepare SQL
	CDBSQL lDBSQL;
	lDBSQL.Clear();
	lDBSQL.AddField(_T("Tags.idTag"));
	lDBSQL.AddField(_T("Tags.Name"));
	lDBSQL.AddField(_T("count(*) as total"));
	lDBSQL.AddField(TagField);

	lDBSQL.AddTable(_T("Tags"));
	lDBSQL.AddTable(_T("Files_has_Tags"));
	lDBSQL.AddTable(_T("Files"));

	//CUtils::UpdateSQLFromCollectionPanel(&lDBSQL);
	//CUtils::UpdateSQLFromBrowserPanel(&lDBSQL);

	CString lCondition = _T("Files_has_Tags.Tags_idTag = Tags.idTag");
	lDBSQL.AddCondition(lCondition);
	lCondition = _T("Files_has_Tags.Files_idFile = Files.idFile");
	lDBSQL.AddCondition(lCondition);

	// group by
	lDBSQL.AddGroup(_T("Tags.idTag"));

	// order by
	CString lSortStr = _T("Tags.Name");
	if (mTagsListBox.GetSortedColumn() == 1)
		lSortStr = _T("total");
	if (mTagsListBox.IsAscendingSort())
		lSortStr += _T(" asc");
	else
		lSortStr += _T(" desc");
	lDBSQL.AddSort(lSortStr);

	// musi to byt pridane tu, lebo limit v selekte moze byt iba na konci!
	CString strBrowserCondition;
	if (pView != NULL)
	{
		pView->GetBrowsePanel()->GetStringForSelect(strBrowserCondition);
	}
	
	pDoc->UpdateSQLFromCollection(lTreeFilterID, &lDBSQL);
	pDoc->UpdateSQLFromBrowser(strBrowserCondition, &lDBSQL);

	CString lSQLStr = lDBSQL.GetSelect();

	/* Lucia comment:
	Tento iny pristup ku selektu zabezpeci, ze sa ukazu vsetky tagy pre v collection vybratu skupinu.
	Napr. ak mam Smart Collection, ktora ma podmienku: Tags contains "te", v gride budu subory s tagmi napr:
	1) test;bold
	2) test
	3) meter; house
	4) peter; house
	V teraz pouzitom selekte sa ukazu v okne tagov iba: 
		test
		meter
		peter
	V tomto druhom navyse aj tagy:
		bold
		house
	Zatial je spravny teraz pouzity selekt, pretoze tagy sa maju vybrat na zaklade podmienok
	v collection panely a v navigatore. Nechavam to len ako priklad selektu zo selektu,
	ak by bolo niekedy treba napr. tagy selektovat na zaklade vybranych suborov.

	CDBSQL lDBSQL1;
	lDBSQL1.AddField(_T("Files.idFile"));
	lDBSQL1.AddTable(_T("Files"));
	CUtils::UpdateSQLFromCollectionPanel(lTreeFilterID, &lDBSQL1);
	CUtils::UpdateSQLFromBrowserPanel(&lDBSQL1);
	//CUtils::UpdateSQLFromTagPanel(&gFilesSQL);
	//CUtils::UpdateSQLFromFind(&lDBSQL1);
	CString lGSQLStr = lDBSQL1.GetSelect();
	lGSQLStr.Delete(lGSQLStr.GetLength()-2,2);
	CString lSQLStr;
	lSQLStr.Format(_T("select Tags.idTag, Tags.Name, count(*) as total from Tags, Files_has_Tags, Files where (Files_has_Tags.Tags_idTag = Tags.idTag) AND (Files_has_Tags.Files_idFile = Files.idFile) AND (Files.idFile IN (%s)) group by Tags.idTag;"), lGSQLStr);
	*/

	if (!lSQLStr.IsEmpty()) 
	{
		CString lStr;
		CTTags lTTags(m_pParamCDB->pDB);
		lTTags.mQ = m_pDB->mDB.execQuery(lSQLStr);
		int lIndex = 0;
		while(!lTTags.mQ.eof())
		{
			CTTags_Entry lEntry;
			lEntry.mID = lTTags.mQ.getIntField(0);
			lEntry.mName = lTTags.mQ.getStringField(1);
			lEntry.mCount = lTTags.mQ.getIntField(2);
			mTagsListBox.mTagsArray.Add(lEntry);
			lTTags.LoadNextRow();
		}
	}

	mTagsListBox.SetItemCountEx((int)mTagsListBox.mTagsArray.GetCount());
	/*
	// Restore selection
	lPos = lSelectedItems.GetHeadPosition();
	while (lPos)
	{
		UINT lID = lSelectedItems.GetAt(lPos);
		for (int i = 0; i < mTagsListBox.GetItemCount(); i++)
		{
			CTTags_Entry lEntry = mTagsListBox.mTagsArray.GetAt(i);
			if (lID == lEntry.mID)
			{
				mTagsListBox.SetItemState(i, LVIS_SELECTED, LVIS_SELECTED);
			}
		}
		lSelectedItems.GetNext(lPos);
	}
	*/
	mTagsListBox.Invalidate();
}

void CTagsPanel::GetStringForSelect(CString& outStr)
{
	outStr.Empty();
	if (!IsBarVisible())
		return;

	CString lStr;
	CString lConjunctionStr;

	if (IsDlgButtonChecked(IDC_MATCH_ALL_RADIO))
		lConjunctionStr = _T(" and ");
	else if (IsDlgButtonChecked(IDC_MATCH_ANY_RADIO))
		lConjunctionStr = _T(" or ");
	else
		return;

	POSITION lPos = mTagsListBox.GetFirstSelectedItemPosition();
	if (lPos)
		outStr += _T("(");
	while(lPos)
	{
		int lSelItemIdx = mTagsListBox.GetNextSelectedItem(lPos);
		int lTagID = mTagsListBox.mTagsArray[lSelItemIdx].mID;
		if (IsDlgButtonChecked(IDC_MATCH_ALL_RADIO))
			lStr.Format(_T(" exists (select Files_has_Tags.Files_idFile from Files_has_Tags where Files_has_Tags.Files_idFile = Files.idFile and Files_has_Tags.Tags_idTag = %d)"), lTagID);
		else if (IsDlgButtonChecked(IDC_MATCH_ANY_RADIO))
			lStr.Format(_T("(Files_has_Tags.Files_idFile = Files.idFile and Files_has_Tags.Tags_idTag = %d)"), lTagID);
		outStr += lStr;
		if (lPos)
			outStr += lConjunctionStr;
	}
	if (!outStr.IsEmpty())
		outStr += _T(")");
}

void CTagsPanel::OnBnClickedMatchAllRadio()
{
//?	lMainFrame->InitFindText();
    CMDIChildWnd * pChild = ((CMDIFrameWnd*)(AfxGetApp()->m_pMainWnd))->MDIGetActive();
	CLibraryDoc* pDoc = DYNAMIC_DOWNCAST(CLibraryDoc, pChild->GetActiveDocument ());
	if (pDoc != NULL)
	{
		pDoc->UpdateLibrary ();
	}
}

void CTagsPanel::OnBnClickedMatchAnyRadio()
{
//?	lMainFrame->InitFindText();
    CMDIChildWnd * pChild = ((CMDIFrameWnd*)(AfxGetApp()->m_pMainWnd))->MDIGetActive();
	CLibraryDoc* pDoc = DYNAMIC_DOWNCAST(CLibraryDoc, pChild->GetActiveDocument ());
	if (pDoc != NULL)
	{
		pDoc->UpdateLibrary ();
	}
}

void CTagsPanel::OnNMClickTagsList(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
//?	lMainFrame->InitFindText();
    CMDIChildWnd * pChild = ((CMDIFrameWnd*)(AfxGetApp()->m_pMainWnd))->MDIGetActive();
	CLibraryDoc* pDoc = DYNAMIC_DOWNCAST(CLibraryDoc, pChild->GetActiveDocument ());
	if (pDoc != NULL)
	{
		pDoc->UpdateLibrary ();
	}
	*pResult = 0;
}

void CTagsPanel::ResetSelection()
{
	for (int i = 0; i < mTagsListBox.GetItemCount(); i++)
	{
		mTagsListBox.SetItemState(i, 0, LVIS_SELECTED);
	}
}

void CTagsPanel::Set_ParamForCDB(PARAM_FOR_CDB *pParamCDB)
{
	m_pParamCDB = pParamCDB;
	m_pDB = m_pParamCDB->pDB;
}