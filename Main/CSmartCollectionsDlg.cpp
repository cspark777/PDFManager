// CSmartCollectionsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Benubird.h"
#include "CSmartCollectionsDlg.h"
#include "resource.h"
#include "LibraryView.h"

#define OFFSET 29

// CSmartCollectionsDlg dialog

IMPLEMENT_DYNAMIC(CSmartCollectionsDlg, CDialog)

CSmartCollectionsDlg::CSmartCollectionsDlg(int inSmartColID /*= -1*/, CWnd* pParent /*=NULL*/)
	: CDialog(CSmartCollectionsDlg::IDD, pParent)
{
	mPos = 0;
	mSmartColID = inSmartColID;

	CMDIChildWnd * pChild = ((CMDIFrameWnd*)(AfxGetApp()->m_pMainWnd))->MDIGetActive();
	CLibraryDoc* pDoc = DYNAMIC_DOWNCAST(CLibraryDoc, pChild->GetActiveDocument ());
	m_pParamCDB = pDoc->m_pParamCDB;

	mTSmartColl = new CTSmartColl(m_pParamCDB);
}

CSmartCollectionsDlg::~CSmartCollectionsDlg()
{
	delete mTSmartColl;
}

void CSmartCollectionsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SC_NOUNS1_COMBO, m_NounsCombo[0]);
	DDX_Control(pDX, IDC_SC_NOUNS2_COMBO, m_NounsCombo[1]);
	DDX_Control(pDX, IDC_SC_NOUNS3_COMBO, m_NounsCombo[2]);
	DDX_Control(pDX, IDC_SC_NOUNS4_COMBO, m_NounsCombo[3]);
	DDX_Control(pDX, IDC_SC_NOUNS5_COMBO, m_NounsCombo[4]);
	DDX_Control(pDX, IDC_SC_NOUNS6_COMBO, m_NounsCombo[5]);
	DDX_Control(pDX, IDC_SC_NOUNS7_COMBO, m_NounsCombo[6]);
	DDX_Control(pDX, IDC_SC_NOUNS8_COMBO, m_NounsCombo[7]);
	DDX_Control(pDX, IDC_SC_NOUNS9_COMBO, m_NounsCombo[8]);
	DDX_Control(pDX, IDC_SC_NOUNS10_COMBO, m_NounsCombo[9]);

	DDX_Control(pDX, IDC_SC_VERBS1_COMBO, m_VerbsCombo[0]);
	DDX_Control(pDX, IDC_SC_VERBS2_COMBO, m_VerbsCombo[1]);
	DDX_Control(pDX, IDC_SC_VERBS3_COMBO, m_VerbsCombo[2]);
	DDX_Control(pDX, IDC_SC_VERBS4_COMBO, m_VerbsCombo[3]);
	DDX_Control(pDX, IDC_SC_VERBS5_COMBO, m_VerbsCombo[4]);
	DDX_Control(pDX, IDC_SC_VERBS6_COMBO, m_VerbsCombo[5]);
	DDX_Control(pDX, IDC_SC_VERBS7_COMBO, m_VerbsCombo[6]);
	DDX_Control(pDX, IDC_SC_VERBS8_COMBO, m_VerbsCombo[7]);
	DDX_Control(pDX, IDC_SC_VERBS9_COMBO, m_VerbsCombo[8]);
	DDX_Control(pDX, IDC_SC_VERBS10_COMBO, m_VerbsCombo[9]);

	DDX_Control(pDX, IDC_SC_VALUE1_EDIT, m_ValueEdit[0]);
	DDX_Control(pDX, IDC_SC_VALUE2_EDIT, m_ValueEdit[1]);
	DDX_Control(pDX, IDC_SC_VALUE3_EDIT, m_ValueEdit[2]);
	DDX_Control(pDX, IDC_SC_VALUE4_EDIT, m_ValueEdit[3]);
	DDX_Control(pDX, IDC_SC_VALUE5_EDIT, m_ValueEdit[4]);
	DDX_Control(pDX, IDC_SC_VALUE6_EDIT, m_ValueEdit[5]);
	DDX_Control(pDX, IDC_SC_VALUE7_EDIT, m_ValueEdit[6]);
	DDX_Control(pDX, IDC_SC_VALUE8_EDIT, m_ValueEdit[7]);
	DDX_Control(pDX, IDC_SC_VALUE9_EDIT, m_ValueEdit[8]);
	DDX_Control(pDX, IDC_SC_VALUE10_EDIT, m_ValueEdit[9]);

	DDX_Control(pDX, IDC_SC_UNIT, m_ValueStatic[0]);
	DDX_Control(pDX, IDC_SC_UNIT2, m_ValueStatic[1]);
	DDX_Control(pDX, IDC_SC_UNIT3, m_ValueStatic[2]);
	DDX_Control(pDX, IDC_SC_UNIT4, m_ValueStatic[3]);
	DDX_Control(pDX, IDC_SC_UNIT5, m_ValueStatic[4]);
	DDX_Control(pDX, IDC_SC_UNIT6, m_ValueStatic[5]);
	DDX_Control(pDX, IDC_SC_UNIT7, m_ValueStatic[6]);
	DDX_Control(pDX, IDC_SC_UNIT8, m_ValueStatic[7]);
	DDX_Control(pDX, IDC_SC_UNIT9, m_ValueStatic[8]);
	DDX_Control(pDX, IDC_SC_UNIT10, m_ValueStatic[9]);	

	DDX_Control(pDX, IDC_SC_REMOVE1_BTN, m_RemoveBtn[0]);
	DDX_Control(pDX, IDC_SC_REMOVE2_BTN, m_RemoveBtn[1]);
	DDX_Control(pDX, IDC_SC_REMOVE3_BTN, m_RemoveBtn[2]);
	DDX_Control(pDX, IDC_SC_REMOVE4_BTN, m_RemoveBtn[3]);
	DDX_Control(pDX, IDC_SC_REMOVE5_BTN, m_RemoveBtn[4]);
	DDX_Control(pDX, IDC_SC_REMOVE6_BTN, m_RemoveBtn[5]);
	DDX_Control(pDX, IDC_SC_REMOVE7_BTN, m_RemoveBtn[6]);
	DDX_Control(pDX, IDC_SC_REMOVE8_BTN, m_RemoveBtn[7]);
	DDX_Control(pDX, IDC_SC_REMOVE9_BTN, m_RemoveBtn[8]);
	DDX_Control(pDX, IDC_SC_REMOVE10_BTN, m_RemoveBtn[9]);

	DDX_Control(pDX, IDC_SC_ADD1_BTN, m_AddBtn[0]);
	DDX_Control(pDX, IDC_SC_ADD2_BTN, m_AddBtn[1]);
	DDX_Control(pDX, IDC_SC_ADD3_BTN, m_AddBtn[2]);
	DDX_Control(pDX, IDC_SC_ADD4_BTN, m_AddBtn[3]);
	DDX_Control(pDX, IDC_SC_ADD5_BTN, m_AddBtn[4]);
	DDX_Control(pDX, IDC_SC_ADD6_BTN, m_AddBtn[5]);
	DDX_Control(pDX, IDC_SC_ADD7_BTN, m_AddBtn[6]);
	DDX_Control(pDX, IDC_SC_ADD8_BTN, m_AddBtn[7]);
	DDX_Control(pDX, IDC_SC_ADD9_BTN, m_AddBtn[8]);
	DDX_Control(pDX, IDC_SC_ADD10_BTN, m_AddBtn[9]);

	DDX_Control(pDX, IDC_SC_LIMIT_TO_CHECK, m_LimitToCheck);
	DDX_Control(pDX, IDC_SC_LIMIT_TO_EDIT, m_LimitToEdit);
	DDX_Control(pDX, IDC_SC_LIMIT_TO_COMBO, m_LimitToCombo);
	DDX_Control(pDX, IDC_SC_OK_BTN, m_OkBtn);
	DDX_Control(pDX, IDC_SC_CANCEL_BTN, m_CancelBtn);
	
}


