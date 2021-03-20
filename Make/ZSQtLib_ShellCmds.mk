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
# Preconditions
#===============================================================================

# Environment Variable PLATFORM
#-------------------------------------------------------------------------------
# Range [Valid Directory Path]
# Specifies the operation system (PLATFORM) to be built for.
ifndef PLATFORM
    $(error PLATFORM is not defined)
endif


#===============================================================================
# Aliases for shell commands
#===============================================================================

# The name of tools to be used may differ depending on the operating system
# or on other installed tools.

# Compiler and Linker
#-------------------------------------------------------------------------------
# Which compiler and linker to be used depend on the one hand on the PLATFORM.
# But also on the other hand to generate site packages for python the same
# compiler and linker must be used with which python itself and the pyside
# site package has been created.
#          | PYTHONVERSION
# PLATFORM | 27        | 34       | Additional Notes
# ---------+-----------+----------+----------------------------------------------
# Win32    | VS2008    | VS2010   | MS Visual Studio Express Editions may be used
# Linux    | gcc, g++  | gcc, g++ |
# Please note that the command line name for the visual studio compiler is "cl"
# and is usually located in the program files directory
# "Microsoft Visual Studio <Version>/VC/bin".
# The compiler for linux is the GNU compiler named "gcc". To link generated
# C++ code "g++" must be used instead of "gcc".
ifeq ($(PLATFORM), Win32)
    CXX := cl
    LNK := link
else ifeq ($(PLATFORM), Linux)
    CXX := g++ 
    LNK := g++
endif
ifdef TALKATIVE_MAKEFILE
    $(info CXX ............. set to $(CXX))
    $(info LNK ............. set to $(LNK))
endif
export CXX
export LNK

ifeq ($(PLATFORM), Win32)
    MOC := $(QTDIR_BIN)/moc
else ifeq ($(PLATFORM), Linux)
    MOC_FOUND := $(wildcard $(QTDIR_BIN)/moc-qt4)
    ifneq ($(MOC_FOUND),)
        $(info $(moc-qt4) found in $(QTDIR_BIN)!)
        MOC := $(QTDIR_BIN)/moc-qt4
    else
        MOC_FOUND := $(wildcard $(QTDIR_BIN)/moc)
        ifneq ($(MOC_FOUND),)
            $(info $(moc) found in $(QTDIR_BIN)!)
            MOC := $(QTDIR_BIN)/moc
        endif
    endif
endif
ifdef TALKATIVE_MAKEFILE
    $(info MOC ............. set to $(MOC))
endif
export MOC

# Directory and file commands
#-------------------------------------------------------------------------------
# The build process need to create directories and must copy generated files.
# On the one hand the commands may be named different and on the other hand also
# different arguments must be provided. E.g. on windows to create a directory
# with all sub directories the command "mkdir" can be used. On linux the command
# is also named "mkdir" but the additional argument "-p" must be provided.
ifeq ($(PLATFORM), Win32)
    CMD_MKDIR := mkdir
    CMD_COPY := copy
    CMD_RENAME := mv -f
    CMD_DEL := del /Q
else ifeq ($(PLATFORM), Linux)
    CMD_MKDIR := mkdir -p
    CMD_COPY := cp -f
    CMD_RENAME := mv -f
    CMD_DEL := rm -f
endif
ifdef TALKATIVE_MAKEFILE
    $(info CMD_MKDIR ....... set to $(CMD_MKDIR))
    $(info CMD_COPY ........ set to $(CMD_COPY))
    $(info CMD_RENAME ...... set to $(CMD_RENAME))
    $(info CMD_DEL ......... set to $(CMD_DEL))
endif
export CMD_MKDIR
export CMD_COPY
export CMD_RENAME
export CMD_DEL

# Shiboken generator
#-------------------------------------------------------------------------------
ifeq ($(PLATFORM), Win32)
    CMD_SHIBOKEN := $(PYTHONDIR)\lib\site-packages\PySide\shiboken.exe
else ifeq ($(PLATFORM), Linux)
    CMD_SHIBOKEN := /usr/bin/shiboken
endif
ifdef TALKATIVE_MAKEFILE
    $(info CMD_SHIBOKEN: ... set to $(CMD_SHIBOKEN))
endif
export CMD_SHIBOKEN

# Very special commands
#-------------------------------------------------------------------------------
# If you run linux in a virtual box the time synchronization between the host and
# guest machines may lack accuracy (drifting away) ending up in a make file warning
# "Clock skew detected" or in a message saying that the make file has a time stamp
# from the future.
# To solve the time drift "/usr/sbin/VBoxService --timesync-set-start" may be executed.
ifndef CMD_SYNCH_CLOCK
    CMD_SYNCH_CLOCK :=
    ifeq ($(PLATFORM), Win32)
    else ifeq ($(PLATFORM), Linux)
        ifneq ("$(wildcard $(/usr/sbin/VBoxService))","")
            #CMD_SYNCH_CLOCK := /usr/sbin/VBoxService --timesync-set-start
        endif
    endif
    export CMD_SYNCH_CLOCK
ifdef TALKATIVE_MAKEFILE
    $(info CMD_SYNCH_CLOCK:  set to $(CMD_SYNCH_CLOCK))
endif
endif


$(info )
$(info <--- $(MAKEFILE_PATH) )
$(info )
