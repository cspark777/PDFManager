// CDocumentFileList.cpp 
//*****************************************************************************

#include "stdafx.h"
#include "Define.h"
#include "resource.h"
#include "CDocumentFileList.h"
#include "LibraryView.h"
#include "ShellContextMenu.h"
#include "MainFrm.h"
#include "CMissingFileDlg.h"
#include "CMissingFilesDlg.h"
#include "CUtils.h"
#include "CProgressDlg.h"
#include "CImportFolderDlg.h"

#include "CBWorkspace.h"
#include "CBRepository.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//*****************************************************************************
// Message map
BEGIN_MESSAGE_MAP(CDocumentFileList, CBCGPListCtrl)
	ON_WM_CONTEXTMENU()
	ON_WM_DROPFILES()
	ON_WM_CREATE()
	ON_NOTIFY_REFLECT(LVN_BEGINDRAG, &CDocumentFileList::OnLvnBegindrag)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDBLCLK()
	ON_NOTIFY_REFLECT(LVN_GETDISPINFO, &CDocumentFileList::OnGetdispinfo)
	ON_NOTIFY_REFLECT(LVN_ITEMCHANGED, &CDocumentFileList::OnLbnSelchange)
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()
//*****************************************************************************


/*
Caching and Virtual List Controls
Because this type of list control is intended for large data sets, 
it is recommended that you cache requested item data to improve retrieval performance. 
The framework provides a cache-hinting mechanism to assist in optimizing the cache by sending 
an LVN_ODCACHEHINT notification message. However, you must use a slightly different method 
to handle this notification. Using the Properties window, override the OnChildNotify function 
of your list control object. In the case of this example, CMyListCtrl.
Inside the body of the handler, check for the LVN_ODCACHEHINT message and, if found, prepare your cache.
*/

BOOL CDocumentFileList::OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	NMLVCACHEHINT* pcachehint=NULL;

	if (message == WM_NOTIFY)
    {
        NMHDR* phdr = (NMHDR*)lParam;

        switch(phdr->code)
        {
        case LVN_ODCACHEHINT:
            pcachehint= (NMLVCACHEHINT*) phdr;
			if(IsPreparedCache(pcachehint->iFrom, pcachehint->iTo)) {
				if (((CLibraryView*)GetParent())->GetViewStyle() == FILE_VIEW_THUMBNAILS) {
					nStartItem = pcachehint->iFrom;
					nEndItem = pcachehint->iTo;
					StartPrepareThumbnail();
					StopPrepareThumbnail();
					//PrepareCache(pcachehint->iFrom, pcachehint->iTo, 0, true);
				} else if (((CLibraryView*)GetParent())->GetViewStyle() == FILE_VIEW_LIST)
					PrepareCache(pcachehint->iFrom, pcachehint->iTo, 2, true);
				else if (((CLibraryView*)GetParent())->GetViewStyle() == FILE_VIEW_ICONS)
					PrepareCache(pcachehint->iFrom, pcachehint->iTo, 1, true);
			}
            break;
        default:
            return CBCGPListCtrl::OnChildNotify(message, wParam, lParam, pResult);
        }
        return FALSE;
    }
    else
        return CBCGPListCtrl::OnChildNotify(message, wParam, lParam, pResult);
}

/*
Virtual list controls maintain very little item information. 
Except for the item selection and focus information, all item information is managed 
by the owner of the control. Information is requested by the framework 
via a LVN_GETDISPINFO notification message. To provide the requested information, 
the owner of the virtual list control (or the control itself) must handle this notification.
*/

void CDocumentFileList::OnGetdispinfo(NMHDR *pNMHDR, LRESULT *pResult)
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	LV_ITEM* pItem= &(pDispInfo)->item;
	int nRow = pItem->iItem;

	if(nRow >= m_pDoc->sFilesArray.GetCount())
		return;

	if(!m_pDoc->sFilesArray[nRow].mWasRead) {

		return;
	}

	CString lTimeStr;
	if (pItem->mask & LVIF_TEXT)
	{
		switch(pItem->iSubItem)
		{
			case 0: //fill in main text
				lstrcpyn(pItem->pszText, m_pDoc->sFilesArray[nRow].mName.GetBuffer(), pItem->cchTextMax);
				break;
			case 1: //fill in sub item 1 text
				lstrcpyn(pItem->pszText, m_pDoc->sFilesArray[nRow].mTitle.GetBuffer(), pItem->cchTextMax);
				break;		
			case 2: // Author
				lstrcpyn(pItem->pszText, m_pDoc->sFilesArray[nRow].mAuthor.GetBuffer(), pItem->cchTextMax);
				break;
			case 3: // Subject
				lstrcpyn(pItem->pszText, m_pDoc->sFilesArray[nRow].mSubject.GetBuffer(), pItem->cchTextMax);
				break;
			case 4:
				lstrcpyn(pItem->pszText, m_pDoc->sFilesArray[nRow].mTags.GetBuffer(), pItem->cchTextMax);
				break;
			case 5:
				lstrcpyn(pItem->pszText, m_pDoc->sFilesArray[nRow].mCollections.GetBuffer(), pItem->cchTextMax);
				break;
			case 6:
				lstrcpyn(pItem->pszText, m_pDoc->sFilesArray[nRow].mExt.GetBuffer(), pItem->cchTextMax);
				break;
			case 7:
				lstrcpyn(pItem->pszText, m_pDoc->sFilesArray[nRow].mSize.GetBuffer(), pItem->cchTextMax);
				break;
			case 8: // Categories
				lstrcpyn(pItem->pszText, m_pDoc->sFilesArray[nRow].mCategory.GetBuffer(), pItem->cchTextMax);
				break;
			case 9: // Path
				lstrcpyn(pItem->pszText, m_pDoc->sFilesArray[nRow].mPath.GetBuffer(), pItem->cchTextMax);
				break;
			case 10:
				lstrcpyn(pItem->pszText, m_pDoc->sFilesArray[nRow].mCreatedTmStmp.GetBuffer(), pItem->cchTextMax);
				break;
			case 11:
				lstrcpyn(pItem->pszText, m_pDoc->sFilesArray[nRow].mModifiedTmStmp.GetBuffer(), pItem->cchTextMax);
				break;
			case 12:
				lstrcpyn(pItem->pszText, m_pDoc->sFilesArray[nRow].mAddedTmStmp.GetBuffer(), pItem->cchTextMax);
				break;
			case 13:
				lstrcpyn(pItem->pszText, m_pDoc->sFilesArray[nRow].mAccessedTmStmp.GetBuffer(), pItem->cchTextMax);
				break;
			case 14:
				lstrcpyn(pItem->pszText, m_pDoc->sFilesArray[nRow].mAttr.GetBuffer(), pItem->cchTextMax);
				break;
			case 15:
				lstrcpyn(pItem->pszText, m_pDoc->sFilesArray[nRow].mComments.GetBuffer(), pItem->cchTextMax);
				break;
		}
	}

	if (pItem->mask & LVIF_IMAGE) //valid image?
	{
		if(m_pDoc->sFilesArray[nRow].mPreviewID != -1)
			pItem->iImage = m_pDoc->sFilesArray[nRow].mPreviewID;
	}

	if (pItem->mask & LVIF_STATE)
	{
		if(m_pDoc->sFilesArray[nRow].mInWorkspace) {
			pItem->state = INDEXTOSTATEIMAGEMASK(1);
			pItem->stateMask = pItem->state;
		}
	}

	*pResult = 0;
}

void CDocumentFileList::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CBCGPListCtrl::DrawItem(lpDrawItemStruct);
}
 

