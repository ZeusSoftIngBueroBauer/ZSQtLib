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
# Compiler options
#===============================================================================

DEFINE_OS :=
CFLAGS :=
INC_DIRS :=
DEFINES :=

# DEFINE_OS
#---------------------------------
# Range [_WINDOWS, __linux__]
# Preprocessor definition defining the operationg system as used in the
# CPP source and header files.
# This definition is used to generated the dependency files by the gcc compiler.
DEFINE_OS := __linux__

# Compile without linking
#---------------------------------
CFLAGS := $(CFLAGS) -c

# 
#---------------------------------
CFLAGS := $(CFLAGS) -pipe

# 
#---------------------------------
CFLAGS := $(CFLAGS) -O2

# Warning Level
#---------------------------------
CFLAGS := $(CFLAGS) -Wall
CFLAGS := $(CFLAGS) -W

# Generate position-independent code
#-----------------------------------
# -fpic    Generate position-independent code (PIC) suitable for use in
#          a shared library, if supported for the target machine.
#          Such code accesses all constant addresses through a global offset table (GOT).
#          The dynamic loader resolves the GOT entries when the program starts
#          (the dynamic loader is not part of GCC; it is part of the operating system).
#          If the GOT size for the linked executable exceeds a machine-specific maximum
#          size, you get an error message from the linker indicating that '-fpic' does
#          not work; in that case, recompile with `-fPIC' instead.
#          (These maximums are 16k on the m88k, 8k on the Sparc, and 32k on the m68k and
#          RS/6000. The 386 has no such limit.) 
#          Position-independent code requires special support, and therefore works only
#          on certain machines. For the 386, GCC supports PIC for System V but not for
#          the Sun 386i. Code generated for the IBM RS/6000 is always position-independent.
# -fPIC    If supported for the target machine, emit position-independent code, suitable
#          for dynamic linking and avoiding any limit on the size of the global offset
#          table. This option makes a difference on the m68k, m88k, and the Sparc.
#          Position-independent code requires special support, and therefore works only
#          on certain machines.
ifeq ($(CONFIG_TYPE), dll)
    CFLAGS := $(CFLAGS) -fPIC
endif

# Preprocessor Definitions
#=================================
DEFINES := -D__linux__ -D_ZSTRACE -D_REENTRANT
ifeq ($(CONFIG_TYPE), dll)
    DEFINES := $(DEFINES) -D$(strip $(MODULE_UPPCASE)DLL_EXPORTS)
endif
ifeq ($(CONFIG), Release)
    DEFINES := $(DEFINES) -DNDEBUG
else ifeq ($(CONFIG), Debug)
    DEFINES := $(DEFINES) -D_DEBUG
endif

# Additional Include Directories
#=================================
INC_DIRS := -I "$(ZSQTLIB_DIR)/Include" -I "$(QTDIR_INC)"


$(info )
$(info <--- $(MAKEFILE_PATH) )
