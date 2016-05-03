#include "stdafx.h"
#include "Benubird.h"
#include "CBrowserPanel.h"
#include "MainFrm.h"
#include "LibraryView.h"
#include "CDB.h"


#define MAX_CLASS_NAME	255
#define STATIC_CLASS	_T("Static")
#define BUTTON_CLASS	_T("Button")

#define ID_AUTHOR_COMBO_ENTRY			13000
#define ID_CATEGORY_COMBO_ENTRY			13001
#define ID_COLLECTIONS_COMBO_ENTRY		13002	
#define ID_DAY_CREATED_COMBO_ENTRY		13003
#define ID_DAY_MODIFIED_COMBO_ENTRY		13004
#define ID_FILE_TYPE_COMBO_ENTRY		13005
#define ID_MONTH_CREATED_COMBO_ENTRY	13006
#define ID_MONTH_MODIFIED_COMBO_ENTRY	13007
#define ID_SUBJECT_COMBO_ENTRY			13008
#define ID_TAGS_COMBO_ENTRY				13009
#define ID_TITLE_COMBO_ENTRY			13010
#define ID_YEAR_CREATED_COMBO_ENTRY		13011
#define ID_YEAR_MODIFIED_COMBO_ENTRY	13012

//*****************************************************************************
// CBBListCtrl
//*****************************************************************************
CBBListCtrl::CBBListCtrl()
{
}

CBBListCtrl::~CBBListCtrl()
{
}

// Message map
BEGIN_MESSAGE_MAP(CBBListCtrl, CBCGPListCtrl)
//{{AFX_MSG_MAP(CBBListCtrl)
	ON_NOTIFY_REFLECT(LVN_GETDISPINFO, &CBBListCtrl::OnGetdispinfo)

//}}AFX_MSG_MAP
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

void CBBListCtrl::SetSel(int inPos, BOOL inState)
{
	UINT lState;

	if (inState)
		lState = LVIS_SELECTED;
	else
		lState = 0;

	SetItemState(inPos, lState, LVIS_SELECTED);

}

int CBBListCtrl::GetSel(int inPos)
{
	return GetItemState(inPos, LVIS_SELECTED);
}

BOOL CBBListCtrl::OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	NMLVCACHEHINT* pcachehint=NULL;

	if (message == WM_NOTIFY)
    {
        NMHDR* phdr = (NMHDR*)lParam;

        switch(phdr->code)
        {
        case LVN_ODCACHEHINT:
            break;
        default:
            return CBCGPListCtrl::OnChildNotify(message, wParam, lParam, pResult);
        }
        return FALSE;
    }
    else
        return CBCGPListCtrl::OnChildNotify(message, wParam, lParam, pResult);
}

void CBBListCtrl::OnGetdispinfo(NMHDR *pNMHDR, LRESULT *pResult)
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	LV_ITEM* pItem= &(pDispInfo)->item;

	int lItemIndex = pItem->iItem;
	// Caching and Virtual List Controls
	if (lItemIndex >= mStringList.GetCount()) return;

	if (pItem->mask & LVIF_TEXT) //valid text buffer?
	{
		POSITION lPos = mStringList.FindIndex(lItemIndex);
		CString lStr = mStringList.GetAt(lPos);
		lstrcpyn(pItem->pszText, lStr.GetBuffer(pItem->cchTextMax), pItem->cchTextMax);
	}

	*pResult = 0;
}

BOOL CBBListCtrl::IsVertScrollBarVisible()
{
	int lCount = GetItemCount();
	CRect lRect;
	GetClientRect(&lRect);
	lRect.bottom++;

	for (int i = 0; i < lCount; i++)
	{
		CRect lItemRect;
		GetItemRect(i, &lItemRect, LVIR_LABEL);
		if ((!lRect.PtInRect(lItemRect.TopLeft())) || (!lRect.PtInRect(lItemRect.BottomRight())))
			return TRUE;
	}
	return FALSE;
}

int CBBListCtrl::GetMaxTextSize()
{
	int lMax = 0;
	for (int i = 0; i < GetItemCount(); i++)
	{
		CRect lRect;
		CString lStr = GetItemText(i, 0);
		int lWidth = GetStringWidth(lStr);
		if (lMax < lWidth)
			lMax = lWidth;
	}
	return lMax + 8;
}

//*****************************************************************************
// CBrowserPanel
//*****************************************************************************
IMPLEMENT_SERIAL(CBrowserPanel, CBBDialogBar, VERSIONABLE_SCHEMA | 1)



CBrowserPanel::CBrowserPanel(CWnd* pParent /*=NULL*/)
{
}

CBrowserPanel::~CBrowserPanel()
{
}

void CBrowserPanel::Set_ParamForCDB(PARAM_FOR_CDB *pParamCDB)
{
	m_pParamCDB = pParamCDB;
	m_pDB = pParamCDB->pDB;
}

