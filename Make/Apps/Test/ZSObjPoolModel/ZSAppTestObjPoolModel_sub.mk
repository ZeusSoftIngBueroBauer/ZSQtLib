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
else
MAKEFILE_DIR := $(shell dirname $(MAKEFILE_PATH))
ZSQTLIB_DIR := $(subst /Apps/Test/ZSObjPoolModel/Make/$(MODULE),,$(MAKEFILE_DIR))
ZSQTLIB_DIR := $(subst PATH_SEP_TMP,/,$(ZSQTLIB_DIR))
$(info ZSQTLIB_DIR detected as: $(ZSQTLIB_DIR))
endif
ifeq ($(wildcard $(ZSQTLIB_DIR)),)
$(error $(ZSQTLIB_DIR) does not exist!)
endif
export ZSQTLIB_DIR


#===============================================================================
# Commonly used environment variables
#===============================================================================

ZSQTLIB_APPS_TEST_OBJPOOLMODEL_MAKEFILE_PATH_TMP := $(MAKEFILE_PATH)
ZSQTLIB_APPS_TEST_OBJPOOLMODEL_MAKEFILE_DIR_TMP := $(MAKEFILE_DIR)
-include $(ZSQTLIB_DIR)/Make/ZSQtLib_EnvVars.mk
MAKEFILE_PATH := $(ZSQTLIB_APPS_TEST_OBJPOOLMODEL_MAKEFILE_PATH_TMP)
MAKEFILE_DIR := $(ZSQTLIB_APPS_TEST_OBJPOOLMODEL_MAKEFILE_DIR_TMP)


#===============================================================================
# Aliases for commands
#===============================================================================

# The name of tools to be used may differ depending on the operating system
# or on other installed tools.

ZSQTLIB_APPS_TEST_OBJPOOLMODEL_MAKEFILE_PATH_TMP := $(MAKEFILE_PATH)
ZSQTLIB_APPS_TEST_OBJPOOLMODEL_MAKEFILE_DIR_TMP := $(MAKEFILE_DIR)
-include $(ZSQTLIB_DIR)/Make/ZSQtLib_ShellCmds.mk
MAKEFILE_PATH := $(ZSQTLIB_APPS_TEST_OBJPOOLMODEL_MAKEFILE_PATH_TMP)
MAKEFILE_DIR := $(ZSQTLIB_APPS_TEST_OBJPOOLMODEL_MAKEFILE_DIR_TMP)


#===============================================================================
# Directory paths for input files
#===============================================================================

# Those directory paths are relative to the base directory of this make file.

# Path: INC_DIR
#-------------------------------------------------------------------------------
# Directory path of the header (*.h) files.
INC_DIR := $(ZSQTLIB_DIR)/Apps/Test/ZSObjPoolModel/Include
ifdef TALKATIVE_MAKEFILE
$(info INC_DIR: $(INC_DIR))
endif
export INC_DIR

# Path: SRC_DIR
#-------------------------------------------------------------------------------
# Directory path of the source (*.cpp) files.
SRC_DIR := $(ZSQTLIB_DIR)/Apps/Test/ZSObjPoolModel/Source
ifdef TALKATIVE_MAKEFILE
$(info SRC_DIR: $(SRC_DIR))
endif
export SRC_DIR

# Path: IMG_DIR
#-------------------------------------------------------------------------------
# Directory path of the image (*.bmp, *.png, *.ico, ..) files.
IMG_DIR := $(ZSQTLIB_DIR)/Apps/Test/ZSObjPoolModel/Images
IMG_DIR := $(subst GUI,,$(IMG_DIR))
ifdef TALKATIVE_MAKEFILE
$(info IMG_DIR: $(IMG_DIR))
endif
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
$(info BUILD_DIR defined as $(BUILD_DIR))
else
ifeq ($(PLATFORM), Win32)
BUILD_DIR := $(ZSQTLIB_DIR)/Tmp
else ifeq ($(PLATFORM), Linux)
BUILD_DIR := /tmp/ZeusSoft/ZSQtLib
endif
$(info BUILD_DIR set to $(BUILD_DIR))
export BUILD_DIR
endif
ifdef TALKATIVE_MAKEFILE
$(info BUILD_DIR: $(BUILD_DIR))
endif

# Path: DEP_DIR
#-------------------------------------------------------------------------------
# Directory path where the dependency files (*.d) while be written to.
# Those dependency files are used to reproduce the AUTO_DEPEND function
# for header files included by CPP source files.
DEP_DIR := $(BUILD_DIR)/Dep/Apps/Test/$(MODULE)/$(PLATFORM)/$(CONFIG)
ifdef TALKATIVE_MAKEFILE
$(info DEP_DIR: $(DEP_DIR))
endif
export DEP_DIR

