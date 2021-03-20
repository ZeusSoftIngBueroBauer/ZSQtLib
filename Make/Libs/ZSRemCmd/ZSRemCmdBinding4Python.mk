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

MODULE := ZSRemCmd
export MODULE

MODULE_UPPCASE := $(shell echo $(MODULE) | tr a-z A-Z)
MODULE_UPPCASE := $(strip $(MODULE_UPPCASE))
export MODULE_UPPCASE

MODULE_LOWCASE := $(shell echo $(MODULE) | tr A-Z a-z)
MODULE_LOWCASE := $(strip $(MODULE_LOWCASE))
export MODULE_LOWCASE

LNK_TARGET := $(MODULE)Binding4Python
export LNK_TARGET

LNK_TARGET_UPPCASE := $(shell echo $(LNK_TARGET) | tr a-z A-Z)
LNK_TARGET_UPPCASE := $(strip $(LNK_TARGET_UPPCASE))
export LNK_TARGET_UPPCASE

LNK_TARGET_LOWCASE := $(shell echo $(LNK_TARGET) | tr A-Z a-z)
LNK_TARGET_LOWCASE := $(strip $(LNK_TARGET_LOWCASE))
export LNK_TARGET_LOWCASE

CONFIG_TYPE := dll
export CONFIG_TYPE

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
$(info ---> $(MAKEFILE_PATH))
$(info ---> ****************************************************************** )
$(info )

ifdef TALKATIVE_MAKEFILE
    TALKATIVE_MAKEFILE_MODULE := true
    MAKEFILE_PATH_TMP := $(MAKEFILE_PATH)
    MAKEFILE_DIR_TMP := $(MAKEFILE_DIR)
    -include $(ZSQTLIB_DIR)/Make/ZSQtLib_TalkativeMakeFileInfo.mk
    MAKEFILE_PATH := $(MAKEFILE_PATH_TMP)
    MAKEFILE_DIR := $(MAKEFILE_DIR_TMP)
    TALKATIVE_MAKEFILE_MODULE :=
endif


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
# To extract the ZSQtLib base directory from the make files directory the
# substring "/Make" will be removed from the end of MAKEFILE_DIR.
PATH_SEP_TMP := \\
PATH_SEP_TMP := $(subst $(PATH_SEP_TMP),\,$(PATH_SEP_TMP))
ifdef ZSQTLIB_DIR
    ZSQTLIB_DIR := $(subst $(PATH_SEP_TMP),/,$(ZSQTLIB_DIR))
    ifdef TALKATIVE_MAKEFILE
        $(info ZSQTLIB_DIR defined as $(ZSQTLIB_DIR))
    endif
else
    MAKEFILE_DIR := $(shell dirname $(MAKEFILE_PATH))
    ZSQTLIB_DIR := $(subst /Make/$(MODULE),,$(MAKEFILE_DIR))
    ZSQTLIB_DIR := $(subst PATH_SEP_TMP,/,$(ZSQTLIB_DIR))
    ifdef TALKATIVE_MAKEFILE
        $(info ZSQTLIB_DIR detected as $(ZSQTLIB_DIR))
    endif
    export ZSQTLIB_DIR
endif
ifeq ($(wildcard $(ZSQTLIB_DIR)),)
    $(error $(ZSQTLIB_DIR) does not exist!)
endif


#===============================================================================
# Commonly used environment variables
#===============================================================================

MAKEFILE_PATH_TMP := $(MAKEFILE_PATH)
MAKEFILE_DIR_TMP := $(MAKEFILE_DIR)
-include $(ZSQTLIB_DIR)/Make/ZSQtLib_EnvVars.mk
MAKEFILE_PATH := $(MAKEFILE_PATH_TMP)
MAKEFILE_DIR := $(MAKEFILE_DIR_TMP)


#===============================================================================
# Aliases for shell commands
#===============================================================================

# The name of tools to be used may differ depending on the operating system
# or on other installed tools.

MAKEFILE_PATH_TMP := $(MAKEFILE_PATH)
MAKEFILE_DIR_TMP := $(MAKEFILE_DIR)
-include $(ZSQTLIB_DIR)/Make/ZSQtLib_ShellCmds.mk
MAKEFILE_PATH := $(MAKEFILE_PATH_TMP)
MAKEFILE_DIR := $(MAKEFILE_DIR_TMP)


