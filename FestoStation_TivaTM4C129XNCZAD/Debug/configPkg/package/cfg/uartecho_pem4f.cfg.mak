# invoke SourceDir generated makefile for uartecho.pem4f
uartecho.pem4f: .libraries,uartecho.pem4f
.libraries,uartecho.pem4f: package/cfg/uartecho_pem4f.xdl
	$(MAKE) -f H:\TI_Workspace\FestoStation_TivaTM4C129XNCZAD/src/makefile.libs

clean::
	$(MAKE) -f H:\TI_Workspace\FestoStation_TivaTM4C129XNCZAD/src/makefile.libs clean

