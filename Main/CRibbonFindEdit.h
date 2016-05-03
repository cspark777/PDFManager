#ifndef __CRibbonFindEdit_H__
#define __CRibbonFindEdit_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "CRibbonFindEditCtrl.h"

#pragma once

class CRibbonFindEdit :
	public CBCGPRibbonEdit
{
public:
	CRibbonFindEdit(UINT nID, int nWidth);
	~CRibbonFindEdit(void);
	virtual CBCGPRibbonEditCtrl* CreateEdit (CWnd* pWndParent, DWORD dwEditStyle);
};
#endif