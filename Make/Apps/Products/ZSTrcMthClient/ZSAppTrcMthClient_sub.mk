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

$(info )
$(info -> Processing $(MAKEFILE_PATH))
$(info -----------------------------------------------------------------------------)

MODULE := ZSAppTrcMthClient
PROJ := $(MODULE)
CONFIG_TYPE := exe

#===============================================================================
# Environment Variables as Preconditions
#===============================================================================

ifndef ZSQTLIB_DIR
$(error ZSQTLIB_DIR is not defined)
endif

ifeq ($(wildcard $(ZSQTLIB_DIR)),)
    $(error $(ZSQTLIB_DIR) does not exist!)
endif

include $(ZSQTLIB_DIR)/Make/ZSQtLib_EnvVars.mk

PROJ_DIR := $(subst /Make,,$(MAKEFILE_DIR))

# Overwrite Output Directories of Lib Make Files
INC_DIR := $(PROJ_DIR)/Include
SRC_DIR := $(PROJ_DIR)/Source
IMG_DIR := $(PROJ_DIR)/Images
DEP_DIR := $(ZSQTLIB_DIR)/Tmp/Dep/Apps/Products/$(PROJ)/$(PLATFORM)/$(CONFIG)
INT_DIR := $(ZSQTLIB_DIR)/Tmp/Int/Apps/Products/$(PROJ)/$(PLATFORM)/$(CONFIG)
OUT_DIR := $(ZSQTLIB_DIR)/Tmp/Out/Apps/Products/$(PROJ)/$(PLATFORM)/$(CONFIG)
MOC_DIR := $(ZSQTLIB_DIR)/Tmp/Moc/Apps/Products/$(PROJ)/$(PLATFORM)/$(CONFIG)

#===============================================================================
# Compiler and Linker Flags
#===============================================================================

include $(ZSQTLIB_DIR)/Make/ZSQtLib_CFlags.mk
include $(ZSQTLIB_DIR)/Make/ZSQtLib_LFlags.mk

INCLUDES := $(INCLUDES) /I "$(PROJ_DIR)/Include"

ifeq ($(CONFIG), Release)
QTLIBS := $(QTLIBS) QtGui4.lib
ZSQTLIBS := ZSSys.lib ZSSysGUI.lib ZSIpc.lib ZSIpcGUI.lib ZSTrace.lib ZSTraceGUI.lib
MAIN_TARGET := $(OUT_DIR)/$(MODULE).$(CONFIG_TYPE)
BIN_DIR_TRG := $(subst /,\,$(BIN_DIR))\$(MODULE).$(CONFIG_TYPE)
BIN_DIR_PDB := $(subst /,\,$(BIN_DIR))\$(MODULE).pdb
else ifeq ($(CONFIG), Debug)
QTLIBS := $(QTLIBS) QtGuid4.lib
ZSQTLIBS := ZSSysd.lib ZSSysGUId.lib ZSIpcd.lib ZSIpcGUId.lib ZSTraced.lib ZSTraceGUId.lib
MAIN_TARGET := $(OUT_DIR)/$(MODULE)d.$(CONFIG_TYPE)
BIN_DIR_TRG := $(subst /,\,$(BIN_DIR))\$(MODULE)d.$(CONFIG_TYPE)
BIN_DIR_PDB := $(subst /,\,$(BIN_DIR))\$(MODULE)d.pdb
endif


#===============================================================================
# Variables
#===============================================================================

CXX = cl