BEGIN_MESSAGE_MAP(CSmartCollectionsDlg, CDialog)

	ON_BN_CLICKED(IDC_SC_ADD1_BTN, &CSmartCollectionsDlg::OnBnClickedScAdd1Btn)
	ON_BN_CLICKED(IDC_SC_ADD2_BTN, &CSmartCollectionsDlg::OnBnClickedScAdd2Btn)
	ON_BN_CLICKED(IDC_SC_ADD3_BTN, &CSmartCollectionsDlg::OnBnClickedScAdd3Btn)
	ON_BN_CLICKED(IDC_SC_ADD4_BTN, &CSmartCollectionsDlg::OnBnClickedScAdd4Btn)
	ON_BN_CLICKED(IDC_SC_ADD5_BTN, &CSmartCollectionsDlg::OnBnClickedScAdd5Btn)
	ON_BN_CLICKED(IDC_SC_ADD6_BTN, &CSmartCollectionsDlg::OnBnClickedScAdd6Btn)
	ON_BN_CLICKED(IDC_SC_ADD7_BTN, &CSmartCollectionsDlg::OnBnClickedScAdd7Btn)
	ON_BN_CLICKED(IDC_SC_ADD8_BTN, &CSmartCollectionsDlg::OnBnClickedScAdd8Btn)
	ON_BN_CLICKED(IDC_SC_ADD9_BTN, &CSmartCollectionsDlg::OnBnClickedScAdd9Btn)
	ON_BN_CLICKED(IDC_SC_ADD10_BTN, &CSmartCollectionsDlg::OnBnClickedScAdd10Btn)
	ON_BN_CLICKED(IDC_SC_REMOVE1_BTN, &CSmartCollectionsDlg::OnBnClickedScRemove1Btn)
	ON_BN_CLICKED(IDC_SC_REMOVE2_BTN, &CSmartCollectionsDlg::OnBnClickedScRemove2Btn)
	ON_BN_CLICKED(IDC_SC_REMOVE3_BTN, &CSmartCollectionsDlg::OnBnClickedScRemove3Btn)
	ON_BN_CLICKED(IDC_SC_REMOVE4_BTN, &CSmartCollectionsDlg::OnBnClickedScRemove4Btn)
	ON_BN_CLICKED(IDC_SC_REMOVE5_BTN, &CSmartCollectionsDlg::OnBnClickedScRemove5Btn)
	ON_BN_CLICKED(IDC_SC_REMOVE6_BTN, &CSmartCollectionsDlg::OnBnClickedScRemove6Btn)
	ON_BN_CLICKED(IDC_SC_REMOVE7_BTN, &CSmartCollectionsDlg::OnBnClickedScRemove7Btn)
	ON_BN_CLICKED(IDC_SC_REMOVE8_BTN, &CSmartCollectionsDlg::OnBnClickedScRemove8Btn)
	ON_BN_CLICKED(IDC_SC_REMOVE9_BTN, &CSmartCollectionsDlg::OnBnClickedScRemove9Btn)
	ON_BN_CLICKED(IDC_SC_REMOVE10_BTN, &CSmartCollectionsDlg::OnBnClickedScRemove10Btn)
	ON_BN_CLICKED(IDC_SC_MATCH_THE_FOLLOWING_RULE_CHECK, &CSmartCollectionsDlg::OnBnClickedScMatchTheFollowingRuleCheck)
	//ON_BN_CLICKED(IDC_SC_LIMIT_TO_CHECK, &CSmartCollectionsDlg::OnBnClickedScLimitToCheck)
	ON_BN_CLICKED(IDC_SC_OK_BTN, &CSmartCollectionsDlg::OnBnClickedScOkBtn)
	ON_BN_CLICKED(IDC_SC_CANCEL_BTN, &CSmartCollectionsDlg::OnBnClickedScCancelBtn)
	ON_CBN_SELENDOK(IDC_SC_NOUNS1_COMBO, &CSmartCollectionsDlg::OnCbnSelendokScNouns1Combo)
	ON_CBN_SELENDOK(IDC_SC_NOUNS2_COMBO, &CSmartCollectionsDlg::OnCbnSelendokScNouns2Combo)
	ON_CBN_SELENDOK(IDC_SC_NOUNS3_COMBO, &CSmartCollectionsDlg::OnCbnSelendokScNouns3Combo)
	ON_CBN_SELENDOK(IDC_SC_NOUNS4_COMBO, &CSmartCollectionsDlg::OnCbnSelendokScNouns4Combo)
	ON_CBN_SELENDOK(IDC_SC_NOUNS5_COMBO, &CSmartCollectionsDlg::OnCbnSelendokScNouns5Combo)
	ON_CBN_SELENDOK(IDC_SC_NOUNS6_COMBO, &CSmartCollectionsDlg::OnCbnSelendokScNouns6Combo)
	ON_CBN_SELENDOK(IDC_SC_NOUNS7_COMBO, &CSmartCollectionsDlg::OnCbnSelendokScNouns7Combo)
	ON_CBN_SELENDOK(IDC_SC_NOUNS8_COMBO, &CSmartCollectionsDlg::OnCbnSelendokScNouns8Combo)
	ON_CBN_SELENDOK(IDC_SC_NOUNS9_COMBO, &CSmartCollectionsDlg::OnCbnSelendokScNouns9Combo)
	ON_CBN_SELENDOK(IDC_SC_NOUNS10_COMBO, &CSmartCollectionsDlg::OnCbnSelendokScNouns10Combo)
