// CTagsSelectionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CTagsSelectionDlg.h"
#include "CManageTagsDlg.h"
#include "LibraryView.h"


// CTagsSelectionDlg dialog

IMPLEMENT_DYNAMIC(CTagsSelectionDlg, CDialog)

//CTagsSelectionDlg::CTagsSelectionDlg(int inFileID, int inTableID /*= TABLEID_FILE*/, CWnd* pParent /*=NULL*/)
//	: CDialog(CTagsSelectionDlg::IDD, pParent)
/*{
	mMainEntryID = inFileID;
	mTableID = inTableID;
	mIDData = NULL;
}
*/
CTagsSelectionDlg::CTagsSelectionDlg(CIDData *inIDData, CWnd* pParent /*= NULL*/)
	: CDialog(CTagsSelectionDlg::IDD, pParent)
{
	mIDData = inIDData;

	CMDIChildWnd * pChild = ((CMDIFrameWnd*)(AfxGetApp()->m_pMainWnd))->MDIGetActive();
	CLibraryDoc* pDoc = DYNAMIC_DOWNCAST(CLibraryDoc, pChild->GetActiveDocument ());
	m_pParamCDB = pDoc->m_pParamCDB;
}

CTagsSelectionDlg::~CTagsSelectionDlg()
{
}

void CTagsSelectionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TS_AVAILABLE_TAGS_LIST, mAvailableTags);
	DDX_Control(pDX, IDC_TS_SELECTED_TAGS_LIST, mSelectedTags);
}


BEGIN_MESSAGE_MAP(CTagsSelectionDlg, CDialog)
	ON_BN_CLICKED(IDC_TS_MANAGE_TAGS_BTN, &CTagsSelectionDlg::OnBnClickedTsManageTagsBtn)
	ON_BN_CLICKED(IDC_TS_ADD_BTN, &CTagsSelectionDlg::OnBnClickedTsAddBtn)
	ON_BN_CLICKED(IDC_TS_REMOVE_BTN, &CTagsSelectionDlg::OnBnClickedTsRemoveBtn)
	ON_BN_CLICKED(IDOK, &CTagsSelectionDlg::OnBnClickedOk)
	ON_LBN_DBLCLK(IDC_TS_AVAILABLE_TAGS_LIST, &CTagsSelectionDlg::OnLbnDblclkTsAvailableTagsList)
	ON_LBN_DBLCLK(IDC_TS_SELECTED_TAGS_LIST, &CTagsSelectionDlg::OnLbnDblclkTsSelectedTagsList)
END_MESSAGE_MAP()


// CTagsSelectionDlg message handlers

void CTagsSelectionDlg::OnBnClickedTsManageTagsBtn()
{
	CManageTagsDlg lDlg;
	if (IDOK == lDlg.DoModal())
	{
		InitControls(true);

		CString lTags;
		CString lTag;
		lTags = lDlg.mAddedTags;
		
		int lPos;
		int lIndex;

		while ((lPos = lTags.Find(_T("\r\n"))) != -1)
		{
			lTag = lTags.Left(lPos);
			if (!lTag.IsEmpty())
			{
				lIndex = mAvailableTags.FindStringExact(-1, lTag);
				if (lIndex != LB_ERR)
					mAvailableTags.SetSel(lIndex);
			}
			lTags = lTags.Right(lTags.GetLength() - lPos - 2);
		}
		lTag = lTags;
		if (!lTag.IsEmpty())
		{
			lIndex = mAvailableTags.FindStringExact(-1, lTag);
			if (lIndex != LB_ERR)
				mAvailableTags.SetSel(lIndex);
		}
	}
}

