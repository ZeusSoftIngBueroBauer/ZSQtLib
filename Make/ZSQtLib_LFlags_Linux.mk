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
$(info ---> ================================================================== )
$(info ---> $(MAKEFILE_PATH) )
$(info ---> ================================================================== )
$(info )


#===============================================================================
# Linker options
#===============================================================================

LFLAGS :=
LIB_DIRS :=
MANIFESTFILE :=

ifeq ($(CONFIG_TYPE), dll)
    LFLAGS := $(LFLAGS) -shared
endif

ifeq ($(CONFIG_TYPE), exe)
    LFLAGS := $(LFLAGS) -Wl,-O1
endif

# Variable LIB_DIRS
#-------------------------------------------------------------------------------
# List of additional library search paths to be used by the linker.

LIB_DIRS := -L "$(QTDIR_LIB)"
LIB_DIRS := $(LIB_DIRS) -L "$(ZSQTLIB_DIR_LIB)"


export LFLAGS
export LIB_DIRS
export MANIFESTFILE

ifdef TALKATIVE_MAKEFILE
    $(info LFLAGS ........ set to $(LFLAGS))
    $(info LIB_DIRS ...... set to $(LIB_DIRS))
    $(info MANIFESTFILE .. set to $(MANIFESTFILE))
endif

$(info )
$(info <--- $(MAKEFILE_PATH) )
$(info )
