#include "stdafx.h"
#include "CIconComboBox.h"

CIconComboBox::CIconComboBox() : CComboBox()
{
	m_ImageList = NULL;
}

CIconComboBox::~CIconComboBox()
{
}

BEGIN_MESSAGE_MAP(CIconComboBox, CComboBox)
	//{{AFX_MSG_MAP(CIconComboBox)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CIconComboBox::SetImageList(CImageList * inImageList)
{
	if (m_ImageList != NULL)
		delete m_ImageList;

	m_ImageList = new CImageList;
	m_ImageList->Create(inImageList);
}

void CIconComboBox::SetItemIcon(int iItem, UINT iconID)
{
	ITEMDATA iData;
	if (m_IconMap.Lookup(iItem,iData))
		iData.iconID = iconID;
	else
	{
		ITEMDATA_Init(&iData);
		iData.iconID = iconID;
	}

	m_IconMap.SetAt(iItem,iData);
	Invalidate();
}

void CIconComboBox::SetItemIndent(int iItem, int inIndent)
{
	ITEMDATA iData;
	if (m_IconMap.Lookup(iItem,iData))
		iData.indent = inIndent;
	else
	{
		ITEMDATA_Init(&iData);
		iData.indent = inIndent;
	}

	m_IconMap.SetAt(iItem,iData);
	Invalidate();
}

void CIconComboBox::SetItemBold(int iItem, bool inBold)
{
	ITEMDATA iData;
	if (m_IconMap.Lookup(iItem,iData))
		iData.bold = inBold;
	else
	{
		ITEMDATA_Init(&iData);
		iData.bold = inBold;
	}

	m_IconMap.SetAt(iItem,iData);
	Invalidate();
}

void CIconComboBox::SetItemItalic(int iItem, bool inItalic)
{
	ITEMDATA iData;
	if (m_IconMap.Lookup(iItem,iData))
		iData.italic = inItalic;
	else
	{
		ITEMDATA_Init(&iData);
		iData.italic = inItalic;
	}

	m_IconMap.SetAt(iItem,iData);
	Invalidate();
}


void CIconComboBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	if (GetCount() == 0) return;
	if (lpDrawItemStruct->itemID == -1) return;

	CString str;
	GetLBText(lpDrawItemStruct->itemID,str);

	CDC dc;
	dc.Attach(lpDrawItemStruct->hDC);

	//Save these value to restore them when done drawing.
	COLORREF crOldTextColor = dc.GetTextColor();
	COLORREF crOldBkColor = dc.GetBkColor();

	//If this item is selected, set the background color 
	//and the text color to appropriate values. Erase
	//the rect by filling it with the background color.
	if ((lpDrawItemStruct->itemAction | ODA_SELECT) && (lpDrawItemStruct->itemState  & ODS_SELECTED))
	{
		dc.SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));
		dc.SetBkColor(::GetSysColor(COLOR_HIGHLIGHT));
		dc.FillSolidRect(&lpDrawItemStruct->rcItem, ::GetSysColor(COLOR_HIGHLIGHT));
	}
	else
		dc.FillSolidRect(&lpDrawItemStruct->rcItem, crOldBkColor);

	CRect rect(lpDrawItemStruct->rcItem);
	rect.DeflateRect(1,0);

	HICON hIcon = NULL;
	BOOL bold = FALSE;
	BOOL italic = FALSE;
	int indent = 0;
	ITEMDATA iData;
	if (m_IconMap.Lookup(lpDrawItemStruct->itemID,iData))
	{
		if (iData.iconID != -1)
		{
			if (m_ImageList == NULL)
				hIcon = (HICON)::LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(iData.iconID), IMAGE_ICON, 16, 16, 0);
			else
				hIcon = m_ImageList->ExtractIcon(iData.iconID);
		}
		bold = iData.bold;
		indent = iData.indent;
		italic = iData.italic;
	}

	rect.left += (indent*10);

	DrawIconEx(dc.GetSafeHdc(),rect.left,rect.top,hIcon,0, 0, 0, NULL, DI_NORMAL);

	rect.left += 20;

	CFont *curFont = dc.GetCurrentFont();
	CFont myFont,*oldFont;
	LOGFONT lf;
	curFont->GetLogFont(&lf);
	if (bold)
		lf.lfWeight = FW_BOLD;
	if (italic)
		lf.lfItalic = TRUE;

	myFont.CreateFontIndirect(&lf);
	oldFont = dc.SelectObject(&myFont);

	//Draw the text.
	dc.DrawText(str, -1, &rect, DT_LEFT|DT_SINGLELINE|DT_VCENTER);

	dc.SelectObject(oldFont);

	myFont.DeleteObject();

	//Reset the background color and the text color back to their original values.
	dc.SetTextColor(crOldTextColor);
	dc.SetBkColor(crOldBkColor);

	dc.Detach();
}