void CBrowserPanel::DoDataExchange(CDataExchange* pDX)
{
	CBBDialogBar::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CBrowserPanel, CBBDialogBar)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_CBN_SELCHANGE(IDC_BROWSER_PANEL1_COMBO, OnCbnSelchangeBrowserPanel1Combo)
	ON_CBN_SELCHANGE(IDC_BROWSER_PANEL2_COMBO, OnCbnSelchangeBrowserPanel2Combo)
	ON_CBN_SELCHANGE(IDC_BROWSER_PANEL3_COMBO, OnCbnSelchangeBrowserPanel3Combo)
	ON_CBN_SELCHANGE(IDC_BROWSER_PANEL4_COMBO, OnCbnSelchangeBrowserPanel4Combo)
	ON_NOTIFY(NM_CLICK, IDC_BROWSER_PANEL1_LIST, OnLbnSelchangeBrowserPanel1List)
	ON_NOTIFY(NM_CLICK, IDC_BROWSER_PANEL2_LIST, OnLbnSelchangeBrowserPanel2List)
	ON_NOTIFY(NM_CLICK, IDC_BROWSER_PANEL3_LIST, OnLbnSelchangeBrowserPanel3List)
	ON_NOTIFY(NM_CLICK, IDC_BROWSER_PANEL4_LIST, OnLbnSelchangeBrowserPanel4List)
	ON_CBN_DROPDOWN(IDC_BROWSER_PANEL1_COMBO, OnCbnDropdownBrowserPanel1Combo)
	ON_CBN_DROPDOWN(IDC_BROWSER_PANEL2_COMBO, OnCbnDropdownBrowserPanel2Combo)
	ON_CBN_DROPDOWN(IDC_BROWSER_PANEL3_COMBO, OnCbnDropdownBrowserPanel3Combo)
	ON_CBN_DROPDOWN(IDC_BROWSER_PANEL4_COMBO, OnCbnDropdownBrowserPanel4Combo)
END_MESSAGE_MAP()

BOOL CBrowserPanel::OnEraseBkgnd(CDC* pDC) 
{
	CRect lClientRect;
	GetClientRect(lClientRect);
	CBCGPVisualManager::GetInstance()->OnFillBarBackground(pDC, this, lClientRect, lClientRect);
	return TRUE;
}

HBRUSH CBrowserPanel::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	if (nCtlColor == CTLCOLOR_STATIC || nCtlColor == CTLCOLOR_BTN)
	{
		TCHAR lClassName [MAX_CLASS_NAME + 1];
		::GetClassName(pWnd->GetSafeHwnd (), lClassName, MAX_CLASS_NAME);
		CString strClass = lClassName;
		if (strClass == STATIC_CLASS || strClass == BUTTON_CLASS)
		{
			pDC->SetBkMode(TRANSPARENT);
			return (HBRUSH) ::GetStockObject (HOLLOW_BRUSH);
		}
	}
	return CBBDialogBar::OnCtlColor(pDC, pWnd, nCtlColor);
}

LRESULT CBrowserPanel::HandleInitDialog(WPARAM wParam, LPARAM lParam)
{
	if (!CBBDialogBar::HandleInitDialog (wParam, lParam))
	{
		return FALSE;
	}

	OnInitDialog ();

	return TRUE;
}

BOOL CBrowserPanel::OnInitDialog ()
{
	mPanelCombo[0].SubclassDlgItem(IDC_BROWSER_PANEL1_COMBO, this);
	mPanelCombo[1].SubclassDlgItem(IDC_BROWSER_PANEL2_COMBO, this);
	mPanelCombo[2].SubclassDlgItem(IDC_BROWSER_PANEL3_COMBO, this);
	mPanelCombo[3].SubclassDlgItem(IDC_BROWSER_PANEL4_COMBO, this);

	mPanelListBox[0].SubclassDlgItem(IDC_BROWSER_PANEL1_LIST, this);
	mPanelListBox[1].SubclassDlgItem(IDC_BROWSER_PANEL2_LIST, this);
	mPanelListBox[2].SubclassDlgItem(IDC_BROWSER_PANEL3_LIST, this);
	mPanelListBox[3].SubclassDlgItem(IDC_BROWSER_PANEL4_LIST, this);

	for (int i = 0; i < NUM_PANELS; i++)
	{
		CString lStr;
		lStr.Format(_T("Column%d"), i);
		mPanelListBox[i].InsertColumn(0, lStr);
		mPanelListBox[i].SetExtendedStyle(LVS_EX_FULLROWSELECT/* | LVS_EX_GRIDLINES*/);
	}
	
	LoadImages();

	int  lPos;
	UINT lID;

	FillComboBox(&mPanelCombo[0]);
	lPos = mPanelCombo[0].FindStringExact(0, _T("Category"));
	mPanelCombo[0].SetCurSel(lPos);
	lID = (UINT)mPanelCombo[0].GetItemData(lPos);
	GetDatas(0, lID);

	FillComboBox(&mPanelCombo[1]);
	lPos = mPanelCombo[1].FindStringExact(0, _T("Subject"));
	mPanelCombo[1].SetCurSel(lPos);
	lID = (UINT)mPanelCombo[1].GetItemData(lPos);
	GetDatas(1, lID);

	FillComboBox(&mPanelCombo[2]);
	lPos = mPanelCombo[2].FindStringExact(0, _T("Author"));
	mPanelCombo[2].SetCurSel(lPos);
	lID = (UINT)mPanelCombo[2].GetItemData(lPos);
	GetDatas(2, lID);
	
	FillComboBox(&mPanelCombo[3]);
	lPos = mPanelCombo[3].FindStringExact(0, _T("File Type"));
	mPanelCombo[3].SetCurSel(lPos);
	lID = (UINT)mPanelCombo[3].GetItemData(lPos);
	GetDatas(3, lID);
	
	ResetSelection();
	mMonths[0].Name = _T("January");
	mMonths[1].Name = _T("February");
	mMonths[2].Name = _T("March");
	mMonths[3].Name = _T("April");
	mMonths[4].Name = _T("May");
	mMonths[5].Name = _T("June");
	mMonths[6].Name = _T("July");
	mMonths[7].Name = _T("August");
	mMonths[8].Name = _T("September");
	mMonths[9].Name = _T("October");
	mMonths[10].Name = _T("November");
	mMonths[11].Name = _T("December");

	//mPanelListBox[0]
	//mPanelListBox[1].ShowScrollBar(SB_HORZ);
	//mPanelListBox[2].ShowScrollBar(SB_HORZ);
	//mPanelListBox[3].ShowScrollBar(SB_HORZ);

	return TRUE;
}

