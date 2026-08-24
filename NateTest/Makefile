PROJECT_NAME := NateTest
TARGET_PATH := $(OUT_DIR)/NateTest$(EXE_EXT)

DEFINES := _CONSOLE
SOURCES := TestComplex.cpp TestExpr.cpp TestList.cpp TestLiterals.cpp TestOutput.cpp TestProgram.cpp TestVariables.cpp
EXTRA_DEPS := $(NATE_LIB) $(MATHPARSER_LIB)

ifeq ($(COMPILER),msvc)
GTEST_INCLUDE ?= $(GTEST_PKG_ROOT)/build/native/include
GTEST_LIB_DIR ?= $(GTEST_PKG_ROOT)/lib/native/v140/windesktop/msvcstl/static/rt-dyn/$(if $(filter x64,$(PLATFORM)),x64,x86)/$(CONFIG)
GTEST_LIB_SUFFIX := $(if $(filter Debug,$(CONFIG)),d,)
GTEST_LIB := $(GTEST_LIB_DIR)/gtest$(GTEST_LIB_SUFFIX).lib
GTEST_MAIN_LIB := $(GTEST_LIB_DIR)/gtest_main$(GTEST_LIB_SUFFIX).lib
INCLUDE_DIRS := ../NateLib ../NateLib/Stats ../utf8 ../MathParser $(GTEST_INCLUDE)
PROJECT_LDFLAGS := /WHOLEARCHIVE:"$(GTEST_MAIN_LIB)"
EXTRA_DEPS += $(GTEST_LIB) $(GTEST_MAIN_LIB)
LDLIBS := "$(NATE_LIB)" "$(MATHPARSER_LIB)" "$(GTEST_LIB)"
else
GTEST_INCLUDE ?=
GTEST_LIBS ?= -lgtest
INCLUDE_DIRS := ../NateLib ../NateLib/Stats ../utf8 ../MathParser $(GTEST_INCLUDE)
LDLIBS := "$(NATE_LIB)" "$(MATHPARSER_LIB)" $(GTEST_LIBS)
endif

include ../build/common.mk
