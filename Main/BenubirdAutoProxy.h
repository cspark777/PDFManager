// BenubirdAutoProxy.h: header file
//

#pragma once
#include "stdafx.h"

class CMainFrame;


// CBenubirdAutoProxy command target

class CBenubirdAutoProxy : public CCmdTarget
{
	DECLARE_DYNCREATE(CBenubirdAutoProxy)

	CBenubirdAutoProxy();           // protected constructor used by dynamic creation

// Attributes
public:
	CMainFrame* m_pDialog;

// Operations
public:

// Overrides
	public:
	virtual void OnFinalRelease();

// Implementation

	virtual ~CBenubirdAutoProxy();
protected:
	// Generated message map functions

	DECLARE_MESSAGE_MAP()
	DECLARE_OLECREATE(CBenubirdAutoProxy)

	// Generated OLE dispatch map functions

	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	void Show(void);
	LONG AddFile(LPCTSTR file);

	enum 
	{
		dispidConvertToPDF = 3L,
		dispidAddFile = 2L,
		dispidShow = 1L
	};
	LONG ConvertToPDF(LPCTSTR files);
};

