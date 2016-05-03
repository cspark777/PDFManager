#include "stdafx.h"
#include "CRibbonFindEditCtrl.h"

CRibbonFindEditCtrl::CRibbonFindEditCtrl(CBCGPRibbonEdit& edit) :
	CBCGPRibbonEditCtrl(edit)
{
}

CRibbonFindEditCtrl::~CRibbonFindEditCtrl(void)
{
}

BEGIN_MESSAGE_MAP(CRibbonFindEditCtrl, CBCGPRibbonEditCtrl)
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

void CRibbonFindEditCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	SetFocus();
	SetSel(0, -1);
}
