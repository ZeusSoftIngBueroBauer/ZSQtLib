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
# Commonly used environment variables
#===============================================================================

# To setup the build process several environment variables will be used.
# If an environment variable has not been explicitly set before processing this
# make file they will be set to default values and exported within this make file.

# Environment Variable PLATFORM
#-------------------------------------------------------------------------------
# Range [Win32, Win64, Linux]
# Specifies the operation system (PLATFORM) to be built for.
# On windows platforms the environment variable OS is defined e.g. as "Windows_NT".
# On linux platforms the command "uname -s" returns the name of the operating system.
ifdef PLATFORM
    ifdef TALKATIVE_MAKEFILE
        $(info PLATFORM defined as $(PLATFORM))
    endif
else
    ifeq ($(OS),Windows_NT)
        PLATFORM := Win32
    else
        UNAME_S := $(shell uname -s)
        ifeq ($(UNAME_S), Linux)
            PLATFORM := Linux
        endif
    endif
    ifdef TALKATIVE_MAKEFILE
        $(info PLATFORM detected as $(PLATFORM))
    endif
    export PLATFORM
endif
ifeq ($(PLATFORM), Win32)
    ifndef VCINSTALLDIR
        $(error VCINSTALLDIR not defined. Did you call vcvarsall.bat?)
    endif
else ifeq ($(PLATFORM), Linux)
else
    $(error Unknown PLATFORM $(PLATFORM) defined!)
endif

# Environment Variable PATH_SEP
#-------------------------------------------------------------------------------
# Range ["\", "/"]
# In order to copy files or create directories the path separator must be
# correctly inserted into the file paths. On linux the separator must be "/".
# On windows the file copy and mkdir commands cannot handle the slash but the
# backslahs must be used.
ifeq ($(PLATFORM), Win32)
    PATH_SEP := \\
    PATH_SEP := $(subst \\,\,$(PATH_SEP))
else
    PATH_SEP := /
endif
export PATH_SEP

# Environment Variable CONFIG_TYPE
#-------------------------------------------------------------------------------
# Range [dll, exe]
# Defines the type of output this make file should generate. The config type
# defines the extension of the generated target file.
# Make files may be used to create two different types of outputs:
# dll .. dynamic link libraries on windows (*.dll) and shared objects on linux (*.so).
# exe .. executables (*.exe) on both windows and linux machines.
ifdef CONFIG_TYPE
    ifdef TALKATIVE_MAKEFILE
        $(info CONFIG_TYPE defined as $(CONFIG_TYPE))
    endif
else
    $(error CONFIG_TYPE [dll, exe] is not defined!)
endif

# Environment Variable CONFIG
#-------------------------------------------------------------------------------
# Range [Release, Debug]
# Specifies whether the release or debug versions should be created.
# Please note that Linux and windows have differing views on this.
# In Linux, there's really not much of a point in offering a debug library.
# And you can mix and match release and debug versions.
ifdef CONFIG
    ifdef TALKATIVE_MAKEFILE
        $(info CONFIG defined as $(CONFIG))
    endif
else
    CONFIG := Release
    ifdef TALKATIVE_MAKEFILE
        $(info CONFIG set to $(CONFIG))
    endif
    export CONFIG
endif

# Environment Variable OBJ_EXT
#-------------------------------------------------------------------------------
# Range [obj, o]
# The extension of the object files generated by the compiler vary between
# the platforms.
ifdef OBJ_EXT
    ifdef TALKATIVE_MAKEFILE
        $(info OBJ_EXT defined as $(OBJ_EXT))
    endif
else
    ifeq ($(PLATFORM), Win32)
        OBJ_EXT := obj
    else ifeq ($(PLATFORM), Linux)
        OBJ_EXT := o
    endif
    ifdef TALKATIVE_MAKEFILE
        $(info OBJ_EXT set to $(OBJ_EXT))
    endif
    export OBJ_EXT
endif