END_MESSAGE_MAP()

BOOL CSmartCollectionsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_NounsCombo[0].SetCurSel(0);
	m_VerbsCombo[0].SetCurSel(0);

	CRect lRect;
	GetWindowRect(lRect);
	ScreenToClient(lRect);
	SetWindowPos(NULL, 0, 0, lRect.Width(), mPos * OFFSET + 193, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);

	CWnd* lWnd;
	lWnd = GetDlgItem(IDC_SC_LIMIT_TO_CHECK);
	lWnd->GetWindowRect(lRect);
	ScreenToClient(lRect); 
	lWnd->SetWindowPos(NULL, lRect.left, lRect.top - OFFSET, lRect.Width(), lRect.Height(), SWP_NOZORDER | SWP_NOACTIVATE);

	m_LimitToCheck.GetWindowRect(lRect);
	ScreenToClient(lRect);
	lRect.top -= 8 * OFFSET;
	lRect.bottom -= 8 * OFFSET;
	m_LimitToCheck.MoveWindow(lRect);

	m_LimitToEdit.GetWindowRect(lRect);
	ScreenToClient(lRect);
	lRect.top -= 9 * OFFSET;
	lRect.bottom -= 9 * OFFSET;
	m_LimitToEdit.MoveWindow(lRect);

	m_LimitToCombo.GetWindowRect(lRect);
	ScreenToClient(lRect);
	lRect.top -= 9 * OFFSET;
	lRect.bottom -= 9 * OFFSET;
	m_LimitToCombo.MoveWindow(lRect);

	m_OkBtn.GetWindowRect(lRect);
	ScreenToClient(lRect);
	lRect.top -= 9 * OFFSET;
	lRect.bottom -= 9 * OFFSET;
	m_OkBtn.MoveWindow(lRect);

	m_CancelBtn.GetWindowRect(lRect);
	ScreenToClient(lRect);
	lRect.top -= 9 * OFFSET;
	lRect.bottom -= 9 * OFFSET;
	m_CancelBtn.MoveWindow(lRect);

	CheckDlgButton(IDC_SC_MATCH_THE_FOLLOWING_RULE_CHECK, TRUE);
	CheckDlgButton(IDC_SC_LIMIT_TO_CHECK, FALSE);

	m_RemoveBtn[0].EnableWindow(FALSE);

	for (int i=0; i<N; i++)
	{
		CRect lRect;
		m_ValueEdit[i].GetWindowRect(&lRect);
		ScreenToClient(&lRect);
		m_ValueButton[i].Create(_T(""), m_ValueEdit[i].GetStyle() | WS_BORDER, lRect, this, IDC_SC_VALUE1_BUTTON+i);
		m_ValueButton[i].SetDate(COleDateTime::GetCurrentTime());
		m_ValueButton[i].SetState(CBCGPDateTimeCtrl::DTM_DATE, /*CBCGPDateTimeCtrl::DTM_SPIN | */CBCGPDateTimeCtrl::DTM_CHECKBOX);
		m_ValueWnd[i] = &m_ValueEdit[i];
	}

	for (int i=0; i<N; i++)
		m_ValueStatic[i].ShowWindow(SW_HIDE);

	mTSmartColl->mID = -1;
	mTSmartColl->mName = _T("New Smart Collection");

	InitComboBoxes();
	InitDialogFromDB();

	return TRUE;
}

