#ifndef __CRibbonFindEditCtrl_H__
#define __CRibbonFindEditCtrl_H__

#include "afxwin.h"
#include "Benubird.h"

#pragma once

class CRibbonFindEditCtrl :
	public CBCGPRibbonEditCtrl
{
public:
	CRibbonFindEditCtrl(CBCGPRibbonEdit& edit);
	~CRibbonFindEditCtrl(void);
protected:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	DECLARE_MESSAGE_MAP()
};
#endif