# Environment Variable LIB_EXT
#-------------------------------------------------------------------------------
# Range [lib, so]
# The extension of the dynamic link libraries (or shared objects on linux)
# vary between the platforms.
ifdef LIB_EXT
    ifdef TALKATIVE_MAKEFILE
        $(info LIB_EXT defined as $(LIB_EXT))
    endif
else
    ifeq ($(PLATFORM), Win32)
        LIB_EXT := lib
    else ifeq ($(PLATFORM), Linux)
        LIB_EXT := so
    endif
    ifdef TALKATIVE_MAKEFILE
        $(info LIB_EXT set to $(LIB_EXT))
    endif
    export LIB_EXT
endif

# Environment Variable DLL_EXT
#-------------------------------------------------------------------------------
# Range [dll, so]
# The extension of the dynamic link libraries (or shared objects on linux)
# vary between the platforms.
ifdef DLL_EXT
    ifdef TALKATIVE_MAKEFILE
        $(info DLL_EXT defined as $(DLL_EXT))
    endif
else
    ifeq ($(PLATFORM), Win32)
        DLL_EXT := dll
    else ifeq ($(PLATFORM), Linux)
        DLL_EXT := so
    endif
    ifdef TALKATIVE_MAKEFILE
        $(info DLL_EXT set to $(DLL_EXT))
    endif
    export DLL_EXT
endif

# Environment Variable LIB_PREFIX
#-------------------------------------------------------------------------------
# Range [lib, ]
# The extension of the dynamic link libraries (or shared objects on linux)
# vary between the platforms.
ifdef LIB_PREFIX
    ifdef TALKATIVE_MAKEFILE
        $(info LIB_PREFIX defined as $(LIB_PREFIX))
    endif
else
    ifeq ($(PLATFORM), Win32)
        LIB_PREFIX :=
    else ifeq ($(PLATFORM), Linux)
        LIB_PREFIX := lib
    endif
    ifdef TALKATIVE_MAKEFILE
        $(info LIB_PREFIX set to $(LIB_PREFIX))
    endif
    export LIB_PREFIX
endif

# Environment Variable DLL_PREFIX
#-------------------------------------------------------------------------------
# Range [dll, so]
# The extension of the dynamic link libraries (or shared objects on linux)
# vary between the platforms.
ifdef DLL_PREFIX
    ifdef TALKATIVE_MAKEFILE
        $(info DLL_PREFIX defined as $(DLL_PREFIX))
    endif
else
    ifeq ($(PLATFORM), Win32)
        DLL_PREFIX :=
    else ifeq ($(PLATFORM), Linux)
        DLL_PREFIX := lib
    endif
    ifdef TALKATIVE_MAKEFILE
        $(info DLL_PREFIX set to $(DLL_PREFIX))
    endif
    export DLL_PREFIX
endif

# Environment Variables (QTDIR, QTDIR_INC, QTDIR_LIB, QTDIR_BIN, QTVERSION)
#-------------------------------------------------------------------------------
# On some platforms Qt may be installed in different ways. Usually the "lib"
# and "include" directories are sub directories below "QTDIR". But sometimes
# the Qt libraries are installed somewhere else (e.g. in /usr/lib/<whatever>").
# Also the include files may be located in a user defined location.
# !! Please note that to detect the QTVERSION the DLL_PREFIX and LIB_PREFIX !!
# !! variables must have been detected.                                     !!

ifdef QTDIR
    QTDIR := $(subst $(PATH_SEP),/,$(QTDIR))
    ifdef TALKATIVE_MAKEFILE
        $(info QTDIR defined as $(QTDIR))
    endif
    export QTDIR
else
    ifdef TALKATIVE_MAKEFILE
        $(info QTDIR is not defined)
    endif
endif

ifdef QTDIR_INC
    QTDIR_INC := $(subst $(PATH_SEP),/,$(QTDIR_INC))
    ifdef TALKATIVE_MAKEFILE
        $(info QTDIR_INC defined as $(QTDIR_INC))
    endif
    export QTDIR_INC