//*****************************************************************************
// CDocumentFileList
//*****************************************************************************
CDocumentFileList::CDocumentFileList()
{
	mRefresh = true;
	m_HeaderOff = FALSE;
	m_iColumn = 0;
	m_bAscending = 0;

	// Drop default values
	m_dropMode.iMask = DL_ACCEPT_FILES | DL_ACCEPT_FOLDERS;

	// Ole initialization
	m_bMustUninitOLE = FALSE;
	_AFX_THREAD_STATE* pState = AfxGetThreadState();
	if (!pState->m_bNeedTerm)
	{
		HRESULT hr = ::OleInitialize(NULL);
		if (FAILED(hr))
			AfxMessageBox(_T("OLE initialization failed.\n\nMake sure that the OLE libraries are the correct version."));
		else
			m_bMustUninitOLE = TRUE;
	}

	m_hWaitingThread = CreateEvent(0, TRUE, FALSE, 0);

	m_iSortedColumn = 0;
	m_bAscending = TRUE;

	m_bTerminate = false;
	m_bRunning = false;
}

//*****************************************************************************
CDocumentFileList::~CDocumentFileList(void)
{
	if(m_bMustUninitOLE)
	{
		::OleUninitialize();
	}
}

//*****************************************************************************
void CDocumentFileList::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	CMDIChildWnd * pChild = ((CMDIFrameWnd*)(AfxGetApp()->m_pMainWnd))->MDIGetActive();
	if(pChild == NULL)
		return;
	CLibraryDoc* pDoc = DYNAMIC_DOWNCAST(CLibraryDoc, pChild->GetActiveDocument ());

	try
	{
		CMenu lMenu;
		CMenu* lPopup;
		CBCGPPopupMenu* lPopupMenu;
		CPoint lPoint;
		lPoint = point;
		ScreenToClient(&lPoint);
		
		RECT rect;
		
		m_wndHeader.GetWindowRect(&rect);

		if((point.x>=rect.left)&&(point.x<=rect.right)&&(point.y>=rect.top)&&(point.y<=rect.bottom))
		{
			lMenu.LoadMenu(IDR_DOCUMENT_LIST_HEADER_MENU);
			lPopup = lMenu.GetSubMenu(0);
			ASSERT(lPopup != NULL);
			lPopupMenu = new CBCGPPopupMenu;
			lPopupMenu->Create(this, point.x, point.y, lPopup->Detach());
		}
		else
		{
			// File context menu
			int lCount = GetSelectedCount();
		
			if (lCount > 0)
			{
				CIntArray lFileIDs;
				CStringArray arrayFiles;
				POSITION lPos = GetFirstSelectedItemPosition();
				CTFiles lTFiles(m_pParamCDB);
				bool lFixMissingFiles = false;

				if (lCount == 1)
					lFixMissingFiles = true;
				
				while(lPos)
				{
					int lSelItemIdx = GetNextSelectedItem(lPos);
					CString lStr;
					lStr.Format(_T("%s"), m_pDoc->sFilesArray[lSelItemIdx].mPath);
					int nItem = 0;

					if (m_pDoc->sFilesArray[lSelItemIdx].mFlags != FILE_FLAG_FNF)	//Not existing files causes the Explorers menu to throw an exception
					{
						arrayFiles.Add(lStr); 
						lFileIDs.Add(m_pDoc->sFilesArray[lSelItemIdx].mID);
					}
					else
					{
						if (lFixMissingFiles == false)
						{
							CMissingFilesDlg lMissingFilesDlg;
							if (lMissingFilesDlg.DoModal() == IDOK)
							{	
								lFixMissingFiles = true;
							}
							else
								return;	//Cancel and Close cancels the current operation
						}
						if (lFixMissingFiles)
						{
							CMissingFileDlg lMissingFileDlg(lStr);
							if (lMissingFileDlg.DoModal() == IDOK)
							{
								((CMainFrame*)AfxGetApp()->m_pMainWnd)->SendMessage(WM_COMMAND, ID_DELETE_FILE);
							}
							else
							{
								CFileDialog lDlg(TRUE, NULL, lStr, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, NULL);
								if (lDlg.DoModal() == IDOK)
								{
									CString lFilePath = lDlg.GetPathName();
									if (m_pDoc->sFilesArray[lSelItemIdx].mName != lDlg.GetFileName())
									{
										AfxMessageBox(_T("The file name did not match! Operation Canceled"), MB_OK|MB_ICONWARNING);
										return;
									}	

									CStringList lNewFileList;
									lNewFileList.AddHead(lFilePath);
								
									if (m_pViewInfo->pRepository->AddFiles(lNewFileList) > 0)
									{
										((CLibraryView*)GetParent())->Refresh();
										arrayFiles.Add(lStr); 
										//TODO: Tono: Add the file to File List and add it to current slelection!!!
									}
									// ASSERT(0); //TODO: Tono ha az adatbazisban az mFlag erteke meg nem volt frissitve, akkor lehet hogy nem letezo file kerul a listaba
									// don't do this		arrayFiles.Add(lStr); 
								}
							}
						}
					}
				}
			
				if (arrayFiles.GetCount() <= 0)
					return;

				lMenu.LoadMenu(IDR_DOCUMENT_LIST_MENU);
				lPopup = lMenu.GetSubMenu(0);
				ASSERT(lPopup != NULL);

				// Fill Collection submenu
				CMenu* lAddToCollectionMenu = m_pDoc->FillAddToCollMenuRecursively(lPopup, -1, _T("Add to Collection"), &lFileIDs);

				// Fill Remove from Collection submenu
				CMenu* lRemoveFromCollectionMenu = m_pDoc->FillRemoveFromCollMenuRecursively(lPopup, -1, _T("Remove from Collection"), &lFileIDs);

				// Fill Add to Tags submenu
				CMenu* lAddToTagsMenu = m_pDoc->FillAddToTagsMenu(lPopup, -1, _T("Add to Tags"), &lFileIDs);

				// Fill Remove from Tags submenu
				CMenu* lRemoveFromTagsMenu = m_pDoc->FillRemoveFromTagsMenu(lPopup, -1, _T("Remove from Tags"), &lFileIDs);

				if (lCount != 1)
					lPopup->EnableMenuItem(ID_RENAME_FILE, MF_BYCOMMAND | MF_GRAYED | MF_DISABLED);
				else
					lPopup->EnableMenuItem(ID_RENAME_FILE, MF_BYCOMMAND | MF_ENABLED);
			
				UINT lRet = g_pContextMenuManager->TrackPopupMenu(lPopup->GetSafeHmenu(), point.x, point.y, this, FALSE);
				if (lAddToCollectionMenu != NULL)      m_pDoc->FreeMenuRecursively(lAddToCollectionMenu);
				if (lRemoveFromCollectionMenu != NULL) m_pDoc->FreeMenuRecursively(lRemoveFromCollectionMenu);
				if (lAddToTagsMenu != NULL)            m_pDoc->FreeMenuRecursively(lAddToTagsMenu);
				if (lRemoveFromTagsMenu != NULL)       m_pDoc->FreeMenuRecursively(lRemoveFromTagsMenu);
			
				CWnd* lWnd;
				lWnd = GetParent();
				lWnd = lWnd->GetParent();
				lWnd->SendMessage(WM_COMMAND, lRet, 0);
			}
		}
	}
	catch(...)
	{
		ASSERT(0);
	}
}

//*****************************************************************************
BOOL CDocumentFileList::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	HD_NOTIFY *lHDN = (HD_NOTIFY*)lParam;
/*
	if((lHDN->hdr.code == HDN_ITEMCHANGINGW || lHDN->hdr.code == HDN_ITEMCHANGINGA) 
		&& (lHDN->pitem->cxy < 10 && !m_HeaderOff))
	{
		//*pResult = TRUE;                // disable change
		//return TRUE;                    // Processed message
	}

	for (int i = 0; i < NUM_HEADER; i++)
	{
		if (((CLibraryView*)GetParent())->GetCurrentHeaderSize(i) == 0)
		{
			if (lHDN->iItem == i)
			{
			//	*pResult = TRUE;
			//	return TRUE;
			}
		}
	}
*/
	//m_HeaderOff = FALSE;
	return CBCGPListCtrl::OnNotify(wParam, lParam, pResult);
}

//*****************************************************************************
int CDocumentFileList::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CBCGPListCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	m_DropTarget.Register(this);

	return 0;
}

