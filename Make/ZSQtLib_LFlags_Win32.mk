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

ifndef APP_TYPE
    APP_TYPE := WINDOWS
    $(info APP_TYPE [CONSOLE|WINDOWS] was not defined. Using WINDOWS as default.)
endif

LFLAGS :=
LIB_DIRS :=
MANIFESTFILE :=

# Implizit
#=================================

# Compile without linking
#---------------------------------
# /DLL
ifeq ($(CONFIG_TYPE), dll)
    LFLAGS := $(LFLAGS) /DLL
endif

# General
#=================================

# Output File
#---------------------------------
# /OUT:[file]
ifeq ($(CONFIG), Release)
    LFLAGS := $(LFLAGS) /OUT:"$(OUT_DIR)/$(LNK_TARGET).$(CONFIG_TYPE)"
else ifeq ($(CONFIG), Debug)
    LFLAGS := $(LFLAGS) /OUT:"$(OUT_DIR)/$(LNK_TARGET)d.$(CONFIG_TYPE)"
endif

# Show Progress
#---------------------------------
#               Not Set (Default)
# /VERBOSE      Display All Progress Messages
# /VERBOSE:LIB  Display Some Progress Messages 
#LFLAGS := $(LFLAGS)

# Version
#---------------------------------
# /VERSION:[version]
#LFLAGS := $(LFLAGS)

# Enable Incremental Linking
#---------------------------------
# /INCREMENTAL     Yes (Default)
# /INCREMENTAL:NO  No
ifeq ($(CONFIG), Release)
    LFLAGS := $(LFLAGS) /INCREMENTAL:NO
else ifeq ($(CONFIG), Debug)
    LFLAGS := $(LFLAGS) /INCREMENTAL
endif

# Suppress Startup Banner
#---------------------------------
#         No (Default)
# /NOLOGO  Yes
LFLAGS := $(LFLAGS) /NOLOGO

# Ignore Import Library
#---------------------------------
#LFLAGS := $(LFLAGS)

# Register Output
#---------------------------------
#LFLAGS := $(LFLAGS)

# Per-user Redirection
#---------------------------------
#LFLAGS := $(LFLAGS)

# Link Library Dependencies
#---------------------------------
#LFLAGS := $(LFLAGS)

# Use Library Dependency Inputs
#---------------------------------
#LFLAGS := $(LFLAGS)

# Use UNICODE Response Files
#---------------------------------
#LFLAGS := $(LFLAGS)

# Input
#=================================

# Ignore All Default Libraries
#---------------------------------
#      No (Default)
#       Yes
#LFLAGS := $(LFLAGS)

# Ignore Specific Library
#---------------------------------
# /NODEFAULTLIB:[name, name, ...]
#LFLAGS := $(LFLAGS)

# Module Definition File
#---------------------------------
# /DEF:name
#LFLAGS := $(LFLAGS)

# Add Module to Assembly
#---------------------------------
# /ASSEMBLYMODULE:file
#LFLAGS := $(LFLAGS)

# Embed Managed Resource File
#---------------------------------
# /ASSEMBLYRESOURCE:file
#LFLAGS := $(LFLAGS)

# Force Symbol References
#---------------------------------
# /INCLUDE:[symbol]
#LFLAGS := $(LFLAGS)

# Delay Loaded DLLs
#---------------------------------
# /DELAYLOAD:[dll_name]
#LFLAGS := $(LFLAGS)

# Assembly Link Resource
#---------------------------------
# /ASSEMBLYLINKRESOURCE
#LFLAGS := $(LFLAGS)


# Manifest File
#=================================

# Generate Manifest
#---------------------------------
#            No (Default)
# /MANIFEST  Yes
LFLAGS := $(LFLAGS) /MANIFEST

# Manifest File
#---------------------------------
# /MANIFESTFILE:[file]
ifeq ($(CONFIG), Release)
    MANIFESTFILE := "$(INT_DIR)/$(LNK_TARGET).$(CONFIG_TYPE).intermediate.manifest"
else ifeq ($(CONFIG), Debug)
    MANIFESTFILE := "$(INT_DIR)/$(LNK_TARGET)d.$(CONFIG_TYPE).intermediate.manifest"
endif
LFLAGS := $(LFLAGS) /MANIFESTFILE:$(MANIFESTFILE)