void CBrowserPanel::Resize (BOOL bFull)
{
	CRect rectClient;
	GetClientRect (rectClient);

	int lPanelWidth = rectClient.Width() / 4;
	int lRemains = rectClient.Width() % 4;

	CRect rectWindow;
	CPoint pt;

	pt.x = rectClient.left;
	pt.y = rectClient.top;
	CSize lPanel1ComboSize;
	mPanelCombo[0].GetWindowRect(rectWindow);
	ScreenToClient(rectWindow);
	lPanel1ComboSize = rectWindow.Size();
	lPanel1ComboSize.cx = lPanelWidth;
	if (lRemains > 0)
	{
		lRemains--;
		lPanel1ComboSize.cx++;
	}
	mPanelCombo[0].SetWindowPos(NULL, pt.x, pt.y, lPanel1ComboSize.cx, lPanel1ComboSize.cy, SWP_NOZORDER | SWP_NOACTIVATE);
	
	pt.x = pt.x + lPanel1ComboSize.cx;
	pt.y = rectClient.top;
	CSize lPanel2ComboSize;
	mPanelCombo[1].GetWindowRect(rectWindow);
	ScreenToClient(rectWindow);
	lPanel2ComboSize = rectWindow.Size();
	lPanel2ComboSize.cx = lPanelWidth;
	if (lRemains > 0)
	{
		lRemains--;
		lPanel2ComboSize.cx++;
	}
	mPanelCombo[1].SetWindowPos(NULL, pt.x, pt.y, lPanel2ComboSize.cx, lPanel2ComboSize.cy, SWP_NOZORDER | SWP_NOACTIVATE);

	pt.x = pt.x + lPanel2ComboSize.cx;
	pt.y = rectClient.top;
	CSize lPanel3ComboSize;
	mPanelCombo[2].GetWindowRect(rectWindow);
	ScreenToClient(rectWindow);
	lPanel3ComboSize = rectWindow.Size();
	lPanel3ComboSize.cx = lPanelWidth;
	if (lRemains > 0)
	{
		lRemains--;
		lPanel3ComboSize.cx++;
	}
	mPanelCombo[2].SetWindowPos(NULL, pt.x, pt.y, lPanel3ComboSize.cx, lPanel3ComboSize.cy, SWP_NOZORDER | SWP_NOACTIVATE);

	pt.x = pt.x + lPanel3ComboSize.cx;
	pt.y = rectClient.top;
	CSize lPanel4ComboSize;
	mPanelCombo[3].GetWindowRect(rectWindow);
	ScreenToClient(rectWindow);
	lPanel4ComboSize = rectWindow.Size();
	lPanel4ComboSize.cx = lPanelWidth;
	if (lRemains > 0)
	{
		lRemains--;
		lPanel4ComboSize.cx++;
	}
	mPanelCombo[3].SetWindowPos(NULL, pt.x, pt.y, lPanel4ComboSize.cx, lPanel4ComboSize.cy, SWP_NOZORDER | SWP_NOACTIVATE);

	int lDelta;
	int lVal1 = 25;
	int lVal2 = 8;
	int lColumnWidth;

		
	CSize lPanel1ListBoxSize;
	lPanel1ListBoxSize.cx = lPanel1ComboSize.cx;
	lPanel1ListBoxSize.cy = rectClient.Height() - lPanel1ComboSize.cy;
	pt.x = rectClient.left;
	pt.y = rectClient.top + lPanel1ComboSize.cy;
	mPanelListBox[0].SetWindowPos(NULL, pt.x, pt.y, lPanel1ListBoxSize.cx, lPanel1ListBoxSize.cy, SWP_NOZORDER | SWP_NOACTIVATE);
	pt.x = pt.x + lPanel1ListBoxSize.cx;
	lColumnWidth = mPanelListBox[0].GetMaxTextSize();
	mPanelListBox[0].IsVertScrollBarVisible() ? lDelta = lVal1 : lDelta = lVal2;
	mPanelListBox[0].SetColumnWidth(0, lColumnWidth < lPanel1ListBoxSize.cx - lDelta ? lPanel1ListBoxSize.cx - lDelta: lColumnWidth);
	
	CSize lPanel2ListBoxSize;
	lPanel2ListBoxSize.cx = lPanel2ComboSize.cx;
	lPanel2ListBoxSize.cy = rectClient.Height() - lPanel2ComboSize.cy;
	mPanelListBox[1].SetWindowPos(NULL, pt.x, pt.y, lPanel2ListBoxSize.cx, lPanel2ListBoxSize.cy, SWP_NOZORDER | SWP_NOACTIVATE);
	pt.x = pt.x + lPanel2ListBoxSize.cx;
	lColumnWidth = mPanelListBox[1].GetMaxTextSize();
	mPanelListBox[1].IsVertScrollBarVisible() ? lDelta = lVal1 : lDelta = lVal2;
	mPanelListBox[1].SetColumnWidth(0, lColumnWidth < lPanel2ListBoxSize.cx - lDelta ? lPanel2ListBoxSize.cx - lDelta: lColumnWidth);
	
	CSize lPanel3ListBoxSize;
	lPanel3ListBoxSize.cx = lPanel3ComboSize.cx;
	lPanel3ListBoxSize.cy = rectClient.Height() - lPanel3ComboSize.cy;
	mPanelListBox[2].SetWindowPos(NULL, pt.x, pt.y, lPanel3ListBoxSize.cx, lPanel3ListBoxSize.cy, SWP_NOZORDER | SWP_NOACTIVATE);
	pt.x = pt.x + lPanel3ListBoxSize.cx;
	lColumnWidth = mPanelListBox[2].GetMaxTextSize();
	mPanelListBox[2].IsVertScrollBarVisible() ? lDelta = lVal1 : lDelta = lVal2;
	mPanelListBox[2].SetColumnWidth(0, lColumnWidth < lPanel3ListBoxSize.cx - lDelta ? lPanel3ListBoxSize.cx - lDelta: lColumnWidth);
	
	CSize lPanel4ListBoxSize;
	lPanel4ListBoxSize.cx = lPanel4ComboSize.cx;
	lPanel4ListBoxSize.cy = rectClient.Height() - lPanel4ComboSize.cy;
	mPanelListBox[3].SetWindowPos(NULL, pt.x, pt.y, lPanel4ListBoxSize.cx, lPanel4ListBoxSize.cy, SWP_NOZORDER | SWP_NOACTIVATE);
	pt.x = pt.x + lPanel4ListBoxSize.cx;
	lColumnWidth = mPanelListBox[3].GetMaxTextSize();
	mPanelListBox[3].IsVertScrollBarVisible() ? lDelta = lVal1 : lDelta = lVal2;
	mPanelListBox[3].SetColumnWidth(0, lColumnWidth < lPanel4ListBoxSize.cx - lDelta ? lPanel4ListBoxSize.cx - lDelta: lColumnWidth);
}