# Path: INT_DIR
#-------------------------------------------------------------------------------
# Directory path where the compiler generated intermediate files
# (*.obj, ..) while be written to.
INT_DIR := $(BUILD_DIR)/Int/Apps/Test/$(MODULE)/$(PLATFORM)/$(CONFIG)
ifdef TALKATIVE_MAKEFILE
$(info INT_DIR: $(INT_DIR))
endif
export INT_DIR

# Path: MOC_DIR
#-------------------------------------------------------------------------------
# Directory path where the "*.cpp" output files of the Qt MOC compiler
# will be written to.
MOC_DIR := $(BUILD_DIR)/Moc/Apps/Test/$(MODULE)/$(PLATFORM)/$(CONFIG)
ifdef TALKATIVE_MAKEFILE
$(info MOC_DIR: $(MOC_DIR))
endif
export MOC_DIR

# Path: OUT_DIR
#-------------------------------------------------------------------------------
# Directory path where the target output files of the compiler and linker
# (*.dll, *.exe, ..) while be written to.
OUT_DIR := $(BUILD_DIR)/Out/Apps/Test/$(MODULE)/$(PLATFORM)/$(CONFIG)
ifdef TALKATIVE_MAKEFILE
$(info OUT_DIR: $(OUT_DIR))
endif
export OUT_DIR

# Path: SBK_DIR
#-------------------------------------------------------------------------------
# Directory path where the output files of the Shiboken generator
# will be written to. The Shiboken generator is used to create site
# packages for the python interpreter.
SBK_DIR := $(BUILD_DIR)/Shiboken/Python$(PYTHONVERSION)/$(PLATFORM)/$(CONFIG)/ZSQtLib/Apps/Test
ifdef TALKATIVE_MAKEFILE
$(info SBK_DIR: $(SBK_DIR))
endif
export SBK_DIR


#===============================================================================
# Directory paths for output files
#===============================================================================

# Those directory paths are relative to the base directory of this make file.

ZSQTLIB_APPS_TEST_OBJPOOLMODEL_MAKEFILE_PATH_TMP := $(MAKEFILE_PATH)
ZSQTLIB_APPS_TEST_OBJPOOLMODEL_MAKEFILE_DIR_TMP := $(MAKEFILE_DIR)
-include $(ZSQTLIB_DIR)/Make/ZSQtLib_OutDirs.mk
MAKEFILE_PATH := $(ZSQTLIB_APPS_TEST_OBJPOOLMODEL_MAKEFILE_PATH_TMP)
MAKEFILE_DIR := $(ZSQTLIB_APPS_TEST_OBJPOOLMODEL_MAKEFILE_DIR_TMP)


#===============================================================================
# Input Files
#===============================================================================

ZSQTLIB_APPS_TEST_OBJPOOLMODEL_MAKEFILE_PATH_TMP := $(MAKEFILE_PATH)
ZSQTLIB_APPS_TEST_OBJPOOLMODEL_MAKEFILE_DIR_TMP := $(MAKEFILE_DIR)
-include $(ZSQTLIB_DIR)/Make/ZSQtLib_InputFiles.mk
MAKEFILE_PATH := $(ZSQTLIB_APPS_TEST_OBJPOOLMODEL_MAKEFILE_DIR_TMP)
MAKEFILE_DIR := $(ZSQTLIB_APPS_TEST_OBJPOOLMODEL_MAKEFILE_PATH_TMP)

# Variable ZSQTLIBS
#-------------------------------------------------------------------------------
# List of ZSQtLib libraries to be used.
# This variable must be defined depending on the library to be built
# and must be provided by the corresponding make file of the module.
ZSQTLIBS := 
export ZSQTLIBS


#===============================================================================
# Compiler and linker options
#===============================================================================

# Variable CFLAGS, INC_DIRS, DEFINES, DEFINE_OS
#-------------------------------------------------------------------------------
# Flags to be used by the C++ compiler.
ZSQTLIB_APPS_TEST_OBJPOOLMODEL_MAKEFILE_PATH_TMP := $(MAKEFILE_PATH)
ZSQTLIB_APPS_TEST_OBJPOOLMODEL_MAKEFILE_DIR_TMP := $(MAKEFILE_DIR)
-include $(ZSQTLIB_DIR)/Make/ZSQtLib_CFlags.mk
MAKEFILE_PATH := $(ZSQTLIB_APPS_TEST_OBJPOOLMODEL_MAKEFILE_PATH_TMP)
MAKEFILE_DIR := $(ZSQTLIB_APPS_TEST_OBJPOOLMODEL_MAKEFILE_DIR_TMP)

