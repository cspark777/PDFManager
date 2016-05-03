// CCategoriesSelectionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Benubird.h"
#include "CCategoriesSelectionDlg.h"
#include "CManageCategoriesDlg.h"


// CCategoriesSelectionDlg dialog

IMPLEMENT_DYNAMIC(CCategoriesSelectionDlg, CDialog)

CCategoriesSelectionDlg::CCategoriesSelectionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCategoriesSelectionDlg::IDD, pParent)
{

}

CCategoriesSelectionDlg::~CCategoriesSelectionDlg()
{
}

void CCategoriesSelectionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCategoriesSelectionDlg, CDialog)
	ON_BN_CLICKED(IDC_CS_MANAGE_CATEGORIES_BTN, &CCategoriesSelectionDlg::OnBnClickedCsManageCategoriesBtn)
END_MESSAGE_MAP()


// CCategoriesSelectionDlg message handlers

void CCategoriesSelectionDlg::OnBnClickedCsManageCategoriesBtn()
{
	CManageCategoriesDlg lDlg;
	lDlg.DoModal();
}
