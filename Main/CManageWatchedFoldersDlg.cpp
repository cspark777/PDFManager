// CManageWatchedFoldersDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Benubird.h"
#include "CManageWatchedFoldersDlg.h"
#include "CWatchedFolderSettingsDlg.h"


// CManageWatchedFoldersDlg dialog

IMPLEMENT_DYNAMIC(CManageWatchedFoldersDlg, CDialog)

CManageWatchedFoldersDlg::CManageWatchedFoldersDlg( CWnd* pParent /*=NULL*/)
	: CDialog(CManageWatchedFoldersDlg::IDD, pParent)
{
}

CManageWatchedFoldersDlg::~CManageWatchedFoldersDlg()
{
}

void CManageWatchedFoldersDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MWF_LIST, m_ListCtrl);
}


BEGIN_MESSAGE_MAP(CManageWatchedFoldersDlg, CDialog)
	ON_BN_CLICKED(IDC_MWF_ADD_BTN, &CManageWatchedFoldersDlg::OnBnClickedMwfAddBtn)
	ON_BN_CLICKED(IDC_MWF_REMOVE_BTN, &CManageWatchedFoldersDlg::OnBnClickedMwfRemoveBtn)
	ON_BN_CLICKED(IDC_MWF_EDIT_BTN, &CManageWatchedFoldersDlg::OnBnClickedMwfEditBtn)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_MWF_LIST, &CManageWatchedFoldersDlg::OnLvnItemchangedMwfList)
	ON_NOTIFY(NM_DBLCLK, IDC_MWF_LIST, &CManageWatchedFoldersDlg::OnNMDblclkMwfList)
	ON_BN_CLICKED(IDOK, &CManageWatchedFoldersDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CManageWatchedFoldersDlg message handlers

void CManageWatchedFoldersDlg::OnBnClickedMwfAddBtn()
{
	CString lStr;
	CWatchedFolderSettingsDlg lDlg(-1);
	lDlg.Set_ParamForCDB(m_pParamCDB);
	if (lDlg.DoModal() == IDOK)
	{
		Refresh();

		int lCount = m_ListCtrl.GetItemCount();
		if (lCount > 0)
			m_ListCtrl.SetItemState(lCount-1, LVIS_SELECTED, LVIS_SELECTED);
	}
	UpdateControls();
}

BOOL CManageWatchedFoldersDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	CRect lRect;
	m_ListCtrl.GetClientRect(&lRect);
	int l1stColumnWidth = (lRect.Width() / 3) * 2;
	int l2ndColumnWidth = lRect.Width() - l1stColumnWidth;

	m_ListCtrl.InsertColumn(0, _T("Directory"), LVCFMT_LEFT, l1stColumnWidth);
	m_ListCtrl.InsertColumn(1, _T("File types"), LVCFMT_LEFT, l2ndColumnWidth);
	m_ListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );

	Refresh();
	UpdateControls();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
void CManageWatchedFoldersDlg::Set_ParamForCDB(PARAM_FOR_CDB *pParamCDB)
{
	m_pParamCDB = pParamCDB;
}
void CManageWatchedFoldersDlg::Refresh()
{
	// get the selection
	int lSelIndex = 0;
	int lSelCount = m_ListCtrl.GetSelectedCount();
	if (lSelCount > 0)
		lSelIndex = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);

	try
	{
		m_ListCtrl.DeleteAllItems();
		CTWatchedFolder lTWFolders(m_pParamCDB->pDB);
		if(lTWFolders.GetWatchedFolders())
		{
			int i = 0;
			while(lTWFolders.mDataIsValid)
			{
				m_ListCtrl.InsertItem(i, lTWFolders.mPath);
				m_ListCtrl.SetItemData(i, lTWFolders.mID);

				//Set the Extension column
				CString lStr;
				if(lTWFolders.mAllFileType)
					lStr.Format(IDS_WFS_ALL);
				else
				{
					CIDData lWFolderExts;
					CTFExtensions lFExt(m_pParamCDB->pDB);
					lFExt.GetCIDDataByWFolderID(lWFolderExts, lTWFolders.mID);
					lWFolderExts.GetStringsFormatted(lStr);
				}
				m_ListCtrl.SetItemText(i, 1, lStr);
			
				i++;
				lTWFolders.LoadNextRow();
			}
		}
	}
	catch(...)
	{
	}

	int lCount = m_ListCtrl.GetItemCount();
	if (lCount > 0)
	{
		if (lSelIndex < 0) lSelIndex=0;
		if (lSelIndex >= lCount) lSelIndex=lCount-1;
		m_ListCtrl.SetItemState(lSelIndex, LVIS_SELECTED, LVIS_SELECTED);
 	}


}

void CManageWatchedFoldersDlg::OnBnClickedMwfRemoveBtn()
{
	if (m_ListCtrl.GetSelectedCount() == 0)
		return;

	if (AfxMessageBox(_T("Are you sure you want to delete this watched folder?"), MB_YESNO) == IDYES)
	{
		CTWatchedFolder lTWFolder(m_pParamCDB->pDB);
		int lNumDeletedWFolders = 0;
		POSITION lPos = m_ListCtrl.GetFirstSelectedItemPosition();
		while(lPos)
		{
			int lSelItemIdx = m_ListCtrl.GetNextSelectedItem(lPos);
			if (lTWFolder.DeleteByID(m_ListCtrl.GetItemData(lSelItemIdx)))
				lNumDeletedWFolders++;
		}
		if (lNumDeletedWFolders > 0)
			Refresh();
	}
	UpdateControls();
}

void CManageWatchedFoldersDlg::OnBnClickedMwfEditBtn()
{
	if(m_ListCtrl.GetSelectedCount() != 1)
		return;

	DWORD_PTR lWFolderID = 0;
	POSITION lPos = m_ListCtrl.GetFirstSelectedItemPosition();
	if(lPos)
	{
		int lSelItemIdx = m_ListCtrl.GetNextSelectedItem(lPos);
		lWFolderID = m_ListCtrl.GetItemData(lSelItemIdx);
	}
	CWatchedFolderSettingsDlg lDlg(lWFolderID);
	lDlg.Set_ParamForCDB(m_pParamCDB);
	if(IDOK == lDlg.DoModal())
		Refresh();

	UpdateControls();
}

void CManageWatchedFoldersDlg::OnLvnItemchangedMwfList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	UpdateControls();
	*pResult = 0;
}

void CManageWatchedFoldersDlg::OnNMDblclkMwfList(NMHDR *pNMHDR, LRESULT *pResult)
{
	OnBnClickedMwfEditBtn();
	*pResult = 0;
}

void CManageWatchedFoldersDlg::UpdateControls()
{
	int lSelCount;
	BOOL lEnabled;
	lSelCount = m_ListCtrl.GetSelectedCount();
	lSelCount > 0 ? lEnabled = TRUE : lEnabled = FALSE;
	GetDlgItem(IDC_MWF_EDIT_BTN)->EnableWindow(lEnabled);
	GetDlgItem(IDC_MWF_REMOVE_BTN)->EnableWindow(lEnabled);
}

void CManageWatchedFoldersDlg::OnBnClickedOk()
{
	/*CppSQLite3Query lQ = m_pDB->mDB.execQuery(_T("BEGIN TRANSACTION"));
	CString lSQL;
	lSQL.Format(_T("delete from WFolders where idWFolder = %u"), 0);
	m_pDB->mDB.execDML(lSQL);
	lQ = m_pDB->mDB.execQuery(_T("COMMIT"));*/

	OnOK();
}