//***************************************************************************
//*                               Drag & Drop                               *
//***************************************************************************
void CDocumentFileList::OnLvnBegindrag(NMHDR *pNMHDR, LRESULT *pResult)
{
	PrepareOleData(FALSE);
}
//*****************************************************************************
void CDocumentFileList::OnDropFiles(HDROP dropInfo)
{
	DropFiles(dropInfo, m_dropMode);
	DragFinish(dropInfo);
}

//*****************************************************************************
// Drag & Copy files

bool CDocumentFileList::PrepareOleData(bool inType)
{
	CMDIChildWnd * pChild = ((CMDIFrameWnd*)(AfxGetApp()->m_pMainWnd))->MDIGetActive();
	CLibraryDoc* pDoc = DYNAMIC_DOWNCAST(CLibraryDoc, pChild->GetActiveDocument ());

	CStringList lDraggedFiles;
	UINT        lBuffSize = 0;

	POSITION pos = GetFirstSelectedItemPosition();
	while (pos != NULL) 
	{		
		int index = GetNextSelectedItem(pos);
		int lFileID = m_pDoc->sFilesArray[index].mID;

		CTFiles lFiles(m_pParamCDB);
		lFiles.GetFileByID(lFileID);

		CString lFilePath = lFiles.mPath;

		lDraggedFiles.AddTail(lFilePath);
		lBuffSize += lstrlen(lFilePath) + 1;
	}
	
	lBuffSize = sizeof(DROPFILES) + sizeof(TCHAR) * (lBuffSize + 1);
	
	HGLOBAL    hgDrop;
	DROPFILES* pDrop;

	// Allocate memory from the heap for the DROPFILES struct.
	hgDrop = GlobalAlloc(GHND | GMEM_SHARE, lBuffSize);
	if (hgDrop == NULL)
		return false;

	pDrop = (DROPFILES*)GlobalLock(hgDrop);
	if (NULL == pDrop)
	{
		GlobalFree(hgDrop);
		return false;
	}

	// Fill in the DROPFILES struct.
	pDrop->pFiles = sizeof(DROPFILES);

#ifdef _UNICODE
	// If we're compiling for Unicode, set the Unicode flag in the struct to
	// indicate it contains Unicode strings.
	pDrop->fWide = TRUE;
#endif

	TCHAR* pszBuff;
	// Copy all the filenames into memory after the end of the DROPFILES struct.
	pos = lDraggedFiles.GetHeadPosition();
	pszBuff = (TCHAR*)(LPBYTE(pDrop) + sizeof(DROPFILES));
	while (pos != NULL)
	{
		lstrcpy(pszBuff, (LPCTSTR)lDraggedFiles.GetNext(pos));
		pszBuff = 1 + _tcschr(pszBuff, '\0');
	}
	GlobalUnlock ( hgDrop );

	if (inType == 1)
	{	//copy
		//COleDataSource* datasrc  = COleDataSource::GetClipboardOwner();
		//if (datasrc == NULL)
		//	datasrc = new COleDataSource();

		COleDataSource* datasrc = new COleDataSource();
		datasrc->CacheGlobalData(CF_HDROP, hgDrop);			//it will work on both Copy or Cut
		HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, sizeof(DWORD));
		//*(DWORD*)GlobalLock(hMem) = DROPEFFECT_MOVE;		//And this show that I want Cut function
		*(DWORD*)GlobalLock(hMem) = DROPEFFECT_COPY;
		GlobalUnlock(hMem);
		
		datasrc->CacheGlobalData(RegisterClipboardFormat(CFSTR_PREFERREDDROPEFFECT), hMem);
		datasrc->SetClipboard();

		/* In my experiment result,I found that the object in datasrc will send to Clipboard
		So I can't 'delete' it; and the pointer is only workable with few function
		keep it is nonsense
		for example,I can't do 'm_pSource->SetClipboard()' again
		but the workaround is still workable, use m_pSource->GetClipboardOwner() is more better
		*/
		datasrc = NULL;		//delete m_pSource can't be done 
	}
	else
	{	// drag&drop
		COleDataSource datasrc;
		FORMATETC etc = {CF_HDROP, NULL, DVASPECT_CONTENT, -1, TYMED_HGLOBAL};

		// Put the data in the data source.
		datasrc.CacheGlobalData(CF_HDROP, hgDrop, &etc);

		HGLOBAL hgBool;
		hgBool = GlobalAlloc(GHND | GMEM_SHARE, sizeof(bool));
		if (hgBool == NULL)
		{
			GlobalFree(hgDrop);
			return false;
		}

		// Put the data in the data source.
		//etc.cfFormat = m_pDoc->sCustomClipbrdFormat;
		//datasrc.CacheGlobalData(m_pDoc->sCustomClipbrdFormat, hgBool, &etc);
		
		//CFSTR_PREFERREDDROPEFFECT

		//do not allow to drag outside the application

		DROPEFFECT dwEffect;
		dwEffect = datasrc.DoDragDrop(DROPEFFECT_LINK /*DROPEFFECT_COPY/* | DROPEFFECT_MOVE*/);
		switch ( dwEffect )
		{
		case DROPEFFECT_COPY:
		case DROPEFFECT_MOVE:
			break;
		case DROPEFFECT_NONE:
			{
				GlobalFree(hgDrop);
				GlobalFree(hgBool);
				break;
			}
		}
	}

	return true;
}

BOOL CDocumentFileList::InsertFiles(CStringList& inFilesList)
{
	CMDIChildWnd * pChild = ((CMDIFrameWnd*)(AfxGetApp()->m_pMainWnd))->MDIGetActive();
	CLibraryView* pView = DYNAMIC_DOWNCAST(CLibraryView, pChild->GetActiveView ());

	CFileProperties_Entry lEntry;
	lEntry.mCollectionID = pView->GetSelectedCollectionGuiID() - FILTERTREE_COLLECTION_MIN;


	if (m_pViewInfo->pRepository->AddFiles(inFilesList, &lEntry) == 0)
		return FALSE;

	return TRUE;
}

BOOL CDocumentFileList::ValidatePathname(const CString& csPathname, UINT& iPathType, DROPLISTMODE dropMode)
{
	BOOL bValid = FALSE;
	struct _stat buf;
	int result = _tstat(csPathname, &buf);
	if (result == 0)
	{
		if ((dropMode.iMask & DL_ACCEPT_FOLDERS) && ((buf.st_mode & _S_IFDIR) == _S_IFDIR)) 
		{
			bValid = TRUE;
			iPathType = DL_FOLDER_TYPE;
		} 
		else if ((dropMode.iMask & DL_ACCEPT_FILES) && ((buf.st_mode & _S_IFREG) == _S_IFREG)) 
		{
			iPathType = DL_FILE_TYPE;
			if (dropMode.iMask & DL_FILTER_EXTENSION)
			{
				LPTSTR pszFileExt = PathFindExtension(csPathname);
				if (CString(pszFileExt).CompareNoCase(dropMode.csFileExt) == 0)
				{
					bValid = TRUE;
				}
			}
			else
			{
				bValid = TRUE;
			}
		}
	}
	return bValid;
}

void CDocumentFileList::DropFiles(HDROP dropInfo, DROPLISTMODE dropMode)
{
	// Insure desired format is there, and open clipboard
	//if (IsClipboardFormatAvailable(CF_HDROP) == false)
	//	return false;
	//if (OpenClipboard(NULL) == false)
	//	return false;

	// Get handle to Dropped Filelist data, and number of files.
	HANDLE lData = GetClipboardData(CF_HDROP);

	UINT nNumFilesDropped = DragQueryFile((HDROP)dropInfo, 0xFFFFFFFF, NULL, 0);
	TCHAR szFilename[MAX_PATH + 1];
	CString csPathname;
	CString csExpandedFilename;
	CStringList lFilesList;

	for (UINT nFile = 0; nFile < nNumFilesDropped; nFile++)
	{
		DragQueryFile(dropInfo, nFile, szFilename, MAX_PATH + 1);

		csPathname = szFilename;
		csExpandedFilename = ExpandShortcut(csPathname);
		if (!csExpandedFilename.IsEmpty())
			csPathname = csExpandedFilename;

		UINT iPathType = 0;
		if (ValidatePathname(csPathname, iPathType, dropMode))
		{
			if (iPathType == DL_FILE_TYPE)
				lFilesList.AddTail(csPathname);
			else
			{
				LoadFilesFromDisk(csPathname, true, &lFilesList);
			}
		}
	}
	
	//DragFinish(dropInfo);
	InsertFiles(lFilesList);
	//CloseClipboard();
}