BOOL CTagsSelectionDlg::InitControls(bool inKeepSelectedTagsList /* = false*/)
{
	CTTags lTTags(m_pParamCDB->pDB);
	int lItemIdx;

	mAvailableTags.ResetContent();
	if(lTTags.GetTags())
	{
		while(lTTags.mDataIsValid)
		{
			lItemIdx = mAvailableTags.AddString(lTTags.mName);
			mAvailableTags.SetItemData(lItemIdx, lTTags.mID);
			lTTags.LoadNextRow();
		}
	}
	
	if(inKeepSelectedTagsList)
	{ //we should have some data in SelectedTags list, and the ManageTag "akction" was done
		int lNumSel = mSelectedTags.GetCount();
		int lNumAvailable = mAvailableTags.GetCount();
		CIntArray lSelectedIDsArray;

		int lItemID1, lItemID2;
		for(int i = 0; i<lNumSel; i++)
		{
			lItemID1 = (int) mSelectedTags.GetItemData(i);
			lSelectedIDsArray.Add(lItemID1);
		}
		mSelectedTags.ResetContent();
		lNumSel = (int) lSelectedIDsArray.GetCount();
		int lItemIdx;
		for(int i = (lNumSel-1); i >= 0; i--)
		{
			lItemID1 = lSelectedIDsArray.GetAt(i);
			for(int j = (mAvailableTags.GetCount()-1); j >=0 ; j--)
			{
				lItemID2 = (int) mAvailableTags.GetItemData(j);
				if(lItemID2 == lItemID1)
				{ //move from Available to Selected
					CString lItemText;
					mAvailableTags.GetText(j, lItemText);
					lItemIdx = mSelectedTags.AddString(lItemText);
					mSelectedTags.SetItemData(lItemIdx, lItemID1);
					mAvailableTags.DeleteString(j);
				}
			}
		}
	}
	else if(mIDData != NULL)
	{	//the selectedTagsList is initialized based on the mIDData values
		//after OK returns the selected Tag IDs
		int lNumAvailable = mAvailableTags.GetCount();
		int lItemIdx, lItemData;

		mSelectedTags.ResetContent();
		for(int i = lNumAvailable-1; i >= 0; i--)
		{
			if(((int)mIDData->GetIdxByID(mAvailableTags.GetItemData(i))) >= 0)
			{	
				CString lItemText;
				mAvailableTags.GetText(i, lItemText);
				lItemData = mAvailableTags.GetItemData(i);
				lItemIdx = mSelectedTags.AddString(lItemText);
				mSelectedTags.SetItemData(lItemIdx, lItemData);
				
				//remove it from Available tags list
				mAvailableTags.DeleteString(i);
			}
		}
	}


/* Delete, Tono
	else
	{	//the old approach immediattly writes back the data to DB after clicking OK
		bool lTagsRes = false;

		if(mTableID == TABLEID_FILES)
			lTagsRes = lTTags.GetTagsByFileID(mMainEntryID);
		else if(mTableID == TABLEID_WATCHEDFOLDERS)
			lTagsRes = lTTags.GetTagsByWFolderID(mMainEntryID);

		if(lTagsRes)
		{
			while(lTTags.mDataIsValid)
			{
				lItemIdx = mSelectedTags.AddString(lTTags.mName);
				mSelectedTags.SetItemData(lItemIdx, lTTags.mID);
				int lNumAvailable = mAvailableTags.GetCount();
				for(int i = lNumAvailable-1; i >= 0; i--)
				{
					if((mAvailableTags.GetItemData(i) == lTTags.mID))
					{	//remove it from Available tags list
						mAvailableTags.DeleteString(i);
					}
				}
				lTTags.LoadNextRow();
			}
		}
	}
*/
	return TRUE;
}

BOOL CTagsSelectionDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	InitControls();

	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CTagsSelectionDlg::OnBnClickedTsAddBtn()
{
	int lCount = mAvailableTags.GetSelCount();
	if(lCount < 1)
		return;

	for (int i = 0; i < mSelectedTags.GetCount(); i++)
		mSelectedTags.SetSel(i, FALSE);

	CIntArray lSelTags;
	lSelTags.SetSize(lCount);
	mAvailableTags.GetSelItems(lCount, lSelTags.GetData()); 
	
	CString lTagStr;
	int lTagStrLen, lIndex;

	int lLastIndex = -1;
	int lSelectedCount = 0;

	DWORD_PTR lItemData;
	for(int i = lCount - 1; i >= 0; i--)
	{
		lIndex = lSelTags.GetAt(i);
		lTagStrLen = mAvailableTags.GetTextLen(lIndex);
		mAvailableTags.GetText(lIndex, lTagStr.GetBuffer(lTagStrLen));
		lTagStr.ReleaseBuffer();
		
		lItemData = mAvailableTags.GetItemData(lIndex);
		if (mAvailableTags.GetSel(lIndex) > 0)
		{
			if (lLastIndex == -1) lLastIndex = lIndex;
			lSelectedCount++;
		}
		
		mAvailableTags.DeleteString(lIndex);
		lIndex = mSelectedTags.AddString(lTagStr);
		mSelectedTags.SetItemData(lIndex, lItemData);
		mSelectedTags.SetSel(lIndex);
	}
	lLastIndex = (lLastIndex + 1) - lSelectedCount;
	if (lLastIndex > mAvailableTags.GetCount() - 1) lLastIndex = mAvailableTags.GetCount() - 1;
	mAvailableTags.SetSel(lLastIndex, TRUE);

}

