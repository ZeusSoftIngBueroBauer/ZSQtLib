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

# Environment Variable CONFIG
#-------------------------------------------------------------------------------
# Range [Release, Debug]
# Specifies whether the release or debug versions should be created.
ifndef CONFIG
    $(error CONFIG is not defined)
endif

# Environment Variable QTVERSION
#-------------------------------------------------------------------------------
# Range [3, 4, 5]
# Specifies the main version number of the Qt libraries to be used.
ifndef QTVERSION
    $(error QTVERSION is not defined)
endif

# Environment Variable OBJ_EXT
#-------------------------------------------------------------------------------
# Range [obj, o]
# The extension of the object files generated by the compiler vary between
# the platforms.
ifndef OBJ_EXT
    $(error OBJ_EXT is not defined)
endif

# Environment Variable LIB_EXT
#-------------------------------------------------------------------------------
# Range [lib, so]
# The extension of the dynamic link libraries (or shared objects on linux)
# vary between the platforms.
ifndef LIB_EXT
    $(error LIB_EXT is not defined)
endif

# Path: DEP_DIR
#-------------------------------------------------------------------------------
# Directory path where the dependency files (*.d) while be written to.
# Those dependency files are used to reproduce the AUTO_DEPEND function
# for header files included by CPP source files.
ifndef DEP_DIR
    $(error DEP_DIR is not defined)
endif

# Path: INT_DIR
#-------------------------------------------------------------------------------
# Directory path where the compiler generated intermediate files
# (*.obj, ..) while be written to.
ifndef INT_DIR
    $(error INT_DIR is not defined)
endif

# Path: MOC_DIR
#-------------------------------------------------------------------------------
# Directory path where the "*.cpp" output files of the Qt MOC compiler
# will be written to.
ifndef MOC_DIR
    $(error MOC_DIR is not defined)
endif

# Path: SBK_DIR
#-------------------------------------------------------------------------------
# Directory path where the output files of the Shiboken generator
# will be written to. The Shiboken generator is used to create site
# packages for the python interpreter.
ifdef BINDING4PYTHON
    ifndef SBK_DIR
        $(error SBK_DIR is not defined)
    endif
endif


#===============================================================================
# Input Files
#===============================================================================

