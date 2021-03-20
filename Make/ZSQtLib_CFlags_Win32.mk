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
# Compiler options
#===============================================================================

DEFINE_OS :=
CFLAGS :=
INC_DIRS :=
DEFINES :=


# Variable DEFINE_OS
#=================================
# Range [_WINDOWS, __linux__]
# Preprocessor definition defining the operationg system as used in the
# CPP source and header files.
# This definition is used to generated the dependency files by the gcc compiler.
DEFINE_OS := "_WINDOWS"


# Implizit
#=================================

# Compile without linking
#---------------------------------
# /c
CFLAGS := $(CFLAGS) /c


# General
#=================================

# Debug Information Format
#---------------------------------
#      Disabled
# /Z7  C7 Compatible
# /Zi  Program Database
# /ZI  Program Database for Edit & Continue
CFLAGS := $(CFLAGS) /Zi

# Suppress Startup Banner
#---------------------------------
#   /nologo
CFLAGS := $(CFLAGS) /nologo

# Warning Level
#---------------------------------
# /W0    Off
# /W1    Level 1
# ..
# /W4    Level 4
CFLAGS := $(CFLAGS) /W3

# Detect 64-bit Portability Issues
#---------------------------------
#        No  (Default)
# /Wp64  Yes
#CFLAGS := $(CFLAGS)

# Treat warnings as errors
#---------------------------------
#        No  (Default)
# /WX    Yes
#CFLAGS := $(CFLAGS)

# Use UNICODE Response Files
#---------------------------------
#CFLAGS := $(CFLAGS)


# Optimization
#=================================

# Optimization
#---------------------------------
# /Od    Disabled
# /O1    Minimize Size
# /O2    Maximize Speed
# /Ox    Full Optimization
ifeq ($(CONFIG), Release)
    CFLAGS := $(CFLAGS) /O2
else ifeq ($(CONFIG), Debug)
    CFLAGS := $(CFLAGS) /Od
endif

# Inline Function Expansion
#---------------------------------
#CFLAGS := $(CFLAGS)

# Enable Instrinsic Functions
#---------------------------------
#CFLAGS := $(CFLAGS)

# Favor Size or Speed
#---------------------------------
#        Neither (Default)
# /Ot    Favor Fast Code
# /Os    Favor Small Code
#CFLAGS := $(CFLAGS)

# Omit Frame Pointers
#---------------------------------
#        No  (Default)
# /Oy    Yes
#CFLAGS := $(CFLAGS)

# Enable Fiber-safe Optimizations
#---------------------------------
#        No (Default)
# /GT    Yes
#CFLAGS := $(CFLAGS)

# Whole Program Optimization
#---------------------------------
#        No (Default)
# /GL    Yes
#CFLAGS := $(CFLAGS)

# Ignore Standard Include Path
#---------------------------------
#     No (Default)
# /X  Yes
#CFLAGS := $(CFLAGS)

# Generate Preprocessed File
#---------------------------------
#         No (Default)
# /P      Yes (with line numbers)
# /EP /P  Yes (without line numbers)
#CFLAGS := $(CFLAGS)

# Keep Comments
#---------------------------------
#     No (Default)
# /C  Yes
#CFLAGS := $(CFLAGS)


# Code Generation
#=================================

# Enable String Pooling
#---------------------------------
#      No (Default)
# /GF  Yes
CFLAGS := $(CFLAGS) /GF

# Enable Minimal Rebuild
#---------------------------------
#      No (Default)
# /Gm  Yes
ifeq ($(CONFIG), Debug)
    CFLAGS := $(CFLAGS) /Gm
endif

# Enable C++ Exceptions
#---------------------------------
#        No (Default)
# /EHsc  Yes
# /EHa   Yes (with SEH Exceptions)
CFLAGS := $(CFLAGS) /EHsc

# Smaller Type Check
#---------------------------------
#        No (Default)
# /RTCc  Yes
#CFLAGS := $(CFLAGS)

