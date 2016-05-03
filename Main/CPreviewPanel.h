#pragma once

#include "CBBDialogBar.h"
#include "CImageExtract.h"
#include "DebenuPDFLibraryAX0912.h"


//*****************************************************************************
// CPreviewPanel class
//*****************************************************************************
class CPreviewPanel : public CBBDialogBar
{
	DECLARE_SERIAL(CPreviewPanel)

private:
	CStatic mPreviewBitmap;
	CStatic mPreviewIcon;
	CImageExtract mThumbExtract;
	HICON lIcon;
	int mFileID;
	BOOL m_bCJKFontFlag;
	wstring m_wszContent;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog ();

public:
	CPreviewPanel(CWnd* pParent = NULL);
	virtual ~CPreviewPanel();
	virtual void Resize (BOOL bFull);
	void Refresh(BOOL inForceRedraw = FALSE);
	BOOL isCJKFont();
	BOOL LoadFile(TCHAR* wszFilePath);
	
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg LRESULT HandleInitDialog(WPARAM wParam, LPARAM lParam);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);

	DECLARE_MESSAGE_MAP()
};
