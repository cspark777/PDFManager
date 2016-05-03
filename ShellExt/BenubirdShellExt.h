// BenubirdShellExt.h : Declaration of the CBenubirdShellExt

#ifndef __BENUBIRDSHELLEXT_H_
#define __BENUBIRDSHELLEXT_H_

#include "resource.h"       // main symbols

#include <shlobj.h>
#include <comdef.h>

#include "CBenubird.h"

struct __declspec(uuid("000214e4-0000-0000-c000-000000000046")) IContextMenu; 
struct __declspec(uuid("000214F4-0000-0000-c000-000000000046")) IContextMenu2;
struct __declspec(uuid("BCFCE0A0-EC17-11D0-8D10-00A0C90F2719")) IContextMenu3; 


/////////////////////////////////////////////////////////////////////////////
// CBenubirdShellExt
class ATL_NO_VTABLE CBenubirdShellExt : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CBenubirdShellExt, &CLSID_BenubirdShellExt>,
	public IDispatchImpl<IBenubirdShellExt, &IID_IBenubirdShellExt, &LIBID_BENUBIRDSHELLLib>,
	public IShellExtInit,
	public IContextMenu3
{
public:
	CBenubirdShellExt()
	{
		m_BenubirdStarted = FALSE;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_BENUBIRDSHELLEXT)
//DECLARE_NOT_AGGREGATABLE(CBenubirdShellExt)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CBenubirdShellExt)
	COM_INTERFACE_ENTRY(IBenubirdShellExt)
//	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IShellExtInit)
	COM_INTERFACE_ENTRY(IContextMenu)
	COM_INTERFACE_ENTRY(IContextMenu2)
	COM_INTERFACE_ENTRY(IContextMenu3)
END_COM_MAP()

protected:
  TCHAR m_szFile[MAX_PATH];
  CStringList m_FileList;

  CBenubird m_Benubird;
  BOOL m_BenubirdStarted;


public:
  // IBenubirdShellExt

public:
  // IShellExtInit

  STDMETHODIMP Initialize(LPCITEMIDLIST, LPDATAOBJECT, HKEY);

public:
    // IContextMenu
    STDMETHOD(QueryContextMenu)(HMENU, UINT, UINT, UINT, UINT);
    STDMETHOD(InvokeCommand)(LPCMINVOKECOMMANDINFO);
//    STDMETHOD(GetCommandString)(UINT, UINT, UINT*, LPSTR, UINT);
	STDMETHOD(GetCommandString(UINT_PTR,UINT,UINT *,LPSTR,UINT));
    // IContextMenu2
    STDMETHOD(HandleMenuMsg)(UINT, WPARAM, LPARAM);

    // IContextMenu3
    STDMETHOD(HandleMenuMsg2)(UINT, WPARAM, LPARAM, LRESULT*);
};

#endif //__BENUBIRDSHELLEXT_H_
