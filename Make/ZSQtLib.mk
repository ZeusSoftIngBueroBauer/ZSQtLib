#*******************************************************************************
#
# Copyright 2004 - 2017 by    ZeusSoft, Ing. Buero Bauer
#                             Gewerbepark 28
#                             D-83670 Bad Heilbrunn
#                             Tel: 0049 8046 9488
#                             www.zeussoft.de
#                             E-Mail: mailbox@zeussoft.de
#
#-------------------------------------------------------------------------------
#
# Content: This file is part of the ZSQtLib.
#
# This file may be used with no license restrictions for your needs. But it is not
# allowed to resell any modules of the ZSQtLib veiling the original developer of
# the modules. Therefore the copyright link to ZeusSoft, Ing. Buero Bauer must not
# be removed from the header of the source code modules.
#
# ZeusSoft, Ing. Buero Bauer provides the source code as is without any guarantee
# that the code is written without faults.
#
# ZeusSoft, Ing. Buero Bauer does not assume any liability for any damages which
# may result in using the software modules.
#
#-------------------------------------------------------------------------------
#
# $Date: $
#
# $Revision: $
#
# $Author: $
#
# $History: $
#
#*******************************************************************************

# Get absolute path and directory name of this make file.
# As make reads various makefiles, including any obtained from the MAKEFILES
# variable, the command line, the default files, or from include directives,
# their names will be automatically appended to the MAKEFILE_LIST variable.
# They are added right before make begins to parse them.
# This means that if the first thing a makefile does is examine the last word
# in this variable, it will be the name of the current makefile.
# But please note that if once the current makefile has used include, however,
# the last word will be the just-included makefile.
# Please note that $(lastword $(MAKEFILE_LIST)) may already return an absolute path.
# If so both $(abspath $(lastword $(MAKEFILE_LIST))) and $(realpath $(lastword $(MAKEFILE_LIST)))
# don't return the absolute path but insert the absolute path of the make file before
# the absolute path again resulting in a MAKEFILE_PATH like "C:\...\Make\ZSSys\C:\...\Make\ZSSys\ZSSys.mk".
# So we only apply realpath to detect the MAKEFILE_PATH if MAKEFILE_LIST does not contain
# an absolute path already.

MAKEFILE_PATH := $(lastword $(MAKEFILE_LIST))
ifeq ($(findstring :,$(MAKEFILE_PATH)), )
    MAKEFILE_PATH := $(realpath $(lastword $(MAKEFILE_LIST)))
endif
MAKEFILE_PATH := $(subst \\,/,$(MAKEFILE_PATH))
MAKEFILE_DIR := $(shell dirname $(MAKEFILE_PATH))

$(info )
$(info ---> ****************************************************************** )
$(info ---> $(MAKEFILE_PATH) )
$(info ---> ****************************************************************** )
$(info )

#===============================================================================
# Preconditions
#===============================================================================

# Environment Variable ZSQTLIB_DIR
#-------------------------------------------------------------------------------
# Range [Valid Directory Path]
# This environment variable specifies the base directory of the ZSQtLib files.
# The ZSQTLIB_DIR environment variable is used to automatically set other
# directory paths to be used on processing this make file as those directories
# should be located relative to this base directory.
# If not yet set before processing this make file it will be evaluated from the
# directory path of this make file.
# As make reads various makefiles, including any obtained from the MAKEFILES
# variable, the command line, the default files, or from include directives,
# their names will be automatically appended to the MAKEFILE_LIST variable.
# They are added right before make begins to parse them.
# This means that if the first thing a makefile does is examine the last word
# in this variable, it will be the name of the current makefile.
# But please note that if once the current makefile has used include, however,
# the last word will be the just-included makefile.
# To extract the ZSQtLib base directory from the make files directory the
# substring "/Make" will be removed from the end of MAKEFILE_DIR.
PATH_SEP_TMP := \\
PATH_SEP_TMP := $(subst $(PATH_SEP_TMP),\,$(PATH_SEP_TMP))
ifdef ZSQTLIB_DIR
    ZSQTLIB_DIR := $(subst $(PATH_SEP_TMP),/,$(ZSQTLIB_DIR))
else
    MAKEFILE_DIR := $(shell dirname $(MAKEFILE_PATH))
    ZSQTLIB_DIR := $(subst /Make,,$(MAKEFILE_DIR))
    ZSQTLIB_DIR := $(subst PATH_SEP_TMP,/,$(ZSQTLIB_DIR))
    export ZSQTLIB_DIR
endif
ifeq ($(wildcard $(ZSQTLIB_DIR)),)
    $(error $(ZSQTLIB_DIR) does not exist!)