void CSmartCollectionsDlg::InitComboBoxes()
{
	int lIdx = 0;
	CString lStr;

	for(int i = 0; i<N; i++)
	{
		m_NounsCombo[i].ResetContent();
	}

	for(int i = 0; i<N; i++) //the nouns combo
	{
		lStr.LoadString(IDS_AUTHOR_STR);		// 0
		m_NounsCombo[i].AddString(lStr);	
		lStr.LoadString(IDS_CATEGORY_STR);		// 1
		m_NounsCombo[i].AddString(lStr);	
		lStr.LoadString(IDS_DESCRIPTION_STR);	// 5
		m_NounsCombo[i].AddString(lStr);	
		lStr.LoadString(IDS_DATEADDED_STR);		// 2
		m_NounsCombo[i].AddString(lStr);	
		lStr.LoadString(IDS_DATECREATED_STR);	// 3
		m_NounsCombo[i].AddString(lStr);	
		lStr.LoadString(IDS_DATEMODIFIED_STR);	// 4
		m_NounsCombo[i].AddString(lStr);	
		lStr.LoadString(IDS_FILETYPE_STR);		// 6
		m_NounsCombo[i].AddString(lStr);	
		lStr.LoadString(IDS_LASTOPENED_STR);	// 7
		m_NounsCombo[i].AddString(lStr);	
		lStr.LoadString(IDS_FILENAME_STR);		// 8
		m_NounsCombo[i].AddString(lStr);	
		lStr.LoadString(IDS_PATH_STR);			// 9
		m_NounsCombo[i].AddString(lStr);	
		lStr.LoadString(IDS_SIZE_STR );			// 10
		m_NounsCombo[i].AddString(lStr);	
		lStr.LoadString(IDS_SUBJECT_STR);		// 11
		m_NounsCombo[i].AddString(lStr);	
		lStr.LoadString(IDS_TAGS_STR);			// 12
		m_NounsCombo[i].AddString(lStr);	
		lStr.LoadString(IDS_TITLE_STR);			// 13
		m_NounsCombo[i].AddString(lStr);	
	}

	for(int j = 0; j < N; j++)
	{
		FillVerbsCombo(j);
	}
}

