#
_XDCBUILDCOUNT = 0
ifneq (,$(findstring path,$(_USEXDCENV_)))
override XDCPATH = C:/ti/tirtos_c2000_2_16_00_08/packages;C:/ti/tirtos_c2000_2_16_00_08/products/tidrivers_c2000_2_16_00_08/packages;C:/ti/tirtos_c2000_2_16_00_08/products/bios_6_45_01_29/packages;C:/ti/tirtos_c2000_2_16_00_08/products/ndk_2_25_00_09/packages;C:/ti/tirtos_c2000_2_16_00_08/products/uia_2_00_05_50/packages;C:/ti/ccsv6/ccs_base;C:/PROJECTS/workspace_v6_2/MITU_M3/.config
override XDCROOT = C:/ti/xdctools_3_32_00_06_core
override XDCBUILDCFG = ./config.bld
endif
ifneq (,$(findstring args,$(_USEXDCENV_)))
override XDCARGS = 
override XDCTARGETS = 
endif
#
ifeq (0,1)
PKGPATH = C:/ti/tirtos_c2000_2_16_00_08/packages;C:/ti/tirtos_c2000_2_16_00_08/products/tidrivers_c2000_2_16_00_08/packages;C:/ti/tirtos_c2000_2_16_00_08/products/bios_6_45_01_29/packages;C:/ti/tirtos_c2000_2_16_00_08/products/ndk_2_25_00_09/packages;C:/ti/tirtos_c2000_2_16_00_08/products/uia_2_00_05_50/packages;C:/ti/ccsv6/ccs_base;C:/PROJECTS/workspace_v6_2/MITU_M3/.config;C:/ti/xdctools_3_32_00_06_core/packages;..
HOSTOS = Windows
endif
