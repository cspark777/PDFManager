#pragma once

#include "stdafx.h"
#include "afxwin.h"

#define N 10

// CSmartCollectionsDlg dialog

class CSmartCollectionsDlg : public CDialog
{
	DECLARE_DYNAMIC(CSmartCollectionsDlg)

	CComboBox	m_NounsCombo[N];
	CComboBox	m_VerbsCombo[N];
	CButton		m_RemoveBtn[N];
	CButton		m_AddBtn[N];
	CBCGPDateTimeCtrl m_ValueButton[N];
	CEdit		m_ValueEdit[N];
	CStatic		m_ValueStatic[N];
	CWnd*		m_ValueWnd[N];

	CButton m_LimitToCheck;
	CEdit m_LimitToEdit;
	CComboBox m_LimitToCombo;

	CButton m_OkBtn;
	CButton m_CancelBtn;

	int mPos;

	int mSmartColID;

public:
	PARAM_FOR_CDB *m_pParamCDB;
public:
	CTSmartColl *mTSmartColl;
	CSmartCollectionsDlg(int inSmartColID = -1, CWnd* pParent = NULL);   // standard constructor
	virtual ~CSmartCollectionsDlg();


// Dialog Data
	enum { IDD = IDD_SMART_COLLECTIONS_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	void InitComboBoxes();
	int GetVerbsComboType(int inComboIdx);
	void InitVerbsComboFromDB(int inComboIdx, int inDBValue);
	void ChangeValueControl(int inRow);
	void FillVerbsCombo(int inComboIdx, int inType = -1);
	void InitDialogFromDB();

	DECLARE_MESSAGE_MAP()
public:
	void UpdateUI();
	void ShowRow(int inCurrentRow);
	void HideRow(int inCurrentRow);
	
public:
	afx_msg void OnBnClickedScAdd1Btn() { ShowRow(1);  }
	afx_msg void OnBnClickedScAdd2Btn() { ShowRow(2);  }
	afx_msg void OnBnClickedScAdd3Btn() { ShowRow(3);  }
	afx_msg void OnBnClickedScAdd4Btn() { ShowRow(4);  }
	afx_msg void OnBnClickedScAdd5Btn() { ShowRow(5);  }
	afx_msg void OnBnClickedScAdd6Btn() { ShowRow(6);  }
	afx_msg void OnBnClickedScAdd7Btn() { ShowRow(7);  }
	afx_msg void OnBnClickedScAdd8Btn() { ShowRow(8);  }
	afx_msg void OnBnClickedScAdd9Btn() { ShowRow(9);  }
	afx_msg void OnBnClickedScAdd10Btn(){ ShowRow(10); }
public:
	afx_msg void OnBnClickedScRemove1Btn() { HideRow(1);  }
	afx_msg void OnBnClickedScRemove2Btn() { HideRow(2);  }
	afx_msg void OnBnClickedScRemove3Btn() { HideRow(3);  }
	afx_msg void OnBnClickedScRemove4Btn() { HideRow(4);  }
	afx_msg void OnBnClickedScRemove5Btn() { HideRow(5);  }
	afx_msg void OnBnClickedScRemove6Btn() { HideRow(6);  }
	afx_msg void OnBnClickedScRemove7Btn() { HideRow(7);  }
	afx_msg void OnBnClickedScRemove8Btn() { HideRow(8);  }
	afx_msg void OnBnClickedScRemove9Btn() { HideRow(9);  }
	afx_msg void OnBnClickedScRemove10Btn(){ HideRow(10); }

	afx_msg void OnBnClickedScMatchTheFollowingRuleCheck();
	afx_msg void OnBnClickedScOkBtn();
	afx_msg void OnBnClickedScCancelBtn();
public:
	afx_msg void OnCbnSelendokScNouns1Combo();
public:
	afx_msg void OnCbnSelendokScNouns2Combo();
public:
	afx_msg void OnCbnSelendokScNouns3Combo();
public:
	afx_msg void OnCbnSelendokScNouns4Combo();
public:
	afx_msg void OnCbnSelendokScNouns5Combo();
public:
	afx_msg void OnCbnSelendokScNouns6Combo();
public:
	afx_msg void OnCbnSelendokScNouns7Combo();
public:
	afx_msg void OnCbnSelendokScNouns8Combo();
public:
	afx_msg void OnCbnSelendokScNouns9Combo();
public:
	afx_msg void OnCbnSelendokScNouns10Combo();
};
