# invoke SourceDir generated makefile for MITU_M3.pem3
MITU_M3.pem3: .libraries,MITU_M3.pem3
.libraries,MITU_M3.pem3: package/cfg/MITU_M3_pem3.xdl
	$(MAKE) -f C:\PROJECTS\workspace_v6_2\MITU_M3/src/makefile.libs

clean::
	$(MAKE) -f C:\PROJECTS\workspace_v6_2\MITU_M3/src/makefile.libs clean