int CSmartCollectionsDlg::GetVerbsComboType(int inComboIdx)
{
	int lType = 1;
	switch(m_NounsCombo[inComboIdx].GetCurSel()) //the verbs combo
	{
		case 0:
		case 1:
		case 2:
		case 6:
		case 8:
		case 9:
		case 11:
		case 12:			
		case 13:
			lType = 1;
			break;
		case 10:
			lType = 2;
			break;
		case 3:
		case 4:			
		case 5:
		case 7:	
			lType = 3;
			break;
		default:
			break;
	}
	return lType;
}

void CSmartCollectionsDlg::ChangeValueControl(int inRow)
{
	int lType = GetVerbsComboType(inRow);
	switch(lType)
	{
	case 2:
		m_ValueButton[inRow].ShowWindow(SW_HIDE);
		m_ValueEdit[inRow].ShowWindow(SW_SHOW);
		m_ValueWnd[inRow] = &m_ValueEdit[inRow];
		break;
	case 3:
		m_ValueEdit[inRow].ShowWindow(SW_HIDE);
		m_ValueButton[inRow].ShowWindow(SW_SHOW);
		m_ValueWnd[inRow] = &m_ValueButton[inRow];
		break;
	default: 
		m_ValueButton[inRow].ShowWindow(SW_HIDE);
		m_ValueEdit[inRow].ShowWindow(SW_SHOW);
		m_ValueWnd[inRow] = &m_ValueEdit[inRow];
		break;
	}
}

void CSmartCollectionsDlg::FillVerbsCombo(int inComboIdx, int inType /*= -1*/)
{
	int lType, lIdx;
	if(inType < 0)
		lType = GetVerbsComboType(inComboIdx);
	else
		lType = inType;

	m_VerbsCombo[inComboIdx].ResetContent();

	CString lStr;
	switch(lType)
	{
	case 1:
		lStr.LoadString(IDS_CONTAINS_STR);
		lIdx = m_VerbsCombo[inComboIdx].AddString(lStr);
		m_VerbsCombo[inComboIdx].SetItemData(lIdx, T1_Contains);

		lStr.LoadString(IDS_DOESNOTCONTAINS_STR);
		lIdx = m_VerbsCombo[inComboIdx].AddString(lStr);
		m_VerbsCombo[inComboIdx].SetItemData(lIdx, T1_DoesNotContains);

		lStr.LoadString(IDS_IS_STR);
		lIdx = m_VerbsCombo[inComboIdx].AddString(lStr);
		m_VerbsCombo[inComboIdx].SetItemData(lIdx, T1_Is);

		lStr.LoadString(IDS_ISNOT_STR);
		lIdx = m_VerbsCombo[inComboIdx].AddString(lStr);
		m_VerbsCombo[inComboIdx].SetItemData(lIdx, T1_IsNot);

		lStr.LoadString(IDS_STARTSWITH_STR);
		lIdx = m_VerbsCombo[inComboIdx].AddString(lStr);
		m_VerbsCombo[inComboIdx].SetItemData(lIdx, T1_StartsWith);

		lStr.LoadString(IDS_ENDSWITH_STR);
		lIdx = m_VerbsCombo[inComboIdx].AddString(lStr);
		m_VerbsCombo[inComboIdx].SetItemData(lIdx, T1_EndsWith);

		break;
	case 2: 
		lStr.LoadString(IDS_ISGREATERTHAN_STR);
		lIdx = m_VerbsCombo[inComboIdx].AddString(lStr);
		m_VerbsCombo[inComboIdx].SetItemData(lIdx, T2_IsGreaterThan);

		lStr.LoadString(IDS_ISLESSTHAN_STR);
		lIdx = m_VerbsCombo[inComboIdx].AddString(lStr);
		m_VerbsCombo[inComboIdx].SetItemData(lIdx, T2_IsLessThan);

		lStr.LoadString(IDS_ISINRANGE_STR);
		lIdx = m_VerbsCombo[inComboIdx].AddString(lStr);
		m_VerbsCombo[inComboIdx].SetItemData(lIdx, T2_IsInTheRange);

		break;
	case 3: 
		lStr.LoadString(IDS_IS_STR);
		lIdx = m_VerbsCombo[inComboIdx].AddString(lStr);
		m_VerbsCombo[inComboIdx].SetItemData(lIdx, T3_Is);

		lStr.LoadString(IDS_ISNOT_STR);
		lIdx = m_VerbsCombo[inComboIdx].AddString(lStr);
		m_VerbsCombo[inComboIdx].SetItemData(lIdx, T3_IsNot);

		lStr.LoadString(IDS_ISAFTER_STR);
		lIdx = m_VerbsCombo[inComboIdx].AddString(lStr);
		m_VerbsCombo[inComboIdx].SetItemData(lIdx, T3_IsAfter);

		lStr.LoadString(IDS_ISBEFORE_STR);
		lIdx = m_VerbsCombo[inComboIdx].AddString(lStr);
		m_VerbsCombo[inComboIdx].SetItemData(lIdx, T3_IsBefore);

		break;
	default: 
		break;
	}
	m_VerbsCombo[inComboIdx].SetCurSel(0);
}