else
    ifdef QTDIR
        QTDIR_INC := $(QTDIR)/include
        ifdef TALKATIVE_MAKEFILE
            $(info QTDIR_INC set to $(QTDIR_INC))
        endif
        export QTDIR_INC
    else
        ifneq ($(wildcard /usr/include/qt4),)
            QTDIR_INC := /usr/include/qt4
            ifdef TALKATIVE_MAKEFILE
                $(info QTDIR_INC detected as $(QTDIR_INC))
            endif
            export QTDIR_INC
        else ifneq ($(wildcard /usr/local/Trolltech/Qt-4.8.7/include),)
            QTDIR_INC := /usr/local/Trolltech/Qt-4.8.7/include
            ifdef TALKATIVE_MAKEFILE
                $(info QTDIR_INC detected as $(QTDIR_INC))
            endif
            export QTDIR_INC
        endif
    endif
endif

ifeq ($(wildcard $(QTDIR_INC)),)
    $(error QTDIR_INC $(QTDIR_INC) does not exist!)
endif

ifdef QTDIR_LIB
    QTDIR_LIB := $(subst $(PATH_SEP),/,$(QTDIR_LIB))
    ifdef TALKATIVE_MAKEFILE
        $(info QTDIR_LIB defined as $(QTDIR_LIB))
    endif
    export QTDIR_LIB
else
    ifdef QTDIR
        QTDIR_LIB := $(QTDIR)/lib
        ifdef TALKATIVE_MAKEFILE
            $(info QTDIR_LIB set to $(QTDIR_LIB))
        endif
        export QTDIR_LIB
    else
        ifneq ($(wildcard /usr/lib/i386-linux-gnu/libQt*),)
            QTDIR_LIB := /usr/lib/i386-linux-gnu
            ifdef TALKATIVE_MAKEFILE
                $(info QTDIR_LIB detected as $(QTDIR_LIB))
            endif
            export QTDIR_LIB
        else ifneq ($(wildcard /usr/local/Trolltech/Qt-4.8.7/lib/libQt*),)
            QTDIR_LIB := /usr/local/Trolltech/Qt-4.8.7/lib
            ifdef TALKATIVE_MAKEFILE
                $(info QTDIR_LIB detected as $(QTDIR_LIB))
            endif
            export QTDIR_LIB
        endif
    endif
endif

ifdef QTDIR_BIN
    QTDIR_BIN := $(subst $(PATH_SEP),/,$(QTDIR_BIN))
    ifdef TALKATIVE_MAKEFILE
        $(info QTDIR_BIN defined as $(QTDIR_BIN))
    endif
    export QTDIR_BIN
else
    ifdef QTDIR
        QTDIR_BIN := $(QTDIR)/bin
        ifdef TALKATIVE_MAKEFILE
            $(info QTDIR_BIN set to $(QTDIR_BIN))
        endif
        export QTDIR_BIN
    else
        ifneq ($(wildcard /usr/bin/moc*),)
            QTDIR_BIN := /usr/bin
            ifdef TALKATIVE_MAKEFILE
                $(info QTDIR_BIN detected as $(QTDIR_BIN))
            endif
            export QTDIR_BIN
        else ifneq ($(wildcard /usr/local/Trolltech/Qt-4.8.7/bin/moc*),)
            QTDIR_BIN := /usr/local/Trolltech/Qt-4.8.7/bin
            export QTDIR_BIN
        endif
    endif
endif

ifeq ($(wildcard $(QTDIR_BIN)),)
    $(error QTDIR_BIN $(QTDIR_BIN) does not exist!)
endif

ifdef QTVERSION
    ifdef TALKATIVE_MAKEFILE
        $(info QTVERSION defined as $(QTVERSION))
    endif