# Variable HDR_ITEMS
#-------------------------------------------------------------------------------
# List of all header files. Extracted from the include directory of the module
# (e.g. list of all *.h" files in "./Include/ZSSys").
INC_DIR_TMP := $(ZSQTLIB_DIR_INC)/$(MODULE)
HDR_ITEMS := $(wildcard $(INC_DIR_TMP)/*.h)
export HDR_ITEMS

# Variable CPP_ITEMS
#-------------------------------------------------------------------------------
# List of all "cpp" source file. Extracted from the source directory of the module
# (e.g. list of all *.cpp" files in "./Source/ZSSys").
SRC_DIR_TMP := $(ZSQTLIB_DIR_SRC)/$(MODULE)
CPP_ITEMS := $(wildcard $(SRC_DIR_TMP)/*.cpp)
export CPP_ITEMS

# Variable CPP_DEP_ITEMS
#-------------------------------------------------------------------------------
# Auto Dependency to header files included by source files.
# Translate each file listed in CPP_ITEMS into its dependency file.
CPP_DEP_ITEMS := $(subst /,,$(subst $(SRC_DIR_TMP),,$(basename $(CPP_ITEMS))))
export CPP_DEP_ITEMS

# Variable OBJ_ITEMS
#-------------------------------------------------------------------------------
# List of all object files. Extracted from the list of "cpp" files as generated
# before and stored in CPP_ITEMS. The "*.cpp" extension of the source files will
# be replaced by the platform dependent extension for object files (e.g. "*.obj"
# or "*.o") and the path will replaced to point to the intermediate output
# directory of the C++ compiler. E.g. on Windows platforms the source file name
# "Source/ZSSys/ZSSysDllMain.cpp" will be replaced by the object file named
# "Tmp/Int/ZSSys/Win32/Debug/ZSSysDllMain.obj".
OBJ_ITEMS := $(patsubst $(SRC_DIR_TMP)/%.cpp,$(INT_DIR)/%.$(OBJ_EXT),$(CPP_ITEMS))
export OBJ_ITEMS

# Variable MOC_HDR_ITEMS
#-------------------------------------------------------------------------------
# List of all header files containing the Q_OBJECT macro.
# Extracted from the list of all header files as generated before.
# Please note that if HDR_ITEMS would be empty the "shell grep" command will
# not return (somehow deadlocked). So we only grep for Q_OBJECT macros in
# header files if the list of header files is not empty.
MOC_HDR_ITEMS := 
ifneq ($(wildcard $(INC_DIR_TMP)/*h),)
    MOC_HDR_ITEMS := $(shell grep -l Q_OBJECT $(HDR_ITEMS))
endif
export MOC_HDR_ITEMS

# Variable MOC_CPP_ITEMS
#-------------------------------------------------------------------------------
# List of all "cpp" source files. Extracted from the list of header files
# containing the Q_OBJECT macro as generated before and stored in MOC_HDR_ITEMS.
# The "*.h" extension of the header files will be replaced by "cpp"
# and the path will replaced to point to the intermediate output
# directory for MOC generated files. E.g. on Windows platforms the header file
# named "Include/ZSSys/ZSSysObjPool.h" will be replaced by the cpp file name
# "Tmp/Moc/ZSSys/Win32/Debug/moc_ZSSysObjPool.cpp".
MOC_CPP_ITEMS := $(patsubst $(INC_DIR_TMP)/%.h,$(MOC_DIR)/moc_%.cpp,$(MOC_HDR_ITEMS))
export MOC_CPP_ITEMS

# Variable MOC_CPP_DEP_ITEMS
#-------------------------------------------------------------------------------
# Auto Dependency to header files included by moc_ source files.
# Translate each file listed in MOC_CPP_ITEMS into its dependency file.
MOC_CPP_DEP_ITEMS := $(subst /,,$(subst $(MOC_DIR),,$(basename $(MOC_CPP_ITEMS))))
export MOC_CPP_DEP_ITEMS

# Variable MOC_OBJ_ITEMS
#-------------------------------------------------------------------------------
# List of all MOC object files. Extracted from the list of header files
# containing the Q_OBJECT macro as generated before and stored in MOC_HDR_ITEMS.
# The "*.h" extension of the header files will be replaced by ".obj" (or "*.o")
# and the path will replaced to point to the intermediate output
# directory for the C++ compiler generated files. E.g. on Windows platforms the
# header file name "Include/ZSSys/ZSSysObjPool.h" will be replaced by the "*.obj"
# file named "Tmp/Int/ZSSys/Win32/Debug/moc_ZSSysObjPool.obj".
MOC_OBJ_ITEMS := $(patsubst $(INC_DIR_TMP)/%.h,$(INT_DIR)/moc_%.$(OBJ_EXT),$(MOC_HDR_ITEMS))
export MOC_OBJ_ITEMS

# Variable DEPS
#-------------------------------------------------------------------------------
# List of auto generated dependency files. Those dependency files contain the
# "*.d" targets with the dependency to the header files included by the source files.
DEPS := $(patsubst %,$(DEP_DIR)/%.d,$(CPP_DEP_ITEMS))
DEPS := $(DEPS) $(patsubst %,$(DEP_DIR)/%.d,$(MOC_CPP_DEP_ITEMS))
export DEPS

# Mark the dependency files precious to make, so they won�t be automatically
# deleted as intermediate files.
.PRECIOUS: $(DEP_DIR)/%.d

# Variable OBJ_ITEMS
#-------------------------------------------------------------------------------
# List of all object files to be created by the C++ compiler.
# This is a collection of the C++ object files and moc C++ object files as
# generated before.
OBJ_ITEMS := $(OBJ_ITEMS) $(MOC_OBJ_ITEMS)
export OBJ_ITEMS

# Variable OSLIBS
#-------------------------------------------------------------------------------
# Must be added at the end of the LFLAGS on invoking the link command.
OSLIBS :=
ifeq ($(PLATFORM), Win32)
    OSLIBS = kernel32.lib \
        user32.lib \
        gdi32.lib \
        winspool.lib \
        comdlg32.lib \
        advapi32.lib \
        shell32.lib \
        ole32.lib \
        oleaut32.lib \
        uuid.lib \
        odbc32.lib \
        odbccp32.lib
endif
export OSLIBS

# Variable QTLIBS
#-------------------------------------------------------------------------------
# List of Qt libraries to be used.
# Please note that on Linux machines debug and release are not distinguished
# and you can mix and match release and debug versions.
QTLIBS :=
ifeq ($(PLATFORM), Win32)
    ifeq ($(CONFIG), Release)
        ifeq ($(QTVERSION), 3)
            QTLIBS := $(QTLIBS) $(LIB_PREFIX)QtCore3.$(LIB_EXT)
            QTLIBS := $(QTLIBS) $(LIB_PREFIX)QtNetwork3.$(LIB_EXT)
            QTLIBS := $(QTLIBS) $(LIB_PREFIX)QtXml3.$(LIB_EXT)
        else ifeq ($(QTVERSION), 4)
            QTLIBS := $(QTLIBS) $(LIB_PREFIX)QtCore4.$(LIB_EXT)
            QTLIBS := $(QTLIBS) $(LIB_PREFIX)QtNetwork4.$(LIB_EXT)
            QTLIBS := $(QTLIBS) $(LIB_PREFIX)QtXml4.$(LIB_EXT)
        else ifeq ($(QTVERSION), 5)
            QTLIBS := $(QTLIBS) $(LIB_PREFIX)Qt5Core.$(LIB_EXT)
            QTLIBS := $(QTLIBS) $(LIB_PREFIX)Qt5Network.$(LIB_EXT)
            QTLIBS := $(QTLIBS) $(LIB_PREFIX)Qt5Xml.$(LIB_EXT)
        endif
    else ifeq ($(CONFIG), Debug)
        ifeq ($(QTVERSION), 3)
            QTLIBS := $(QTLIBS) $(LIB_PREFIX)QtCored3.$(LIB_EXT)
            QTLIBS := $(QTLIBS) $(LIB_PREFIX)QtNetworkd3.$(LIB_EXT)
            QTLIBS := $(QTLIBS) $(LIB_PREFIX)QtXmld3.$(LIB_EXT)
        else ifeq ($(QTVERSION), 4)
            QTLIBS := $(QTLIBS) $(LIB_PREFIX)QtCored4.$(LIB_EXT)
            QTLIBS := $(QTLIBS) $(LIB_PREFIX)QtNetworkd4.$(LIB_EXT)
            QTLIBS := $(QTLIBS) $(LIB_PREFIX)QtXmld4.$(LIB_EXT)
        else ifeq ($(QTVERSION), 5)
            QTLIBS := $(QTLIBS) $(LIB_PREFIX)Qt5Cored.$(LIB_EXT)
            QTLIBS := $(QTLIBS) $(LIB_PREFIX)Qt5Networkd.$(LIB_EXT)
            QTLIBS := $(QTLIBS) $(LIB_PREFIX)Qt5Xmld.$(LIB_EXT)
        endif
    endif
else ifeq ($(PLATFORM), Linux)
    # On linux machines the Qt version is resolved using symbolic links and
    # the version is encode in the file extensions (e.g. libQtCore.so.4.8.7).
    # In addition the linux gcc and g++ compilier does not provide an option
    # to specify search paths for libraries. So we have to specify the full
    # path for the libraries.
    ifeq ($(CONFIG), Release)
        ifeq ($(QTVERSION), 3)
            QTLIBS := $(QTLIBS) $(QTDIR_LIB)/$(LIB_PREFIX)QtCore.$(LIB_EXT)
            QTLIBS := $(QTLIBS) $(QTDIR_LIB)/$(LIB_PREFIX)QtNetwork.$(LIB_EXT)
            QTLIBS := $(QTLIBS) $(QTDIR_LIB)/$(LIB_PREFIX)QtXml.$(LIB_EXT)
        else ifeq ($(QTVERSION), 4)
            QTLIBS := $(QTLIBS) $(QTDIR_LIB)/$(LIB_PREFIX)QtCore.$(LIB_EXT)
            QTLIBS := $(QTLIBS) $(QTDIR_LIB)/$(LIB_PREFIX)QtNetwork.$(LIB_EXT)
            QTLIBS := $(QTLIBS) $(QTDIR_LIB)/$(LIB_PREFIX)QtXml.$(LIB_EXT)
        else ifeq ($(QTVERSION), 5)
            QTLIBS := $(QTLIBS) $(QTDIR_LIB)/$(LIB_PREFIX)Qt5Core.$(LIB_EXT)
            QTLIBS := $(QTLIBS) $(QTDIR_LIB)/$(LIB_PREFIX)Qt5Network.$(LIB_EXT)
            QTLIBS := $(QTLIBS) $(QTDIR_LIB)/$(LIB_PREFIX)Qt5Xml.$(LIB_EXT)
        endif
    else ifeq ($(CONFIG), Debug)
        ifeq ($(QTVERSION), 3)
            QTLIBS := $(QTLIBS) $(QTDIR_LIB)/$(LIB_PREFIX)QtCored.$(LIB_EXT)
            QTLIBS := $(QTLIBS) $(QTDIR_LIB)/$(LIB_PREFIX)QtNetworkd.$(LIB_EXT)
            QTLIBS := $(QTLIBS) $(QTDIR_LIB)/$(LIB_PREFIX)QtXmld.$(LIB_EXT)
        else ifeq ($(QTVERSION), 4)
            QTLIBS := $(QTLIBS) $(QTDIR_LIB)/$(LIB_PREFIX)QtCored.$(LIB_EXT)
            QTLIBS := $(QTLIBS) $(QTDIR_LIB)/$(LIB_PREFIX)QtNetworkd.$(LIB_EXT)
            QTLIBS := $(QTLIBS) $(QTDIR_LIB)/$(LIB_PREFIX)QtXmld.$(LIB_EXT)
        else ifeq ($(QTVERSION), 5)
            QTLIBS := $(QTLIBS) $(QTDIR_LIB)/$(LIB_PREFIX)icui18n.$(LIB_EXT)
            QTLIBS := $(QTLIBS) $(QTDIR_LIB)/$(LIB_PREFIX)icuuc.$(LIB_EXT)
            QTLIBS := $(QTLIBS) $(QTDIR_LIB)/$(LIB_PREFIX)icudata.$(LIB_EXT)
            QTLIBS := $(QTLIBS) $(QTDIR_LIB)/$(LIB_PREFIX)Qt5Cored.$(LIB_EXT)
            QTLIBS := $(QTLIBS) $(QTDIR_LIB)/$(LIB_PREFIX)Qt5Networkd.$(LIB_EXT)
            QTLIBS := $(QTLIBS) $(QTDIR_LIB)/$(LIB_PREFIX)Qt5Xmld.$(LIB_EXT)
        endif
    endif
endif
export QTLIBS

# Variable 3RDPARTYLIBS
#-------------------------------------------------------------------------------
# List of 3rd party libraries to be used.
# This variable must be defined depending on the library to be built
# and must be provided by the corresponding make file of the module.
3RDPARTYLIBS :=
export 3RDPARTYLIBS

# Variable ZSQTLIBS
#-------------------------------------------------------------------------------
# List of ZSQtLib libraries to be used.
# This variable must be defined depending on the library to be built
# and must be provided by the corresponding make file of the module.
ZSQTLIBS :=
export ZSQTLIBS


$(info )
$(info <--- $(MAKEFILE_PATH) )
$(info )
