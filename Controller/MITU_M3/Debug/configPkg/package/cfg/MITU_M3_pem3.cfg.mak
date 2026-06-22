# invoke SourceDir generated makefile for MITU_M3.pem3
MITU_M3.pem3: .libraries,MITU_M3.pem3
.libraries,MITU_M3.pem3: package/cfg/MITU_M3_pem3.xdl
	$(MAKE) -f C:\Users\user\ekb\Controller\MITU_M3/src/makefile.libs

clean::
	$(MAKE) -f C:\Users\user\ekb\Controller\MITU_M3/src/makefile.libs clean