else
    ifeq ($(PLATFORM), Win32)
        QTCORELIB := $(LIB_PREFIX)QtCore3.$(LIB_EXT)
    else ifeq ($(PLATFORM), Linux)
        QTCORELIB := $(LIB_PREFIX)QtCore.$(LIB_EXT).3
    endif
    QTCORELIB_FOUND := $(wildcard $(QTDIR_LIB)/$(QTCORELIB))
    ifneq ($(QTCORELIB_FOUND),)
        $(info $(QTCORELIB) found in $(QTDIR_LIB)!)
        QTVERSION := 3
    else
        ifeq ($(PLATFORM), Win32)
            QTCORELIB := $(LIB_PREFIX)QtCore4.$(LIB_EXT)
        else ifeq ($(PLATFORM), Linux)
            QTCORELIB := $(LIB_PREFIX)QtCore.$(LIB_EXT).4
        endif
        QTCORELIB_FOUND := $(wildcard $(QTDIR_LIB)/$(QTCORELIB))
        ifneq ($(QTCORELIB_FOUND),)
            $(info $(QTCORELIB) found in $(QTDIR_LIB)!)
            QTVERSION := 4
        else
            ifeq ($(PLATFORM), Win32)
                QTCORELIB := $(LIB_PREFIX)Qt5Core.$(LIB_EXT)
            else ifeq ($(PLATFORM), Linux)
                QTCORELIB := $(LIB_PREFIX)Qt5Core.$(LIB_EXT)
            endif
            QTCORELIB_FOUND := $(wildcard $(QTDIR_LIB)/$(QTCORELIB))
            ifneq ($(QTCORELIB_FOUND),)
                $(info $(QTCORELIB) found in $(QTDIR_LIB)!)
                QTVERSION := 5
            endif
        endif
    endif
    ifndef QTVERSION
        $(error QTVERSION [3, 4, 5] not defined!)
    else
        ifdef TALKATIVE_MAKEFILE
            $(info QTVERSION detected as $(QTVERSION))
        endif
        export QTVERSION
    endif
endif

# Environment Variable PYTHONDIR, PYTHONDIR_INC, PYTHONDIR_LIB, PYTHONVERSION
#-------------------------------------------------------------------------------
# PYTHONDIR, PYTHONDIR_INC, PYTHONDIR_LIB:
#   Range ["Valid Directory Path"]
#   Defines the directory in which python is installed.
#   Used to locate the python libraries to be linked with in order to create site
#   packages.
# PYTHONVERSION: Range [27, 34]
#   Specifies the python version for which site packages should be created.
ifdef PYTHONDIR
    PYTHONDIR := $(subst $(PATH_SEP),/,$(PYTHONDIR))
    ifdef TALKATIVE_MAKEFILE
        $(info PYTHONDIR defined as $(PYTHONDIR))
    endif
    export PYTHONDIR
else
    ifeq ($(PLATFORM), Win32)
        PYTHONDIR := C:/Python27
        PYTHONDIR := $(subst $(PATH_SEP),/,$(PYTHONDIR))
        ifdef TALKATIVE_MAKEFILE
            $(info PYTHONDIR set to $(PYTHONDIR))
        endif
    else
        ifdef TALKATIVE_MAKEFILE
            $(info PYTHONDIR is not defined)
        endif
    endif
    export PYTHONDIR
endif

ifdef PYTHONDIR_INC
    PYTHONDIR_INC := $(subst $(PATH_SEP),/,$(PYTHONDIR_INC))
    ifdef TALKATIVE_MAKEFILE
        $(info PYTHONDIR_INC defined as $(PYTHONDIR_INC))
    endif
    export PYTHONDIR_INC
else
    ifdef PYTHONDIR
        PYTHONDIR_INC := $(PYTHONDIR)/include
        ifdef TALKATIVE_MAKEFILE
            $(info PYTHONDIR_INC set to $(PYTHONDIR_INC))
        endif
        export PYTHONDIR_INC
    else
        ifneq ($(wildcard /usr/include/python2.7),)
            PYTHONDIR_INC := /usr/include/python2.7
            ifdef TALKATIVE_MAKEFILE
                $(info PYTHONDIR_INC detected as $(PYTHONDIR_INC))
            endif
            export PYTHONDIR_INC
        endif
    endif
endif