#===============================================================================
# Directory paths for intermediately generated files
#===============================================================================

# On Windows systems those directory paths are relative to the base directory
# of the library modules.
# On Linux machines those directory paths are relative to the root in "/tmp".

MAKEFILE_PATH_TMP := $(MAKEFILE_PATH)
MAKEFILE_DIR_TMP := $(MAKEFILE_DIR)
-include $(ZSQTLIB_DIR)/Make/ZSQtLib_TmpDirs_Binding4Python.mk
MAKEFILE_PATH := $(MAKEFILE_PATH_TMP)
MAKEFILE_DIR := $(MAKEFILE_DIR_TMP)


#===============================================================================
# Directory paths for output files
#===============================================================================

# On Windows systems those directory paths are relative to the base directory
# of the ZS Qt library.
# On Linux machines those directory paths are relative to "/usr/local/ZSQtLib".

# Path: ZSQTLIB_DIR_BINDING4PYTHON_STP
#-------------------------------------------------------------------------------
ifdef ZSQTLIB_DIR_BINDING4PYTHON_STP
    ifdef TALKATIVE_MAKEFILE
        $(info ZSQTLIB_DIR_BINDING4PYTHON_STP defined as $(ZSQTLIB_DIR_BINDING4PYTHON_STP))
    endif
else
    ZSQTLIB_DIR_BINDING4PYTHON_STP := $(ZSQTLIB_DIR_STP)/Bindings/Python$(PYTHONVERSION)/ZSQtLib
    ifdef TALKATIVE_MAKEFILE
        $(info ZSQTLIB_DIR_BINDING4PYTHON_STP set to $(ZSQTLIB_DIR_BINDING4PYTHON_STP))
    endif
    export ZSQTLIB_DIR_BINDING4PYTHON_STP
endif


#===============================================================================
# Input Files
#===============================================================================

# Those directory paths are relative to the base directory of this make file.

MAKEFILE_PATH_TMP := $(MAKEFILE_PATH)
MAKEFILE_DIR_TMP := $(MAKEFILE_DIR)
-include $(ZSQTLIB_DIR)/Make/ZSQtLib_InputFiles.mk
MAKEFILE_PATH := $(MAKEFILE_PATH_TMP)
MAKEFILE_DIR := $(MAKEFILE_DIR_TMP)

# Variable 3RDPARTYLIBS
#-------------------------------------------------------------------------------
# Here: List of shiboken libraries to be used.
3RDPARTYLIBS := $(3RDPARTYLIBS)
ifeq ($(PLATFORM), Win32)
    ifeq ($(CONFIG), Release)
        3RDPARTYLIBS := $(3RDPARTYLIBS) $(LIB_PREFIX)shiboken-python2.7.$(LIB_EXT)
        3RDPARTYLIBS := $(3RDPARTYLIBS) $(LIB_PREFIX)pyside-python2.7.$(LIB_EXT)
    else ifeq ($(CONFIG), Debug)
        3RDPARTYLIBS := $(3RDPARTYLIBS) $(LIB_PREFIX)shiboken-python2.7_d.lib
        3RDPARTYLIBS := $(3RDPARTYLIBS) $(LIB_PREFIX)pyside-python2.7_d.lib
    endif
else ifeq ($(PLATFORM), Linux)
    3RDPARTYLIBS := $(3RDPARTYLIBS) $(PYTHONDIR_LIB)/$(LIB_PREFIX)shiboken-python2.7.$(LIB_EXT)
    3RDPARTYLIBS := $(3RDPARTYLIBS) $(PYTHONDIR_LIB)/$(LIB_PREFIX)pyside-python2.7.$(LIB_EXT)
endif
export 3RDPARTYLIBS

# Variable QTLIBS
#-------------------------------------------------------------------------------
# List of Qt libraries to be used.
# InputFiles provides the default Qt libraries which are always used (Core, Network, Xml).
# If additional Qt libraries are necessary they have to be added to QTLIBS here.
# Please note that on Linux machines debug and release are not distinguished
# and you can mix and match release and debug versions.
QTLIBS := $(QTLIBS)
export QTLIBS

