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
    ZSQTLIB_DIR := $(subst /Apps/Test/ZSMakeHelloWorld/Make/$(MODULE),,$(MAKEFILE_DIR))
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

ZSQTLIB_SUB_MAKEFILE_PATH_TMP := $(MAKEFILE_PATH)
ZSQTLIB_SUB_MAKEFILE_DIR_TMP := $(MAKEFILE_DIR)
-include $(ZSQTLIB_DIR)/Make/ZSQtLib_EnvVars.mk
MAKEFILE_PATH := $(ZSQTLIB_SUB_MAKEFILE_PATH_TMP)
MAKEFILE_DIR := $(ZSQTLIB_SUB_MAKEFILE_DIR_TMP)


#===============================================================================
# Aliases for shell commands
#===============================================================================

# The name of tools to be used may differ depending on the operating system
# or on other installed tools.

ZSQTLIB_SUB_MAKEFILE_PATH_TMP := $(MAKEFILE_PATH)
ZSQTLIB_SUB_MAKEFILE_DIR_TMP := $(MAKEFILE_DIR)
-include $(ZSQTLIB_DIR)/Make/ZSQtLib_ShellCmds.mk
MAKEFILE_PATH := $(ZSQTLIB_SUB_MAKEFILE_PATH_TMP)
MAKEFILE_DIR := $(ZSQTLIB_SUB_MAKEFILE_DIR_TMP)


#===============================================================================
# Directory paths for input files
#===============================================================================

# Those directory paths are relative to the base directory of this make file.

# Path: INC_DIR
#-------------------------------------------------------------------------------
# Directory path of the header (*.h) files.
INC_DIR := $(ZSQTLIB_DIR)/Apps/Test/ZSMakeHelloWorld/Include
export INC_DIR

# Path: SRC_DIR
#-------------------------------------------------------------------------------
# Directory path of the source (*.cpp) files.
SRC_DIR := $(ZSQTLIB_DIR)/Apps/Test/ZSMakeHelloWorld/Source
export SRC_DIR

# Path: IMG_DIR
#-------------------------------------------------------------------------------
# Directory path of the image (*.bmp, *.png, *.ico, ..) files.
IMG_DIR := $(ZSQTLIB_DIR)/Apps/Test/ZSMakeHelloWorld/Images
IMG_DIR := $(subst GUI,,$(IMG_DIR))
export IMG_DIR


#===============================================================================
# Directory paths for intermediately generated files
#===============================================================================

# On Windows systems those directory paths are relative to the base directory
# of the library modules.
# On Linux machines those directory paths are relative to the root in "/tmp".

# Path: BUILD_DIR
#-------------------------------------------------------------------------------

ifdef BUILD_DIR
    ifdef TALKATIVE_MAKEFILE
        $(info BUILD_DIR defined as $(BUILD_DIR))
    endif
else
    ifeq ($(PLATFORM), Win32)
        BUILD_DIR := $(ZSQTLIB_DIR)/Tmp
    else ifeq ($(PLATFORM), Linux)
        BUILD_DIR := /tmp/ZeusSoft/ZSQtLib
    endif
    ifdef TALKATIVE_MAKEFILE
        $(info BUILD_DIR set to $(BUILD_DIR))
    endif
    export BUILD_DIR
endif

# Path: DEP_DIR
#-------------------------------------------------------------------------------
# Directory path where the dependency files (*.d) while be written to.
# Those dependency files are used to reproduce the AUTO_DEPEND function
# for header files included by CPP source files.
DEP_DIR := $(BUILD_DIR)/Dep/Apps/Test/$(MODULE)/$(PLATFORM)/$(CONFIG)
export DEP_DIR

# Path: INT_DIR
#-------------------------------------------------------------------------------
# Directory path where the compiler generated intermediate files
# (*.obj, ..) while be written to.
INT_DIR := $(BUILD_DIR)/Int/Apps/Test/$(MODULE)/$(PLATFORM)/$(CONFIG)
export INT_DIR

# Path: MOC_DIR
#-------------------------------------------------------------------------------
# Directory path where the "*.cpp" output files of the Qt MOC compiler
# will be written to.
MOC_DIR := $(BUILD_DIR)/Moc/Apps/Test/$(MODULE)/$(PLATFORM)/$(CONFIG)
export MOC_DIR

# Path: OUT_DIR
#-------------------------------------------------------------------------------
# Directory path where the target output files of the compiler and linker
# (*.dll, *.exe, ..) while be written to.
OUT_DIR := $(BUILD_DIR)/Out/Apps/Test/$(MODULE)/$(PLATFORM)/$(CONFIG)
export OUT_DIR

# Path: SBK_DIR
#-------------------------------------------------------------------------------
# Directory path where the output files of the Shiboken generator
# will be written to. The Shiboken generator is used to create site
# packages for the python interpreter.
SBK_DIR := $(BUILD_DIR)/Shiboken/Python$(PYTHONVERSION)/$(PLATFORM)/$(CONFIG)/ZSQtLib/Apps/Test
export SBK_DIR