#ifeq ($(wildcard $(PYTHONDIR_INC)),)
#    $(error PYTHONDIR_INC $(PYTHONDIR_INC) does not exist!)
#endif

ifdef PYTHONDIR_LIB
    PYTHONDIR_LIB := $(subst $(PATH_SEP),/,$(PYTHONDIR_LIB))
    ifdef TALKATIVE_MAKEFILE
        $(info PYTHONDIR_LIB defined as $(PYTHONDIR_LIB))
    endif
    export PYTHONDIR_LIB
else
    ifdef PYTHONDIR
        PYTHONDIR_LIB := $(PYTHONDIR)/lib
        ifdef TALKATIVE_MAKEFILE
            $(info PYTHONDIR_LIB set to $(PYTHONDIR_LIB))
        endif
        export PYTHONDIR_LIB
    else
        PYTHON_LIBS := $(wildcard /usr/lib/libp*)
        $(info PYTHON_LIBS: $(PYTHON_LIBS))
        ifneq ($(wildcard /usr/lib/libpython*),)
            PYTHONDIR_LIB := /usr/lib
            ifdef TALKATIVE_MAKEFILE
                $(info PYTHONDIR_LIB detected as $(PYTHONDIR_LIB))
            endif
            export PYTHONDIR_LIB
        else ifneq ($(wildcard /usr/lib/i386-linux-gnu/libpython*),)
            PYTHONDIR_LIB := /usr/lib/i386-linux-gnu
            ifdef TALKATIVE_MAKEFILE
                $(info PYTHONDIR_LIB detected as $(PYTHONDIR_LIB))
            endif
            export PYTHONDIR_LIB
        endif
    endif
endif

#ifeq ($(wildcard $(PYTHONDIR_LIB)),)
#    $(error PYTHONDIR_LIB $(PYTHONDIR_LIB) does not exist!)
#endif

ifdef PYTHONVERSION
    ifdef TALKATIVE_MAKEFILE
        $(info PYTHONVERSION defined as $(PYTHONVERSION))
    endif
else
    PYTHONVERSION := 27
    ifdef TALKATIVE_MAKEFILE
        $(info PYTHONVERSION set to $(PYTHONVERSION))
    endif
    export PYTHONVERSION
endif

# Environment Variables (ZSQTLIB_DIR, ZSQTLIB_DIR_INC, ZSQTLIB_DIR_LIB)
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
ifdef ZSQTLIB_DIR
    ZSQTLIB_DIR := $(subst $(PATH_SEP),/,$(ZSQTLIB_DIR))
    ifdef TALKATIVE_MAKEFILE
        $(info ZSQTLIB_DIR defined as $(ZSQTLIB_DIR))
    endif
    export ZSQTLIB_DIR
else
    MAKEFILE_DIR := $(shell dirname $(MAKEFILE_PATH))
    ZSQTLIB_DIR := $(subst /Make/$(MODULE),,$(MAKEFILE_DIR))
    ZSQTLIB_DIR := $(subst $(PATH_SEP),/,$(ZSQTLIB_DIR))
    ifdef TALKATIVE_MAKEFILE
        $(info ZSQTLIB_DIR detected as $(ZSQTLIB_DIR))
    endif
    export ZSQTLIB_DIR
endif
ifeq ($(wildcard $(ZSQTLIB_DIR)),)
    $(error ZSQTLIB_DIR $(ZSQTLIB_DIR) does not exist!)
endif

# Path: ZSQTLIB_DIR_INC
#-------------------------------------------------------------------------------
# Directory path of the header (*.h) files.
ifdef ZSQTLIB_DIR_INC
    ZSQTLIB_DIR_INC := $(subst $(PATH_SEP),/,$(ZSQTLIB_DIR_INC))
    ifdef TALKATIVE_MAKEFILE
        $(info ZSQTLIB_DIR_INC defined as $(ZSQTLIB_DIR_INC))
    endif
    export ZSQTLIB_DIR_INC
