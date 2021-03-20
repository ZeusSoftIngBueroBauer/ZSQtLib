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
MAKEFILE_DIR := $(shell dirname $(MAKEFILE_PATH))

$(info )
$(info -> Processing $(MAKEFILE_PATH))
$(info -----------------------------------------------------------------------------)

#===============================================================================
# Preconditions
#===============================================================================

# The following environment variables will be used. If they have not been set
# on processing this make file they will be set to default values.

ifndef PYTHONDIR
PYTHONDIR := C:/Python27
endif

ifndef PYTHONVERSION
PYTHONVERSION := 27
endif

ifndef PLATFORM
PLATFORM := Win32
endif

ifndef CONFIG
CONFIG=Release
endif

ifeq ($(CONFIG), release)
CONFIG=Release
else ifeq ($(CONFIG), debug)
CONFIG=Debug
endif

ifndef ZSQTLIB_DIR
# Replace \ with /
ZSQTLIB_DIR := $(subst \,/,$(MAKEFILE_DIR))
# Remove /Make from the end of ZSQTLIB_DIR:
ZSQTLIB_DIR := $(subst /Make,,$(ZSQTLIB_DIR))
# Remove /RemCmdServer from the end of ZSQTLIB_DIR:
ZSQTLIB_DIR := $(subst /RemCmdServer,,$(ZSQTLIB_DIR))
# Append /ZSQtLib to ZSQTLIB_DIR:
ZSQTLIB_DIR := $(ZSQTLIB_DIR)/ZSQtLib
endif

ifeq ($(wildcard $(ZSQTLIB_DIR)),)
    $(error $(ZSQTLIB_DIR) does not exist!)
endif

$(info PYTHONDIR:     ${PYTHONDIR})
$(info PYTHONVERSION: ${PYTHONVERSION})
$(info PLATFORM:      ${PLATFORM})
$(info ZSQTLIB_DIR:   ${ZSQTLIB_DIR})

export ZSQTLIB_DIR
export PLATFORM
export CONFIG

#===============================================================================
# Make Targets
#===============================================================================

build:
	make -f $(MAKEFILE_DIR)/ZSAppRemCmdClient_sub.mk

clean:
	make -f $(MAKEFILE_DIR)/ZSAppRemCmdClient_sub.mk clean

help:
	@echo "Make file to be used with GNU make."
	@echo.
	@echo "Aufruf   : make -f ZSAppRemCmdClient.mk [CONFIG=<Debug|Release>] [Target]"
	@echo.
	@echo "Targets  : build   - (default) Creates the target."
	@echo "           clean   - Deletes all output files."
	@echo "           help    - Shows this help text."
	@echo.
	@echo "Example  : make -f ZSAppRemCmdClient.mk CONFIG=Debug build
	@echo.

