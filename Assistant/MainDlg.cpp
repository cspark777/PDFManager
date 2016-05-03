
//#include "stdafx.h"
//#include "Main.h"
#include "afxwin.h"
#include "MainDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include "CDB.h"

///////////////////////////////////////////////////////////////////////////////////
extern HWND g_AssistantWnd;
extern void ProcessMessage_FromBenubird(int msg, int len,LPCTSTR strBuf);

// CAboutDlg class
///////////////////////////////////////////////////////////////////////////////////
CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


///////////////////////////////////////////////////////////////////////////////////
// CBenubirdAssistantDlg class
///////////////////////////////////////////////////////////////////////////////////
CBenubirdAssistantDlg::CBenubirdAssistantDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBenubirdAssistantDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CBenubirdAssistantDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CBenubirdAssistantDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//ON_REGISTERED_MESSAGE(BENUBIRD_ASSISTANT_STOP, OnAssistantStop)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CBenubirdAssistantDlg::OnBnClickedOk)
	ON_WM_COPYDATA()
END_MESSAGE_MAP()

DWORD CBenubirdAssistantDlg::AllowMsg(UINT msg)
{

	typedef BOOL (WINAPI *FPCHANGEWINDOWMESSAGEFILTEREX)(HWND hWnd,UINT message,DWORD action,PCHANGEFILTERSTRUCT pChangeFilterStruct);
	typedef BOOL (WINAPI *FPCHANGEWINDOWMESSAGEFILTER)(UINT message,DWORD dwFlag);
	

	HMODULE hUser32 = LoadLibrary(_T("user32.dll"));
	if (hUser32)
	{
		// Try ChangeWindowMessageFilterEx first (Win7+)
		FPCHANGEWINDOWMESSAGEFILTEREX ChangeWindowMessageFilterEx = (FPCHANGEWINDOWMESSAGEFILTEREX)GetProcAddress(hUser32, "ChangeWindowMessageFilterEx");
		if (ChangeWindowMessageFilterEx)
		{
			ChangeWindowMessageFilterEx(this->GetSafeHwnd(), msg, MSGFLT_ALLOW, nullptr);
		}
		else
		{
			// Try ChangeWindowMessageFilter (Vista)
			FPCHANGEWINDOWMESSAGEFILTER ChangeWindowMessageFilter = (FPCHANGEWINDOWMESSAGEFILTER)GetProcAddress(hUser32, "ChangeWindowMessageFilter");
			if (ChangeWindowMessageFilter)
			{
				ChangeWindowMessageFilter(msg, MSGFLT_ADD);
			}
		}

		FreeLibrary(hUser32);
	}
	return msg;
}


BOOL CBenubirdAssistantDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	g_AssistantWnd = this->GetSafeHwnd();

	AllowMsg(WM_COPYDATA);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CBenubirdAssistantDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.
void CBenubirdAssistantDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CBenubirdAssistantDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

LRESULT CBenubirdAssistantDlg::OnAssistantStop( WPARAM, LPARAM lParam )
{
    int lID = (int) lParam;
	return 0;
}

void CBenubirdAssistantDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialog::OnOK();
}


BOOL CBenubirdAssistantDlg::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct)
{
	ProcessMessage_FromBenubird(pCopyDataStruct->dwData,pCopyDataStruct->cbData,LPCTSTR(pCopyDataStruct->lpData));
	return true;
}
