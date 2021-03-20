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
MAKEFILE_DIR := $(shell dirname $(MAKEFILE_PATH))

$(info ---> $(MAKEFILE_PATH) )
$(info )


#===============================================================================
# Compiler options
#===============================================================================

DEFINE_OS :=
CFLAGS :=
INC_DIRS :=
DEFINES :=

# DEFINE_OS
#=================================
# Range [_WINDOWS, __linux__]
# Preprocessor definition defining the operationg system as used in the
# CPP source and header files.
# This definition is used to generated the dependency files by the gcc compiler.
DEFINE_OS := "_WINDOWS"


# CFLAGS
#=================================

# Compile without linking
#---------------------------------
CFLAGS := $(CFLAGS) /c

# Debug Information Format
#---------------------------------
CFLAGS := $(CFLAGS) /Zi

# Suppress Startup Banner
#---------------------------------
CFLAGS := $(CFLAGS) /nologo

# Warning Level
#---------------------------------
CFLAGS := $(CFLAGS) /W3

# Optimization
#---------------------------------
ifeq ($(CONFIG), Release)
    CFLAGS := $(CFLAGS) /O2
else ifeq ($(CONFIG), Debug)
    CFLAGS := $(CFLAGS) /Od
endif

# Enable String Pooling
#---------------------------------
#      No (Default)
# /GF  Yes
CFLAGS := $(CFLAGS) /GF

# Enable Minimal Rebuild
#---------------------------------
ifeq ($(CONFIG), Debug)
    CFLAGS := $(CFLAGS) /Gm
endif

# Enable C++ Exceptions
#---------------------------------
CFLAGS := $(CFLAGS) /EHsc

# Basic Runtime Checks
#---------------------------------
ifeq ($(CONFIG), Debug)
    CFLAGS := $(CFLAGS) /RTC1
endif

# Runtime Library
#---------------------------------
ifeq ($(CONFIG), Release)
    CFLAGS := $(CFLAGS) /MD
else ifeq ($(CONFIG), Debug)
    CFLAGS := $(CFLAGS) /MDd
endif

# Enable Function-Level Linking
#---------------------------------
CFLAGS := $(CFLAGS) /Gy

# Floating Point Model
#---------------------------------
CFLAGS := $(CFLAGS) /fp:precise

# Treat wchar_t as Built-in Type
#---------------------------------
CFLAGS := $(CFLAGS) /Zc:wchar_t

# Force Conformance In For Loop Source
#---------------------------------
CFLAGS := $(CFLAGS) /Zc:forScope

# Enable Run-Time Type Info
#---------------------------------
CFLAGS := $(CFLAGS) /GR

# Object File Name
#---------------------------------
CFLAGS := $(CFLAGS) /Fo"$(INT_DIR)/"

# Program Database File Name
#---------------------------------
CFLAGS := $(CFLAGS) /Fd"$(INT_DIR)/$(MODULE).pdb"

# Enable Browse Information
#---------------------------------
# Note: "/" needed at the end otherwise you get "cl : Command line error D8003 : missing source file name
CFLAGS := $(CFLAGS) /FR"$(INT_DIR)/$(MODULE).bsc"

# Calling Convention
#---------------------------------
CFLAGS := $(CFLAGS) /Gd

# Compile As
#---------------------------------
CFLAGS := $(CFLAGS) /TP

# Error Reporting
#---------------------------------
CFLAGS := $(CFLAGS) /errorReport:prompt

# Additional Include Directories
#---------------------------------
INC_DIRS := /I "$(ZSQTLIB_DIR)/Include" /I "$(QTDIR_INC)"

# Preprocessor Definitions
#---------------------------------
DEFINES := /D "WIN32" /D "_WINDOWS"
DEFINES := /D "_UNICODE" /D "UNICODE" /D "_ZSTRACE"
ifeq ($(CONFIG_TYPE), dll)
    DEFINES := $(DEFINES) /D $(strip "$(MODULE_UPPCASE)DLL_EXPORTS")
    DEFINES := $(DEFINES) /D "_USRDLL" /D "_WINDLL"
endif
ifeq ($(CONFIG), Release)
    DEFINES := $(DEFINES) /D "NDEBUG"
else ifeq ($(CONFIG), Debug)
    DEFINES := $(DEFINES) /D_DEBUG
endif


$(info )
$(info <--- $(MAKEFILE_PATH) )