bool CDocumentFileList::PasteFiles()
{
	// Insure desired format is there, and open clipboard
	if (IsClipboardFormatAvailable(CF_HDROP) == false)
		return false;
	
	if (::OpenClipboard(NULL) == false)
		return false;

	// Get handle to Dropped Filelist data, and number of files
	bool lResult = true;
	try
	{
		HANDLE lData = GetClipboardData(CF_HDROP);
		if (lData != NULL)
		{
			DROPLISTMODE lDropMode;
			lDropMode.iMask = DL_ACCEPT_FILES | DL_ACCEPT_FOLDERS;
			DropFiles((HDROP)lData, lDropMode);
			lResult = true;
		}
	}
	catch(...)
	{
		lResult = false;
	}
	
	CloseClipboard();
	return lResult;
}

///////////////////////////////////////////////////////////////////////////////

void CDocumentFileList::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CMDIChildWnd * pChild = ((CMDIFrameWnd*)(AfxGetApp()->m_pMainWnd))->MDIGetActive();
	CLibraryView* pView = DYNAMIC_DOWNCAST(CLibraryView, pChild->GetActiveView ());
	pView->OnOpenFileWrapper();

	//AfxGetMainWnd()->SendMessage(WM_COMMAND, ID_OPEN);
	
	CBCGPListCtrl::OnLButtonDblClk(nFlags, point);
}

//*****************************************************************************
void CDocumentFileList::Sort (int iColumn, BOOL bAscending, BOOL bAdd)
{
	// Set Sort column
	SetSortColumn(iColumn, bAscending, bAdd);

	m_iColumn = iColumn;
	m_bAscending = bAscending;

	((CLibraryView*)GetParent())->Refresh();
}

//*****************************************************************************
//void CDocumentFileList::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
//{
//	BOOL lCtrl = FALSE;
//	if (GetKeyState(VK_LCONTROL) & 0x8000 || GetKeyState(VK_RCONTROL) & 0x8000)
//		lCtrl = TRUE;
//
//	if (nChar == VK_DELETE)
//	{
//		((CMainFrame*)AfxGetApp()->m_pMainWnd)->SendMessage(WM_COMMAND, ID_DELETE_FILE);
//	}
//	else if (nChar == VK_RETURN)    // open file
//	{
//		((CMainFrame*)AfxGetApp()->m_pMainWnd)->SendMessage(WM_COMMAND, ID_OPEN);
//	} 
//	else if (nChar == VK_F2)	// rename file
//	{
//		((CMainFrame*)AfxGetApp()->m_pMainWnd)->SendMessage(WM_COMMAND, ID_RENAME_FILE);
//	} 
//	else CBCGPListCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
//}

//*****************************************************************************
// selection
void CDocumentFileList::OnLbnSelchange(NMHDR *pNMHDR, LRESULT *pResult)
{
	if (!mRefresh) return;
	
	((CLibraryView*)GetParent())->RefreshPreview();
}

void CDocumentFileList::SelectAll()
{
	int lCount = GetItemCount();
	if (lCount == 0) return;

	::AfxGetApp()->DoWaitCursor(TRUE);
	mRefresh = false;
	try
	{
		for (int i = 0; i < lCount; i++)
			SetItemState(i, LVIS_SELECTED, LVIS_SELECTED);
	}
	catch(...)
	{
	}
	mRefresh = true;
	::AfxGetApp()->DoWaitCursor(FALSE);

	((CLibraryView*)GetParent())->RefreshPreview();
}

void CDocumentFileList::SelectItems(CIntArray& inSelItemsArray)
{
	int lCount = (int)inSelItemsArray.GetCount();
	if(lCount <= 0) return;

	int lItemsCount = GetItemCount();
	if (lItemsCount == 0) return;

	::AfxGetApp()->DoWaitCursor(TRUE);
	mRefresh = false;
	try
	{
		int lItemIndex = 0;
		for (int i = 0; i < lCount; i++)
		{
			lItemIndex = inSelItemsArray.GetAt(i);
			if (lItemIndex < lItemsCount)
				SetItemState(lItemIndex, LVIS_SELECTED, LVIS_SELECTED);
		}
	}
	catch(...)
	{
	}
	::AfxGetApp()->DoWaitCursor(FALSE);
	mRefresh = true;

	int lItemIndex = inSelItemsArray.GetAt(0);
	EnsureVisible(lItemIndex, FALSE);

	((CLibraryView*)GetParent())->RefreshPreview();
	SetFocus();
}

void CDocumentFileList::RowsFromIDs(CIntArray* inIDs, CIntArray& outRows)
{
	CMDIChildWnd * pChild = ((CMDIFrameWnd*)(AfxGetApp()->m_pMainWnd))->MDIGetActive();
	if(pChild == NULL)
		return;
	CLibraryDoc* pDoc = DYNAMIC_DOWNCAST(CLibraryDoc, pChild->GetActiveDocument ());

	CIntArray lIDs;
	lIDs.Copy(*inIDs);
	outRows.RemoveAll();
	int lCount = lIDs.GetCount();
	if (lCount == 0) return;

	for (int i = 0; i < GetItemCount(); i++)
	{
		int lID = m_pDoc->sFilesArray[i].mID;
		for (int j = 0; j < lIDs.GetCount(); j++)
		{
			if (lID == lIDs.GetAt(j))
			{
				outRows.Add(i);
				lIDs.RemoveAt(j);
				lCount--;
				break;
			}
		}
		if (lCount == 0) break;
	}
}

void CDocumentFileList::SetupListCtrl(int nStyle)
{
	CMDIChildWnd * pChild = ((CMDIFrameWnd*)(AfxGetApp()->m_pMainWnd))->MDIGetActive();
	if(pChild == NULL)
		return;
	CLibraryDoc* pDoc = DYNAMIC_DOWNCAST(CLibraryDoc, pChild->GetActiveDocument ());

	PrepareImageLists(m_pDoc->sFilesArray.GetCount());

	ClearCache(0, m_pDoc->sFilesArray.GetCount()-1);

	LONG mListStyle = GetWindowLong(GetSafeHwnd(), GWL_STYLE);
	SetImageList(&sSmallImageList, LVSIL_SMALL);	
	
	switch(nStyle)
	{
	case FILE_VIEW_ICONS:
		SetImageList(&sLargeImageList, LVSIL_NORMAL);
		SetImageList(&sStateImageList24, LVSIL_STATE);	
		mListStyle &= ~LVS_TYPEMASK;
		mListStyle |= LVS_ICON;
		break;
	case FILE_VIEW_THUMBNAILS:
		SetImageList(&sThmbImageList, LVSIL_NORMAL);
		SetImageList(&sStateImageList32, LVSIL_STATE);	
		mListStyle &= ~LVS_TYPEMASK;
		mListStyle |= LVS_ICON;
		break;
	case FILE_VIEW_LIST:
		SetImageList(&sLargeImageList, LVSIL_NORMAL);
		SetImageList(&sStateImageList16, LVSIL_STATE);	
		mListStyle &= ~LVS_TYPEMASK;
		mListStyle |= LVS_REPORT;
		break;
	default:
		break;
	}

	SetWindowLong(GetSafeHwnd(), GWL_STYLE, mListStyle);
	
	SetImageList(&sSmallImageList, LVSIL_SMALL);	
	SetItemCountEx(m_pDoc->sFilesArray.GetCount());
}