void CSmartCollectionsDlg::InitVerbsComboFromDB(int inComboIdx, int inDBValue)
{
	if(inDBValue < T2_IsGreaterThan)
	{
		FillVerbsCombo(inComboIdx, 1);
		m_VerbsCombo[inComboIdx].SetCurSel(inDBValue - T1_Contains);
		ChangeValueControl(inComboIdx);
	}
	else if(inDBValue < T3_Is)
	{
		FillVerbsCombo(inComboIdx, 2);
		m_VerbsCombo[inComboIdx].SetCurSel(inDBValue - T2_IsGreaterThan);
		ChangeValueControl(inComboIdx);
	}
	else if(inDBValue < (T3_IsBefore +1))
	{
		FillVerbsCombo(inComboIdx, 3);
		m_VerbsCombo[inComboIdx].SetCurSel(inDBValue - T3_Is);
		ChangeValueControl(inComboIdx);
	}
	else
		return;
}

void CSmartCollectionsDlg::InitDialogFromDB()
{
	if(mSmartColID < 0)
	{ // use default settings
		m_NounsCombo[0].SetCurSel(0);
		FillVerbsCombo(0);
		ChangeValueControl(0);
		m_LimitToCombo.SetCurSel(0);
		return;
	}

	if(mTSmartColl->GetSmartCollByID(mSmartColID))
	{
		if(mTSmartColl->mDataIsValid)
		{
			for(int i=1; i<mTSmartColl->mNumActiveFilters; i++)
			{
				ShowRow(i-1);//show the required number of rows
			}
			for(int i = 0; i<=mPos; i++)
			{
				m_NounsCombo[i].SetCurSel(mTSmartColl->mCombo1[i]);
				InitVerbsComboFromDB(i, mTSmartColl->mCombo2[i]);

				int lType = GetVerbsComboType(i);
				switch(lType)
				{
				case 3:{
					COleDateTime lDT;
					if (lDT.ParseDateTime(mTSmartColl->mValue[i]) == false)
						lDT = COleDateTime::GetCurrentTime();
					m_ValueButton[i].SetDate(DATE(lDT));
					}break;
				default: 
					m_ValueEdit[i].SetWindowText(mTSmartColl->mValue[i]);
					break;
				}
			}
			if(mTSmartColl->mMatchTheFollowingRuleBool)
				CheckDlgButton(IDC_SC_MATCH_THE_FOLLOWING_RULE_CHECK, TRUE);
			else
				CheckDlgButton(IDC_SC_MATCH_THE_FOLLOWING_RULE_CHECK, FALSE);
			
			if(mTSmartColl->mLimitToBool)
				CheckDlgButton(IDC_SC_LIMIT_TO_CHECK, TRUE);
			else
				CheckDlgButton(IDC_SC_LIMIT_TO_CHECK, FALSE);

			SetDlgItemText(IDC_SC_LIMIT_TO_EDIT, mTSmartColl->mLimitToStr);
			m_LimitToCombo.SetCurSel(mTSmartColl->mLimitToCombo);
		}
	}
}
void CSmartCollectionsDlg::ShowRow(int inCurrentRow)
{
	if (mPos < N - 1)
	{
		mPos++;
		m_NounsCombo[mPos].ShowWindow(SW_SHOW);
		m_VerbsCombo[mPos].ShowWindow(SW_SHOW);
		m_RemoveBtn[mPos].ShowWindow(SW_SHOW);
		m_AddBtn[mPos].ShowWindow(SW_SHOW);
		if (m_ValueWnd[mPos])
			m_ValueWnd[mPos]->ShowWindow(SW_SHOW);

		CRect lRect;
		GetWindowRect(lRect);
		ScreenToClient(lRect);
		SetWindowPos(NULL, 0, 0, lRect.Width(), mPos * OFFSET + 193, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);

		m_LimitToCheck.GetWindowRect(lRect);
		ScreenToClient(lRect);
		lRect.top += OFFSET;
		lRect.bottom += OFFSET;
		m_LimitToCheck.MoveWindow(lRect);

		m_LimitToEdit.GetWindowRect(lRect);
		ScreenToClient(lRect);
		lRect.top += OFFSET;
		lRect.bottom += OFFSET;
		m_LimitToEdit.MoveWindow(lRect);

		m_LimitToCombo.GetWindowRect(lRect);
		ScreenToClient(lRect);
		lRect.top += OFFSET;
		lRect.bottom += OFFSET;
		m_LimitToCombo.MoveWindow(lRect);

		m_OkBtn.GetWindowRect(lRect);
		ScreenToClient(lRect);
		lRect.top += OFFSET;
		lRect.bottom += OFFSET;
		m_OkBtn.MoveWindow(lRect);

		m_CancelBtn.GetWindowRect(lRect);
		ScreenToClient(lRect);
		lRect.top += OFFSET;
		lRect.bottom += OFFSET;
		m_CancelBtn.MoveWindow(lRect);

		BOOL lEnable = TRUE;
		if (mPos == N -1) lEnable = FALSE;

		for (int i = 0; i < N; i++)
		{
			m_AddBtn[i].EnableWindow(lEnable);
			m_RemoveBtn[i].EnableWindow(TRUE);
		}
	}
	CString lStr;
	int lIndex;
	int i;
	for (i = mPos; i > inCurrentRow; i--)
	{	
		m_NounsCombo[i - 1].GetWindowText(lStr);
		lIndex = m_NounsCombo[i - 1].FindStringExact(0, lStr);
		m_NounsCombo[i].SetCurSel(lIndex != CB_ERR ? lIndex : 0);

		FillVerbsCombo(i);

		m_VerbsCombo[i - 1].GetWindowText(lStr);
		lIndex = m_VerbsCombo[i - 1].FindStringExact(0, lStr);
		m_VerbsCombo[i].SetCurSel(lIndex != CB_ERR ? lIndex : 0);
		
		ChangeValueControl(i);

		//values
		m_ValueEdit[i - 1].GetWindowText(lStr);
		m_ValueEdit[i].SetWindowText(lStr);
		DATE lDate = m_ValueButton[i - 1].GetDate();
		m_ValueButton[i].SetDate(lDate);
	}
	m_NounsCombo[i].SetCurSel(0);
	FillVerbsCombo(i);
	m_VerbsCombo[i].SetCurSel(0);
	ChangeValueControl(i);
	m_ValueEdit[i].SetWindowText(_T(""));
}

