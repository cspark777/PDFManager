// Machine generated IDispatch wrapper class(es) created with Add Class from Typelib Wizard

#ifdef _DEBUG
//#import "Q:\\Benubird\\runtime\\debug\\BenubirdPro.exe" no_namespace
#else
//#import "Q:\\Benubird\\runtime\\release\\BenubirdPro.exe" no_namespace
#endif
// CBenubird wrapper class

class CBenubird : public COleDispatchDriver
{
public:
	CBenubird(){} // Calls COleDispatchDriver default constructor
	CBenubird(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CBenubird(const CBenubird& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
public:

	// Operations
public:


	// IBenubird methods
public:
	void Show()
	{
		InvokeHelper(0x1, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	long AddFile(LPCTSTR file)
	{
		long result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x2, DISPATCH_METHOD, VT_I4, (void*)&result, parms, file);
		return result;
	}
	long ConvertToPDF(LPCTSTR fileList)
	{
		long result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x3, DISPATCH_METHOD, VT_I4, (void*)&result, parms, fileList);
		return result;
	}

	// IBenubird properties
public:

};
