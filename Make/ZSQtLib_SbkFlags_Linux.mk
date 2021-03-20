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
# Shiboken commad line options
#===============================================================================

# Exclusive command line options.
#================================

# --help
#---------------------------------
# To display help and exit.

# --documentation-only
# Do not generate any code, just the documentation.

# --version
# Output version information and exit.

# Options
#================================

SBK_OPTIONS :=

# avoid-protected-hack
#---------------------------------
# Protected methods or members in a C++ class behave as public for classes inheriting
# from it, and as private for everyone else. Since Python doesn’t have protected or
# private members, in order to Python classes inheriting from wrapped classes to have
# access to protected members, the latter must be turned public.
# When the generated binding is compiled, Shiboken uses this protected-public hack,
# which is simply a line "#define protected public" in every header where a C++ wrapper
# is declared. This causes the compiler to treat protected declarations as public
# declarations. That’s may seem strange, but save a lot of space. Otherwise one has to
# make C++ wrappers with public accessor methods for all classes containing protected
# members, even if they don’t have virtual methods.
SBK_OPTIONS := $(SBK_OPTIONS) --avoid-protected-hack

# disable-verbose-error-messages
#---------------------------------
# Disable verbose error messages. Turn the CPython code hard to debug but saves a
# few kilobytes in the generated binding.
#SBK_OPTIONS := $(SBK_OPTIONS) --disable-verbose-error-messages

# enable-parent-ctor-heuristic
# This flag enable an useful heuristic which can save a lot of work related to object ownership when writing the typesystem. For more info, check Parentship heuristics.
#SBK_OPTIONS := $(SBK_OPTIONS) --enable-parent-ctor-heuristic

# enable-pyside-extensions
# Enable pyside extensions like support for signal/slots. Use this if you are
# creating a binding based on PySide.
SBK_OPTIONS := $(SBK_OPTIONS) --enable-pyside-extensions

# enable-return-value-heuristic
# Enable heuristics to detect parent relationship on return values. For more info,
# check Return value heuristics.
#SBK_OPTIONS := $(SBK_OPTIONS) --enable-return-value-heuristic

# api-version=<version>
# Specify the supported api version of Qt used to generate the bindings.
SBK_OPTIONS := $(SBK_OPTIONS) --api-version=4.8.7

# debug-level=[sparse|medium|full]
# Set the debug level.
#SBK_OPTIONS := $(SBK_OPTIONS) --debug-level=sparse

# drop-type-entries="<TypeEntry0>[;TypeEntry1;...]"
# Semicolon separated list of type system entries (classes, namespaces,
# global functions and enums) to be dropped from generation.
#SBK_OPTIONS := $(SBK_OPTIONS) --drop-type-entries="<TypeEntry0>[;TypeEntry1;...]"

# generation-set
# Generator set to be used (e.g. qtdoc).
#SBK_OPTIONS := $(SBK_OPTIONS) --generation-set

# license-file=[license-file]
# File used for copyright headers of generated files.
#SBK_OPTIONS := $(SBK_OPTIONS) --license-file=[license-file]

# no-suppress-warnings
# Show all warnings.
#SBK_OPTIONS := $(SBK_OPTIONS) --no-suppress-warnings

# silent
# Avoid printing any message.
#SBK_OPTIONS := $(SBK_OPTIONS) --silent

# output-directory=[dir]
# The directory where the generated files will be written.
SBK_OUT_DIR := $(ZSQTLIB_DIR_TMP)/Shiboken/Python$(PYTHONVERSION)/$(PLATFORM)/$(CONFIG)
SBK_OUT_DIR := $(subst /,$(PATH_SEP),$(SBK_OUT_DIR))
SBK_OPTIONS := $(SBK_OPTIONS) --output-directory=$(SBK_OUT_DIR)

# Search Paths for Header Files
#==============================

# include-paths=<path>[:<path>:...]
# Semicolon separated list of include paths used by the C++ parser.
#SBK_FLAGS := $(SBK_FLAGS) --include-paths==<path>[;<path>;...]
SBK_INCPATH := /usr/include/PySide
SBK_INCPATH := $(SBK_INCPATH):$(QTDIR_INC)
SBK_INCPATH := $(SBK_INCPATH):$(ZSQTLIB_DIR)/Include
SBK_INCPATH := $(SBK_INCPATH):$(ZSQTLIB_DIR)/Make
SBK_INCPATH := $(SBK_INCPATH):$(ZSQTLIB_DIR)/Make/$(MODULE)
SBK_INCPATH := $(subst /,$(PATH_SEP),$(SBK_INCPATH))

# Search Paths for Typesystem Files
#==================================

# typesystem-paths=<path>[:<path>:...]
# Semicolon separated list of paths used when searching for type system files.
#SBKFLAGS := $(SBKFLAGS) --typesystem-paths=<path>[;<path>;...]
SBK_TYPESYSPATH := /usr/share/PySide/typesystems
SBK_TYPESYSPATH := $(SBK_TYPESYSPATH):$(ZSQTLIB_DIR)/Make/ZSSys
#SBK_TYPESYSPATH := $(SBK_TYPESYSPATH):$(ZSQTLIB_DIR)/Make/ZSSysGUI
#SBK_TYPESYSPATH := $(SBK_TYPESYSPATH):$(ZSQTLIB_DIR)/Make/ZSIpc
#SBK_TYPESYSPATH := $(SBK_TYPESYSPATH):$(ZSQTLIB_DIR)/Make/ZSIpcGUI
#SBK_TYPESYSPATH := $(SBK_TYPESYSPATH):$(ZSQTLIB_DIR)/Make/ZSTrace
#SBK_TYPESYSPATH := $(SBK_TYPESYSPATH):$(ZSQTLIB_DIR)/Make/ZSTraceGUI
#SBK_TYPESYSPATH := $(SBK_TYPESYSPATH):$(ZSQTLIB_DIR)/Make/ZSRemCmd
#SBK_TYPESYSPATH := $(SBK_TYPESYSPATH):$(ZSQTLIB_DIR)/Make/ZSRemCmdGUI
SBK_TYPESYSPATH := $(subst /,$(PATH_SEP),$(SBK_TYPESYSPATH))

# Resulting Command Line Options
#===============================

SBK_MODULE_HEADER_FILE := $(ZSQTLIB_DIR)/Make/$(MODULE)/$(MODULE)Binding4Python.h
SBK_MODULE_HEADER_FILE := $(subst /,$(PATH_SEP),$(SBK_MODULE_HEADER_FILE))

SBK_MODULE_TYPESYSTEM_FILE := $(ZSQTLIB_DIR)/Make/$(MODULE)/$(MODULE)Binding4Python.xml
SBK_MODULE_TYPESYSTEM_FILE := $(subst /,$(PATH_SEP),$(SBK_MODULE_TYPESYSTEM_FILE))

SBK_FLAGS := $(SBK_OPTIONS) --include-paths=$(SBK_INCPATH) --typesystem-paths=$(SBK_TYPESYSPATH) $(SBK_MODULE_HEADER_FILE) $(SBK_MODULE_TYPESYSTEM_FILE)
#SBK_FLAGS := $(SBK_OPTIONS) --typesystem-paths=$(SBK_TYPESYSPATH) $(SBK_MODULE_HEADER_FILE) $(SBK_MODULE_TYPESYSTEM_FILE)
export SBK_FLAGS


$(info )
$(info <--- $(MAKEFILE_PATH) )
$(info )
