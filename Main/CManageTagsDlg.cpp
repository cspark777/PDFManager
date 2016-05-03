// CManageTagsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Benubird.h"
#include "CManageTagsDlg.h"
#include "CAddTagsDlg.h"
#include "CAddMultipleTagsDlg.h"
#include "CTagsEditTagDlg.h"
#include "LibraryView.h"

#define MANAGETAGS_NEWTAGID  -1

IMPLEMENT_DYNAMIC(CManageTagsDlg, CDialog)

CManageTagsDlg::CManageTagsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CManageTagsDlg::IDD, pParent)
{
	CMDIChildWnd * pChild = ((CMDIFrameWnd*)(AfxGetApp()->m_pMainWnd))->MDIGetActive();
	CLibraryDoc* pDoc = DYNAMIC_DOWNCAST(CLibraryDoc, pChild->GetActiveDocument ());
	m_pParamCDB = pDoc->m_pParamCDB;
}

CManageTagsDlg::~CManageTagsDlg()
{
}

void CManageTagsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MANAGE_TAGS_LIST, m_ManageTagsList);
}


BEGIN_MESSAGE_MAP(CManageTagsDlg, CDialog)
	ON_BN_CLICKED(IDC_MANAGE_TAGS_ADD_BTN, &CManageTagsDlg::OnBnClickedManageTagsAddBtn)
	ON_BN_CLICKED(IDC_MANAGE_TAGS_EDIT_BTN, &CManageTagsDlg::OnBnClickedManageTagsEditBtn)
	ON_BN_CLICKED(IDC_MANAGE_TAGS_DELETE_BTN, &CManageTagsDlg::OnBnClickedManageTagsDeleteBtn)
	ON_BN_CLICKED(IDC_MANAGE_TAGS_IMPORT_BTN, &CManageTagsDlg::OnBnClickedManageTagsImportBtn)
	ON_BN_CLICKED(IDC_MANAGE_TAGS_EXPORT_BTN, &CManageTagsDlg::OnBnClickedManageTagsExportBtn)
	ON_BN_CLICKED(IDOK, &CManageTagsDlg::OnBnClickedOk)
	ON_LBN_DBLCLK(IDC_MANAGE_TAGS_LIST, &CManageTagsDlg::OnLbnDblclkManageTagsList)
	ON_LBN_SELCHANGE(IDC_MANAGE_TAGS_LIST, &CManageTagsDlg::OnLbnSelchangeManageTagsList)
END_MESSAGE_MAP()

void CManageTagsDlg::OnBnClickedManageTagsAddBtn()
{
	CString lTags;
	CString lTag;
	CAddTagsDlg lAddTagsDlg;
	CAddMultipleTagsDlg lAddMultipleTagsDlg;
	int lPos;

	if (lAddTagsDlg.DoModal() == IDOK)
	{
		m_ManageTagsList.SelItemRange(FALSE, 0, m_ManageTagsList.GetCount() - 1);

		// Add Multiple Tags dialog
		if (lAddTagsDlg.NeedShowAddMultipleTagsDlg())
		{
			if (lAddMultipleTagsDlg.DoModal() == IDOK)
			{
				
				mAddedTags = lTags = lAddMultipleTagsDlg.GetText();				
				while ((lPos = lTags.Find(_T("\r\n"))) != -1)
				{
					lTag = lTags.Left(lPos);
					if (!lTag.IsEmpty())
					{
						if (m_ManageTagsList.FindStringExact(-1, lTag) == LB_ERR)
						{
							CString lStr;
							lStr = lTag;
							lStr.Remove(' ');
							if (!lStr.IsEmpty())
							{
								int lSelTagIdx = m_ManageTagsList.AddString(lTag);
								m_ManageTagsList.SetItemData(lSelTagIdx, MANAGETAGS_NEWTAGID);
								m_ManageTagsList.SetSel(lSelTagIdx);
							}
						}
					}
					lTags = lTags.Right(lTags.GetLength() - lPos - 2);
				}
				lTag = lTags;
				if (!lTag.IsEmpty())
				{
					if (m_ManageTagsList.FindStringExact(-1, lTag) == LB_ERR)
					{
						CString lStr;
						lStr = lTag;
						lStr.Remove(' ');
						if (!lStr.IsEmpty())
						{
							//m_ManageTagsList.SetFocus();
							//m_ManageTagsList.SetSel(-1);
							int lTagIdx = m_ManageTagsList.AddString(lTag);
							m_ManageTagsList.SetItemData(lTagIdx, MANAGETAGS_NEWTAGID);
							m_ManageTagsList.SetSel(lTagIdx);
						}
					}
				}
			}
		}

		// Add Tag
		else
		{
			mAddedTags = lTag = lAddTagsDlg.GetText();
			if (!lTag.IsEmpty())
			{
				if (m_ManageTagsList.FindStringExact(-1, lTag) == LB_ERR)
				{
					CString lStr;
					lStr = lTag;
					lStr.Remove(' ');
					if (!lStr.IsEmpty())
					{
						//m_ManageTagsList.SetFocus();
//						m_ManageTagsList.s SetSel(-1);
						int lTagIdx = m_ManageTagsList.AddString(lTag);
						m_ManageTagsList.SetItemData(lTagIdx, MANAGETAGS_NEWTAGID);
						m_ManageTagsList.SetSel(lTagIdx);
					}
				}
				else
				{
					// Show error message if it's necessary
				}
			}
		}
	}
	UpdateUI();
}

