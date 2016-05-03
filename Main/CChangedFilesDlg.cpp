// CChangedFilesDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Benubird.h"
#include "CChangedFilesDlg.h"
#include "CBWorkspace.h" 

// CChangedFilesDlg dialog

IMPLEMENT_DYNAMIC(CChangedFilesDlg, CDialog)

CChangedFilesDlg::CChangedFilesDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CChangedFilesDlg::IDD, pParent)
{
	mFileID = -1;
}

CChangedFilesDlg::~CChangedFilesDlg()
{
}

BEGIN_MESSAGE_MAP(CChangedFilesDlg, CDialog)
	//ON_BN_CLICKED(IDC_SELECT_ALL, &CChangedFilesDlg::OnBnClickedSelectAll)
	//ON_BN_CLICKED(IDC_DESELECT_ALL, &CChangedFilesDlg::OnBnClickedDeselectAll)
	//ON_BN_CLICKED(IDOK, &CChangedFilesDlg::OnBnClickedOk)
END_MESSAGE_MAP()

/*
void CChangedFilesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHANGED_FILE_LIST, m_ChangedFileList);
}

// CChangedFilesDlg message handlers

BOOL CChangedFilesDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	m_ChangedFileList.SetExtendedStyle(m_ChangedFileList.GetExtendedStyle() | LVS_EX_CHECKBOXES);

	m_ChangedFileList.InsertColumn(0, _T("File name"), 0 , 200);
//	m_ChangedFileList.InsertColumn(1, _T("Modified date"), 0, 80);


	for (int i = 0; i < gWorkspace.GetNumChangedFiles() ; i++)
	{
		int lID = gWorkspace.GetChangedFileID(i);

		CTFiles lFiles;
		lFiles.GetFileByID(lID);

		LVITEM lItem;
		lItem.mask = LVIF_TEXT;
		lItem.iItem = i;
		lItem.iSubItem = 0;
		lItem.pszText = lFiles.mName.GetBuffer();
		int lPos = m_ChangedFileList.InsertItem(&lItem);
		m_ChangedFileList.SetItemData(i, lID);

		m_ChangedFileList.SetCheck(lPos, (mFileID == -1 || lID == mFileID));

		if (mFileID == lID)
		{
			m_ChangedFileList.SetItemState(i, LVIS_FOCUSED, LVIS_FOCUSED);
			m_ChangedFileList.SetItemState(i, LVIS_SELECTED, LVIS_SELECTED);
		}
	}



	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CChangedFilesDlg::OnBnClickedSelectAll()
{
	for (int i = 0; i < m_ChangedFileList.GetItemCount(); i++) 
	{
//		m_ChangedFileList.SetItemState(i, LVIS_FOCUSED, LVIS_FOCUSED);
//		m_ChangedFileList.SetItemState(i, LVIS_SELECTED, LVIS_SELECTED);
		m_ChangedFileList.SetCheck(i, true);
	}
}

void CChangedFilesDlg::OnBnClickedDeselectAll()
{
	for (int i = 0; i < m_ChangedFileList.GetItemCount(); i++) 
	{
//		m_ChangedFileList.SetItemState(i, 0, LVIS_FOCUSED);
//		m_ChangedFileList.SetItemState(i, 0, LVIS_SELECTED);
		m_ChangedFileList.SetCheck(i, false);
	}
}

void CChangedFilesDlg::OnBnClickedOk()
{
	for (int i = m_ChangedFileList.GetItemCount() -1; i >= 0 ; i--) 
	{
		if (m_ChangedFileList.GetCheck(i))
		{
			int lFileID = (int)m_ChangedFileList.GetItemData(i);
			if (gWorkspace.SaveBackToRepository(lFileID))
			{
				m_ChangedFileList.SetCheck(i, false);
				gWorkspace.RemoveFile(lFileID);
			}
		}
	}
	OnOK();
}
*/