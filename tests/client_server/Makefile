# The TARGET variable determines what target system the application is
# compiled for. It either refers to an XN file in the source directories
# or a valid argument for the -target option when compiling
TARGET = XCORE-200-EXPLORER

# The APP_NAME variable determines the name of the final .xe file. It should
# not include the .xe postfix. If left blank the name will default to
# the project name
APP_NAME = 

# The USED_MODULES variable lists other module used by the application.
USED_MODULES = lib_logging lib_xcore_c

# The flags passed to xcc when building the application
# You can also set the following to override flags for a particular language:
# XCC_XC_FLAGS, XCC_C_FLAGS, XCC_ASM_FLAGS, XCC_CPP_FLAGS
# If the variable XCC_MAP_FLAGS is set it overrides the flags passed to
# xcc for the final link (mapping) stage.
FLAGS_COMMON = -g -O2 -save-temps -DDEBUG_PRINT_ENABLE=1
XCC_FLAGS_XS1 = $(FLAGS_COMMON)
XCC_FLAGS_XS2 = $(FLAGS_COMMON)

ifeq ($(CONFIG),XS1)
	TARGET = SLICEKIT-L16
endif

ifeq ($(CONFIG),XS2)
	TARGET = XCORE-200-EXPLORER
endif

# The VERBOSE variable, if set to 1, enables verbose output from the make
# system.
VERBOSE = 0

#=============================================================================
# The following part of the Makefile includes the common build infrastructure
# for compiling XMOS applications. You should not need to edit below here.

XMOS_MAKE_PATH ?= ../..
include $(XMOS_MAKE_PATH)/xcommon/module_xcommon/build/Makefile.common

#=============================================================================
# This depends on the common makefile
.PHONY: run
run: $(BIN_DIR)/$(APP_NAME).xe
	xsim --xscope "-offline xscope.xmt" $(BIN_DIR)/$(APP_NAME).xe