void CManageTagsDlg::OnBnClickedManageTagsEditBtn()
{
	int lCount = m_ManageTagsList.GetSelCount() ;
	if(lCount != 1 )
		return;

	CString lTagName;
	CIntArray lSelItems;
	lSelItems.SetSize(lCount);
	m_ManageTagsList.GetSelItems(lCount, lSelItems.GetData()); 
	int lSelTagIdx = lSelItems.GetAt(0);
	m_ManageTagsList.GetText(lSelTagIdx, lTagName);
	CTagsEditTagDlg lEditTagsDlg(lTagName);
	if(lEditTagsDlg.DoModal() == IDOK)
	{
		DWORD_PTR lTagID = m_ManageTagsList.GetItemData(lSelTagIdx);
		m_ManageTagsList.DeleteString(lSelTagIdx);
		lSelTagIdx = m_ManageTagsList.AddString(lTagName);
		m_ManageTagsList.SetItemData(lSelTagIdx, lTagID);
	}
	UpdateUI();
}

// Szabi, case 195
void CManageTagsDlg::OnBnClickedManageTagsDeleteBtn()
{
	CTTags lTags(m_pParamCDB->pDB);
	if (AfxMessageBox(_T("Are you sure you want to delete this tag?"), MB_YESNO|MB_ICONQUESTION) == IDYES)
	{
		int lCount = m_ManageTagsList.GetSelCount();
		if(lCount <= 0)
			return;

		CIntArray lListBoxSelArray;
		lListBoxSelArray.SetSize(lCount);
		m_ManageTagsList.GetSelItems(lCount, lListBoxSelArray.GetData()); 

		int lIndex;
		CString lTagStr;
		int lTagStrLen;
		int lLastIndex = -1;

		for(int i = lCount - 1; i >= 0; i--)
		{
			lIndex = lListBoxSelArray.GetAt(i);
			lTagStrLen = m_ManageTagsList.GetTextLen(lIndex);
			m_ManageTagsList.GetText(lIndex, lTagStr.GetBuffer(lTagStrLen));
			lTagStr.ReleaseBuffer();

			if(	lTags.DeleteByName(lTagStr))
				m_ManageTagsList.DeleteString(lIndex);

			if (lLastIndex == -1) lLastIndex = lIndex;
		}
		lLastIndex = (lLastIndex + 1) - lCount;
		if (lLastIndex > m_ManageTagsList.GetCount() - 1) lLastIndex = m_ManageTagsList.GetCount() - 1;
		m_ManageTagsList.SetSel(lLastIndex, TRUE);
	}
	UpdateUI();
}

void CManageTagsDlg::OnBnClickedManageTagsImportBtn()
{
	CFileDialog lDlg(TRUE, _T("*.csv"),NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("Comma Delimited ( *.csv)|*.csv||"));
	if(lDlg.DoModal() == IDOK)
	{
		CString lFileName = lDlg.GetPathName();
		CStdioFile lFile(lFileName, CFile::modeRead | CFile::typeText);
		CString lLine;
		int lNumReadStr;
//		CAddMultipleTagsDlg lAddMultipleTagsDlg;
		do
		{
			lNumReadStr = lFile.ReadString(lLine);
			if(lLine.GetLength() > 0)
			{
		//		lLine.Remove(' ');
				if(m_ManageTagsList.FindString(-1, lLine) == LB_ERR)
					m_ManageTagsList.AddString(lLine);
			}
		}while(lNumReadStr);


		//http://www.experts-exchange.com/Programming/Languages/CPP/Q_22020832.html
	//	AfxMessageBox(_T("Not supported yet"));
	}
	UpdateUI();
}