void CSmartCollectionsDlg::HideRow(int inCurrentRow)
{
	inCurrentRow--;
	CString lStr;
	int lIndex;
	int i;
	for (i = inCurrentRow; i < mPos; i++)
	{
		m_NounsCombo[i + 1].GetWindowText(lStr);
		lIndex = m_NounsCombo[i + 1].FindStringExact(0, lStr);
		m_NounsCombo[i].SetCurSel(lIndex != CB_ERR ? lIndex : 0);

		FillVerbsCombo(i);

		m_VerbsCombo[i + 1].GetWindowText(lStr);
		lIndex = m_VerbsCombo[i + 1].FindStringExact(0, lStr);
		m_VerbsCombo[i].SetCurSel(lIndex != CB_ERR ? lIndex : 0);

		ChangeValueControl(i);

		// values
		m_ValueEdit[i + 1].GetWindowText(lStr);
		m_ValueEdit[i].SetWindowText(lStr);
		DATE lDate = m_ValueButton[i + 1].GetDate();
		m_ValueButton[i].SetDate(lDate);
	}
	if (mPos > 0)
	{
		m_NounsCombo[mPos].ShowWindow(SW_HIDE);
		m_VerbsCombo[mPos].ShowWindow(SW_HIDE);
		m_RemoveBtn[mPos].ShowWindow(SW_HIDE);
		m_AddBtn[mPos].ShowWindow(SW_HIDE);
		if (m_ValueWnd[mPos])
			m_ValueWnd[mPos]->ShowWindow(SW_HIDE);

		mPos--;

		CRect lRect;
		GetWindowRect(lRect);
		ScreenToClient(lRect);
		SetWindowPos(NULL, 0, 0, lRect.Width(), mPos * OFFSET + 193, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);

		m_LimitToCheck.GetWindowRect(lRect);
		ScreenToClient(lRect);
		lRect.top -= OFFSET;
		lRect.bottom -= OFFSET;
		m_LimitToCheck.MoveWindow(lRect);

		m_LimitToEdit.GetWindowRect(lRect);
		ScreenToClient(lRect);
		lRect.top -= OFFSET;
		lRect.bottom -= OFFSET;
		m_LimitToEdit.MoveWindow(lRect);

		m_LimitToCombo.GetWindowRect(lRect);
		ScreenToClient(lRect);
		lRect.top -= OFFSET;
		lRect.bottom -= OFFSET;
		m_LimitToCombo.MoveWindow(lRect);

		m_OkBtn.GetWindowRect(lRect);
		ScreenToClient(lRect);
		lRect.top -= OFFSET;
		lRect.bottom -= OFFSET;
		m_OkBtn.MoveWindow(lRect);

		m_CancelBtn.GetWindowRect(lRect);
		ScreenToClient(lRect);
		lRect.top -= OFFSET;
		lRect.bottom -= OFFSET;
		m_CancelBtn.MoveWindow(lRect);

		BOOL lEnable = TRUE;
		if (mPos == 0) lEnable = FALSE;
		for (int i = 0; i < N; i++)
		{
			m_AddBtn[i].EnableWindow(TRUE);
			m_RemoveBtn[i].EnableWindow(lEnable);
		}
	}
}

