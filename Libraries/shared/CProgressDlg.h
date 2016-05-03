#pragma once

#include "afxwin.h"
#include <BCGCBProInc.h>			// BCGPro Control Bar

class CProgressDlg : public CBCGPProgressDlg
{
	CBCGPProgressDlgParams mParams;

public:
	CProgressDlg();
	~CProgressDlg();

	void SetMin(long inMin){ mParams.m_nRangeMin = inMin; };
	void SetMax(long inMax){ mParams.m_nRangeMax = inMax; };
	void SetStep(long inStep){ mParams.m_nStep = inStep; };
	void SetCaption(LPCTSTR inCaption){ mParams.m_strCaption = inCaption; };
	void ShowCancel(BOOL inShow){ mParams.m_bShowCancel = inShow; };
	void SetNeverEndingMode();
	BOOL CreateProgressDlg(CWnd* inParent = NULL);
	virtual BOOL PumpMessages();
	
protected:
	//afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	DECLARE_MESSAGE_MAP()
};