// TODO: Tono full support of CSV files based on next link:
//http://en.wikipedia.org/wiki/Comma-separated_values
void CManageTagsDlg::OnBnClickedManageTagsExportBtn()
{
	CFileDialog lDlg(FALSE, _T("*.csv"),NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("Comma Delimited ( *.csv)|*.csv||"));
	if(lDlg.DoModal() == IDOK)
	{
		CString lTagsInDlg, lTagName;
		int lTagsCount = m_ManageTagsList.GetCount();
		for(int i = 0; i<lTagsCount; i++)
		{	
			m_ManageTagsList.GetText(i, lTagName);
			if(i == 0)
				lTagsInDlg = lTagName;
			else
				lTagsInDlg = lTagsInDlg + _T("\n") + lTagName;
		}
		CString lFileName = lDlg.GetPathName();
		CStdioFile lFile(lFileName, CFile::modeReadWrite | CFile::modeCreate | CFile::typeText);
		lFile.WriteString(lTagsInDlg);
		lFile.Flush();
		lFile.Close();
	}
	UpdateUI();
}

BOOL CManageTagsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CTTags lTTags(m_pParamCDB->pDB);
	if(lTTags.GetTags())
	{
		while(lTTags.mDataIsValid)
		{
			int lSelTagIdx = m_ManageTagsList.AddString(lTTags.mName);
			m_ManageTagsList.SetItemData(lSelTagIdx, lTTags.mID);
			lTTags.LoadNextRow();
		}
	}
	UpdateUI();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CManageTagsDlg::OnBnClickedOk()
{
//1. find all Tags in DB
//2. for each Tag from DB check the entry in the TagsList by the ID
//3. if the ID match check the Tags name, if Names equal remove the Tag from the list, if not equal update the current row in the table
//4. if the ID is not in the list remove the Tag from the DB
//5. Add the remaining Tags into the DB
	CTTags lTTags(m_pParamCDB->pDB);
	int lCount, lIdx, lItemData;
	CString lTagName;

//	CDBTransaction lDBTtrans;
	if(lTTags.GetTags())
	{
		while(lTTags.mDataIsValid)
		{
			lCount =  m_ManageTagsList.GetCount();
			for(lIdx = lCount; lIdx >= 0; lIdx--)
			{
				lItemData = m_ManageTagsList.GetItemData(lIdx);
				if(lItemData == lTTags.mID)
				{
					m_ManageTagsList.GetText(lIdx, lTagName);
					if(lTagName == lTTags.mName)
					{		
						//do nothing in the DB
						//remove the tag from the list
						m_ManageTagsList.DeleteString(lIdx);
						break;
					}
					else
					{
						//update the DB table
						lTTags.mName = lTagName;
						lTTags.UpdateByID();
						//remove the Tag from the list
						m_ManageTagsList.DeleteString(lIdx);
						break;
					}
				}
			}
			lTTags.LoadNextRow();
		}
	}
	
	//add the remaining Tags to DB
	lCount =  m_ManageTagsList.GetCount();
	for(lIdx = 0; lIdx < lCount; lIdx++)
	{
		m_ManageTagsList.GetText(lIdx, lTagName);
		lTTags.AddNewTag(lTagName);
	}
//	lDBTtrans.mCommit = true;

	OnOK();
}

void CManageTagsDlg::OnLbnDblclkManageTagsList()
{
	OnBnClickedManageTagsEditBtn();
}

void CManageTagsDlg::OnLbnSelchangeManageTagsList()
{
	UpdateUI();
}

void CManageTagsDlg::UpdateUI()
{
	int lSelCount = m_ManageTagsList.GetSelCount();	
	if (lSelCount == LB_ERR)
		return;
	((CButton*)GetDlgItem(IDC_MANAGE_TAGS_EDIT_BTN))->EnableWindow(lSelCount > 0 ? TRUE : FALSE);
	((CButton*)GetDlgItem(IDC_MANAGE_TAGS_DELETE_BTN))->EnableWindow(lSelCount > 0 ? TRUE : FALSE);
	((CButton*)GetDlgItem(IDC_MANAGE_TAGS_EDIT_BTN))->EnableWindow(lSelCount == 1 ? TRUE : FALSE);
}