else
    ZSQTLIB_DIR_INC := $(ZSQTLIB_DIR)/Include
    ifdef TALKATIVE_MAKEFILE
        $(info ZSQTLIB_DIR_INC set to $(ZSQTLIB_DIR_INC))
    endif
    export ZSQTLIB_DIR_INC
endif

# Path: ZSQTLIB_DIR_SRC
#-------------------------------------------------------------------------------
# Directory path of the source (*.cpp) files.
ifdef ZSQTLIB_DIR_SRC
    ZSQTLIB_DIR_SRC := $(subst $(PATH_SEP),/,$(ZSQTLIB_DIR_SRC))
    ifdef TALKATIVE_MAKEFILE
        $(info ZSQTLIB_DIR_SRC defined as $(ZSQTLIB_DIR_SRC))
    endif
    export ZSQTLIB_DIR_SRC
else
    ZSQTLIB_DIR_SRC := $(ZSQTLIB_DIR)/Source
    ifdef TALKATIVE_MAKEFILE
        $(info ZSQTLIB_DIR_SRC set to $(ZSQTLIB_DIR_SRC))
    endif
    export ZSQTLIB_DIR_SRC
endif

# Path: ZSQTLIB_DIR_IMG
#-------------------------------------------------------------------------------
# Directory path of the image (*.bmp, *.png, *.ico, ..) files.
ifdef ZSQTLIB_DIR_IMG
    ZSQTLIB_DIR_IMG := $(subst $(PATH_SEP),/,$(ZSQTLIB_DIR_IMG))
    ifdef TALKATIVE_MAKEFILE
        $(info ZSQTLIB_DIR_IMG defined as $(ZSQTLIB_DIR_IMG))
    endif
    export ZSQTLIB_DIR_IMG
else
    ZSQTLIB_DIR_IMG := $(ZSQTLIB_DIR)/Images
    ifdef TALKATIVE_MAKEFILE
        $(info ZSQTLIB_DIR_IMG set to $(ZSQTLIB_DIR_IMG))
    endif
    export ZSQTLIB_DIR_IMG
endif

# Path: ZSQTLIB_DIR_TMP
#-------------------------------------------------------------------------------

ifdef ZSQTLIB_DIR_TMP
    ZSQTLIB_DIR_TMP := $(subst $(PATH_SEP),/,$(ZSQTLIB_DIR_TMP))
    ifdef TALKATIVE_MAKEFILE
        $(info ZSQTLIB_DIR_TMP defined as $(ZSQTLIB_DIR_TMP))
    endif
    export ZSQTLIB_DIR_TMP
else
    ifeq ($(PLATFORM), Win32)
        ZSQTLIB_DIR_TMP := $(ZSQTLIB_DIR)/Tmp
    else ifeq ($(PLATFORM), Linux)
        #ZSQTLIB_DIR_TMP := /tmp/ZeusSoft/ZSQtLib
        ZSQTLIB_DIR_TMP := $(ZSQTLIB_DIR)/Tmp
    endif
    ifdef TALKATIVE_MAKEFILE
        $(info ZSQTLIB_DIR_TMP set to $(ZSQTLIB_DIR_TMP))
    endif
    export ZSQTLIB_DIR_TMP
endif

# Path: ZSQTLIB_DIR_LIB
#-------------------------------------------------------------------------------
# Directory path where the library files will be written to. Other
# make files use this LIB directory to locate libraries they depend on.
ifdef ZSQTLIB_DIR_LIB
    ZSQTLIB_DIR_LIB := $(subst $(PATH_SEP),/,$(ZSQTLIB_DIR_LIB))
    ifdef TALKATIVE_MAKEFILE
        $(info ZSQTLIB_DIR_LIB defined as $(ZSQTLIB_DIR_LIB))
    endif
    export ZSQTLIB_DIR_LIB