# Additional Manifest Dependencies
#---------------------------------
# /MANIFESTDEPENDENCY:[depenency]
#LFLAGS := $(LFLAGS)

# Allow Isolation
#---------------------------------
#                    Yes (Default)
# /ALLOWISOLATION:NO  NO
#LFLAGS := $(LFLAGS)

# Enable User Account Control (UAC)
#---------------------------------
#                  Yes (Default)
# /MANIFESTUAC:NO  No
# if Yes:
#   /MANIFESTUAC:level=[value]          UAC Exececution Level
#   /MANIFESTUAC:uiAccess=[true|false]  Bypass UI Protection
LFLAGS := $(LFLAGS) /MANIFESTUAC:"level='asInvoker' uiAccess='false'"


# Debugging
#=================================

# Generate Debug Info
#---------------------------------
#         No (Default)
# /DEBUG  Yes
LFLAGS := $(LFLAGS) /DEBUG

# Generate Program Database File
#---------------------------------
# /PDB:name
ifeq ($(CONFIG), Release)
    LFLAGS := $(LFLAGS) /PDB:"$(OUT_DIR)/$(LNK_TARGET).pdb"
else ifeq ($(CONFIG), Debug)
    LFLAGS := $(LFLAGS) /PDB:"$(OUT_DIR)/$(LNK_TARGET)d.pdb"
endif

# Strip Private Symbols
#---------------------------------
# PDBSTRIPPED:file
#LFLAGS := $(LFLAGS)

# Generate Map File
#---------------------------------
# /MAP
#LFLAGS := $(LFLAGS)

# Map File Name
#---------------------------------
# /MAP:[filename]
#LFLAGS := $(LFLAGS)

# Map Exports
#---------------------------------
# /MAPINFO:EXPORTS
#LFLAGS := $(LFLAGS)

# Debuggable Assembly
#---------------------------------
# ASSEMBLYDEBUG          Yes (Default)
# ASSEMBLYDEBUG:DISABLE  No
#LFLAGS := $(LFLAGS)

# System
#=================================

# SubSystem
#---------------------------------
# /SUBSYSTEM:[type]
ifeq ($(APP_TYPE), CONSOLE)
    LFLAGS := $(LFLAGS) /SUBSYSTEM:CONSOLE
else ifeq ($(APP_TYPE), WINDOWS)
    LFLAGS := $(LFLAGS) /SUBSYSTEM:WINDOWS
endif

# Heap Reserve Size
#---------------------------------
# /HEAP:reserve
#LFLAGS := $(LFLAGS)

# Heap Commit Size
#---------------------------------
# /HEAP:reserve,commit
#LFLAGS := $(LFLAGS)

# Stack Reserve Size
#---------------------------------
# /STACK:reserve
#LFLAGS := $(LFLAGS)

# Stack Commit Size
#---------------------------------
# /STACK:reserve,commit
#LFLAGS := $(LFLAGS)

# Enable Large Addresses
#---------------------------------
# /LARGEADDRESSAWARE
# /LARGEADDRESSAWARE:NO
#LFLAGS := $(LFLAGS)

# Terminal Server
#---------------------------------
# /TSAWARE
# /TSAWARE:NO
#LFLAGS := $(LFLAGS)

# Swap Run From CD
#---------------------------------
# /SWAPRUN:CD
#LFLAGS := $(LFLAGS)

# Swap Run From Network
#---------------------------------
# /SWAPRUN:NET
#LFLAGS := $(LFLAGS)

# Driver
#---------------------------------
# /DRIVER:[type]
#LFLAGS := $(LFLAGS)

# Optimization
#=================================

# References
#---------------------------------
#             Default (Default)
# /OPT:REF    Eliminate Unreferenced Data
# /OPT:NOREF  Keep Unreferenced Data
ifeq ($(CONFIG), Release)
    LFLAGS := $(LFLAGS) /OPT:REF
endif

# Enable COMDAT Folding
#---------------------------------
#             Default (Default)
# /OPT:ICF    Remove Redundant COMDATs
# /OPT:NOICF  Do Not Remove Redundant COMDATs
ifeq ($(CONFIG), Release)
    LFLAGS := $(LFLAGS) /OPT:ICF
endif