endif


#===============================================================================
# Make Targets
#===============================================================================

ZSQTLIB_TARGETS := 
ZSQTLIB_TARGETS := $(ZSQTLIB_TARGETS) build_ZSQtLib_ZSSys build_ZSQtLib_ZSSysGUI
ZSQTLIB_TARGETS := $(ZSQTLIB_TARGETS) build_ZSQtLib_ZSIpc build_ZSQtLib_ZSIpcGUI
ZSQTLIB_TARGETS := $(ZSQTLIB_TARGETS) build_ZSQtLib_ZSTrace build_ZSQtLib_ZSTraceGUI
ZSQTLIB_TARGETS := $(ZSQTLIB_TARGETS) build_ZSQtLib_ZSRemCmd build_ZSQtLib_ZSRemCmdGUI
#ZSQTLIB_TARGETS := $(ZSQTLIB_TARGETS) build_ZSQtLib_ZSApp_RemCmdClient
#ZSQTLIB_TARGETS := $(ZSQTLIB_TARGETS) build_ZSQtLib_ZSApp_TrcMthClient

build: build_ZSQtLibs build_Bindings4Python

build_ZSQtLibs: $(ZSQTLIB_TARGETS)

build_ZSQtLib_ZSSys:
	make -f $(ZSQTLIB_DIR)/Make/ZSSys/ZSSys.mk

build_ZSQtLib_ZSSysGUI:
	make -f $(ZSQTLIB_DIR)/Make/ZSSysGUI/ZSSysGUI.mk

build_ZSQtLib_ZSIpc:
	make -f $(ZSQTLIB_DIR)/Make/ZSIpc/ZSIpc.mk

build_ZSQtLib_ZSIpcGUI:
	make -f $(ZSQTLIB_DIR)/Make/ZSIpcGUI/ZSIpcGUI.mk

build_ZSQtLib_ZSTrace:
	make -f $(ZSQTLIB_DIR)/Make/ZSTrace/ZSTrace.mk

build_ZSQtLib_ZSTraceGUI:
	make -f $(ZSQTLIB_DIR)/Make/ZSTraceGUI/ZSTraceGUI.mk

build_ZSQtLib_ZSRemCmd:
	make -f $(ZSQTLIB_DIR)/Make/ZSRemCmd/ZSRemCmd.mk

build_ZSQtLib_ZSRemCmdGUI:
	make -f $(ZSQTLIB_DIR)/Make/ZSRemCmdGUI/ZSRemCmdGUI.mk

build_ZSQtLib_App_ZSRemCmdClient:
	make -f $(ZSQTLIB_DIR)/Apps/Products/ZSRemCmdClient/Make/ZSAppRemCmdClient.mk

build_ZSQtLib_App_ZSTrcMthClient:
	make -f $(ZSQTLIB_DIR)/Apps/Products/ZSTrcMthClient/Make/ZSAppTrcMthClient.mk