# Variable INC_DIRS
#-------------------------------------------------------------------------------
# List of additional include paths of header files to be used by the C++ compiler.
ifeq ($(PLATFORM), Win32)
	INC_DIRS := $(INC_DIRS) -I "../Include"
else ifeq ($(PLATFORM), Linux)
	INC_DIRS := $(INC_DIRS) -I "../Include"
endif

# Variable LFLAGS, LIB_DIRS
#-------------------------------------------------------------------------------
# Flags to be used by the linker.
ZSQTLIB_APPS_TEST_OBJPOOLMODEL_MAKEFILE_PATH_TMP := $(MAKEFILE_PATH)
ZSQTLIB_APPS_TEST_OBJPOOLMODEL_MAKEFILE_DIR_TMP := $(MAKEFILE_DIR)
-include $(ZSQTLIB_DIR)/Make/ZSQtLib_LFlags.mk
MAKEFILE_PATH := $(ZSQTLIB_APPS_TEST_OBJPOOLMODEL_MAKEFILE_PATH_TMP)
MAKEFILE_DIR := $(ZSQTLIB_APPS_TEST_OBJPOOLMODEL_MAKEFILE_DIR_TMP)


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
ifdef TALKATIVE_MAKEFILE
$(info )
$(info MAIN_TARGET: $(MAIN_TARGET))
$(info BIN_DIR_TRG: $(BIN_DIR_TRG))
$(info BIN_DIR_PDB: $(BIN_DIR_PDB))
$(info )
endif

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
	del $(subst /,\,$(BIN_DIR_TRG))
	del $(subst /,\,$(BIN_DIR_PDB))
	del $(subst /,\,$(MAIN_TARGET))
	del $(subst /,\,$(DEP_DIR))\*.* /Q
	del $(subst /,\,$(INT_DIR))\*.* /Q
	del $(subst /,\,$(MOC_DIR))\*.* /Q
	del $(subst /,\,$(OUT_DIR))\*.* /Q

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

$(MOC_DIR)/moc_%.cpp: $(INC_DIR)/Common/%.h
	moc $< -o $@

$(MOC_DIR)/moc_%.cpp: $(INC_DIR)/$(MODULE_SUBDIR_NAME)/%.h
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

$(BUILD_DIR)/Int/Apps/Test/$(MODULE)/Win32/$(CONFIG)/%.$(OBJ_EXT): $(SRC_DIR)/Common/%.cpp $(DEP_DIR)/%.d
	gcc $(subst -Include,/Include,$(subst /I,-I,$(INC_DIRS))) -D$(DEFINE_OS) $(DEFINES) -MM -MT $@ -MP -MF $(DEP_DIR)/$*.Td $<
	$(CXX) -c $(INC_DIRS) $(DEFINES) $(CFLAGS) $<
	$(CMD_RENAME) $(DEP_DIR)/$*.Td $(DEP_DIR)/$*.d

$(BUILD_DIR)/Int/Apps/Test/$(MODULE)/Linux/$(CONFIG)/%.$(OBJ_EXT): $(SRC_DIR)/Common/%.cpp $(DEP_DIR)/%.d
	gcc $(subst -Include,/Include,$(subst /I,-I,$(INC_DIRS))) -D$(DEFINE_OS) $(DEFINES) -MM -MT $@ -MP -MF $(DEP_DIR)/$*.Td $<
	$(CXX) -c $(INC_DIRS) $(DEFINES) -o $(INT_DIR)/$*.$(OBJ_EXT) $(CFLAGS) $<
	$(CMD_RENAME) $(DEP_DIR)/$*.Td $(DEP_DIR)/$*.d

$(BUILD_DIR)/Int/Apps/Test/$(MODULE)/Linux/$(CONFIG)/%.$(OBJ_EXT): $(SRC_DIR)/$(MODULE_SUBDIR_NAME)/%.cpp $(DEP_DIR)/%.d
	gcc $(subst -Include,/Include,$(subst /I,-I,$(INC_DIRS))) -D$(DEFINE_OS) $(DEFINES) -MM -MT $@ -MP -MF $(DEP_DIR)/$*.Td $<
	$(CXX) -c $(INC_DIRS) $(DEFINES) -o $(INT_DIR)/$*.$(OBJ_EXT) $(CFLAGS) $<
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
	$(CXX) -c $(INC_DIRS) $(DEFINES) -o $(INT_DIR)/moc_$*.$(OBJ_EXT) $(CFLAGS) $<
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
