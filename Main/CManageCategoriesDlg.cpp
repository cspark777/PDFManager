// CManageCategoriesDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Benubird.h"
#include "CManageCategoriesDlg.h"


// CManageCategoriesDlg dialog

IMPLEMENT_DYNAMIC(CManageCategoriesDlg, CDialog)

CManageCategoriesDlg::CManageCategoriesDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CManageCategoriesDlg::IDD, pParent)
{

}

CManageCategoriesDlg::~CManageCategoriesDlg()
{
}

void CManageCategoriesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CManageCategoriesDlg, CDialog)
	ON_BN_CLICKED(IDC_MANAGE_TAGS_ADD_BTN, &CManageCategoriesDlg::OnBnClickedManageTagsAddBtn)
	ON_BN_CLICKED(IDC_MANAGE_TAGS_EDIT_BTN, &CManageCategoriesDlg::OnBnClickedManageTagsEditBtn)
	ON_BN_CLICKED(IDC_MANAGE_TAGS_DELETE_BTN, &CManageCategoriesDlg::OnBnClickedManageTagsDeleteBtn)
	ON_BN_CLICKED(IDC_MANAGE_TAGS_IMPORT_BTN, &CManageCategoriesDlg::OnBnClickedManageTagsImportBtn)
	ON_BN_CLICKED(IDC_MANAGE_TAGS_EXPORT_BTN, &CManageCategoriesDlg::OnBnClickedManageTagsExportBtn)
END_MESSAGE_MAP()


// CManageCategoriesDlg message handlers

void CManageCategoriesDlg::OnBnClickedManageTagsAddBtn()
{
	AfxMessageBox(_T("Add new tag"));
}

void CManageCategoriesDlg::OnBnClickedManageTagsEditBtn()
{
	AfxMessageBox(_T("Edit existing tag"));
}

void CManageCategoriesDlg::OnBnClickedManageTagsDeleteBtn()
{
	AfxMessageBox(_T("Delete existing tag"));
}

void CManageCategoriesDlg::OnBnClickedManageTagsImportBtn()
{
	AfxMessageBox(_T("Import tags"));
}

void CManageCategoriesDlg::OnBnClickedManageTagsExportBtn()
{
	AfxMessageBox(_T("Export tags"));
}