# Basic Runtime Checks
#---------------------------------
#        Default (Default)
# /RTCs  Stack Frames
# /RTCu  Unitinialzed Variables
# /RTC1  Both
# /RTCus Both
ifeq ($(CONFIG), Debug)
    CFLAGS := $(CFLAGS) /RTC1
endif

# Runtime Library
#---------------------------------
# /MT    Multi Threaded
# /MTd   Multi Threaded Debug
# /MD    Multi Threaded DLL
# /MDd   Multi Threaded Debug DLL
ifeq ($(CONFIG), Release)
    CFLAGS := $(CFLAGS) /MD
else ifeq ($(CONFIG), Debug)
    CFLAGS := $(CFLAGS) /MDd
endif

# Struct Member Alignment
#---------------------------------
#        Default (Default)
# /Zp1   1 Byte
# /Zp2   2 Bytes
# /Zp4   4 Bytes
# /Zp8   8 Bytes
# /Zp16  16 Bytes
#CFLAGS := $(CFLAGS)

# Buffer Security Check
#---------------------------------
#        Yes (Default)
# /GS-   No
#CFLAGS := $(CFLAGS)

# Enable Function-Level Linking
#---------------------------------
#        No
# /Gy    Yes
CFLAGS := $(CFLAGS) /Gy

# Enable Enhanced Instruction Set
#---------------------------------
#              Not Set (Default)
# /arch:SSE    Streaming SIMD Extensions
# /arch:SSE2   Streaming SIMD Extensions 2
#CFLAGS := $(CFLAGS)

# Floating Point Model
#---------------------------------
# /fp:precise
# /fp:strict
# /fp:fast
CFLAGS := $(CFLAGS) /fp:precise

# Enable Floating Point Exceptions
#---------------------------------
#             No (Default)
# /fp:except  Yes
#CFLAGS := $(CFLAGS)


# Language
#=================================

# Disable Language Extensions
#---------------------------------
#         No
# /Za     Yes
#CFLAGS := $(CFLAGS)

# Default Char Unsigned
#---------------------------------
#         No
# /J      Yes
#CFLAGS := $(CFLAGS)

# Treat wchar_t as Built-in Type
#---------------------------------
# /Zc:wchar_t-   No
# /Zc:wchar_t    Yes (Default)
CFLAGS := $(CFLAGS) /Zc:wchar_t-

# Force Conformance In For Loop Source
#---------------------------------
# /Zc:forScope-  No
# /Zc:forScope   Yes (Default)
CFLAGS := $(CFLAGS) /Zc:forScope

# Enable Run-Time Type Info
#---------------------------------
# /GR-    No
# /GR     Yes (Default)
CFLAGS := $(CFLAGS) /GR

# OpenMP Support
#---------------------------------
#          No (Default)
# /openmp  Yes
#CFLAGS := $(CFLAGS)


# Precompiled Header
#=================================

# Create/Use Precompiled Header
#---------------------------------
#         No (Default)
# /Yc     Create
# /Yu     Use
#CFLAGS := $(CFLAGS)

# Create/Use PCH Through File
#---------------------------------
#            No (Default)
# /Yc[name]  Create
# /Yu[name]  Use
#CFLAGS := $(CFLAGS)

# Precompiled Header File
#---------------------------------
#            No (Default)
# /Fp[name]
#CFLAGS := $(CFLAGS)


# Output Files
#=================================

# Expand Attributed Source
#---------------------------------
#         No  (Default)
# /Fx     Yes
#CFLAGS := $(CFLAGS)

# Assembler Output
#---------------------------------
#         No Listing (Default)
# /FA     Assembly-Only Listing
# /FAc    Assembly With Machine Code
# /FAs    Assembly With Source Code
# /FAcs   Assembly, Machine Code and Source
#CFLAGS := $(CFLAGS)

# ASM List Location
#---------------------------------
#         None (Default)
# /Fa[name]
#CFLAGS := $(CFLAGS)

# Object File Name
#---------------------------------
#           Use Default
# /Fo[name]
# Note: "/" needed at the end otherwise you get "cl : Command line error D8003 : missing source file name
CFLAGS := $(CFLAGS) /Fo"$(INT_DIR)/"

