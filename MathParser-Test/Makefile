PROJECT_NAME := MathParser-Test
TARGET_PATH := $(BIN_DIR)/MathParser-Test$(EXE_EXT)

DEFINES := _CONSOLE HAS_GOOGLETESTMOCK
SOURCES := main.cpp test.cpp
EXTRA_DEPS := $(MATHPARSER_LIB)

GMOCK_INCLUDE ?= $(GMOCK_PKG_ROOT)/include
GMOCK_LIB_DIR ?= $(GMOCK_PKG_ROOT)/lib/$(if $(filter x64,$(PLATFORM)),x64,Win32)/$(CONFIG)
INCLUDE_DIRS := ../MathParser ../utf8 $(GMOCK_INCLUDE)
LDLIBS := "$(MATHPARSER_LIB)" "$(GMOCK_LIB_DIR)/gmock.lib" "$(GMOCK_LIB_DIR)/gtest.lib"

include ../build/common.mk