build_Bindings4Python: $((BINDING4PYTHON_TARGETS)

build_Binding4Python_ZSSys:
	make -f $(ZSQTLIB_DIR)/Make/ZS/ZSSysBinding4Python.mk

build_Binding4Python_ZSSysGUI:
	make -f $(ZSQTLIB_DIR)/Make/ZSGUI/ZSSysGUIBinding4Python.mk

build_Binding4Python_ZSIpc:
	make -f $(ZSQTLIB_DIR)/Make/ZSIpc/ZSIpcBinding4Python.mk

build_Binding4Python_ZSIpcGUI:
	make -f $(ZSQTLIB_DIR)/Make/ZSIpcGUI/ZSIpcGUIBinding4Python.mk

build_Binding4Python_ZSTrace:
	make -f $(ZSQTLIB_DIR)/Make/ZSTrace/ZSTraceBinding4Python.mk

build_Binding4Python_ZSTraceGUI:
	make -f $(ZSQTLIB_DIR)/Make/ZSTraceGUI/ZSTraceGUIBinding4Python.mk

build_Binding4Python_ZSRemCmd:
	make -f $(ZSQTLIB_DIR)/Make/ZSRemCmd/ZSRemCmdBinding4Python.mk

build_Binding4Python_ZSRemCmdGUI:
	make -f $(ZSQTLIB_DIR)/Make/ZSRemCmdGUI/ZSRemCmdGUIBinding4Python.mk

install: install_zsqtlibs

install_zsqtlibs:
	make -f $(ZSQTLIB_DIR)/Make/ZSSys/ZSSys.mk install
	make -f $(ZSQTLIB_DIR)/Make/ZSSysGUI/ZSSysGUI.mk install
	make -f $(ZSQTLIB_DIR)/Make/ZSIpc/ZSIpc.mk install
	make -f $(ZSQTLIB_DIR)/Make/ZSIpcGUI/ZSIpcGUI.mk install
	make -f $(ZSQTLIB_DIR)/Make/ZSTrace/ZSTrace.mk install
	make -f $(ZSQTLIB_DIR)/Make/ZSTraceGUI/ZSTraceGUI.mk install
	make -f $(ZSQTLIB_DIR)/Make/ZSRemCmd/ZSRemCmd.mk install
	make -f $(ZSQTLIB_DIR)/Make/ZSRemCmdGUI/ZSRemCmdGUI.mk install
	#make -f $(ZSQTLIB_DIR)/Apps/Products/ZSRemCmdClient/Make/ZSAppRemCmdClient.mk install
	#make -f $(ZSQTLIB_DIR)/Apps/Products/ZSTrcMthClient/Make/ZSAppTrcMthClient.mk install

clean: clean_zsqtlibs clean_bindings4python

clean_zsqtlibs:
	make -f $(ZSQTLIB_DIR)/Make/ZSSys/ZSSys.mk clean
	make -f $(ZSQTLIB_DIR)/Make/ZSSysGUI/ZSSysGUI.mk clean
	make -f $(ZSQTLIB_DIR)/Make/ZSIpc/ZSIpc.mk clean
	make -f $(ZSQTLIB_DIR)/Make/ZSIpcGUI/ZSIpcGUI.mk clean
	make -f $(ZSQTLIB_DIR)/Make/ZSTrace/ZSTrace.mk clean
	make -f $(ZSQTLIB_DIR)/Make/ZSTraceGUI/ZSTraceGUI.mk clean
	make -f $(ZSQTLIB_DIR)/Make/ZSRemCmd/ZSRemCmd.mk clean
	make -f $(ZSQTLIB_DIR)/Make/ZSRemCmdGUI/ZSRemCmdGUI.mk clean
	#make -f $(ZSQTLIB_DIR)/Apps/Products/ZSRemCmdClient/Make/ZSAppRemCmdClient.mk clean
	#make -f $(ZSQTLIB_DIR)/Apps/Products/ZSTrcMthClient/Make/ZSAppTrcMthClient.mk clean

clean_bindings4python:
	make -f $(ZSQTLIB_DIR)/Make/ZSSys/ZSSysBinding4Python.mk clean
	make -f $(ZSQTLIB_DIR)/Make/ZSSysGUI/ZSSysGUIBinding4Python.mk clean
	make -f $(ZSQTLIB_DIR)/Make/ZSIpc/ZSIpcBinding4Python.mk clean
	make -f $(ZSQTLIB_DIR)/Make/ZSIpcGUI/ZSIpcGUIBinding4Python.mk clean
	make -f $(ZSQTLIB_DIR)/Make/ZSTrace/ZSTraceBinding4Python.mk clean
	make -f $(ZSQTLIB_DIR)/Make/ZSTraceGUI/ZSTraceGUIBinding4Python.mk clean
	make -f $(ZSQTLIB_DIR)/Make/ZSRemCmd/ZSRemCmdBinding4Python.mk clean
	make -f $(ZSQTLIB_DIR)/Make/ZSRemCmdGUI/ZSRemCmdGUIBinding4Python.mk clean

help:
	@echo "Make file to be used with GNU make."
	@echo .
	@echo "Aufruf   : make -f ZSQtLib.mk [CONFIG=<Debug|Release>] [Target]"
	@echo .
	@echo "Targets  : build                 - (default) Creates all ZSQtLib targets together with the bindings 4 python for the specified CONFIG version."
	@echo "           build_ZSQtLibs        - Creates all ZSQtLib targets for the specified CONFIG version."
	@echo "           build_bindings4python - Creates the bindings 4 python for the specified CONFIG version."
	@echo "           clean                 - Deletes all output files of all ZSQtLib targets together with the bindings 4 python for the specified CONFIG version."
	@echo "           clean_zsqtlibs        - Deletes all output files of all ZSQtLib targets for the specified CONFIG version."
	@echo "           clean_bindings4python - Deletes all output files of the bindings 4 python for the specified CONFIG version."
	@echo "           help                  - Shows this help text."
	@echo .
	@echo "Example  : make -f ZSQtLib.mk CONFIG=Debug build
	@echo .


$(info )
$(info <--- $(MAKEFILE_PATH) )
$(info )
