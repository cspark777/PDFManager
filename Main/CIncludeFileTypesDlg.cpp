// CIncludeFileTypesDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Benubird.h"
#include "CIncludeFileTypesDlg.h"
#include "LibraryView.h"


// CIncludeFileTypesDlg dialog

IMPLEMENT_DYNAMIC(CIncludeFileTypesDlg, CDialog)

CIncludeFileTypesDlg::CIncludeFileTypesDlg(CIDData *ioIDData, CWnd* pParent /*=NULL*/)
	: CDialog(CIncludeFileTypesDlg::IDD, pParent)
{
	mFileTypes.Empty();
	mIDData = ioIDData;

	CMDIChildWnd * pChild = ((CMDIFrameWnd*)(AfxGetApp()->m_pMainWnd))->MDIGetActive();
	CLibraryDoc* pDoc = DYNAMIC_DOWNCAST(CLibraryDoc, pChild->GetActiveDocument ());
	m_pParamCDB = pDoc->m_pParamCDB;

}	

CIncludeFileTypesDlg::~CIncludeFileTypesDlg()
{
}

void CIncludeFileTypesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_INCLUDE_FILE_TYPES_LIST, m_IncludeFileTypesListCtrl);
}


BEGIN_MESSAGE_MAP(CIncludeFileTypesDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CIncludeFileTypesDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_SELECT_ALL, &CIncludeFileTypesDlg::OnBnClickedSelectAll)
	ON_BN_CLICKED(IDC_DESELECT_ALL, &CIncludeFileTypesDlg::OnBnClickedDeselectAll)
END_MESSAGE_MAP()


// CIncludeFileTypesDlg message handlers

BOOL CIncludeFileTypesDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CRect lRect;
	m_IncludeFileTypesListCtrl.GetClientRect(&lRect);
	int l1stColumnWidth = lRect.Width() / 4;
	int l2ndColumnWidth = lRect.Width() - l1stColumnWidth;

	m_IncludeFileTypesListCtrl.InsertColumn(0, _T("File type"), LVCFMT_LEFT, l1stColumnWidth);
	m_IncludeFileTypesListCtrl.InsertColumn(1, _T("Description"), LVCFMT_LEFT, l2ndColumnWidth);
	m_IncludeFileTypesListCtrl.SetExtendedStyle(m_IncludeFileTypesListCtrl.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_CHECKBOXES);
	//m_IncludeFileTypesListCtrl.EnableMultipleSort(FALSE);
//	m_IncludeFileTypesListCtrl.SetSortColumn(0,TRUE,TRUE);

	// -------------------------------
	m_IncludeFileTypesListCtrl.EnableMultipleSort();
	m_IncludeFileTypesListCtrl.SetSortColumn(0,TRUE,TRUE);
	m_IncludeFileTypesListCtrl.SetSortColumn(1,TRUE,TRUE);
	m_IncludeFileTypesListCtrl.SetSortColumn(2,TRUE,TRUE);
	// -------------------------------

	CTFExtensions lTFExt(m_pParamCDB->pDB);
	if(lTFExt.GetExtensions())
	{
		int lIdx = 0;
		while(lTFExt.mDataIsValid)
		{			
			int iItem = m_IncludeFileTypesListCtrl.InsertItem(lIdx, (LPCTSTR) lTFExt.mName);
			m_IncludeFileTypesListCtrl.SetItemText(iItem, 1, (LPCTSTR) lTFExt.mDescription);
			m_IncludeFileTypesListCtrl.SetItemData(iItem, (DWORD_PTR) lTFExt.mID);
			if(mIDData->GetIdxByID(lTFExt.mID) >= 0)
				m_IncludeFileTypesListCtrl.SetCheck(iItem);
				//m_IncludeFileTypesListCtrl.SetCheck(lIdx);
			lTFExt.LoadNextRow();
			lIdx++;
		}
	}

	//m_IncludeFileTypesListCtrl.Sort(1);