void CBrowserPanel::OnCbnSelchangeBrowserPanel1Combo()
{
	ComboBoxChanged(0);
}

void CBrowserPanel::OnCbnSelchangeBrowserPanel2Combo()
{
	ComboBoxChanged(1);
}

void CBrowserPanel::OnCbnSelchangeBrowserPanel3Combo()
{
	ComboBoxChanged(2);
}

void CBrowserPanel::OnCbnSelchangeBrowserPanel4Combo()
{
	ComboBoxChanged(3);
}

/*
Fill each list box in the navigator according to the following rules (in order):
1. Selection in the Library/Collection
2. Tags
3. Selection(s) in the list boxes 1 to 4.
*/
void CBrowserPanel::GetDatas(int index, UINT inID)
{
	CBBListCtrl* inPanelListBox = &mPanelListBox[index];

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

	CDBSQL lDBSQL;
	lDBSQL.AddTable(_T("Files"));

	pDoc->UpdateSQLFromCollection(lTreeFilterID, &lDBSQL);

	// update lSQL from previous panel lists
	if ((index == 1) || (index == 2) || (index == 3))
	{
		CString lCondition; 
		GetStringFromPanel(0, lCondition);
		if (!lCondition.IsEmpty())
			lDBSQL.AddCondition(lCondition);
	}
	if ((index == 2) || (index == 3))
	{
		CString lCondition; 
		GetStringFromPanel(1, lCondition);
		if (!lCondition.IsEmpty())
			lDBSQL.AddCondition(lCondition);
	}
	if (index == 3)
	{
		CString lCondition; 
		GetStringFromPanel(2, lCondition);
		if (!lCondition.IsEmpty())
			lDBSQL.AddCondition(lCondition);
	}

	// prepare lSQL
	switch (inID)
	{
		case ID_AUTHOR_COMBO_ENTRY:
			lDBSQL.AddField(_T("distinct Files.Author"));
			lDBSQL.AddSort(_T("Files.Author asc"));
			lDBSQL.AddGroup(_T("Files.Author"));
			break;
		case ID_CATEGORY_COMBO_ENTRY:
			lDBSQL.AddField(_T("distinct Files.Category"));
			lDBSQL.AddSort(_T("Files.Category asc"));
			lDBSQL.AddGroup(_T("Files.Category"));
			break;	
		case ID_COLLECTIONS_COMBO_ENTRY:
			// do nothing
			lDBSQL.Clear();
			break;
		case ID_DAY_CREATED_COMBO_ENTRY:
			lDBSQL.AddField(_T("distinct strftime('%d', Files.Created_TmStmp)"));
			lDBSQL.AddGroup(_T("strftime('%d', Files.Created_TmStmp)"));
			break;
		case ID_DAY_MODIFIED_COMBO_ENTRY:
			lDBSQL.AddField(_T("distinct strftime('%d', Files.Modified_TmStmp)"));
			lDBSQL.AddGroup(_T("strftime('%d', Files.Modified_TmStmp)"));
			break;
		case ID_FILE_TYPE_COMBO_ENTRY:
			lDBSQL.AddField(_T("distinct FExtensions.Name"));
			lDBSQL.AddTable(_T("FExtensions"));
			lDBSQL.AddCondition(_T("(Files.FExtensions_idFExtension = FExtensions.idFExtension)"));
			lDBSQL.AddGroup(_T("FExtensions.Name"));
			break;
		case ID_MONTH_CREATED_COMBO_ENTRY:
			lDBSQL.AddField(_T("distinct strftime('%m', Files.Created_TmStmp)"));
			lDBSQL.AddGroup(_T("strftime('%m', Files.Created_TmStmp)"));
			break;
		case ID_MONTH_MODIFIED_COMBO_ENTRY:
			lDBSQL.AddField(_T("distinct strftime('%m', Files.Modified_TmStmp)"));
			lDBSQL.AddGroup(_T("strftime('%m', Files.Modified_TmStmp)"));
			break;
		case ID_SUBJECT_COMBO_ENTRY:
			lDBSQL.AddField(_T("distinct Files.Subject"));
			lDBSQL.AddSort(_T("Files.Subject asc"));
			lDBSQL.AddGroup(_T("Files.Subject"));
			break;					
		case ID_TAGS_COMBO_ENTRY:
			// do nothing
			lDBSQL.Clear();
			break;
		case ID_TITLE_COMBO_ENTRY:
			lDBSQL.AddField(_T("distinct Files.Title"));
			lDBSQL.AddGroup(_T("Files.Title"));
			break;
		case ID_YEAR_CREATED_COMBO_ENTRY:
			lDBSQL.AddField(_T("distinct strftime('%Y', Files.Created_TmStmp)"));
			lDBSQL.AddGroup(_T("strftime('%Y', Files.Created_TmStmp)"));
			break;
		case ID_YEAR_MODIFIED_COMBO_ENTRY:
			lDBSQL.AddField(_T("distinct strftime('%Y', Files.Modified_TmStmp)"));
			lDBSQL.AddGroup(_T("strftime('%Y', Files.Modified_TmStmp)"));
			break;
	}

	lDBSQL.AddField(TagField); // should be added in here because distinct must be first

	CString lSQLStr = lDBSQL.GetSelect();
	if (lSQLStr.IsEmpty()) 
		return;
	
	CTFiles lTFiles(m_pParamCDB);
	lTFiles.mQ = m_pDB->mDB.execQuery(lSQLStr);
	
//	inPanelListBox->DeleteAllItems();
	inPanelListBox->mStringList.RemoveAll();
	
	int lPos = 0;
	inPanelListBox->mStringList.AddTail(_T("- All -"));
		
	lPos++;
	for (int i = 0; i < 12; i++) mMonths[i].ID = 0;

	while(!lTFiles.mQ.eof())
	{
		CString lStr = lTFiles.mQ.getStringField(0);
		if (!lStr.IsEmpty())
			if (inID == ID_MONTH_CREATED_COMBO_ENTRY || inID == ID_MONTH_MODIFIED_COMBO_ENTRY)
			{
				int lMonth;
				lMonth = _ttoi(lStr);
				if (lMonth >= 1 && lMonth <= 12)
					mMonths[lMonth - 1].ID++;
			}
			else
			{
				//LVFINDINFO lFindInfo;
				//lFindInfo.flags = LVFI_STRING;
				//lFindInfo.psz = lStr;
				//if (inPanelListBox->FindItem(lFindInfo) == -1)
				inPanelListBox->mStringList.AddTail(lStr);
			}
		lTFiles.mQ.nextRow();	
	}

	if (inID == ID_MONTH_CREATED_COMBO_ENTRY || inID == ID_MONTH_MODIFIED_COMBO_ENTRY)
	{
		for (int i = 0; i < 12; i++)
			if (mMonths[i].ID != 0)
				inPanelListBox->mStringList.AddTail(mMonths[i].Name);
	}

	inPanelListBox->SetItemCountEx(inPanelListBox->mStringList.GetCount());
}