# Variable ZSQTLIBS
#-------------------------------------------------------------------------------
# List of ZSQtLib libraries to be used.
# This variable must be defined depending on the library to be built
# and must be provided by the corresponding make file of the module.
ifeq ($(PLATFORM), Win32)
    ifeq ($(CONFIG), Release)
        ZSQTLIBS := $(ZSQTLIBS) $(LIB_PREFIX)ZSSys.$(LIB_EXT)
        ZSQTLIBS := $(ZSQTLIBS) $(LIB_PREFIX)ZSSysGUI.$(LIB_EXT)
        ZSQTLIBS := $(ZSQTLIBS) $(LIB_PREFIX)ZSIpc.$(LIB_EXT)
        ZSQTLIBS := $(ZSQTLIBS) $(LIB_PREFIX)ZSIpcGUI.$(LIB_EXT)
        ZSQTLIBS := $(ZSQTLIBS) $(LIB_PREFIX)ZSTrace.$(LIB_EXT)
        ZSQTLIBS := $(ZSQTLIBS) $(LIB_PREFIX)ZSTraceGUI.$(LIB_EXT)
        ZSQTLIBS := $(ZSQTLIBS) $(LIB_PREFIX)ZSRemCmd.$(LIB_EXT)
    else ifeq ($(CONFIG), Debug)
        ZSQTLIBS := $(ZSQTLIBS) $(LIB_PREFIX)ZSSysd.$(LIB_EXT)
        ZSQTLIBS := $(ZSQTLIBS) $(LIB_PREFIX)ZSSysGUId.$(LIB_EXT)
        ZSQTLIBS := $(ZSQTLIBS) $(LIB_PREFIX)ZSIpcd.$(LIB_EXT)
        ZSQTLIBS := $(ZSQTLIBS) $(LIB_PREFIX)ZSIpcGUId.$(LIB_EXT)
        ZSQTLIBS := $(ZSQTLIBS) $(LIB_PREFIX)ZSTraced.$(LIB_EXT)
        ZSQTLIBS := $(ZSQTLIBS) $(LIB_PREFIX)ZSTraceGUId.$(LIB_EXT)
        ZSQTLIBS := $(ZSQTLIBS) $(LIB_PREFIX)ZSRemCmdd.$(LIB_EXT)
    endif
else ifeq ($(PLATFORM), Linux)
    # On linux machines the Qt version is resolved using symbolic links and
    # the version is encode in the file extensions (e.g. libQtCore.so.4.8.7).
    # In addition the linux gcc and g++ compilier does not provide an option
    # to specify search paths for libraries. So we have to specify the full
    # path for the libraries.
    ifeq ($(CONFIG), Release)
        ZSQTLIBS := $(ZSQTLIBS) $(ZSQTLIB_DIR_LIB)/$(LIB_PREFIX)ZSSys.$(LIB_EXT)
        ZSQTLIBS := $(ZSQTLIBS) $(ZSQTLIB_DIR_LIB)/$(LIB_PREFIX)ZSSysGUI.$(LIB_EXT)
        ZSQTLIBS := $(ZSQTLIBS) $(ZSQTLIB_DIR_LIB)/$(LIB_PREFIX)ZSIpc.$(LIB_EXT)
        ZSQTLIBS := $(ZSQTLIBS) $(ZSQTLIB_DIR_LIB)/$(LIB_PREFIX)ZSIpcGUI.$(LIB_EXT)
        ZSQTLIBS := $(ZSQTLIBS) $(ZSQTLIB_DIR_LIB)/$(LIB_PREFIX)ZSTrace.$(LIB_EXT)
        ZSQTLIBS := $(ZSQTLIBS) $(ZSQTLIB_DIR_LIB)/$(LIB_PREFIX)ZSTraceGUI.$(LIB_EXT)
        ZSQTLIBS := $(ZSQTLIBS) $(ZSQTLIB_DIR_LIB)/$(LIB_PREFIX)ZSRemCmd.$(LIB_EXT)
    else ifeq ($(CONFIG), Debug)
        ZSQTLIBS := $(ZSQTLIBS) $(ZSQTLIB_DIR_LIB)/$(LIB_PREFIX)ZSSysd.$(LIB_EXT)
        ZSQTLIBS := $(ZSQTLIBS) $(ZSQTLIB_DIR_LIB)/$(LIB_PREFIX)ZSSysGUId.$(LIB_EXT)
        ZSQTLIBS := $(ZSQTLIBS) $(ZSQTLIB_DIR_LIB)/$(LIB_PREFIX)ZSIpcd.$(LIB_EXT)
        ZSQTLIBS := $(ZSQTLIBS) $(ZSQTLIB_DIR_LIB)/$(LIB_PREFIX)ZSIpcGUId.$(LIB_EXT)
        ZSQTLIBS := $(ZSQTLIBS) $(ZSQTLIB_DIR_LIB)/$(LIB_PREFIX)ZSTraced.$(LIB_EXT)
        ZSQTLIBS := $(ZSQTLIBS) $(ZSQTLIB_DIR_LIB)/$(LIB_PREFIX)ZSTraceGUId.$(LIB_EXT)
        ZSQTLIBS := $(ZSQTLIBS) $(ZSQTLIB_DIR_LIB)/$(LIB_PREFIX)ZSRemCmdd.$(LIB_EXT)
    endif
