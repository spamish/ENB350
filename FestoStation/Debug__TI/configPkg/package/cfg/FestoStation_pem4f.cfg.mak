# invoke SourceDir generated makefile for FestoStation.pem4f
FestoStation.pem4f: .libraries,FestoStation.pem4f
.libraries,FestoStation.pem4f: package/cfg/FestoStation_pem4f.xdl
	$(MAKE) -f H:\TI_Workspace\FestoStation/src/makefile.libs

clean::
	$(MAKE) -f H:\TI_Workspace\FestoStation/src/makefile.libs clean