void CDocumentFileList::PrepareImageLists(int inCount)
{
	int lColorBit = 16;
	
	if (sThmbImageList.GetSafeHandle() != NULL)
		sThmbImageList.DeleteImageList();
	sThmbImageList.Create(100, 100, lColorBit, 1, 1);
	sThmbImageList.SetBkColor(GetBkColor());
	sThmbImageList.SetImageCount(inCount);

	if (sSmallImageList.GetSafeHandle() != NULL)
		sSmallImageList.DeleteImageList();
	sSmallImageList.Create(18, 18, lColorBit, 1, 1);
	sSmallImageList.SetBkColor(GetBkColor());
	sSmallImageList.SetImageCount(inCount);

	if (sLargeImageList.GetSafeHandle() != NULL)
		sLargeImageList.DeleteImageList();
	sLargeImageList.Create(32, 32, lColorBit, 1, 1);
	sLargeImageList.SetBkColor(GetBkColor());
	sLargeImageList.SetImageCount(inCount);

	if (sStateImageList16.GetSafeHandle() != NULL)
		sStateImageList16.DeleteImageList();
	sStateImageList16.Create(16, 16, lColorBit, 1, 1);
	sStateImageList16.SetBkColor(GetBkColor());

	HICON hIcon = (HICON)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_IN_WORKSPACE), IMAGE_ICON, 16, 16, 0);
	sStateImageList16.Add(hIcon);
	DestroyIcon(hIcon);

	if (sStateImageList24.GetSafeHandle() != NULL)
		sStateImageList24.DeleteImageList();
	sStateImageList24.Create(24, 24, lColorBit, 1, 1);
	sStateImageList24.SetBkColor(GetBkColor());

	hIcon = (HICON)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_IN_WORKSPACE), IMAGE_ICON, 24, 24, 0);
	sStateImageList24.Add(hIcon);
	DestroyIcon(hIcon);

	if (sStateImageList32.GetSafeHandle() != NULL)
		sStateImageList32.DeleteImageList();
	sStateImageList32.Create(32, 32, lColorBit, 1, 1);
	sStateImageList32.SetBkColor(GetBkColor());

	hIcon = (HICON)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_IN_WORKSPACE), IMAGE_ICON, 32, 32, 0);
	sStateImageList32.Add(hIcon);
	DestroyIcon(hIcon);
}

void CDocumentFileList::StopPrepareThumbnail()
{
//	CWinApp *pApp = AfxGetApp();
	if ( !m_bRunning )
	{
        return;
	}

	ResetEvent(m_hWaitingThread);
//    m_bTerminate = true;

    for(;;)
	{
        if ( WaitForSingleObject(m_hWaitingThread, 0) == WAIT_OBJECT_0 )
            break;
	    MSG msg;
		PeekMessage(&msg,NULL,0,0,PM_NOREMOVE);
/*        while (PeekMessage(&msg,NULL,0,0,PM_NOREMOVE)) 
		{ 
			if(pApp)
	            if (!pApp->PumpMessage())
	                break;
		} */
	}
//    CloseHandle(m_hThread);
    return;
}

void CDocumentFileList::StartPrepareThumbnail()
{
	m_hThread = AfxBeginThread((AFX_THREADPROC)LoadThumbnail,
		 					   (LPVOID)this, 
							   THREAD_PRIORITY_NORMAL,
							   0,
							   0/* CREATE_SUSPENDED*/,
							   NULL); 
	m_bRunning = true;
}

//*****************************************************************************
// Caching for DocumentFilesControls (List, Grid)
bool CDocumentFileList::IsPreparedCache(int inFrom, int inTo)
{
	CMDIChildWnd * pChild = ((CMDIFrameWnd*)(AfxGetApp()->m_pMainWnd))->MDIGetActive();
	CLibraryDoc* pDoc = DYNAMIC_DOWNCAST(CLibraryDoc, pChild->GetActiveDocument ());
	if(pDoc == NULL)
		return false;

	int lCount = (int)m_pDoc->sFilesArray.GetCount();
	if (inFrom > lCount) 
		return false;
	if (inTo >= lCount) 
		inTo = lCount-1;

	for (int i=inFrom; i<=inTo; i++)
	{
		if (m_pDoc->sFilesArray[i].mWasRead == false)
			return true;
	}
	return false;
}

bool CDocumentFileList::ClearCache(int inFrom, int inTo)
{
	CMDIChildWnd * pChild = ((CMDIFrameWnd*)(AfxGetApp()->m_pMainWnd))->MDIGetActive();
	CLibraryDoc* pDoc = DYNAMIC_DOWNCAST(CLibraryDoc, pChild->GetActiveDocument ());
	if(pDoc == NULL)
		return false;

	try
	{
		int lCount = (int)m_pDoc->sFilesArray.GetCount();
		if (inFrom >= lCount)
			return false;
		if (inTo >= lCount)
			inTo = lCount-1;

		for (int i = inFrom; i<=inTo; i++)
		{
			m_pDoc->sFilesArray[i].mWasRead = false;
			m_pDoc->sFilesArray[i].mPreviewID = -1;
			m_pDoc->sFilesArray[i].mInWorkspace = false;
		}
	}
	catch(...)
	{
		return false;
	}
	return true;
}