void CTagsSelectionDlg::OnBnClickedTsRemoveBtn()
{
	int lCount = mSelectedTags.GetSelCount();
	if(lCount < 1)
		return;

	for (int i = 0; i < mAvailableTags.GetCount(); i++)
		mAvailableTags.SetSel(i, FALSE);

	CIntArray lSelTags;
	lSelTags.SetSize(lCount);
	mSelectedTags.GetSelItems(lCount, lSelTags.GetData()); 

	CString lTagStr;
	int lTagStrLen, lIndex;

	int lLastIndex = -1;
	int lSelectedCount = 0;

	DWORD_PTR lItemData;
	for(int i = lCount - 1; i >= 0; i--)
	{
		lIndex = lSelTags.GetAt(i);
		lTagStrLen = mSelectedTags.GetTextLen(lIndex);
		mSelectedTags.GetText(lIndex, lTagStr.GetBuffer(lTagStrLen));
		lTagStr.ReleaseBuffer();

		lItemData = mSelectedTags.GetItemData(lIndex);
		if (mSelectedTags.GetSel(lIndex) > 0)
		{
			if (lLastIndex == -1) lLastIndex = lIndex;
			lSelectedCount++;
		}

		mSelectedTags.DeleteString(lIndex);
		lIndex = mAvailableTags.AddString(lTagStr);
		mAvailableTags.SetItemData(lIndex, lItemData);
		mAvailableTags.SetSel(lIndex);
	}
	lLastIndex = (lLastIndex + 1) - lSelectedCount;
	if (lLastIndex > mSelectedTags.GetCount() - 1) lLastIndex = mSelectedTags.GetCount() - 1;
	mSelectedTags.SetSel(lLastIndex, TRUE);
}

void CTagsSelectionDlg::OnBnClickedOk()
{
	CTTags lTTags(m_pParamCDB->pDB);
	int lIdx;
	CString lTagName;
	bool lTagsRetVal = false;

	if(mIDData != NULL)
	{
//the new approach, changes the selected IDs array
		mIDData->RemoveAll();
		int lID;
		CString lStr;
		for(lIdx = mSelectedTags.GetCount() - 1; lIdx >= 0; lIdx--)
		{
			lID = mSelectedTags.GetItemData(lIdx);
			mSelectedTags.GetText(lIdx, lStr);
			mIDData->AddData(lID, lStr);
		}
	}

/*
//the old approach, will be deleted!!!!

	if(mTableID == TABLEID_FILES)
		lTagsRetVal = lTTags.GetTagsByFileID(mMainEntryID);
	else if(mTableID == TABLEID_WATCHEDFOLDERS)
		lTagsRetVal = lTTags.GetTagsByWFolderID(mMainEntryID);

	if(lTagsRetVal)
	{
		while(lTTags.mDataIsValid)
		{
			lCount =  mSelectedTags.GetCount();
			for(lIdx = lCount; lIdx >= 0; lIdx--)
			{
				lItemData = mSelectedTags.GetItemData(lIdx);
				if(lItemData == lTTags.mID)
				{
					mSelectedTags.GetText(lIdx, lTagName);
					if(lTagName == lTTags.mName)
					{		
						//do nothing in the DB
						//remove the tag from the list
						mSelectedTags.DeleteString(lIdx);
						break;
					}
					else
					{
						//update the DB table
						lTTags.mName = lTagName;
						lTTags.UpdateByID();
						//remove the Tag from the list
						mSelectedTags.DeleteString(lIdx);
						break;
					}
				}
			}
			lTTags.LoadNextRow();
		}
	}

	lCount = mSelectedTags.GetCount();
	for(int i = 0; i< lCount; i++)
	{
		lTagID = mSelectedTags.GetItemData(i);
		if(mTableID == TABLEID_FILES)
			m_pDB->AddTagToFile( lTagID, mMainEntryID);
		else
			m_pDB->AddTagToWFolder( lTagID, mMainEntryID);
	}
*/
	OnOK();
}

void CTagsSelectionDlg::OnLbnDblclkTsAvailableTagsList()
{
	OnBnClickedTsAddBtn();
}

void CTagsSelectionDlg::OnLbnDblclkTsSelectedTagsList()
{
	OnBnClickedTsRemoveBtn();
}
