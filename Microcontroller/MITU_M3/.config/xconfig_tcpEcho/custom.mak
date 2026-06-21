## THIS IS A GENERATED FILE -- DO NOT EDIT
.configuro: .libraries,em3 linker.cmd package/cfg/tcpEcho_pem3.oem3

# To simplify configuro usage in makefiles:
#     o create a generic linker command file name 
#     o set modification times of compiler.opt* files to be greater than
#       or equal to the generated config header
#
linker.cmd: package/cfg/tcpEcho_pem3.xdl
	$(SED) 's"^\"\(package/cfg/tcpEcho_pem3cfg.cmd\)\"$""\"C:/Users/Volkodav/workspace_v6_2/MITU_M3/.config/xconfig_tcpEcho/\1\""' package/cfg/tcpEcho_pem3.xdl > $@
	-$(SETDATE) -r:max package/cfg/tcpEcho_pem3.h compiler.opt compiler.opt.defs