void CBrowserPanel::SelchangeBrowserPanelList(int index)
{
	if (mPanelListBox[index].GetSel(0) && mPanelListBox[index].GetSelectedCount() > 1)
	{
		mPanelListBox[index].SetSel(0, FALSE);
		if (index==0) mPanelListBox[index].SetFocus();
	}
	else if (mPanelListBox[index].GetSelectedCount() < 1)
		mPanelListBox[index].SetSel(0, TRUE);

	for (int i = index + 1; i < NUM_PANELS; i++)
		ComboBoxChanged(i);

    CMDIChildWnd * pChild = ((CMDIFrameWnd*)(AfxGetApp()->m_pMainWnd))->MDIGetActive();
	CLibraryDoc* pDoc = DYNAMIC_DOWNCAST(CLibraryDoc, pChild->GetActiveDocument ());
	if (pDoc != NULL)
	{
		pDoc->UpdateLibrary ();
	}
//?	lMainFrame->InitFindText();
}

void CBrowserPanel::OnLbnSelchangeBrowserPanel1List(NMHDR *pNMHDR, LRESULT *pResult)
{
	SelchangeBrowserPanelList(0);
	mPanelListBox[0].SetFocus();
}

void CBrowserPanel::OnLbnSelchangeBrowserPanel2List(NMHDR *pNMHDR, LRESULT *pResult)
{
	SelchangeBrowserPanelList(1);
	mPanelListBox[1].SetFocus();
}

void CBrowserPanel::OnLbnSelchangeBrowserPanel3List(NMHDR *pNMHDR, LRESULT *pResult)
{
	SelchangeBrowserPanelList(2);
	mPanelListBox[2].SetFocus();
}

void CBrowserPanel::OnLbnSelchangeBrowserPanel4List(NMHDR *pNMHDR, LRESULT *pResult)
{
	SelchangeBrowserPanelList(3);
	mPanelListBox[3].SetFocus();
}

void CBrowserPanel::GetStringForSelect(CString& outStr)
{
	outStr.Empty();
	if (!IsBarVisible())
		return;

	CString	lStr;
	CString lConjunctionStr = _T(" and ");

	for (int i = 0; i < NUM_PANELS; i++)
	{
		GetStringFromPanel(i, lStr);
		if (!lStr.IsEmpty())
		{
			if (!outStr.IsEmpty())
				outStr += lConjunctionStr;
			outStr += lStr;
		}
	}

	if (!outStr.IsEmpty())
	{
		outStr = _T("(") + outStr;
		outStr = outStr + _T(")");
	}
}