// inShowType 0: Thumbnail
//			  1: Icon
//			  2: List
bool CDocumentFileList::PrepareCache(int inFrom, int inTo, int nType, bool bPrepareAll)
{
	if (inFrom >= m_pDoc->sFilesArray.GetCount())
		return false;
	if (inTo >= m_pDoc->sFilesArray.GetCount())
		inTo = (int)m_pDoc->sFilesArray.GetCount()-1;

	try
	{
		/*m_pFilesSQL->mSpecCondition.RemoveAll();
		CString lStr;
		lStr.Format(_T("Files.idFile = %d"), m_pDoc->sFilesArray[inFrom].mID);
		m_pFilesSQL->AddSpecCondition(lStr);*/

		m_pFilesSQL->mSpecCondition.RemoveAll();
		CString lCondition = _T(""); 
		CString lStr;
		int i;
		for (i=inFrom; i<=inTo; i++)
		{
			if (i>inFrom) // or between ids
				lCondition += _T(" or ");

			if (i == inFrom) // starting (
				lCondition += _T("(");

			lStr.Format(_T("(Files.idFile = %d)"), m_pDoc->sFilesArray[i].mID);
			lCondition += lStr;

			if (i == inTo) // ending )
				lCondition += _T(")");
		}
		m_pFilesSQL->AddSpecCondition(lCondition);
		
		CString lSQLStr = m_pFilesSQL->GetSelect();
		if (lSQLStr.IsEmpty())
			return false;

		CTFiles lTFiles(m_pParamCDB);
		lTFiles.ExecSelect(lSQLStr);
		i = inFrom;
		while ((lTFiles.mDataIsValid) && (i<m_pDoc->sFilesArray.GetCount()))
		{
			m_pDoc->sFilesArray[i].mID = lTFiles.mID;
			m_pDoc->sFilesArray[i].mPath = lTFiles.mPath;
			m_pDoc->sFilesArray[i].mName = lTFiles.mName;

			m_pDoc->sFilesArray[i].mExt = _T("");
			if (lTFiles.mName.Find('.') != -1)
				m_pDoc->sFilesArray[i].mExt = lTFiles.mName.Right(lTFiles.mName.GetLength() - lTFiles.mName.ReverseFind('.') - 1);

			if (bPrepareAll)
			{
				//memcpy(&m_pDoc->sFilesArray[i].mFindData, &lTFiles.mFindData, sizeof(WIN32_FIND_DATA));
				m_pDoc->sFilesArray[i].mFindData.nFileSizeHigh = lTFiles.mFindData.nFileSizeHigh;
				m_pDoc->sFilesArray[i].mFindData.nFileSizeLow = lTFiles.mFindData.nFileSizeLow;
				m_pDoc->sFilesArray[i].mFindData.ftCreationTime.dwHighDateTime = lTFiles.mFindData.ftCreationTime.dwHighDateTime;
				m_pDoc->sFilesArray[i].mFindData.ftCreationTime.dwLowDateTime = lTFiles.mFindData.ftCreationTime.dwLowDateTime;
				m_pDoc->sFilesArray[i].mFindData.ftLastWriteTime.dwHighDateTime = lTFiles.mFindData.ftLastWriteTime.dwHighDateTime;
				m_pDoc->sFilesArray[i].mFindData.ftLastWriteTime.dwLowDateTime = lTFiles.mFindData.ftLastWriteTime.dwLowDateTime;
				m_pDoc->sFilesArray[i].mFindData.ftLastAccessTime.dwHighDateTime = lTFiles.mFindData.ftLastAccessTime.dwHighDateTime;
				m_pDoc->sFilesArray[i].mFindData.ftLastAccessTime.dwLowDateTime = lTFiles.mFindData.ftLastAccessTime.dwLowDateTime;
				m_pDoc->sFilesArray[i].mFindData.dwFileAttributes;
				m_pDoc->sFilesArray[i].mTitle = lTFiles.mTitle;
				m_pDoc->sFilesArray[i].mSubject = lTFiles.mSubject;
				m_pDoc->sFilesArray[i].mAuthor = lTFiles.mAuthor;
				m_pDoc->sFilesArray[i].mCategory = lTFiles.mCategory;
				m_pDoc->sFilesArray[i].mComments = lTFiles.mComments;
				m_pDoc->sFilesArray[i].mAddedDate.dwHighDateTime = lTFiles.mAddedDate.dwHighDateTime;
				m_pDoc->sFilesArray[i].mAddedDate.dwLowDateTime = lTFiles.mAddedDate.dwLowDateTime;
				m_pDoc->sFilesArray[i].mFlags = lTFiles.mFlags;
				m_pDoc->sFilesArray[i].mExtID = lTFiles.mExtID;

				m_pDoc->sFilesArray[i].mTags = lTFiles.mQ.getStringField(25); // virtual field TagsStr
				/*
				m_pDoc->sFilesArray[i].mTags = _T("");
				CTTags lTTags;
				CString lStr = _T("");
				if(	lTTags.GetTagsByFileID_Formatted(lStr, lTFiles.mID) > 0)
					m_pDoc->sFilesArray[i].mTags = lStr;*/
				
				m_pDoc->sFilesArray[i].mSize = _T("");
				CUtils::UFileSize2Str(m_pDoc->sFilesArray[i].mSize, 
					lTFiles.mFindData.nFileSizeHigh, lTFiles.mFindData.nFileSizeLow);
								
				// prepare date/times
				CTime lTimeCr(lTFiles.mFindData.ftCreationTime);
				m_pDoc->sFilesArray[i].mCreatedTmStmp = lTimeCr.Format(_T("%d %B %Y")); 

				CTime lTimeMod(lTFiles.mFindData.ftLastWriteTime);
				m_pDoc->sFilesArray[i].mModifiedTmStmp = lTimeMod.Format(_T("%d %B %Y")); 

				CTime lTimeAdded(lTFiles.mAddedDate);
				m_pDoc->sFilesArray[i].mAccessedTmStmp = lTimeAdded.Format(_T("%d %B %Y")); 

				CTime lTimeOpened(lTFiles.mFindData.ftLastAccessTime);
				m_pDoc->sFilesArray[i].mAddedTmStmp = lTimeOpened.Format(_T("%d %B %Y")); 

				// Atributes
				m_pDoc->sFilesArray[i].mAttr = _T("");
				if(lTFiles.mFlags == FILE_FLAG_FNF)
					m_pDoc->sFilesArray[i].mAttr = _T("MISSING FROM DISC");
				else
				{
					if ((lTFiles.mFindData.dwFileAttributes  & FILE_ATTRIBUTE_SYSTEM) == FILE_ATTRIBUTE_SYSTEM)
						m_pDoc->sFilesArray[i].mAttr +=	_T("S");
					if ((lTFiles.mFindData.dwFileAttributes  & FILE_ATTRIBUTE_READONLY) == FILE_ATTRIBUTE_READONLY)
						m_pDoc->sFilesArray[i].mAttr +=	_T("R");	
					if ((lTFiles.mFindData.dwFileAttributes  & FILE_ATTRIBUTE_HIDDEN) == FILE_ATTRIBUTE_HIDDEN)
						m_pDoc->sFilesArray[i].mAttr +=	_T("H");	
					if ((lTFiles.mFindData.dwFileAttributes  & FILE_ATTRIBUTE_ARCHIVE) == FILE_ATTRIBUTE_ARCHIVE)
						m_pDoc->sFilesArray[i].mAttr += _T("A");
					if ((lTFiles.mFindData.dwFileAttributes  & FILE_ATTRIBUTE_COMPRESSED) == FILE_ATTRIBUTE_COMPRESSED)
						m_pDoc->sFilesArray[i].mAttr +=	_T("C");	
				}
			}

			m_pDoc->sFilesArray[i].mInWorkspace = m_pViewInfo->pWorkspace->FileIsInWorkspace(lTFiles.mID);

			m_pDoc->sFilesArray[i].mCollections = lTFiles.mQ.getStringField(26); // virtual field CollectionsStr
			m_pDoc->sFilesArray[i].mWasRead = true;

			if(nType == 0 && m_pDoc->sFilesArray[i].mPreviewID == -1) {
				// done by thread
				break;
			} else if (nType == 2 && m_pDoc->sFilesArray[i].mPreviewID == -1) {

				CImageExtract mThumbExtract;
				HICON lIcon = mThumbExtract.ExtractIcon(m_pDoc->sFilesArray[i].mPath, true);
				sSmallImageList.Replace(i, lIcon);
				DestroyIcon(lIcon);

			} else if (nType == 1 && m_pDoc->sFilesArray[i].mPreviewID == -1) {

				CImageExtract mThumbExtract;
				HICON lIcon = mThumbExtract.ExtractIcon(m_pDoc->sFilesArray[i].mPath, false);
				sLargeImageList.Replace(i, lIcon);
				DestroyIcon(lIcon);
			}

			m_pDoc->sFilesArray[i].mPreviewID = i;
			lTFiles.LoadNextRow();
			i++;
		}
	}
	catch(...)
	{
		return false;
	}
	return true;
}

