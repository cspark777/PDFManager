#include "stdafx.h"
#include "afxwin.h"
#include "Benubird.h"
#include "CRibbonFindEdit.h"

CRibbonFindEdit::CRibbonFindEdit(UINT nID, int nWidth) :
	CBCGPRibbonEdit(nID, nWidth)
{
}

CRibbonFindEdit::~CRibbonFindEdit(void)
{
}

CBCGPRibbonEditCtrl* CRibbonFindEdit::CreateEdit(CWnd* pWndParent, DWORD dwEditStyle)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pWndParent);

	CRibbonFindEditCtrl* pWndEdit = new CRibbonFindEditCtrl (*this);

	if (!pWndEdit->Create (dwEditStyle, CRect (0, 0, 0, 0), 
		pWndParent, m_nID))
	{
		delete pWndEdit;
		return NULL;
	}

	return pWndEdit;
}