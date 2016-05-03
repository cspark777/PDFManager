
BenubirdShellps.dll: dlldata.obj BenubirdShell_p.obj BenubirdShell_i.obj
	link /dll /out:BenubirdShellps.dll /def:BenubirdShellps.def /entry:DllMain dlldata.obj BenubirdShell_p.obj BenubirdShell_i.obj \
		kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib \

.c.obj:
	cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0400 /DREGISTER_PROXY_DLL \
		$<

clean:
	@del BenubirdShellps.dll
	@del BenubirdShellps.lib
	@del BenubirdShellps.exp
	@del dlldata.obj
	@del BenubirdShell_p.obj
	@del BenubirdShell_i.obj