/*
	// Only for test
	int iItem = 0;
	iItem  = m_IncludeFileTypesListCtrl.InsertItem(0, _T(".doc"));
	m_IncludeFileTypesListCtrl.SetItemText(iItem, 1, _T("Microsoft Word 1997-2003 Document"));
	
	iItem = m_IncludeFileTypesListCtrl.InsertItem(1, _T(".xls"));
	m_IncludeFileTypesListCtrl.SetItemText(iItem, 1, _T("Microsoft Excel 1997-2003 Worksheet"));

	iItem = m_IncludeFileTypesListCtrl.InsertItem(2, _T(".ppt"));
	m_IncludeFileTypesListCtrl.SetItemText(iItem , 1, _T("Microsoft PowerPoint 1997-2003 Presentation"));

	iItem = m_IncludeFileTypesListCtrl.InsertItem(3, _T(".pdf"));
	m_IncludeFileTypesListCtrl.SetItemText(iItem, 1, _T("PDF Document"));

	iItem = m_IncludeFileTypesListCtrl.InsertItem(4, _T(".rtf"));
	m_IncludeFileTypesListCtrl.SetItemText(iItem, 1, _T("Rich Text Format"));

	iItem = m_IncludeFileTypesListCtrl.InsertItem(5, _T(".xps"));
	m_IncludeFileTypesListCtrl.SetItemText(iItem, 1, _T("XPS Document"));

	iItem = m_IncludeFileTypesListCtrl.InsertItem(6, _T(".jpg"));
	m_IncludeFileTypesListCtrl.SetItemText(iItem, 1, _T("JPG Image"));
	m_IncludeFileTypesListCtrl.InsertItem(7, _T(".gif"));
	m_IncludeFileTypesListCtrl.SetItemText(7, 1, _T("GIF Image"));
	m_IncludeFileTypesListCtrl.InsertItem(8, _T(".tif"));
	m_IncludeFileTypesListCtrl.SetItemText(8, 1, _T("TIF Image"));
	m_IncludeFileTypesListCtrl.InsertItem(9, _T(".bmp"));
	m_IncludeFileTypesListCtrl.SetItemText(9, 1, _T("BMP Image"));
	m_IncludeFileTypesListCtrl.InsertItem(10, _T(".png"));
	m_IncludeFileTypesListCtrl.InsertItem(11, _T(".ico"));
	m_IncludeFileTypesListCtrl.InsertItem(12, _T(".html"));
	m_IncludeFileTypesListCtrl.InsertItem(13, _T(".js"));
	m_IncludeFileTypesListCtrl.InsertItem(14, _T(".css"));
	m_IncludeFileTypesListCtrl.InsertItem(15, _T(".txt"));
	m_IncludeFileTypesListCtrl.InsertItem(16, _T(".zip"));
	m_IncludeFileTypesListCtrl.InsertItem(17, _T(".rar"));
	m_IncludeFileTypesListCtrl.InsertItem(18, _T(".iso"));
	m_IncludeFileTypesListCtrl.InsertItem(19, _T(".csv"));
	m_IncludeFileTypesListCtrl.InsertItem(20, _T(".xml"));
	m_IncludeFileTypesListCtrl.InsertItem(21, _T(".docx"));
	m_IncludeFileTypesListCtrl.InsertItem(22, _T(".dot"));
	m_IncludeFileTypesListCtrl.InsertItem(23, _T(".dotx"));
	m_IncludeFileTypesListCtrl.InsertItem(24, _T(".egt"));
	m_IncludeFileTypesListCtrl.InsertItem(25, _T(".odt"));
	m_IncludeFileTypesListCtrl.InsertItem(26, _T(".ott"));
	m_IncludeFileTypesListCtrl.InsertItem(27, _T(".wpd"));
	m_IncludeFileTypesListCtrl.InsertItem(28, _T(".wpt"));
	m_IncludeFileTypesListCtrl.InsertItem(29, _T(".wri"));
	m_IncludeFileTypesListCtrl.InsertItem(30, _T(".xhtml"));
	m_IncludeFileTypesListCtrl.InsertItem(31, _T(".ps"));
	m_IncludeFileTypesListCtrl.InsertItem(32, _T(".odp"));
	m_IncludeFileTypesListCtrl.InsertItem(33, _T(".otp"));
	m_IncludeFileTypesListCtrl.InsertItem(34, _T(".pps"));
	m_IncludeFileTypesListCtrl.InsertItem(35, _T(".sti"));
	m_IncludeFileTypesListCtrl.InsertItem(36, _T(".sxi"));
	m_IncludeFileTypesListCtrl.InsertItem(37, _T(".ods"));
	m_IncludeFileTypesListCtrl.InsertItem(38, _T(".ots"));
	m_IncludeFileTypesListCtrl.InsertItem(39, _T(".stc"));
	m_IncludeFileTypesListCtrl.InsertItem(40, _T(".sxc"));
	m_IncludeFileTypesListCtrl.InsertItem(41, _T(".xlk"));
	m_IncludeFileTypesListCtrl.InsertItem(42, _T(".xlsx"));
	m_IncludeFileTypesListCtrl.InsertItem(43, _T(".exe"));
	m_IncludeFileTypesListCtrl.InsertItem(44, _T(".msi"));
	m_IncludeFileTypesListCtrl.InsertItem(45, _T(".psd"));
	m_IncludeFileTypesListCtrl.InsertItem(46, _T(".tiff"));
*/
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CIncludeFileTypesDlg::OnBnClickedOk()
{
	CString lExtension;
	int lNumCount = m_IncludeFileTypesListCtrl.GetItemCount();
	mIDData->RemoveAll();
	for(int i = 0; i < lNumCount; i++)
	{
		if (m_IncludeFileTypesListCtrl.GetCheck(i))
		{
			int lID = (int) m_IncludeFileTypesListCtrl.GetItemData(i);
			CString lExt = m_IncludeFileTypesListCtrl.GetItemText(i, 0);
			mIDData->AddData(lID, lExt);
		}
	}
/*	for (int i =0; i <= 46; i++)
	{
		if (m_IncludeFileTypesListCtrl.GetCheck(i))
		{
			lExtension = m_IncludeFileTypesListCtrl.GetItemText(i, 0);
			mFileTypes += lExtension;
			mFileTypes += _T("; ");
		}
	}
	if (!mFileTypes.IsEmpty())
	{
		mFileTypes = mFileTypes.Left(mFileTypes.ReverseFind(','));
	}
*/
	OnOK();
}
//*****************************************************************************
int CIncludeFileTypesDlg::OnCompareItems (LPARAM lParam1, LPARAM lParam2, int iColumn)
{
	CString str1 = m_IncludeFileTypesListCtrl.GetItemText ((int)lParam1, iColumn);
	CString str2 = m_IncludeFileTypesListCtrl.GetItemText ((int)lParam2, iColumn);
	return str1.Compare (str2);
}
void CIncludeFileTypesDlg::OnBnClickedSelectAll()
{
	for (int i = 0; i < m_IncludeFileTypesListCtrl.GetItemCount(); i++) 
	{
		m_IncludeFileTypesListCtrl.SetCheck(i, true);
	}
}

void CIncludeFileTypesDlg::OnBnClickedDeselectAll()
{
	for (int i = 0; i < m_IncludeFileTypesListCtrl.GetItemCount(); i++) 
	{
		m_IncludeFileTypesListCtrl.SetCheck(i, false);
	}
}