# Optimize for Windows98
#---------------------------------
#             Default (Default)
# /OPT:WIN98
# /OPT:NOWIN98
#LFLAGS := $(LFLAGS)

# Function Order
#---------------------------------
# /ORDER:[file]
#LFLAGS := $(LFLAGS)

# Profile Guided Database
#---------------------------------
# /PGD:database
#LFLAGS := $(LFLAGS)

# Link Time Code Generation
#---------------------------------
#          No (Default)
# /LTCG    Use Link Time Code Generation
#LFLAGS := $(LFLAGS)

# Embedded IDL
#=================================

# MIDL Commands
#---------------------------------
#LFLAGS := $(LFLAGS)

# Ignore Embedded IDL
#---------------------------------
#LFLAGS := $(LFLAGS)

# Merged IDL Base File Name
#---------------------------------
#LFLAGS := $(LFLAGS)

# Type Library
#---------------------------------
#LFLAGS := $(LFLAGS)

# TypeLib Resource ID
#---------------------------------
#LFLAGS := $(LFLAGS)

# Advanced
#=================================

# Entry Point
#---------------------------------
# /ENTRY:[symbol]
#LFLAGS := $(LFLAGS)

# No Entry Point
#---------------------------------
# /NOENTRY
#LFLAGS := $(LFLAGS)

# Set Checksum
#---------------------------------
# /RELEASE
#LFLAGS := $(LFLAGS)

# Base Address
#---------------------------------
# /BASE:[address|filename,key]
#LFLAGS := $(LFLAGS)

# Randomized Base Address
#---------------------------------
# /DYNAMICBASE
# /DYNAMICBASE:NO
LFLAGS := $(LFLAGS) /DYNAMICBASE:NO

# Fixed Base Address
#---------------------------------
# /FIXED
# /FIXED:NO
#LFLAGS := $(LFLAGS)

# Data Execution Prevention (DEP)
#---------------------------------
# /NXCOMPAT
# /NXCOMPAT:NO
#LFLAGS := $(LFLAGS)

# Turn Off Assembly Generation
#---------------------------------
# /NOASSEMBLY
#LFLAGS := $(LFLAGS)

# Delay Loaded DLL
#---------------------------------
#                Don' Support Unload (Default)
# /DELAY:UNLOAD  Support Unload
#LFLAGS := $(LFLAGS)

# Import Library
#---------------------------------
# /IMPLIB:[name,name,...]
ifeq ($(CONFIG_TYPE), dll)
    ifeq ($(CONFIG), Release)
        LFLAGS := $(LFLAGS) /IMPLIB:"$(OUT_DIR)/$(LNK_TARGET).lib"
    else ifeq ($(CONFIG), Debug)
        LFLAGS := $(LFLAGS) /IMPLIB:"$(OUT_DIR)/$(LNK_TARGET)d.lib"
    endif
endif

# Merge Sections
#---------------------------------
# /MERGE:[from=to]
#LFLAGS := $(LFLAGS)

# Target Machine
#---------------------------------
# /MACHINE:[type]
LFLAGS := $(LFLAGS) /MACHINE:X86

# Profile
#---------------------------------
#           No (Default)
# /PROFILE  Yes
#LFLAGS := $(LFLAGS)

# CLR Thread Attribute
#---------------------------------
#                          No threading attribute set
# /CLRTHREADATTRIBUTE:STA
# /CLRTHREADATTRIBUTE:MTA
#LFLAGS := $(LFLAGS)

# CLR Image Type
#---------------------------------
#LFLAGS := $(LFLAGS)

# Key File
#---------------------------------
#LFLAGS := $(LFLAGS)

# Key Container
#---------------------------------
#LFLAGS := $(LFLAGS)

# Delay Sign
#---------------------------------
#LFLAGS := $(LFLAGS)

# Error Reporting
#---------------------------------
# /ERRORREPORT:[method]
LFLAGS := $(LFLAGS) /ERRORREPORT:PROMPT

# CLR Unmanaged Code Check
#---------------------------------
#LFLAGS := $(LFLAGS)


# Variable LIB_DIRS
#=================================
# List of additional library search paths to be used by the linker.
LIB_DIRS := /LIBPATH:"$(QTDIR_LIB)"
LIB_DIRS := $(LIB_DIRS) /LIBPATH:"$(ZSQTLIB_DIR)/Lib/$(PLATFORM)"


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
