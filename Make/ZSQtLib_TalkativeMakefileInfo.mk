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
# TALKATIVE_MAKEFILE info
#===============================================================================

ifdef TALKATIVE_MAKEFILE
    ifeq ($(TALKATIVE_MAKEFILE_MODULE), true)
        $(info Module:)
        $(info ===================================================)
        $(info MODULE: ......................... $(MODULE))
        $(info LNK_TARGET: ..................... $(LNK_TARGET))
        $(info CONFIG_TYPE: .................... $(CONFIG_TYPE))
        $(info BINDING4PYTHON: ................. $(BINDING4PYTHON))
        $(info )
    endif
    ifeq ($(TALKATIVE_MAKEFILE_ENVVARS), true)
        $(info Commonly used environment variables:)
        $(info ===================================================)
        $(info PLATFORM: ....................... $(PLATFORM))
        $(info PATH_SEP: ....................... $(PATH_SEP))
        $(info CONFIG: ......................... $(CONFIG))
        $(info OBJ_EXT: ........................ $(OBJ_EXT))
        $(info LIB_EXT: ........................ $(LIB_EXT))
        $(info DLL_EXT: ........................ $(DLL_EXT))
        $(info LIB_PREFIX: ..................... $(LIB_PREFIX))
        $(info DLL_PREFIX: ..................... $(DLL_PREFIX))
        $(info QTVERSION: ...................... $(QTVERSION))
        $(info QTDIR: .......................... $(QTDIR))
        $(info QTDIR_INC: ...................... $(QTDIR_INC))
        $(info QTDIR_LIB: ...................... $(QTDIR_LIB))
        $(info QTDIR_BIN: ...................... $(QTDIR_BIN))
        $(info PYTHONVERSION: ...................$(PYTHONVERSION))
        $(info PYTHONDIR: ...................... $(PYTHONDIR))
        $(info PYTHONDIR_INC: .................. $(PYTHONDIR_INC))
        $(info PYTHONDIR_LIB: .................. $(PYTHONDIR_LIB))
        $(info ZSQTLIB_DIR: .................... $(ZSQTLIB_DIR))
        $(info ZSQTLIB_DIR_INC: ................ $(ZSQTLIB_DIR_INC))
        $(info ZSQTLIB_DIR_SRC: ................ $(ZSQTLIB_DIR_SRC))
        $(info ZSQTLIB_DIR_IMG: ................ $(ZSQTLIB_DIR_IMG))
        $(info ZSQTLIB_DIR_TMP: ................ $(ZSQTLIB_DIR_TMP))
        $(info ZSQTLIB_DIR_LIB: ................ $(ZSQTLIB_DIR_LIB))
        $(info ZSQTLIB_DIR_BIN: ................ $(ZSQTLIB_DIR_BIN))
        $(info ZSQTLIB_DIR_STP: ................ $(ZSQTLIB_DIR_STP))
        $(info ZSQTLIB_DIR_INSTALL: ............ $(ZSQTLIB_DIR_INSTALL))
        $(info ZSQTLIB_DIR_BINDING4PYTHON_STP: . $(ZSQTLIB_DIR_BINDING4PYTHON_STP))
        $(info )
    endif
    ifeq ($(TALKATIVE_MAKEFILE_SHELLCMDS), true)
        $(info Aliases for shell commands:)
        $(info ===================================================)
        $(info CXX: ............................ $(CXX))
        $(info LNK: ............................ $(LNK))
        $(info MOC: ............................ $(MOC))
        $(info CMD_MKDIR: ...................... $(CMD_MKDIR))
        $(info CMD_COPY: ....................... $(CMD_COPY))
        $(info CMD_RENAME: ..................... $(CMD_RENAME))
        $(info CMD_DEL: ........................ $(CMD_DEL))
        $(info CMD_SHIBOKEN: ................... $(CMD_SHIBOKEN))
        $(info CMD_SYNCH_CLOCK: .................$(CMD_SYNCH_CLOCK))
        $(info )
    endif
    ifeq ($(TALKATIVE_MAKEFILE_TMPDIRS), true)
        $(info Directory paths for intermediately generated files:)
        $(info ===================================================)
        $(info DEP_DIR: ........................ $(DEP_DIR))
        $(info INT_DIR: ........................ $(INT_DIR))
        $(info MOC_DIR: ........................ $(MOC_DIR))
        $(info OUT_DIR: ........................ $(OUT_DIR))
        $(info SBK_DIR: ........................ $(SBK_DIR))
        $(info )
    endif
    ifeq ($(TALKATIVE_MAKEFILE_INPUTFILES), true)
        $(info Input Files:)
        $(info ===================================================)
        $(info HDR_ITEMS: ...................... $(HDR_ITEMS))
        $(info )
        $(info CPP_ITEMS: ...................... $(CPP_ITEMS))
        $(info )
        $(info CPP_DEP_ITEMS: .................. $(CPP_DEP_ITEMS))
        $(info )
        $(info MOC_HDR_ITEMS: .................. $(MOC_HDR_ITEMS))
        $(info )
        $(info MOC_CPP_ITEMS: .................. $(MOC_CPP_ITEMS))
        $(info )
        $(info MOC_CPP_DEP_ITEMS: .............. $(MOC_CPP_DEP_ITEMS))
        $(info )
        $(info MOC_OBJ_ITEMS: .................. $(MOC_OBJ_ITEMS))
        $(info )
        $(info DEPS: ........................... $(DEPS))
        $(info )
        $(info OBJ_ITEMS: ...................... $(OBJ_ITEMS))
        $(info )
        $(info OSLIBS: ......................... $(OSLIBS))
        $(info )
        $(info QTLIBS: ......................... $(QTLIBS))
        $(info )
        $(info 3RDPARTYLIBS: ................... $(3RDPARTYLIBS))
        $(info )
        $(info ZSQTLIBS: ....................... $(ZSQTLIBS))
        $(info )
    endif
    ifeq ($(TALKATIVE_MAKEFILE_CFLAGS), true)
        $(info Compiler options:)
        $(info ===================================================)
        $(info DEFINE_OS: ...................... $(DEFINE_OS))
        $(info CFLAGS: ......................... $(CFLAGS))
        $(info INC_DIRS: ....................... $(INC_DIRS))
        $(info DEFINES: ........................ $(DEFINES))
        $(info )
    endif
    ifeq ($(TALKATIVE_MAKEFILE_LFLAGS), true)
        $(info Linker options:)
        $(info ===================================================)
        $(info LFLAGS: ......................... $(LFLAGS))
        $(info LIB_DIRS: ....................... $(LIB_DIRS))
        $(info MANIFESTFILE: ................... $(MANIFESTFILE))
        $(info )
    endif
    ifeq ($(TALKATIVE_MAKEFILE_SBKFLAGS), true)
        $(info Shiboken command arguments:)
        $(info ===================================================)
        $(info SBK_FLAGS: ...................... $(SBK_FLAGS))
        $(info )
    endif
    ifeq ($(TALKATIVE_MAKEFILE_MAKETARGETS), true)
        $(info Make Targets:)
        $(info ===================================================)
        $(info MAIN_TARGET: .................... $(MAIN_TARGET))
        $(info LIB_DIR_TRG: .................... $(LIB_DIR_TRG))
        $(info BIN_DIR_TRG: .................... $(BIN_DIR_TRG))
        $(info BIN_DIR_PDB: .................... $(BIN_DIR_PDB))
        $(info SBK_PYTHON_HDR_TRG: ............. $(SBK_PYTHON_HDR_TRG))
        $(info STP_DIR_MODULE_TRG: ............. $(STP_DIR_MODULE_TRG))
        $(info STP_DIR_BINDING_TRG: ............ $(STP_DIR_BINDING_TRG))
        $(info )
    endif
endif


$(info )
$(info <--- $(MAKEFILE_PATH) )
$(info )