#===============================================================================
# Directory paths for output files
#===============================================================================

# On Windows systems those directory paths are relative to the base directory
# of the ZS Qt library.
# On Linux machines those directory paths are relative to "/usr/local/ZSQtLib".

# Path: LIB_DIR
#-------------------------------------------------------------------------------
# Directory path where the library files will be written to. Other
# make files use this LIB directory to locate libraries they depend on.

ifdef LIB_DIR
    ifdef TALKATIVE_MAKEFILE
        $(info LIB_DIR defined as $(LIB_DIR))
    endif
else
    ifeq ($(PLATFORM), Win32)
        LIB_DIR := $(ZSQTLIB_DIR)/Lib/$(PLATFORM)
    else ifeq ($(PLATFORM), Linux)
        LIB_DIR := /usr/local/ZeusSoft/ZSQtLib
    endif
    ifdef TALKATIVE_MAKEFILE
        $(info LIB_DIR set to $(LIB_DIR))
    endif
    export LIB_DIR
endif

# Path: BIN_DIR
#-------------------------------------------------------------------------------
# Directory path where the binary output files will be written to.
# This bin directory should include all dynamic link libaries which are
# necessary to run the application.
ifdef BIN_DIR
    ifdef TALKATIVE_MAKEFILE
        $(info BIN_DIR defined as $(BIN_DIR))
    endif
else
    ifeq ($(PLATFORM), Win32)
        BIN_DIR := $(ZSQTLIB_DIR)/Bin/$(PLATFORM)
    else ifeq ($(PLATFORM), Linux)
        BIN_DIR := /usr/local/ZeusSoft/ZSQtLib
    endif
    ifdef TALKATIVE_MAKEFILE
        $(info BIN_DIR set to $(BIN_DIR))
    endif
    export BIN_DIR
endif

# Path: STP_DIR
#-------------------------------------------------------------------------------
# Directory path where the generated setup files will be written to.
ifdef STP_DIR
    ifdef TALKATIVE_MAKEFILE
        $(info STP_DIR defined as $(STP_DIR))
    endif
else
    ifeq ($(PLATFORM), Win32)
        STP_DIR := $(ZSQTLIB_DIR)/Setup/$(PLATFORM)
    else ifeq ($(PLATFORM), Linux)
        STP_DIR := /usr/local/ZeusSoft/ZSQtLib
    endif
    ifdef TALKATIVE_MAKEFILE
        $(info STP_DIR set to $(STP_DIR))
    endif
    export STP_DIR
endif

# Path: BINDING4PYTHON_STP_DIR
#-------------------------------------------------------------------------------
ifdef BINDING4PYTHON_STP_DIR
    ifdef TALKATIVE_MAKEFILE
        $(info BINDING4PYTHON_STP_DIR defined as $(BINDING4PYTHON_STP_DIR))
    endif
else
    BINDING4PYTHON_STP_DIR := $(STP_DIR)/Bindings/Python$(PYTHONVERSION)/ZSQtLib
    ifdef TALKATIVE_MAKEFILE
        $(info BINDING4PYTHON_STP_DIR set to $(BINDING4PYTHON_STP_DIR))
    endif
    export BINDING4PYTHON_STP_DIR
endif


#===============================================================================
# Input Files
#===============================================================================