void CBrowserPanel::GetStringFromPanel(int index, CString& outStr)
{
	CComboBox* inComboBox = &mPanelCombo[index];
	CBBListCtrl* inListBox = &mPanelListBox[index];

	CString lStr;
	CString lConjunctionStr;
	outStr.Empty();
	
	lConjunctionStr = _T(" or ");
	BOOL lFirst = TRUE;

	switch (inComboBox->GetItemData(inComboBox->GetCurSel()))
	{
		// Author
		case ID_AUTHOR_COMBO_ENTRY:
			for (int i = 1; i < inListBox->GetItemCount(); i++)
			{
				if (inListBox->GetSel(i))
				{
					if (!lFirst)
					{
						outStr += lConjunctionStr;
					}
					CString lAuthor;
					//inListBox->GetText(i, lAuthor);
					lAuthor = inListBox->GetItemText(i, 0);
					EncodeString(lAuthor, lAuthor);
					lStr.Format(_T("(Files.Author = '%s')"), lAuthor);
					outStr += lStr;
					lFirst = FALSE;
				}
			}
			break;

		// Category
		case ID_CATEGORY_COMBO_ENTRY:
			for (int i = 1; i < inListBox->GetItemCount(); i++)
			{
				if (inListBox->GetSel(i))
				{
					if (!lFirst)
					{
						outStr += lConjunctionStr;
					}
					CString lCategory;
					lCategory = inListBox->GetItemText(i, 0);
					EncodeString(lCategory, lCategory);
					lStr.Format(_T("(Files.Category = '%s')"), lCategory);
					outStr += lStr;
					lFirst = FALSE;
				}
			}
			break;

		// Collections
		case ID_COLLECTIONS_COMBO_ENTRY:

			break;

		// Day created
		case ID_DAY_CREATED_COMBO_ENTRY:
			for (int i = 1; i < inListBox->GetItemCount(); i++)
			{
				if (inListBox->GetSel(i))
				{
					if (!lFirst)
					{
						outStr += lConjunctionStr;
					}
					CString lDay;
					lDay = inListBox->GetItemText(i, 0);
					EncodeString(lDay, lDay);
					lStr.Format(_T("(strftime('%%d', Files.Created_TmStmp) = '%s')"), lDay);
					outStr += lStr;
					lFirst = FALSE;
				}
			}
			break;
			
		// Day modified
		case ID_DAY_MODIFIED_COMBO_ENTRY:
			for (int i = 1; i < inListBox->GetItemCount(); i++)
			{
				if (inListBox->GetSel(i))
				{
					if (!lFirst)
					{
						outStr += lConjunctionStr;
					}
					CString lDay;
					lDay = inListBox->GetItemText(i, 0);
					EncodeString(lDay, lDay);
					lStr.Format(_T("(strftime('%%d', Files.Modified_TmStmp) = '%s')"), lDay);
					outStr += lStr;
					lFirst = FALSE;
				}
			}
			break;

		// File type
		case ID_FILE_TYPE_COMBO_ENTRY:
			for (int i = 1; i < inListBox->GetItemCount(); i++)
			{
				if (inListBox->GetSel(i))
				{
					if (!lFirst)
					{
						outStr += lConjunctionStr;
					}
					CString lType;
					lType = inListBox->GetItemText(i, 0);

					CTFExtensions lTFExtension(m_pParamCDB->pDB);
					lTFExtension.GetExtensions();
					int lID = lTFExtension.GetExtensionIDWithCreate(lType);
					
					lStr.Format(_T("(Files.FExtensions_idFExtension = %d)"), lID);
					outStr += lStr;
					lFirst = FALSE;
				}
			}
			break;

		// Month created
		case ID_MONTH_CREATED_COMBO_ENTRY:
			for (int i = 1; i < inListBox->GetItemCount(); i++)
			{
				if (inListBox->GetSel(i))
				{
					if (!lFirst)
					{
						outStr += lConjunctionStr;
					}
					CString lMonthStr;
					lMonthStr = inListBox->GetItemText(i, 0);
					EncodeString(lMonthStr, lMonthStr);
					int lMonth = GetMonthNumFromStr(lMonthStr);
					lStr.Format(_T("(strftime('%%m', Files.Created_TmStmp) = '%.2d')"), lMonth);
					outStr += lStr;
					lFirst = FALSE;
				}
			}
			break;

		// Month modified
		case ID_MONTH_MODIFIED_COMBO_ENTRY:
			for (int i = 1; i < inListBox->GetItemCount(); i++)
			{
				if (inListBox->GetSel(i))
				{
					if (!lFirst)
					{
						outStr += lConjunctionStr;
					}
					CString lMonthStr;
					lMonthStr = inListBox->GetItemText(i, 0);
					EncodeString(lMonthStr, lMonthStr);
					int lMonth = GetMonthNumFromStr(lMonthStr);
					lStr.Format(_T("(strftime('%%m', Files.Modified_TmStmp) = '%.2d')"), lMonth);
					outStr += lStr;
					lFirst = FALSE;
				}
			}
			break;

		// Subject
		case ID_SUBJECT_COMBO_ENTRY:
			for (int i = 1; i < inListBox->GetItemCount(); i++)
			{
				if (inListBox->GetSel(i))
				{
					if (!lFirst)
					{
						outStr += lConjunctionStr;
					}
					CString lSubject;
					lSubject = inListBox->GetItemText(i, 0);
					EncodeString(lSubject, lSubject);
					lStr.Format(_T("(Files.Subject = '%s')"), lSubject);
					outStr += lStr;
					lFirst = FALSE;
				}
			}
			break;

		// Tags
		case ID_TAGS_COMBO_ENTRY:
			break;

		// Title
		case ID_TITLE_COMBO_ENTRY:
			for (int i = 1; i < inListBox->GetItemCount(); i++)
			{
				if (inListBox->GetSel(i))
				{
					if (!lFirst)
					{
						outStr += lConjunctionStr;
					}
					CString lTitle;
					lTitle = inListBox->GetItemText(i, 0);
					EncodeString(lTitle, lTitle);
					lStr.Format(_T("(Files.Title = '%s')"), lTitle);
					outStr += lStr;
					lFirst = FALSE;
				}
			}
			break;

		// Year created
		case ID_YEAR_CREATED_COMBO_ENTRY:
			for (int i = 1; i < inListBox->GetItemCount(); i++)
			{
				if (inListBox->GetSel(i))
				{
					if (!lFirst)
					{
						outStr += lConjunctionStr;
					}
					CString lYear;
					lYear = inListBox->GetItemText(i, 0);
					EncodeString(lYear, lYear);
					lStr.Format(_T("(strftime('%%Y', Files.Created_TmStmp) = '%s')"), lYear);
					outStr += lStr;
					lFirst = FALSE;
				}
			}
			break;

		// Year modified
		case ID_YEAR_MODIFIED_COMBO_ENTRY:
			for (int i = 1; i < inListBox->GetItemCount(); i++)
			{
				if (inListBox->GetSel(i))
				{
					if (!lFirst)
					{
						outStr += lConjunctionStr;
					}
					CString lYear;
					lYear = inListBox->GetItemText(i, 0);
					EncodeString(lYear, lYear);
					lStr.Format(_T("(strftime('%%Y', Files.Modified_TmStmp) = '%s')"), lYear);
					outStr += lStr;
					lFirst = FALSE;
				}
			}
			break;
	}

	if (!outStr.IsEmpty())
	{
		CString lOutStr = outStr; 
		outStr = _T("( ") + lOutStr + _T(" )");
	}
}