void CSmartCollectionsDlg::UpdateUI()
{
	BOOL lEnable;
	lEnable = IsDlgButtonChecked(IDC_SC_MATCH_THE_FOLLOWING_RULE_CHECK);
	for (int i = 0; i < N; i++)
	{
		m_NounsCombo[i].EnableWindow(lEnable);
		m_VerbsCombo[i].EnableWindow(lEnable);
		m_ValueEdit[i].EnableWindow(lEnable);
		m_ValueButton[i].EnableWindow(lEnable);

		m_RemoveBtn[i].EnableWindow(lEnable);
		m_AddBtn[i].EnableWindow(lEnable);
	}
	m_OkBtn.EnableWindow(lEnable);
}

void CSmartCollectionsDlg::OnBnClickedScMatchTheFollowingRuleCheck()
{
	UpdateUI();
}

void CSmartCollectionsDlg::OnBnClickedScOkBtn()
{
	mTSmartColl->mMatchTheFollowingRuleBool = IsDlgButtonChecked(IDC_SC_MATCH_THE_FOLLOWING_RULE_CHECK);
	mTSmartColl->mLimitToBool = IsDlgButtonChecked(IDC_SC_LIMIT_TO_CHECK);
	GetDlgItemText(IDC_SC_LIMIT_TO_EDIT, mTSmartColl->mLimitToStr);
	mTSmartColl->mLimitToCombo = m_LimitToCombo.GetCurSel();
	mTSmartColl->mNumActiveFilters = mPos+1;

	for(int i = 0; i<=mPos; i++)
	{
		mTSmartColl->mCombo1[i] = m_NounsCombo[i].GetCurSel();
		mTSmartColl->mCombo2[i] =  m_VerbsCombo[i].GetItemData(m_VerbsCombo[i].GetCurSel());
		
		int lType = GetVerbsComboType(i);
		switch(lType)
		{
		case 3:{
			DATE lDate = m_ValueButton[i].GetDate();
			COleDateTime lDT(lDate);
			mTSmartColl->mValue[i].Format(_T("%d-%02d-%02d %02d:%02d:%02d"),  lDT.GetYear(), lDT.GetMonth(), lDT.GetDay(), lDT.GetHour(), lDT.GetMinute(), lDT.GetSecond());
			}break;
		default: 
			m_ValueEdit[i].GetWindowText(mTSmartColl->mValue[i]);
			break;
		}
	}

	OnOK();
}

void CSmartCollectionsDlg::OnBnClickedScCancelBtn()
{
	OnCancel();
}

void CSmartCollectionsDlg::OnCbnSelendokScNouns1Combo()
{
	FillVerbsCombo(0);
	ChangeValueControl(0);
}

void CSmartCollectionsDlg::OnCbnSelendokScNouns2Combo()
{
	FillVerbsCombo(1);
	ChangeValueControl(1);
}

void CSmartCollectionsDlg::OnCbnSelendokScNouns3Combo()
{
	FillVerbsCombo(2);
	ChangeValueControl(2);
}

void CSmartCollectionsDlg::OnCbnSelendokScNouns4Combo()
{
	FillVerbsCombo(3);
	ChangeValueControl(3);
}

void CSmartCollectionsDlg::OnCbnSelendokScNouns5Combo()
{
	FillVerbsCombo(4);
	ChangeValueControl(4);
}

void CSmartCollectionsDlg::OnCbnSelendokScNouns6Combo()
{
	FillVerbsCombo(5);
	ChangeValueControl(5);
}

void CSmartCollectionsDlg::OnCbnSelendokScNouns7Combo()
{
	FillVerbsCombo(6);
	ChangeValueControl(6);
}

void CSmartCollectionsDlg::OnCbnSelendokScNouns8Combo()
{
	FillVerbsCombo(7);
	ChangeValueControl(7);
}

void CSmartCollectionsDlg::OnCbnSelendokScNouns9Combo()
{
	FillVerbsCombo(8);
	ChangeValueControl(8);
}

void CSmartCollectionsDlg::OnCbnSelendokScNouns10Combo()
{
	FillVerbsCombo(9);
	ChangeValueControl(9);
}