SRC_ITEMS := $(wildcard $(SRC_DIR)/*.cpp)
HDR_ITEMS := $(wildcard $(INC_DIR)/*.h)
# Function file did not work as expected on all machines:
# MOC_ITEMS := $(foreach hdr_item,$(HDR_ITEMS),$(if $(findstring Q_OBJECT,$(file < $(hdr_item))), $(hdr_item)))
# Using shell grep instead to detect MOC_ITEMS:
MOC_ITEMS := $(shell grep -l Q_OBJECT $(HDR_ITEMS))

OBJECTS := $(patsubst $(SRC_DIR)/%.cpp,$(INT_DIR)/%.obj,$(SRC_ITEMS))
MOC_CPPS := $(patsubst $(INC_DIR)/%.h,$(MOC_DIR)/moc_%.cpp,$(MOC_ITEMS))
MOC_OBJECTS := $(patsubst $(INC_DIR)/%.h,$(INT_DIR)/moc_%.obj,$(MOC_ITEMS))

#===============================================================================
# Make Targets
#===============================================================================

build: $(BIN_DIR) $(LIB_DIR) $(OUT_DIR) $(DEP_DIR) $(INT_DIR) $(MOC_DIR) $(MAIN_TARGET) $(BIN_DIR_TRG) $(BIN_DIR_PDB)

clean:
	del $(subst /,\,$(BIN_DIR_TRG))
	del $(subst /,\,$(BIN_DIR_PDB))
	del $(subst /,\,$(MAIN_TARGET))
	del $(subst /,\,$(DEP_DIR))\*.* /Q
	del $(subst /,\,$(INT_DIR))\*.* /Q
	del $(subst /,\,$(MOC_DIR))\*.* /Q
	del $(subst /,\,$(OUT_DIR))\*.* /Q

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
# Main Target
#===============================================================================

$(MAIN_TARGET): $(OBJECTS) $(MOC_OBJECTS)
	link $(LFLAGS) $(OBJECTS) $(MOC_OBJECTS) $(WINLIBS) $(QTLIBS) $(ZSQTLIBS)
	mt -manifest "$(MANIFESTFILE)" -outputresource:"$(MAIN_TARGET);1"

#===============================================================================
# Sub Targets
#===============================================================================

# Object Files
#-------------------------------------------------------------------------------

# We use the GCC compiler to create a list of dependencies to included
# header files using the flags -MM, -MT and -MF (see ZSQtLib_DepFlags.mk)
# as those flags are only supported by GCC but not by the visual studio compiler.
#   -MT $@:
#      Set the name of the target in the generated dependency file.
#   -MMD:
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

$(INT_DIR)/%.obj: $(SRC_DIR)/%.cpp $(DEP_DIR)/%.d
	gcc $(subst -Include,/Include,$(subst /I,-I,$(INCLUDES))) -D_WINDOWS -MM -MT $@ -MP -MF $(DEP_DIR)/$*.Td $<
	$(CXX) $(INCLUDES) $(DEFINES) $(CFLAGS) $<
	mv -f $(DEP_DIR)/$*.Td $(DEP_DIR)/$*.d

# MOC Object Files
#-------------------------------------------------------------------------------

$(INT_DIR)/moc_%.obj: $(MOC_DIR)/moc_%.cpp
	gcc $(subst -Include,/Include,$(subst /I,-I,$(INCLUDES))) -D_WINDOWS -MM -MT $@ -MP -MF $(DEP_DIR)/moc_$*.Td $<
	$(CXX) $(INCLUDES) $(DEFINES) $(CFLAGS) $<
	mv -f $(DEP_DIR)/moc_$*.Td $(DEP_DIR)/moc_$*.d

# MOC Source Files
#-------------------------------------------------------------------------------

$(MOC_DIR)/moc_%.cpp: $(INC_DIR)/%.h
	moc $< -o $@

# DEPendency Files
#-------------------------------------------------------------------------------

# Create a pattern rule with an empty recipe, so that make won’t fail if the
# dependency file doesn’t exist.
$(DEP_DIR)/%.d: ;

# Mark the dependency files precious to make, so they won’t be automatically
# deleted as intermediate files.
.PRECIOUS: $(DEP_DIR)/%.d

# Output Directories
#-------------------------------------------------------------------------------

$(DEP_DIR):
	mkdir $(subst /,\,$(DEP_DIR))

$(INT_DIR):
	mkdir $(subst /,\,$(INT_DIR))

$(OUT_DIR):
	mkdir $(subst /,\,$(OUT_DIR))

$(MOC_DIR):
	mkdir $(subst /,\,$(MOC_DIR))

$(BIN_DIR):
	mkdir $(subst /,\,$(BIN_DIR))

$(subst /,\,$(BIN_DIR))\$(MODULE).exe: $(subst /,\,$(OUT_DIR))\$(MODULE).exe
	copy $(subst /,\,$(OUT_DIR))\$(MODULE).exe $(subst /,\,$(BIN_DIR))\$(MODULE).exe

$(subst /,\,$(BIN_DIR))\$(MODULE).pdb: $(subst /,\,$(OUT_DIR))\$(MODULE).pdb
	copy $(subst /,\,$(OUT_DIR))\$(MODULE).pdb $(subst /,\,$(BIN_DIR))\$(MODULE).pdb

$(subst /,\,$(BIN_DIR))\$(MODULE)d.exe: $(subst /,\,$(OUT_DIR))\$(MODULE)d.exe
	copy $(subst /,\,$(OUT_DIR))\$(MODULE)d.exe $(subst /,\,$(BIN_DIR))\$(MODULE)d.exe

$(subst /,\,$(BIN_DIR))\$(MODULE)d.pdb: $(subst /,\,$(OUT_DIR))\$(MODULE)d.pdb
	copy $(subst /,\,$(OUT_DIR))\$(MODULE)d.pdb $(subst /,\,$(BIN_DIR))\$(MODULE)d.pdb

#===============================================================================
# Auto Dependency to header files included by source files
#===============================================================================

# Include the dependency files that exist: translate each file listed in SRC_ITEMS
# into its dependency file. Use -include to avoid failing on non-existent files.
SRC_DEP_ITEMS := $(subst /,,$(subst $(SRC_DIR),,$(basename $(SRC_ITEMS)))))
MOC_DEP_ITEMS := $(subst /,,$(subst $(MOC_DIR),,$(basename $(MOC_CPPS)))))
DEPS := $(patsubst %,$(DEP_DIR)/%.d,$(SRC_DEP_ITEMS))
DEPS := $(DEPS) $(patsubst %,$(DEP_DIR)/%.d,$(MOC_DEP_ITEMS))
-include $(DEPS)