# Program Database File Name
#---------------------------------
#           Use Default
# /Fd[name]
# Note: "/" needed at the end otherwise you get "cl : Command line error D8003 : missing source file name
CFLAGS := $(CFLAGS) /Fd"$(INT_DIR)/$(MODULE).pdb"

# Generate XML Documentation Files
#---------------------------------
#         No (Default)
# /doc    Yes
#CFLAGS := $(CFLAGS)

# XML Documentation File Name
#---------------------------------
#             No (Default)
# /doc[name]  Yes
#CFLAGS := $(CFLAGS)

# Browse Information
#=================================

# Enable Browse Information
#---------------------------------
#            None (Default)
# /FR[name]  Include All Browse Information
# /Fr[name]  No Local Symbols
# Note: "/" needed at the end otherwise you get "cl : Command line error D8003 : missing source file name
CFLAGS := $(CFLAGS) /FR"$(INT_DIR)/$(MODULE).bsc"

# Advanced
#=================================

# Calling Convention
#---------------------------------
# /Gd    __cdecl
# /Gr    __fastcall
# /Gz    __stdcall
CFLAGS := $(CFLAGS) /Gd

# Compile As
#---------------------------------
# /TC     Compile as C Code
# /TP     Compile as C++ Code
CFLAGS := $(CFLAGS) /TP

# Disable Specific Warnings
#---------------------------------
# /wd<num, ..>
#CFLAGS := $(CFLAGS)

# Force Includes
#---------------------------------
# /FI[name|
#CFLAGS := $(CFLAGS)

# Force #using
#---------------------------------
# /FU[name|
#CFLAGS := $(CFLAGS)

# Show Includes
#---------------------------------
# /showIncludes
#CFLAGS := $(CFLAGS)

# Undefine Preprocessor Definitions
#--------------------------------------
# /U[macro]
#CFLAGS := $(CFLAGS)

# Undefine All Preprocessor Definitions
#--------------------------------------
# /u
#CFLAGS := $(CFLAGS)

# Use Full Paths
#---------------------------------
#         No (Default)
# /FC     Yes
#CFLAGS := $(CFLAGS)

# Omit Default Library Names
#---------------------------------
#         No (Default)
# /ZI     Yes
#CFLAGS := $(CFLAGS)

# Error Reporting
#---------------------------------
CFLAGS := $(CFLAGS) /errorReport:prompt


# Additional Include Directories
#=================================
# List of additional include paths of header files to be used by the C++ compiler.
INC_DIRS := /I "$(ZSQTLIB_DIR)/Include"
INC_DIRS := $(INC_DIRS) /I "$(QTDIR_INC)"

# Preprocessor definitions
#=================================
# Preprocessor definitions to be used by the C compiler.
DEFINES := /D "WIN32" /D "_WINDOWS"
DEFINES := /D "_UNICODE" /D "UNICODE" /D "_ZSTRACE"
ifeq ($(CONFIG_TYPE), dll)
    DEFINES := $(DEFINES) /D $(strip "$(LNK_TARGET_UPPCASE)DLL_EXPORTS")
    DEFINES := $(DEFINES) /D "_USRDLL" /D "_WINDLL"
endif
ifeq ($(CONFIG), Release)
    DEFINES := $(DEFINES) /D "NDEBUG"
else ifeq ($(CONFIG), Debug)
    DEFINES := $(DEFINES) /D "_DEBUG"
endif


export DEFINE_OS
export CFLAGS
export INC_DIRS
export DEFINES

ifdef TALKATIVE_MAKEFILE
    $(info DEFINE_OS ..... set to $(DEFINE_OS))
    $(info CFLAGS ........ set to $(CFLAGS))
    $(info INC_DIRS ...... set to $(INC_DIRS))
    $(info DEFINES ....... set to $(DEFINES))
endif

$(info )
$(info <--- $(MAKEFILE_PATH) )
$(info )