endif
export ZSQTLIBS


#===============================================================================
# Compiler and linker options
#===============================================================================

# Variable CFLAGS, INC_DIRS, DEFINES, DEFINE_OS
#-------------------------------------------------------------------------------
# Flags to be used by the C++ compiler.
MAKEFILE_PATH_TMP := $(MAKEFILE_PATH)
MAKEFILE_DIR_TMP := $(MAKEFILE_DIR)
ifeq ($(PLATFORM), Win32)
    -include $(ZSQTLIB_DIR)/Make/ZSQtLib_CFlags_Win32.mk
else ifeq ($(PLATFORM), Linux)
    -include $(ZSQTLIB_DIR)/Make/ZSQtLib_CFlags_Linux.mk
endif
MAKEFILE_PATH := $(MAKEFILE_PATH_TMP)
MAKEFILE_DIR := $(MAKEFILE_DIR_TMP)

ifeq ($(PLATFORM), Win32)
    INC_DIRS := $(INC_DIRS) /I "$(MAKEFILE_DIR)"
    INC_DIRS := $(INC_DIRS) /I "$(ZSQTLIB_DIR)/Make"
    INC_DIRS := $(INC_DIRS) /I "$(ZSQTLIB_DIR)/Include/$(MODULE)"
    INC_DIRS := $(INC_DIRS) /I "$(QTDIR_INC)/QtCore"
    INC_DIRS := $(INC_DIRS) /I "$(QTDIR_INC)/QtXml"
    INC_DIRS := $(INC_DIRS) /I "$(QTDIR_INC)/QtNetwork"
    INC_DIRS := $(INC_DIRS) /I "$(QTDIR_INC)/QtGui"
    INC_DIRS := $(INC_DIRS) /I "$(PYTHONDIR)/Include"
    INC_DIRS := $(INC_DIRS) /I "$(PYTHONDIR)/Lib/site-packages/PySide/include/shiboken"
    INC_DIRS := $(INC_DIRS) /I "$(PYTHONDIR)/Lib/site-packages/PySide/include/PySide"
    INC_DIRS := $(INC_DIRS) /I "$(PYTHONDIR)/Lib/site-packages/PySide/include/PySide/QtCore"
    INC_DIRS := $(INC_DIRS) /I "$(PYTHONDIR)/Lib/site-packages/PySide/include/PySide/QtXml"
    INC_DIRS := $(INC_DIRS) /I "$(PYTHONDIR)/Lib/site-packages/PySide/include/PySide/QtNetwork"
    INC_DIRS := $(INC_DIRS) /I "$(PYTHONDIR)/Lib/site-packages/PySide/include/PySide/QtGui"