UINT CDocumentFileList::LoadThumbnail(LPVOID lpParam)
{
	CImageExtract mThumbExtract;
//	int inFrom = 0, inTo = m_pDoc->sFilesArray.GetCount() - 1;
	CDocumentFileList *pFileList = (CDocumentFileList *)lpParam;
	int inFrom = pFileList->nStartItem, inTo = pFileList->nEndItem;
	
	CLibraryDoc *pDocument = pFileList->m_pDoc;
	CBWorkspace *pWorkspace = pDocument->m_ViewInfo.pWorkspace;
	try
	{
		/*m_pFilesSQL->mSpecCondition.RemoveAll();
		CString lStr;
		lStr.Format(_T("Files.idFile = %d"), m_pDoc->sFilesArray[inFrom].mID);
		m_pFilesSQL->AddSpecCondition(lStr);*/

		// Attempt to lock the shared resource
		int i;
		/*
		CString lCondition = _T(""); 
		CString lStr;
		for (i=inFrom; i<=inTo; i++)
		{
			if (i>inFrom) // or between ids
				lCondition += _T(" or ");

			if (i == inFrom) // starting (
				lCondition += _T("(");

			lStr.Format(_T("(Files.idFile = %d)"), m_pDoc->sFilesArray[i].mID);
			lCondition += lStr;

			if (i == inTo) // ending )
				lCondition += _T(")");
		}
		m_pFilesSQL->AddSpecCondition(lCondition);
		

		CString lSQLStr = m_pFilesSQL->GetSelect();
		if (lSQLStr.IsEmpty())
			return -1;

		CTFiles lTFiles(m_pParamCDB);
		lTFiles.ExecSelect(lSQLStr);*/
		i = inFrom;
		while (i <= inTo && pFileList->m_bTerminate != true)
		{
			CString lStr;
			CTFiles lTFiles(pDocument->m_pParamCDB);
			CString lSQLStr;

			pFileList->m_pFilesSQL->mSpecCondition.RemoveAll();
			lStr.Format(_T("Files.idFile = %d"), pDocument->sFilesArray[i].mID);
			pFileList->m_pFilesSQL->AddSpecCondition(lStr);
			lSQLStr = pFileList->m_pFilesSQL->GetSelect();
			if (lSQLStr.IsEmpty())
				break;

			lTFiles.ExecSelect(lSQLStr);
			if(!lTFiles.mDataIsValid)
				break;

			pDocument->sFilesArray[i].mID = lTFiles.mID;
			pDocument->sFilesArray[i].mPath = lTFiles.mPath;
			pDocument->sFilesArray[i].mName = lTFiles.mName;

			pDocument->sFilesArray[i].mExt = _T("");
			if (lTFiles.mName.Find('.') != -1)
				pDocument->sFilesArray[i].mExt = lTFiles.mName.Right(lTFiles.mName.GetLength() - lTFiles.mName.ReverseFind('.') - 1);

			pDocument->sFilesArray[i].mInWorkspace = pWorkspace->FileIsInWorkspace(lTFiles.mID); 

			pDocument->sFilesArray[i].mCollections = lTFiles.mQ.getStringField(26); // virtual field CollectionsStr
			pDocument->sFilesArray[i].mWasRead = true;

			if(pDocument->sFilesArray[i].mPreviewID == -1) { // thumbnail

				// prepare preview
				CString s = pDocument->sFilesArray[i].mPath;

				mThumbExtract.SetPath(pDocument->sFilesArray[i].mPath);	
				mThumbExtract.SetThumbnailSize(100, 100);	
				HBITMAP lHBitmap = NULL;
				bool bRet = mThumbExtract.ExtractThumbnail(lHBitmap);
				if (bRet && (lHBitmap != NULL))
				{
					CBitmap* lBitmap = CBitmap::FromHandle(lHBitmap);
					BITMAP lBitmapInfo;
					lBitmap->GetBitmap(&lBitmapInfo);
					
					if (lBitmapInfo.bmWidth == 100 && lBitmapInfo.bmHeight == 100)
					{
						pFileList->sThmbImageList.Replace(i, lBitmap, NULL);
					}
					else
					{ // need to be centered
						CDC lBitmapDC;
						lBitmapDC.CreateCompatibleDC(NULL);

						// Moved this here to select the bitmap before creating the compatible destination bitmap
						CBitmap *lOldBitmap1 = lBitmapDC.SelectObject(lBitmap);
						
						CBitmap lDestBitmap;
						lDestBitmap.CreateCompatibleBitmap(&lBitmapDC, 100, 100);
						CDC lDestDC;
						lDestDC.CreateCompatibleDC(NULL);

						CBitmap *lOldBitmap2 = lDestDC.SelectObject(&lDestBitmap);

						// center the bitmap
						ldiv_t lLeft = div((long)100-lBitmapInfo.bmWidth, (long)2);
						ldiv_t lTop = div((long)100-lBitmapInfo.bmHeight, (long)2);
						lDestDC.FillSolidRect(0, 0, 100, 100, pFileList->GetBkColor());
						lDestDC.BitBlt(lLeft.quot, lTop.quot, lBitmapInfo.bmWidth, lBitmapInfo.bmHeight, &lBitmapDC, 0, 0, SRCCOPY);

						lBitmapDC.SelectObject(lOldBitmap1);
						lDestDC.SelectObject(lOldBitmap2);

						pFileList->sThmbImageList.Replace(i, &lDestBitmap, NULL);
						lDestBitmap.DeleteObject();
						lBitmapDC.DeleteDC();
					}
					lBitmap->DeleteObject();
				}
				else
				{
					HICON lIcon = mThumbExtract.ExtractIcon(pDocument->sFilesArray[i].mPath, false);
					pFileList->sThmbImageList.Replace(i, lIcon);
					DestroyIcon(lIcon);
				}
			}
			pDocument->sFilesArray[i].mPreviewID = i;
			pFileList->RedrawItems(i, i);
			//lTFiles.LoadNextRow();
			i++;
		}
	}
	catch(...)
	{
	}

	pFileList->m_bRunning = false;
	pFileList->m_bTerminate = false;

	SetEvent(pFileList->m_hWaitingThread);
	AfxEndThread( 0 );

	return 0;
}