void CBrowserPanel::OnCbnDropdownBrowserPanel1Combo()
{
	FillComboBox(&mPanelCombo[0]);
}

void CBrowserPanel::OnCbnDropdownBrowserPanel2Combo()
{
	FillComboBox(&mPanelCombo[1]);
}

void CBrowserPanel::OnCbnDropdownBrowserPanel3Combo()
{
	FillComboBox(&mPanelCombo[2]);
}

void CBrowserPanel::OnCbnDropdownBrowserPanel4Combo()
{
	FillComboBox(&mPanelCombo[3]);
}

void CBrowserPanel::FillComboBox(CComboBox* inComboBox)
{
	CString lCombo1Str;
	CString lCombo2Str;
	CString lCombo3Str;
	CString lCombo4Str;
	CString lComboStr;

	lCombo1Str.Empty();
	lCombo2Str.Empty();
	lCombo3Str.Empty();
	lCombo4Str.Empty();

	if (mPanelCombo[0] != *inComboBox) mPanelCombo[0].GetWindowText(lCombo1Str);
	if (mPanelCombo[1] != *inComboBox) mPanelCombo[1].GetWindowText(lCombo2Str);
	if (mPanelCombo[2] != *inComboBox) mPanelCombo[2].GetWindowText(lCombo3Str);
	if (mPanelCombo[3] != *inComboBox) mPanelCombo[3].GetWindowText(lCombo4Str);

	inComboBox->GetWindowText(lComboStr);
	inComboBox->ResetContent();
	
	CString lStr;
	int lPos = 0;

	lStr = _T("Author");
	if (lStr != lCombo1Str && lStr != lCombo2Str && lStr != lCombo3Str && lStr != lCombo4Str)
	{
		inComboBox->AddString(lStr);
		inComboBox->SetItemData(lPos++, (DWORD_PTR)ID_AUTHOR_COMBO_ENTRY);
	}
	
	lStr = _T("Category");
	if (lStr != lCombo1Str && lStr != lCombo2Str && lStr != lCombo3Str && lStr != lCombo4Str)
	{
		inComboBox->AddString(lStr);
		inComboBox->SetItemData(lPos++, (DWORD_PTR)ID_CATEGORY_COMBO_ENTRY);
	}

	//lStr = _T("Collections");
	//if (lStr != lCombo1Str && lStr != lCombo2Str && lStr != lCombo3Str && lStr != lCombo4Str)
	//{
	//	inComboBox->AddString(lStr);
	//	inComboBox->SetItemData(lPos++, (DWORD_PTR)ID_COLLECTIONS_COMBO_ENTRY);
	//}

	lStr = _T("Day Created");
	if (lStr != lCombo1Str && lStr != lCombo2Str && lStr != lCombo3Str && lStr != lCombo4Str)
	{
		inComboBox->AddString(lStr);
		inComboBox->SetItemData(lPos++, (DWORD_PTR)ID_DAY_CREATED_COMBO_ENTRY);
	}

	lStr = _T("Day Modified");
	if (lStr != lCombo1Str && lStr != lCombo2Str && lStr != lCombo3Str && lStr != lCombo4Str)
	{
		inComboBox->AddString(lStr);
		inComboBox->SetItemData(lPos++, (DWORD_PTR)ID_DAY_MODIFIED_COMBO_ENTRY);
	}

	lStr = _T("File Type");
	if (lStr != lCombo1Str && lStr != lCombo2Str && lStr != lCombo3Str && lStr != lCombo4Str)
	{
		inComboBox->AddString(lStr);
		inComboBox->SetItemData(lPos++, (DWORD_PTR)ID_FILE_TYPE_COMBO_ENTRY);
	}

	lStr = _T("Month Created");
	if (lStr != lCombo1Str && lStr != lCombo2Str && lStr != lCombo3Str && lStr != lCombo4Str)
	{
		inComboBox->AddString(lStr);
		inComboBox->SetItemData(lPos++, (DWORD_PTR)ID_MONTH_CREATED_COMBO_ENTRY);
	}

	lStr = _T("Month Modified");
	if (lStr != lCombo1Str && lStr != lCombo2Str && lStr != lCombo3Str && lStr != lCombo4Str)
	{
		inComboBox->AddString(lStr);
		inComboBox->SetItemData(lPos++, (DWORD_PTR)ID_MONTH_MODIFIED_COMBO_ENTRY);
	}

	lStr = _T("Subject");
	if (lStr != lCombo1Str && lStr != lCombo2Str && lStr != lCombo3Str && lStr != lCombo4Str)
	{
		inComboBox->AddString(lStr);
		inComboBox->SetItemData(lPos++, (DWORD_PTR)ID_SUBJECT_COMBO_ENTRY);
	}

	//lStr = _T("Tags");
	//if (lStr != lCombo1Str && lStr != lCombo2Str && lStr != lCombo3Str && lStr != lCombo4Str)
	//{
	//	inComboBox->AddString(lStr);
	//	inComboBox->SetItemData(lPos++, (DWORD_PTR)ID_TAGS_COMBO_ENTRY);
	//}

	lStr = _T("Title");
	if (lStr != lCombo1Str && lStr != lCombo2Str && lStr != lCombo3Str && lStr != lCombo4Str)
	{
		inComboBox->AddString(lStr);
		inComboBox->SetItemData(lPos++, (DWORD_PTR)ID_TITLE_COMBO_ENTRY);
	}

	lStr = _T("Year Created");
	if (lStr != lCombo1Str && lStr != lCombo2Str && lStr != lCombo3Str && lStr != lCombo4Str)
	{
		inComboBox->AddString(lStr);
		inComboBox->SetItemData(lPos++, (DWORD_PTR)ID_YEAR_CREATED_COMBO_ENTRY);
	}

	lStr = _T("Year Modified");
	if (lStr != lCombo1Str && lStr != lCombo2Str && lStr != lCombo3Str && lStr != lCombo4Str)
	{
		inComboBox->AddString(lStr);
		inComboBox->SetItemData(lPos++, (DWORD_PTR)ID_YEAR_MODIFIED_COMBO_ENTRY);
	}

	lPos = inComboBox->FindStringExact(0, lComboStr);
	inComboBox->SetCurSel(lPos);
}