else
    ifeq ($(PLATFORM), Win32)
        ZSQTLIB_DIR_LIB := $(ZSQTLIB_DIR)/Lib/$(PLATFORM)
    else ifeq ($(PLATFORM), Linux)
        #ZSQTLIB_DIR_LIB := /usr/local/ZeusSoft/ZSQtLib
        ZSQTLIB_DIR_LIB := $(ZSQTLIB_DIR)/Lib/$(PLATFORM)
    endif
    ifdef TALKATIVE_MAKEFILE
        $(info ZSQTLIB_DIR_LIB set to $(ZSQTLIB_DIR_LIB))
    endif
    export ZSQTLIB_DIR_LIB
endif

# Path: ZSQTLIB_DIR_BIN
#-------------------------------------------------------------------------------
# Directory path where the binary output files (*.exe, *.dll, *.pdb) will be written to.
ifdef ZSQTLIB_DIR_BIN
    ZSQTLIB_DIR_BIN := $(subst $(PATH_SEP),/,$(ZSQTLIB_DIR_BIN))
    ifdef TALKATIVE_MAKEFILE
        $(info ZSQTLIB_DIR_BIN defined as $(ZSQTLIB_DIR_BIN))
    endif
    export ZSQTLIB_DIR_BIN
else
    ifeq ($(PLATFORM), Win32)
        ZSQTLIB_DIR_BIN := $(ZSQTLIB_DIR)/Bin/$(PLATFORM)
    else ifeq ($(PLATFORM), Linux)
        #ZSQTLIB_DIR_BIN := /usr/local/ZeusSoft/ZSQtLib
        ZSQTLIB_DIR_BIN := $(ZSQTLIB_DIR)/Bin/$(PLATFORM)
    endif
    ifdef TALKATIVE_MAKEFILE
        $(info ZSQTLIB_DIR_BIN set to $(ZSQTLIB_DIR_BIN))
    endif
    export ZSQTLIB_DIR_BIN
endif

# Path: ZSQTLIB_DIR_STP
#-------------------------------------------------------------------------------
# Directory path where the setup files (*.msi) will be written to.
ifdef ZSQTLIB_DIR_STP
    ZSQTLIB_DIR_STP := $(subst $(PATH_SEP),/,$(ZSQTLIB_DIR_STP))
    ifdef TALKATIVE_MAKEFILE
        $(info ZSQTLIB_DIR_STP defined as $(ZSQTLIB_DIR_STP))
    endif
    export ZSQTLIB_DIR_STP
else
    ifeq ($(PLATFORM), Win32)
        ZSQTLIB_DIR_STP := $(ZSQTLIB_DIR)/Setup/$(PLATFORM)
    else ifeq ($(PLATFORM), Linux)
        #ZSQTLIB_DIR_STP := /usr/local/ZeusSoft/ZSQtLib
        ZSQTLIB_DIR_STP := $(ZSQTLIB_DIR)/Setup/$(PLATFORM)
    endif
    ifdef TALKATIVE_MAKEFILE
        $(info ZSQTLIB_DIR_STP set to $(ZSQTLIB_DIR_STP))
    endif
    export ZSQTLIB_DIR_STP
endif

# Path: ZSQTLIB_DIR_INSTALL
#-------------------------------------------------------------------------------
# Directory path where the binaries will be copied to.
ifdef ZSQTLIB_DIR_INSTALL
    ZSQTLIB_DIR_INSTALL := $(subst $(PATH_SEP),/,$(ZSQTLIB_DIR_INSTALL))
    ifdef TALKATIVE_MAKEFILE
        $(info ZSQTLIB_DIR_INSTALL defined as $(ZSQTLIB_DIR_INSTALL))
    endif
    export ZSQTLIB_DIR_INSTALL
else
    ifeq ($(PLATFORM), Win32)
    else ifeq ($(PLATFORM), Linux)
        ZSQTLIB_DIR_INSTALL := /usr/local/ZeusSoft
    endif
    ifdef TALKATIVE_MAKEFILE
        $(info ZSQTLIB_DIR_INSTALL set to $(ZSQTLIB_DIR_INSTALL))
    endif
    export ZSQTLIB_DIR_INSTALL
endif


$(info )
$(info <--- $(MAKEFILE_PATH) )
$(info )