//*****************************************************************************
void CDocumentFileList::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{

	CMDIChildWnd * pChild = ((CMDIFrameWnd*)(AfxGetApp()->m_pMainWnd))->MDIGetActive();
	CLibraryView* pView = DYNAMIC_DOWNCAST(CLibraryView, pChild->GetActiveView ());

	const BOOL lShift = (::GetAsyncKeyState (VK_SHIFT) & 0x8000) != 0;
	const BOOL lCtrl = (::GetAsyncKeyState (VK_CONTROL) & 0x8000) != 0;

	// delete selected
	if (nChar == VK_DELETE)
	{
		((CMainFrame*)AfxGetApp()->m_pMainWnd)->SendMessage(WM_COMMAND, ID_DELETE_FILE);
	}
	else if(nChar == VK_RETURN) // open selected file
	{
		pView->OnOpenFileWrapper();
	}
	else if(nChar == VK_F2) // rename file
	{
		((CMainFrame*)AfxGetApp()->m_pMainWnd)->SendMessage(WM_COMMAND, ID_RENAME_FILE);
	}
	else if(nChar == 'A' && lCtrl)
	{
		SelectAll();
	}
	//?else if((nChar>='0') && (nChar<='Z') && !lCtrl)
	//{ 
	//	/* vytvorim select taky isty ako je current select, ale aby zaciatocne pismeno bolo mensie 
	//	ako stlacene pismeno. Ak zistim pocet takych zaznamov, dostanem cislo riadku, na ktory sa
	//	mam posunut.*/
	//	try
	//	{
	//		CString lKeyDownChar; // current key
	//		lKeyDownChar.Format(_T("%c"), nChar);

	//		CTime lCurTime = CTime::GetCurrentTime();
	//		CTimeSpan lDiff = lCurTime - mKeyDownTime;
	//		if (lDiff < 1)
	//			mKeyDownStr += lKeyDownChar;
	//		else
	//			mKeyDownStr = lKeyDownChar;

	//		bool lSameChars = false;
	//		if (mKeyDownStr.GetLength() > 1)
	//		{
	//			lSameChars = true;
	//			for (int i=0; i<mKeyDownStr.GetLength(); i++)
	//			if (mKeyDownStr.GetAt(i) != lKeyDownChar)
	//				lSameChars = false;
	//		}

	//		CString lWhatToFindStr = mKeyDownStr;
	//		if (lSameChars)
	//			lWhatToFindStr = lKeyDownChar;

	//		mKeyDownTime = lCurTime;

	//		CDBSQL lDBSQL;
	//		m_pFilesSQL->Copy(&lDBSQL);

	//		/* tento select mozem vyskladat aj inym sposobom - select count from select....
	//		lDBSQL.mField.RemoveAll();
	//		lDBSQL.AddField(_T("count (*)"));
	//		lDBSQL.mField.AddHead(_T("count (*)"));
	//		
	//		alebo:
	//		SELECT * FROM table
	//			WHERE rowid > (SELECT rowid FROM table
	//			WHERE folder_key = 1
	//			AND file_name='cbc.txt')
	//			ORDER BY rowid
	//			LIMIT 1
	//		*/

	//		lDBSQL.mSort.RemoveAll();
	//		m_pDoc->UpdateSQLSort(&lDBSQL);

	//		lDBSQL.mSpecCondition.RemoveAll();
	//		CString lStr;
	//		
	//		// starts width
	//		int lColumn = this->m_iColumn;
	//		if (lColumn == -1) lColumn = 0;
	//		int lColumnState = m_bAscending;
	//		CString lFieldName;
	//		switch(lColumn){
	//			case 1: lFieldName = _T("upper(Files.Title)"); break;
	//			case 2: lFieldName = _T("upper(Files.Author)"); break;
	//			case 3: lFieldName = _T("upper(Files.Subject)"); break;
	//			case 4: lFieldName = _T("upper(TagsStr)"); break;
	//			case 5: lFieldName = _T("upper(CollectionsStr)"); break;
	//			case 6: lFieldName = _T("upper(FExtensions.Name)"); break;
	//			case 7: lFieldName = _T("Files.SizeLo"); break; 
	//			case 8: lFieldName = _T("upper(Files.Category)"); break;
	//			case 9: lFieldName = _T("upper(Files.Path)"); break;
	//			case 10: lFieldName = _T("Files.Created_TmStmp"); break;
	//			case 11: lFieldName = _T("Files.Modified_TmStmp"); break;
	//			case 12: lFieldName = _T("Files.Added_TmStmp"); break;
	//			case 13: lFieldName = _T("Files.Accessed_TmStmp"); break;
	//			case 14: lFieldName = _T("Files.Attributes"); break;
	//			case 15: lFieldName = _T("upper(Files.Comments)"); break;
	//			default: lFieldName = _T("upper(Files.Name)"); break;
	//		}

	//		/////////////////////////////////////////////////////////////////
	//		/* first check if there is such string starting with mKeyDownStr. mKeyDownStr cannot be made
	//		from the same letters.
	//		Only in that case make the appropriate selection.*/
	//		CString lSQLStr1;
	//		CTFiles lTable(m_pParamCDB);
	//		if (lColumn == 7)
	//			lStr.Format(_T("%s = %s%%"), lFieldName, lWhatToFindStr);
	//		else
	//			lStr.Format(_T("%s LIKE '%s%%'"), lFieldName, lWhatToFindStr); //starts with

	//		lDBSQL.AddSpecCondition(lStr);
	//		lSQLStr1 = lDBSQL.GetSelect();
	//		if (lSQLStr1.IsEmpty())
	//			throw(0);

	//		lTable.mQ = m_pDB->mDB.execQuery(lSQLStr1);
	//		if(lTable.mQ.eof())
	//			throw(0);

	//		lDBSQL.mSpecCondition.RemoveAll();

	//		/////////////////////////////////////////////////////////////////
	//		if (lColumn == 7)
	//		{
	//			if (!lColumnState)
	//				lStr.Format(_T("%s > %s"), lFieldName, lWhatToFindStr);
	//			else //asc
	//				lStr.Format(_T("%s < %s"), lFieldName, lWhatToFindStr);
	//		}
	//		else
	//		{
	//			/* replacment in the select:
	//			 "'", "''"
	//			"[", "[[]"
	//			"%", "[%]"
	//			"^", "[^]"
	//			"#", "[#]"
	//			"_", "[_]"
	//			*/

	//			// _ robi problemy a ak je subor, ktory ma _ na zaciatku v DB, nevie ho porovnat!
	//			if (!lColumnState)
	//				lStr.Format(_T("%s > '%s%%'"), lFieldName, lWhatToFindStr);
	//			else //asc
	//				//lStr.Format(_T("((%s < '%s %%') OR (%s is NULL) OR (%s = '') OR (%s = upper('[_]%%')))"), lFieldName, mKeyDownStr, lFieldName, lFieldName, lFieldName);
	//				lStr.Format(_T("((%s < '%s %%') OR (%s is NULL) OR (%s = ''))"), lFieldName, lWhatToFindStr, lFieldName, lFieldName);
	//		}
	//		lDBSQL.AddSpecCondition(lStr);

	//		lSQLStr1 = lDBSQL.GetSelect();
	//		lSQLStr1.Delete(lSQLStr1.GetLength()-2,2);
	//		CString lSQLStr = _T("select count(*) from (") + lSQLStr1 + _T(");");
	//		
	//		if (lSQLStr.IsEmpty()) 
	//			throw(0);
	//		
	//		lTable.mQ = m_pDB->mDB.execQuery(lSQLStr);
	//		if(!lTable.mQ.eof())
	//		{
	//			int lCount = lTable.mQ.getIntField(0);
	//			if (!lColumnState) lCount-=1;
	//			if (lCount < 0)
	//				lCount = 0;
	//			if (lCount >= GetItemCount())
	//				lCount = GetItemCount()-1;

	//			/////////////////////////////////////////////////////////////////
	//			/* When you press a letter/number -- the first file after the currently selected file 
	//			should be selected. It is the way that Windows Explorer operates. But if you type
	//			more then one characters (e.g. "fo"), I ignore the selection*/
	//			
	//			int lSelRow = lCount;
	//			/* go through the selection and find if there is a row with the same started string already selected*/
	//			if (lWhatToFindStr.GetLength() <= 1)
	//			{
	//				POSITION pos = GetFirstSelectedItemPosition();
	//				if (pos != NULL)
	//				{ // there was a selection
	//					bool lIsSelection = false;
	//					while (pos != NULL) 
	//					{		
	//						int index = GetNextSelectedItem(pos);
	//						/*CBCGPGridRow* lRow = GetRow(index);
	//						CBCGPGridItem* lItem = lRow->GetItem(lColumn);*/
	//						CString lStr = m_pDoc->sFilesArray[index].mName;
	//						lStr.MakeUpper();
	//						int lFind = lStr.Find(lWhatToFindStr);
	//						if (lFind == 0)
	//						{
	//							lIsSelection = true;
	//							if (index > lCount)
	//								lCount = index;
	//						}
	//					}

	//					if (lIsSelection)
	//					{
	//						lCount++;
	//						if (lCount >= GetItemCount())
	//							lCount = lSelRow;

	//						// check the selection - if it still starts with mKeyDownStr
	//						//if (m_pDoc->CTFilesArrayNeedRead(lCount, lCount))
	//						//	m_pDoc->PrepareCache(lCount, lCount);
	//						//CBCGPGridRow* lRow = GetRow(lCount);
	//						//CBCGPGridItem* lItem = lRow->GetItem(lColumn);
	//						CString lStr = m_pDoc->sFilesArray[lCount].mName;
	//						lStr.MakeUpper();
	//						int lFind = lStr.Find(lWhatToFindStr);
	//						if (lFind != 0)
	//							lCount = lSelRow;
	//					}
	//				}
	//			}

	//			/////////////////////////////////////////////////////////////////
	//			//// select the desire row
	//			//CBCGPGridItemID lGridItem;
	//			//GetCurSel(lGridItem);
	//			//lGridItem.m_nRow = lCount;
	//			//SetCurSel(lGridItem);

	//			//CBCGPGridRow* lRow = GetRow(lCount);
	//			//if (lRow) 
	//			//	EnsureVisible(lRow);
	//			//SetFocus();
	//			SetItemState(lCount, LVIS_SELECTED, LVIS_SELECTED | LVIS_FOCUSED);
	//			ASSERT(GetItemState(lCount, LVIS_SELECTED) == LVIS_SELECTED);
	//			EnsureVisible(lCount, TRUE);
	//			
	//		}
	//	}
	//	catch(...)
	//	{
	//	}
	//} else
		CBCGPListCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CDocumentFileList::Set_ParamForCDB(PARAM_FOR_CDB *pParamCDB,LIBRARY_VIEW_INFO *pViewInfo)
{
	m_pParamCDB = pParamCDB;
	m_pFilesSQL = m_pParamCDB->pFilesSQL;
	m_pDB = m_pParamCDB->pDB;

	m_pViewInfo = pViewInfo;
}

void CDocumentFileList::Wrapper_OnLButtonDblClk(UINT nFlags, CPoint point)
{
	OnLButtonDblClk(nFlags,point);
}
void CDocumentFileList::Wrapper_OnLButtonDown(UINT nFlags, CPoint point)
{
	OnLButtonDown(nFlags,point);
}

CBCGPHeaderCtrl* CDocumentFileList::GetHeaderCtrl_MY()
{
	return &m_wndHeader;
}