# Variable HDR_ITEMS
#-------------------------------------------------------------------------------
# List of all header files. Extracted from the include directory of the module
# (e.g. list of all *.h" files in "./Include/ZSSys").
HDR_ITEMS := $(wildcard $(INC_DIR)/*.h)

# Variable CPP_ITEMS
#-------------------------------------------------------------------------------
# List of all "cpp" source file. Extracted from the source directory of the module
# (e.g. list of all *.cpp" files in "./Source/ZSSys").
CPP_ITEMS := $(wildcard $(SRC_DIR)/*.cpp)

# Variable OBJ_ITEMS
#-------------------------------------------------------------------------------
# List of all object files. Extracted from the list of "cpp" files as generated
# before and stored in CPP_ITEMS. The "*.cpp" extension of the source files will
# be replaced by the platform dependent extension for object files (e.g. "*.obj"
# or "*.o") and the path will replaced to point to the intermediate output
# directory of the C++ compiler. E.g. on Windows platforms the source file name
# "Source/ZSSys/ZSSysDllMain.cpp" will be replaced by the object file named
# "Tmp/Int/ZSSys/Win32/Debug/ZSSysDllMain.obj".
OBJ_ITEMS := $(patsubst $(SRC_DIR)/%.cpp,$(INT_DIR)/%.$(OBJ_EXT),$(CPP_ITEMS))

# Variable MOC_HDR_ITEMS
#-------------------------------------------------------------------------------
# List of all header files containing the Q_OBJECT macro.
# Extracted from the list of all header files as generated before.
# Please note that if HDR_ITEMS would be empty the "shell grep" command will
# not return (somehow deadlocked). So we only grep for Q_OBJECT macros in
# header files if the list of header files is not empty.
MOC_HDR_ITEMS := 
ifneq ($(wildcard $(INC_DIR)/*h),)
    MOC_HDR_ITEMS := $(shell grep -l Q_OBJECT $(HDR_ITEMS))
endif

# Variable MOC_CPP_ITEMS
#-------------------------------------------------------------------------------
# List of all "cpp" source files. Extracted from the list of header files
# containing the Q_OBJECT macro as generated before and stored in MOC_HDR_ITEMS.
# The "*.h" extension of the header files will be replaced by "cpp"
# and the path will replaced to point to the intermediate output
# directory for MOC generated files. E.g. on Windows platforms the header file
# named "Include/ZSSys/ZSSysObjPool.h" will be replaced by the cpp file name
# "Tmp/Moc/ZSSys/Win32/Debug/moc_ZSSysObjPool.cpp".
MOC_CPP_ITEMS := $(patsubst $(INC_DIR)/%.h,$(MOC_DIR)/moc_%.cpp,$(MOC_HDR_ITEMS))

# Variable CPP_DEP_ITEMS
#-------------------------------------------------------------------------------
# Auto Dependency to header files included by source files.
# Translate each file listed in CPP_ITEMS into its dependency file.
CPP_DEP_ITEMS := $(subst /,,$(subst $(SRC_DIR),,$(basename $(CPP_ITEMS))))

# Variable MOC_CPP_DEP_ITEMS
#-------------------------------------------------------------------------------
# Auto Dependency to header files included by moc_ source files.
# Translate each file listed in MOC_CPP_ITEMS into its dependency file.
MOC_CPP_DEP_ITEMS := $(subst /,,$(subst $(MOC_DIR),,$(basename $(MOC_CPP_ITEMS))))

# Variable DEPS
#-------------------------------------------------------------------------------
# List of auto generated dependency files. Those dependency files contain the
# "*.d" targets with the dependency to the header files included by the source files.
DEPS := $(patsubst %,$(DEP_DIR)/%.d,$(CPP_DEP_ITEMS))
DEPS := $(DEPS) $(patsubst %,$(DEP_DIR)/%.d,$(MOC_CPP_DEP_ITEMS))

# Mark the dependency files precious to make, so they won’t be automatically
# deleted as intermediate files.
.PRECIOUS: $(DEP_DIR)/%.d

# Variable MOC_OBJ_ITEMS
#-------------------------------------------------------------------------------
# List of all MOC object files. Extracted from the list of header files
# containing the Q_OBJECT macro as generated before and stored in MOC_HDR_ITEMS.
# The "*.h" extension of the header files will be replaced by ".obj" (or "*.o")
# and the path will replaced to point to the intermediate output
# directory for the C++ compiler generated files. E.g. on Windows platforms the
# header file name "Include/ZSSys/ZSSysObjPool.h" will be replaced by the "*.obj"
# file named "Tmp/Int/ZSSys/Win32/Debug/moc_ZSSysObjPool.obj".
MOC_OBJ_ITEMS := $(patsubst $(INC_DIR)/%.h,$(INT_DIR)/moc_%.$(OBJ_EXT),$(MOC_HDR_ITEMS))

# Variable OBJ_ITEMS
#-------------------------------------------------------------------------------
# List of all object files to be created by the C++ compiler.
# This is a collection of the C++ object files and moc C++ object files as
# generated before.
OBJ_ITEMS := $(OBJ_ITEMS) $(MOC_OBJ_ITEMS)

# Variable QTLIBS
#-------------------------------------------------------------------------------
# List of Qt libraries to be used.
# Please note that on Linux machines debug and release are not distinguished
# and you can mix and match release and debug versions.
#ifeq ($(PLATFORM), Win32)
#    ifeq ($(CONFIG), Release)
#        QTLIBS = QtCore4.$(LIB_EXT) \
#            QtNetwork4.$(LIB_EXT) \
#            QtXml4.$(LIB_EXT)
#    else ifeq ($(CONFIG), Debug)
#        QTLIBS = QtCored4.$(LIB_EXT) \
#            QtNetworkd4.$(LIB_EXT) \
#            QtXmld4.$(LIB_EXT)
#    endif
#else ifeq ($(PLATFORM), Linux)
#    QTLIBS = $(QTDIR_LIB)/libQtCore.$(LIB_EXT) \
#        $(QTDIR_LIB)/libQtNetwork.$(LIB_EXT) \
#        $(QTDIR_LIB)/libQtXml.$(LIB_EXT)
#endif

# Variable OSLIBS
#-------------------------------------------------------------------------------
# Must be added at the end of the LFLAGS on invoking the link command.
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

# Variable ZSQTLIBS
#-------------------------------------------------------------------------------
# List of ZSQtLib libraries to be used.
# This variable must be defined depending on the library to be built
# and must be provided by the corresponding make file of the module.
ZSQTLIBS := $(ZSQTLIBS)


#===============================================================================
# Compiler options
#===============================================================================

ifeq ($(PLATFORM), Win32)
    -include $(MAKEFILE_DIR)/ZSAppTestMakeHelloWorld_CFlags_Win32.mk
else ifeq ($(PLATFORM), Linux)
    -include $(MAKEFILE_DIR)/ZSAppTestMakeHelloWorld_CFlags_Linux.mk
endif

#===============================================================================
# Linker options
#===============================================================================

ifeq ($(PLATFORM), Win32)
    -include $(MAKEFILE_DIR)/ZSAppTestMakeHelloWorld_LFlags_Win32.mk
else ifeq ($(PLATFORM), Linux)
    -include $(MAKEFILE_DIR)/ZSAppTestMakeHelloWorld_LFlags_Linux.mk
endif


#===============================================================================
# Make Targets
#===============================================================================

# Variable: Make File Targets
#-------------------------------------------------------------------------------
# Target files to be created by this make file.
# MAIN_TARGET .. The dynamic link library (or shared object) or executable
#                generated by the linker for this module.
# BIN_DIR_TRG .. The executable generated by the linker for this module must
#                be copied into the binary directory.
# BIN_DIR_PDB .. Only on windows machines:
#                The program debug file of the dynamic link library or executable
#                generated by the linker for this module must also be copied
#                into the binary directory.
ifeq ($(CONFIG), Release)
    ifeq ($(PLATFORM), Win32)
        MAIN_TARGET := $(OUT_DIR)/$(MODULE).exe
        BIN_DIR_TRG := $(BIN_DIR)/$(MODULE).exe
        BIN_DIR_PDB := $(BIN_DIR)/$(MODULE).pdb
    else ifeq ($(PLATFORM), Linux)
        MAIN_TARGET := $(OUT_DIR)/$(MODULE)
        BIN_DIR_TRG := $(BIN_DIR)/$(MODULE)
    endif
else ifeq ($(CONFIG), Debug)
    ifeq ($(PLATFORM), Win32)
        MAIN_TARGET := $(OUT_DIR)/$(MODULE)d.exe
        BIN_DIR_TRG := $(BIN_DIR)/$(MODULE)d.exe
        BIN_DIR_PDB := $(BIN_DIR)/$(MODULE)d.pdb
    else ifeq ($(PLATFORM), Linux)
        MAIN_TARGET := $(OUT_DIR)/$(MODULE)d
        BIN_DIR_TRG := $(BIN_DIR)/$(MODULE)d
    endif
endif

#===============================================================================
# TALKATIVE_MAKEFILE info
#===============================================================================

ifdef TALKATIVE_MAKEFILE
    $(info )
    $(info PLATFORM: .............. $(PLATFORM))
    $(info QTDIR: ................. $(QTDIR))
    $(info QTDIR_INC: ............. $(QTDIR_INC))
    $(info QTDIR_LIB: ............. $(QTDIR_LIB))
    $(info CONFIG_TYPE: ........... $(CONFIG_TYPE))
    $(info CONFIG: ................ $(CONFIG))
    $(info PATH_SEP: .............. $(PATH_SEP))
    $(info OBJ_EXT: ............... $(OBJ_EXT))
    $(info LIB_EXT: ............... $(LIB_EXT))
    $(info DLL_EXT: ............... $(DLL_EXT))
    $(info LIB_PREFIX: ............ $(LIB_PREFIX))
    $(info DLL_PREFIX: ............ $(DLL_PREFIX))
    $(info CXX: ................... $(CXX))
    $(info LNK: ................... $(LNK))
    $(info INC_DIR: ............... $(INC_DIR))
    $(info SRC_DIR: ............... $(SRC_DIR))
    $(info IMG_DIR: ............... $(IMG_DIR))
    $(info BUILD_DIR: ............. $(BUILD_DIR))
    $(info DEP_DIR: ............... $(DEP_DIR))
    $(info INT_DIR: ............... $(INT_DIR))
    $(info MOC_DIR: ............... $(MOC_DIR))
    $(info OUT_DIR: ............... $(OUT_DIR))
    $(info SBK_DIR: ............... $(SBK_DIR))
    $(info LIB_DIR: ............... $(LIB_DIR))
    $(info BIN_DIR: ............... $(BIN_DIR))
    $(info STP_DIR: ............... $(STP_DIR))
    $(info BINDING4PYTHON_STP_DIR:  $(BINDING4PYTHON_STP_DIR))
    $(info )
    $(info CFLAGS: ................ $(CFLAGS))
    $(info INC_DIRS: .............. $(INC_DIRS))
    $(info DEFINES: ............... $(DEFINES))
    $(info DEFINE_OS: ............. $(DEFINE_OS))
    $(info )
    $(info LFLAGS: ................ $(LFLAGS))
    $(info LIB_DIRS: .............. $(LIB_DIRS))
    $(info )
    $(info HDR_ITEMS: ............. $(HDR_ITEMS))
    $(info )
    $(info CPP_ITEMS: ............. $(CPP_ITEMS))
    $(info )
    $(info OBJ_ITEMS: ............. $(OBJ_ITEMS))
    $(info )
    $(info MOC_HDR_ITEMS: ......... $(MOC_HDR_ITEMS))
    $(info )
    $(info MOC_CPP_ITEMS: ......... $(MOC_CPP_ITEMS))
    $(info )
    $(info CPP_DEP_ITEMS: ......... $(CPP_DEP_ITEMS))
    $(info )
    $(info MOC_CPP_DEP_ITEMS: ..... $(MOC_CPP_DEP_ITEMS))
    $(info )
    $(info DEPS: .................. $(DEPS))
    $(info )
    $(info MOC_OBJ_ITEMS: ......... $(MOC_OBJ_ITEMS))
    $(info )
    $(info OBJ_ITEMS: ............. $(OBJ_ITEMS))
    $(info )
    $(info OSLIBS: ................ $(OSLIBS))
    $(info )
    $(info ZSQTLIBS: .............. $(ZSQTLIBS))
    $(info )
    $(info QTLIBS: ................ $(QTLIBS))
    $(info )
    $(info MAIN_TARGET: ........... $(MAIN_TARGET))
    $(info BIN_DIR_TRG: ........... $(BIN_DIR_TRG))
    $(info BIN_DIR_PDB: ........... $(BIN_DIR_PDB))
    $(info )
endif


#===============================================================================
# Make Targets
#===============================================================================

# Make-Target: build
#-------------------------------------------------------------------------------
# Main build target of the make files to generate the dynamic link library of the
# specified module and all necessary intermediate directories and files.
# The main build target again is split into several sub build targets referenced
# as environment variables which have been generated and set before on processing
# this make file as follows:
# SYNCH_CLOCK .. Only used if linux is run as a guist machine in oracle's virtual
#                box to synchronizes the clock time of the linux guest machine
#                with the windows host.
# BIN_DIR ...... Creates the binary output directory for the generated target
#                (ZSQTLIB_DIR/Bin/PLATFORM).
# OUT_DIR ...... Creates the output directory of the C++ compiler.
# DEP_DIR ...... Creates the output directory for intermediate dependency files
#                (AUTO_DEPEND of header files included by source and other header files)
#                of the C++ compiler.
# INT_DIR ...... Creates the intermediate output directory of the C++ compiler.
# MOC_DIR ...... Creates the output directory of Qt's MOC compiler.
# MAIN_TARGET .. Target of this make file. This is the dynamic link library,
#                shared object or executable created by this make file.
# BIN_DIR_TRG .. The target file of this make file has to be copied into the
#                bin directory containing the collection of all libraries and
#                exe files necessary to run an application of the ZS Qt library.
# BIN_DIR_PDB .. The program debug file of this make file has to be copied into the
#                bin directory containing the collection of all libraries and
#                exe files necessary to run an application of the ZS Qt library.
build: $(BIN_DIR) $(LIB_DIR) $(OUT_DIR) $(DEP_DIR) $(INT_DIR) $(MOC_DIR) $(MAIN_TARGET) $(BIN_DIR_TRG) $(BIN_DIR_PDB)

# Make-Target: clean
#-------------------------------------------------------------------------------
# Deletes all files craeated during the build process running this make file.
clean:
	$(CMD_DEL) $(subst /,$(PATH_SEP),$(BIN_DIR_TRG))
	$(CMD_DEL) $(subst /,$(PATH_SEP),$(BIN_DIR_PDB))
	$(CMD_DEL) $(subst /,$(PATH_SEP),$(MAIN_TARGET))
	$(CMD_DEL) $(subst /,$(PATH_SEP),$(DEP_DIR)/)*
	$(CMD_DEL) $(subst /,$(PATH_SEP),$(DEP_DIR)/)*.*
	$(CMD_DEL) $(subst /,$(PATH_SEP),$(INT_DIR)/)*
	$(CMD_DEL) $(subst /,$(PATH_SEP),$(INT_DIR)/)*.*
	$(CMD_DEL) $(subst /,$(PATH_SEP),$(MOC_DIR)/)*
	$(CMD_DEL) $(subst /,$(PATH_SEP),$(MOC_DIR)/)*.*
	$(CMD_DEL) $(subst /,$(PATH_SEP),$(OUT_DIR)/)*
	$(CMD_DEL) $(subst /,$(PATH_SEP),$(OUT_DIR)/)*.*

# Make-Target: help
#-------------------------------------------------------------------------------
# Show help text.
help:
	@echo off
	@echo "Usage: make -f ZSAppTrcMthClient_sub.mk [CONFIG=<Debug|Release>] [target]"
	@echo ""
	@echo "Targets  : build   - (default) Creates the target."
	@echo "           clean   - Deletes all output files."
	@echo "           help    - Shows this help text."
	@echo 
	@echo "Example  : make -f ZSAppTrcMthClient_sub.mk CONFIG=Debug build
	@echo 


#===============================================================================
# Build Targets
#===============================================================================

# Build-Target: SYNCH_CLOCK
#-------------------------------------------------------------------------------
# If you run linux in a virtual box the time synchronization between the host and
# guest machines may lack accuracy (drifting away) ending up in a make file warning
# "Clock skew detected" or in a message saying that the make file has a time stamp
# from the future.
# To solve the time drift "/usr/sbin/VBoxService --timesync-set-start" may be executed.
SYNCH_CLOCK:
	$(CMD_SYNCH_CLOCK)

# Build-Target: $(BIN_DIR)
#-------------------------------------------------------------------------------
# Output Directory Bin/<Platform>
$(BIN_DIR):
	$(CMD_MKDIR) $(subst /,$(PATH_SEP),$(BIN_DIR))

# Build-Target: $(STP_DIR)
#-------------------------------------------------------------------------------
# Output Directory Setup/<Platform>
$(STP_DIR):
	$(CMD_MKDIR) $(subst /,$(PATH_SEP),$(STP_DIR))

# Build-Target: $(DEP_DIR)
#-------------------------------------------------------------------------------
# Temporary Directory Tmp/Dep/<Module>/<Platform>/<Config>
$(DEP_DIR):
	$(CMD_MKDIR) $(subst /,$(PATH_SEP),$(DEP_DIR))

# Build-Target: $(INT_DIR)
#-------------------------------------------------------------------------------
# Temporary Directory Tmp/Int/<Module>/<Platform>/<Config>
$(INT_DIR):
	$(CMD_MKDIR) $(subst /,$(PATH_SEP),$(INT_DIR))

# Build-Target: $(MOC_DIR)
#-------------------------------------------------------------------------------
# Temporary Directory Tmp/Moc/<Module>/<Platform>/<Config>
$(MOC_DIR):
	$(CMD_MKDIR) $(subst /,$(PATH_SEP),$(MOC_DIR))

# Build-Target: $(OUT_DIR)
#-------------------------------------------------------------------------------
# Temporary Directory Tmp/Out/<Module>/<Platform>/<Config>
$(OUT_DIR):
	$(CMD_MKDIR) $(subst /,$(PATH_SEP),$(OUT_DIR))

# Build-Target: $(SBK_DIR)
#-------------------------------------------------------------------------------
# Temporary Directory Tmp/Shiboken/Python<Version>/<Platform>/<Config>/ZSQtLib/<Module>
$(SBK_DIR):
	$(CMD_MKDIR) $(subst /,$(PATH_SEP),$(SBK_DIR))

# Build-Target: $(MAIN_TARGET)
#-------------------------------------------------------------------------------
# The dll (or shared object) or executable file to be created by this make file.
#   MAIN_TARGET := $(OUT_DIR)/$(MODULE)[d].exe
#     OUT_DIR := $(LTTTARGETSERVERAPP_DIR)/Tmp/Out/$(MODULE)/$(PLATFORM)/$(CONFIG)
#     MODULE = LTTTargetServerService|LTTTargetServerDemoGUI
#     DLL_EXT = exe
# Because the argument list for the link tools on linux and windows is different
# it is not possible to use just one target here. Instead for each platform and
# config (debug/release) a separate target must be specified.

$(BUILD_DIR)/Out/Apps/Test/$(MODULE)/Win32/$(CONFIG)/$(MODULE).exe: $(OBJ_ITEMS) $(ZSQTLIBS) $(LTTLIBS)
	$(LNK) $(OBJ_ITEMS) $(LIB_DIRS) $(LFLAGS) $(QTLIBS) $(OSLIBS) $(ZSQTLIBS) $(LTTLIBS)
	mt -manifest "$(MANIFESTFILE)" -outputresource:"$(MAIN_TARGET);#2"

$(BUILD_DIR)/Out/Apps/Test/$(MODULE)/Win32/$(CONFIG)/$(MODULE)d.exe: $(OBJ_ITEMS) $(ZSQTLIBS) $(LTTLIBS)
	$(LNK) $(OBJ_ITEMS) $(LIB_DIRS) $(LFLAGS) $(QTLIBS) $(OSLIBS) $(ZSQTLIBS) $(LTTLIBS)
	mt -manifest "$(MANIFESTFILE)" -outputresource:"$(MAIN_TARGET);#2"

$(BUILD_DIR)/Out/Apps/Test/$(MODULE)/Linux/$(CONFIG)/$(MODULE): $(OBJ_ITEMS) $(ZSQTLIBS) $(LTTLIBS)
	$(LNK) $(OBJ_ITEMS) $(LIB_DIRS) $(LFLAGS) $(QTLIBS) $(OSLIBS) $(ZSQTLIBS) $(LTTLIBS) -o $(MAIN_TARGET)

$(BUILD_DIR)/Out/Apps/Test/$(MODULE)/Linux/$(CONFIG)/$(MODULE)d: $(OBJ_ITEMS) $(ZSQTLIBS) $(LTTLIBS)
	$(LNK) $(OBJ_ITEMS) $(LIB_DIRS) $(LFLAGS) $(QTLIBS) $(OSLIBS) $(ZSQTLIBS) $(LTTLIBS) -o $(MAIN_TARGET)

# Build-Target: $(BIN_DIR_TRG)
#-------------------------------------------------------------------------------
# File: Bin/<Platform>/<Module>[d].exe
$(BIN_DIR_TRG): $(MAIN_TARGET)
	$(CMD_COPY) $(subst /,$(PATH_SEP),$(MAIN_TARGET)) $(subst /,$(PATH_SEP),$(BIN_DIR_TRG))

# Build-Target: $(BIN_DIR_PDB)
#-------------------------------------------------------------------------------

# File: Bin/<Platform>/<Module>[d].pdb (Release Version)
$(BIN_DIR)/$(MODULE).pdb: $(OUT_DIR)/$(MODULE).pdb
	$(CMD_COPY) $(subst /,$(PATH_SEP),$(OUT_DIR)/$(MODULE).pdb) $(subst /,$(PATH_SEP),$(BIN_DIR_PDB))

# File: Bin/<Platform>/<Module>d.pdb (Debug Version)
$(BIN_DIR)/$(MODULE)d.pdb: $(OUT_DIR)/$(MODULE)d.pdb
	$(CMD_COPY) $(subst /,$(PATH_SEP),$(OUT_DIR)/$(MODULE)d.pdb) $(subst /,$(PATH_SEP),$(BIN_DIR_PDB))


#===============================================================================
# Sub-Build Targets
#===============================================================================

# Sub-Build-Target: Tmp/Moc/<Module>/<Platform>/<Config>/moc_<Module>.cpp
#-------------------------------------------------------------------------------
# The moc compiler of Qt takes the header files with Q_OBJECT macros as input
# and generated "moc_<>.cpp" files from those header files.

$(MOC_DIR)/moc_%.cpp: $(INC_DIR)/%.h
	moc $< -o $@

# Mark the moc source files precious to make, so they won’t be automatically
# deleted as intermediate files.
.PRECIOUS: $(MOC_DIR)/moc_%.cpp

# Sub-Build-Target: <*.o|obj>
#-------------------------------------------------------------------------------
# The object files to be created by this make file 
# INT_DIR := $(LTTTARGETSERVERAPP_DIR)/Tmp/Int/$(MODULE)/$(PLATFORM)/$(CONFIG)
# Because the argument list for the compiler tools on linux and windows is different
# it is not possible to use just one target here. Instead for each platform
# a separate target must be specified.
# We use the GCC compiler to create a list of dependencies to included
# header files as only the GCC compiler but not the Visual C compiler
# supports this feature by using the preprocessor options -MM, -MT and -MF.
#   -E
#      Run only the C preprocessor. Preprocess all the C source files specified
#      and output the results to standard output or to the specified output file.
#   -M
#      Instead of outputting the result of preprocessing, output a rule
#      suitable for make describing the dependencies of the main source file.
#      The preprocessor outputs one make rule containing the object file name
#      for that source file, a colon, and the names of all the included files.
#      Unless overridden explicitly, the object file name consists of the
#      basename of the source file with any suffix replaced with object file
#      suffix. If there are many included files then the rule is split into
#      several lines using '\'-newline.
#      '-M' implies '-E'.
#   -MM
#      Like '-M', but mention only the files included with '#include "file"'.
#      System header files included with `#include <file>' are omitted.
#   -MD
#      Like '-M' but the dependency information is written to a file rather
#      than stdout. gcc will use the same file name and directory as the object
#      file, but with the suffix '.d' instead.
#      This is in addition to compiling the main file as specified.
#      '-MD' does not inhibit ordinary compilation the way '-M' does, unless
#      you also specify '-MG'.
#      With Mach, you can use the utility md to merge multiple dependency files
#      into a single dependency file suitable for using with the 'make' command.
#   -MMD:
#      Like `-MD' except mention only user header files, not system header files.
#   -MT $@:
#      Set the name of the target in the generated dependency file.
#      Generate dependency information as a side-effect of compilation, not instead
#      of compilation. This version omits system headers from the generated dependencies.
#      If you prefer to preserve system headers as prerequisites, use -MD.
#   -MP:
#      Adds a target for each prerequisite in the list, to avoid errors when deleting files.
#   -MF $(DEP_DIR)/$*.Td
#      Write the generated dependency file to a temporary location $(DEP_DIR)/$*.Td.
# After the source files have been compiled into object files the generated temporary
# dependency files will be renamed to the real dependency files. We do this in a separate
# step so that failures during the compilation won’t leave a corrupted dependency file.

$(BUILD_DIR)/Int/Apps/Test/$(MODULE)/Win32/$(CONFIG)/%.$(OBJ_EXT): $(SRC_DIR)/%.cpp $(DEP_DIR)/%.d
	gcc $(subst -Include,/Include,$(subst /I,-I,$(INC_DIRS))) -D$(DEFINE_OS) $(subst /D ,-D,$(DEFINES)) -MM -MT $@ -MP -MF $(DEP_DIR)/$*.Td $<
	$(CXX) -c $(INC_DIRS) $(DEFINES) $(CFLAGS) $<
	$(CMD_RENAME) $(DEP_DIR)/$*.Td $(DEP_DIR)/$*.d

$(BUILD_DIR)/Int/Apps/Test/$(MODULE)/Linux/$(CONFIG)/%.$(OBJ_EXT): $(SRC_DIR)/%.cpp $(DEP_DIR)/%.d
	gcc $(subst -Include,/Include,$(subst /I,-I,$(INC_DIRS))) -D$(DEFINE_OS) $(DEFINES) -MM -MT $@ -MP -MF $(DEP_DIR)/$*.Td $<
	$(CXX) $(CFLAGS) $(DEFINES) $(INC_DIRS) -o $(INT_DIR)/$*.$(OBJ_EXT) $<
	$(CMD_RENAME) $(DEP_DIR)/$*.Td $(DEP_DIR)/$*.d

# Sub-Build-Target: <moc_*.obj>
#-------------------------------------------------------------------------------
# See object files above.

$(BUILD_DIR)/Int/Apps/Test/$(MODULE)/Win32/$(CONFIG)/moc_%.$(OBJ_EXT): $(MOC_DIR)/moc_%.cpp #$(DEP_DIR)/moc_%.d
	gcc $(subst -Include,/Include,$(subst /I,-I,$(INC_DIRS))) -D$(DEFINE_OS) $(DEFINES) -MM -MT $@ -MP -MF $(DEP_DIR)/moc_$*.Td $<
	$(CXX) -c $(INC_DIRS) $(DEFINES) $(CFLAGS) $<
	$(CMD_RENAME) $(DEP_DIR)/moc_$*.Td $(DEP_DIR)/moc_$*.d

$(BUILD_DIR)/Int/Apps/Test/$(MODULE)/Linux/$(CONFIG)/moc_%.$(OBJ_EXT): $(MOC_DIR)/moc_%.cpp #$(DEP_DIR)/moc_%.d
	gcc $(subst -Include,/Include,$(subst /I,-I,$(INC_DIRS))) -D$(DEFINE_OS) $(DEFINES) -MM -MT $@ -MP -MF $(DEP_DIR)/moc_$*.Td $<
	$(CXX) $(CFLAGS) $(DEFINES) $(INC_DIRS) -o $(INT_DIR)/moc_$*.$(OBJ_EXT) $<
	$(CMD_RENAME) $(DEP_DIR)/moc_$*.Td $(DEP_DIR)/moc_$*.d

# Sub-Build-Target: <*.d>
#-------------------------------------------------------------------------------
# Auto generated dependency files.
# The dependency files are generated by executing the first command line of the
# recipe to create the object files. Here we specify the *.d target with an empty
# recipe so that make doesn’t fail if the dependency file doesn’t exist yet.
$(DEP_DIR)/%.d: ;

# Auto Dependency to header files included by source files
#-------------------------------------------------------------------------------
# Include the auto generated dependency files.

ZSQTLIB_APPS_TEST_OBJPOOLMODEL_MAKEFILE_PATH_TMP := $(MAKEFILE_PATH)
ZSQTLIB_APPS_TEST_OBJPOOLMODEL_MAKEFILE_DIR_TMP := $(MAKEFILE_DIR)
-include $(DEPS)
MAKEFILE_PATH := $(ZSQTLIB_APPS_TEST_OBJPOOLMODEL_MAKEFILE_PATH_TMP)
MAKEFILE_DIR := $(ZSQTLIB_APPS_TEST_OBJPOOLMODEL_MAKEFILE_DIR_TMP)


$(info )
$(info <--- $(MAKEFILE_PATH) )