void CBrowserPanel::Refresh()
{
	for (int i = 0; i < NUM_PANELS; i++) {
		CComboBox* inPanelCombo = &mPanelCombo[i];
		int  lPos;
		UINT lID;
		lPos = inPanelCombo->GetCurSel();
		lID = (UINT)inPanelCombo->GetItemData(lPos);
		GetDatas(i, lID);
	}
	Invalidate();
}

void CBrowserPanel::ResetSelection()
{
	for (int index = 0; index < NUM_PANELS; index++)
	{
		for (int i = 0; i < mPanelListBox[0].GetItemCount(); i++)
			mPanelListBox[index].SetSel(i, FALSE);
		mPanelListBox[index].SetFocus();
		mPanelListBox[index].SetSel(0, TRUE);
	}
}

void CBrowserPanel::ComboBoxChanged(int index)
{
	CComboBox* inPanelCombo = &mPanelCombo[index];
	CBBListCtrl* inPanelListBox = &mPanelListBox[index];

	int  lPos;
	UINT lID;
	lPos = inPanelCombo->GetCurSel();
	lID = (UINT)inPanelCombo->GetItemData(lPos);
	GetDatas(index, lID);

	for (int i = 0; i < inPanelListBox->GetItemCount(); i++)
		inPanelListBox->SetSel(i, FALSE);
	
	inPanelListBox->SetFocus();
	inPanelListBox->SetSel(0);
	Resize(TRUE);
}

int CBrowserPanel::GetMonthNumFromStr(LPCTSTR inStr)
{
	CString lStr = inStr;
	for (int i = 0; i < 12; i++)
	{
		if (mMonths[i].Name == lStr)
			return i + 1;
	}
	return -1;
}

void CBBListCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CBCGPListCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
	switch (nChar)
	{
		case VK_RETURN:
			//AfxGetMainWnd()->SendMessage(WM_COMMAND, ID_OPEN);
//			AfxMessageBox(_T("BBListCtrl::OnKeyDown - VK_RETURN"));
			break;
		//break;
		case VK_UP:
		case VK_DOWN:
		case VK_PRIOR:
		case VK_NEXT:
		case VK_HOME:
		case VK_END:
		{
			CBrowserPanel* lBrowserPanel = (CBrowserPanel*)GetParent();
			for (int i = 0; i < NUM_PANELS; i++)
			{
				if (this == &lBrowserPanel->mPanelListBox[i])
				{
					lBrowserPanel->SelchangeBrowserPanelList(i);
					lBrowserPanel->mPanelListBox[i].SetFocus();
					break;
				}
			}
		}
	}
}

BOOL CBBListCtrl::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
			case VK_UP:
			case VK_DOWN:
			case VK_PRIOR:
			case VK_NEXT:
			case VK_HOME:
			case VK_END:
				break;
			default:
				return SelectItemStartingWith(pMsg->wParam);
		}
	}
	return CBCGPListCtrl::PreTranslateMessage(pMsg);
}

BOOL CBBListCtrl::SelectItemStartingWith(int inASCIICode)
{
	if (inASCIICode >= 32 && inASCIICode <= 126)
	{
		int lOldIndex = -1;
		int lNewIndex = -1;

		POSITION lPos = GetFirstSelectedItemPosition();
		if (lPos != NULL)
			lOldIndex = GetNextSelectedItem(lPos);

		CString lEnteredChar;
		CString lFirstChar;
		lEnteredChar.Format(_T("%c"), inASCIICode);
		for (int i = 0; i < GetItemCount(); i++)
		{
			lFirstChar = GetItemText(i, 0);
			lFirstChar = lFirstChar.Left(1);

			if (lEnteredChar.MakeLower() == lFirstChar.MakeLower())
			{
				lNewIndex = i;
				break;
			}
		}
		if (lNewIndex != -1 && lNewIndex != lOldIndex)
		{
			for (int i = 0; i < GetItemCount(); i++)
				SetSel(i, FALSE);

			SetItemState(lNewIndex, LVIS_FOCUSED, LVIS_FOCUSED);
			SetItemState(lNewIndex, LVIS_SELECTED, LVIS_SELECTED);

			CBrowserPanel* lBrowserPanel = (CBrowserPanel*)GetParent();
			for (int i = 0; i < NUM_PANELS; i++)
			{
				if (this == &lBrowserPanel->mPanelListBox[i])
				{
					lBrowserPanel->SelchangeBrowserPanelList(i);
					lBrowserPanel->mPanelListBox[i].SetFocus();
					break;
				}
			}
		}
		return TRUE;
	}
	else if (inASCIICode >= 127 && inASCIICode <= 255)
		return TRUE;
	
	return TRUE;
}