else ifeq ($(PLATFORM), Linux)
    INC_DIRS := $(INC_DIRS) -I "$(MAKEFILE_DIR)"
    INC_DIRS := $(INC_DIRS) -I "$(ZSQTLIB_DIR)/Make"
    INC_DIRS := $(INC_DIRS) -I "$(ZSQTLIB_DIR)/Include/$(MODULE)"
    INC_DIRS := $(INC_DIRS) -I "$(QTDIR_INC)/QtCore"
    INC_DIRS := $(INC_DIRS) -I "$(QTDIR_INC)/QtXml"
    INC_DIRS := $(INC_DIRS) -I "$(QTDIR_INC)/QtNetwork"
    INC_DIRS := $(INC_DIRS) -I "$(QTDIR_INC)/QtGui"
    INC_DIRS := $(INC_DIRS) -I "/usr/include/python2.7"
    INC_DIRS := $(INC_DIRS) -I "/usr/include/shiboken"
    INC_DIRS := $(INC_DIRS) -I "/usr/include/PySide"
    INC_DIRS := $(INC_DIRS) -I "/usr/include/PySide/QtCore"
    INC_DIRS := $(INC_DIRS) -I "/usr/include/PySide/QtXml"
    INC_DIRS := $(INC_DIRS) -I "/usr/include/PySide/QtNetwork"
    INC_DIRS := $(INC_DIRS) -I "/usr/include/PySide/QtGui"
endif

# Variable LFLAGS, LIB_DIRS
#-------------------------------------------------------------------------------
# Flags to be used by the linker.
MAKEFILE_PATH_TMP := $(MAKEFILE_PATH)
MAKEFILE_DIR_TMP := $(MAKEFILE_DIR)
ifeq ($(PLATFORM), Win32)
    -include $(ZSQTLIB_DIR)/Make/ZSQtLib_LFlags_Win32.mk
else ifeq ($(PLATFORM), Linux)
    -include $(ZSQTLIB_DIR)/Make/ZSQtLib_LFlags_Linux.mk
endif
MAKEFILE_PATH := $(MAKEFILE_PATH_TMP)
MAKEFILE_DIR := $(MAKEFILE_DIR_TMP)

ifeq ($(PLATFORM), Win32)
    LIB_DIRS := $(LIB_DIRS) /LIBPATH:"$(PYTHONDIR)/libs"
    LIB_DIRS := $(LIB_DIRS) /LIBPATH:"$(PYTHONDIR)/Lib/site-packages/PySide"
else ifeq ($(PLATFORM), Linux)
    LIB_DIRS := $(LIB_DIRS) -L "usr/lib/shiboken"
    LIB_DIRS := $(LIB_DIRS) -L "usr/lib/PySide"
endif

# Variable SBK_FLAGS
#-------------------------------------------------------------------------------
# Flags to be used by the shiboken generator.
MAKEFILE_PATH_TMP := $(MAKEFILE_PATH)
MAKEFILE_DIR_TMP := $(MAKEFILE_DIR)
ifeq ($(PLATFORM), Win32)
    -include $(ZSQTLIB_DIR)/Make/ZSQtLib_SbkFlags_Win32.mk
else ifeq ($(PLATFORM), Linux)
    -include $(ZSQTLIB_DIR)/Make/ZSQtLib_SbkFlags_Linux.mk
endif
MAKEFILE_PATH := $(MAKEFILE_PATH_TMP)
MAKEFILE_DIR := $(MAKEFILE_DIR_TMP)


#===============================================================================
# Make Targets
#===============================================================================

build:
	make -f $(ZSQTLIB_DIR)/Make/ZSQtLib_sub_Binding4Python.mk shiboken
	make -f $(ZSQTLIB_DIR)/Make/ZSQtLib_sub_Binding4Python.mk

build_shiboken_only:
	make -f $(ZSQTLIB_DIR)/Make/ZSQtLib_sub_Binding4Python.mk shiboken

build_wrappers_only:
	make -f $(ZSQTLIB_DIR)/Make/ZSQtLib_sub_Binding4Python.mk

clean:
	make -f $(ZSQTLIB_DIR)/Make/ZSQtLib_sub_Binding4Python.mk clean

help:
	@echo "Make file to be used with GNU make."
	@echo .
	@echo "Aufruf   : make -f ZSRemCmdBinding4Python.mk [CONFIG=<Debug|Release>] [Target]"
	@echo .
	@echo "Targets  : build   - (default) Creates the target."
	@echo "           clean   - Deletes all output files."
	@echo "           help    - Shows this help text."
	@echo .
	@echo "Example  : make -f ZSRemCmdBinding4Python.mk CONFIG=Debug build
	@echo .


$(info )
$(info <--- $(MAKEFILE_PATH) )
$(info )
