
AnimAlgps.dll: dlldata.obj AnimAlg_p.obj AnimAlg_i.obj
	link /dll /out:AnimAlgps.dll /def:AnimAlgps.def /entry:DllMain dlldata.obj AnimAlg_p.obj AnimAlg_i.obj \
		kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib \

.c.obj:
	cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0400 /DREGISTER_PROXY_DLL \
		$<

clean:
	@del AnimAlgps.dll
	@del AnimAlgps.lib
	@del AnimAlgps.exp
	@del dlldata.obj
	@del AnimAlg_p.obj
	@del AnimAlg_i.obj
