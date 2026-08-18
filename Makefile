ROOT_DIR := $(CURDIR)
CONFIG ?= Debug
.DEFAULT_GOAL := all

ifeq ($(OS),Windows_NT)
HOST_OS := windows
SHELL := cmd.exe
WINDOWS_ARCH := $(if $(PROCESSOR_ARCHITEW6432),$(PROCESSOR_ARCHITEW6432),$(PROCESSOR_ARCHITECTURE))
DETECTED_PLATFORM := $(if $(filter AMD64,$(WINDOWS_ARCH)),x64,$(if $(filter ARM64,$(WINDOWS_ARCH)),arm64,x86))
USER_HOME := $(subst \,/,$(USERPROFILE))
else
HOST_OS := $(shell uname -s | tr A-Z a-z)
DETECTED_PLATFORM := $(shell uname -m)
USER_HOME := $(HOME)
endif

PLATFORM ?= $(DETECTED_PLATFORM)
OUT_DIR := $(ROOT_DIR)/$(PLATFORM)/$(CONFIG)
OBJ_ROOT := $(ROOT_DIR)/.make/$(PLATFORM)/$(CONFIG)

ifeq ($(origin CXX),default)
ifeq ($(HOST_OS),windows)
CXX := cl
else
CXX := c++
endif
endif

CXX_NAME := $(notdir $(firstword $(CXX)))
ifneq ($(filter cl cl.exe clang-cl clang-cl.exe,$(CXX_NAME)),)
COMPILER := msvc
else
COMPILER := gnu
endif

ifeq ($(HOST_OS),windows)
ifeq ($(COMPILER),msvc)
MSVC_COMPILER_AVAILABLE := $(shell where $(CXX_NAME) >NUL 2>&1 && echo yes)
ifeq ($(MSVC_COMPILER_AVAILABLE),)
ifeq ($(MSVC_ENV_READY),)
NEEDS_MSVC_ENV := yes
VSWHERE_DIR := C:/Program Files (x86)/Microsoft Visual Studio/Installer
VSWHERE ?= $(VSWHERE_DIR)/vswhere.exe
VCVARS_ARCH := $(if $(filter arm64,$(PLATFORM)),amd64_arm64,$(PLATFORM))
endif
endif
endif
endif

ifeq ($(COMPILER),msvc)
ifeq ($(origin AR),default)
AR := lib
endif
EXE_EXT := .exe
LIB_PREFIX :=
STATIC_LIB_EXT := .lib
COMMON_CXXFLAGS := /nologo /EHsc /W3 /permissive- /FS
COMMON_ARFLAGS := /nologo
COMMON_LDFLAGS := /nologo
ifeq ($(CONFIG),Debug)
COMMON_CXXFLAGS += /MDd /Od /Zi
COMMON_DEFINES := _DEBUG
else
COMMON_CXXFLAGS += /MD /O2
COMMON_DEFINES := NDEBUG
endif
else
EXE_EXT := $(if $(filter windows,$(HOST_OS)),.exe,)
LIB_PREFIX := lib
STATIC_LIB_EXT := .a
COMMON_CXXFLAGS := -Wall -Wextra
COMMON_ARFLAGS := rcs
COMMON_LDFLAGS :=
ifeq ($(CONFIG),Debug)
COMMON_CXXFLAGS += -O0 -g
COMMON_DEFINES := _DEBUG
else
COMMON_CXXFLAGS += -O2
COMMON_DEFINES := NDEBUG
endif
endif

ifeq ($(HOST_OS),windows)
COMMON_DEFINES += OS_WIN WIN32
endif

CXX_STANDARD ?= c++20

REFLEX_ROOT ?= $(firstword $(wildcard $(ROOT_DIR)/../Reflex $(ROOT_DIR)/../RE-flex $(USER_HOME)/Reflex $(USER_HOME)/RE-flex))
REFLEX_INCLUDE ?= $(if $(REFLEX_ROOT),$(REFLEX_ROOT)/include)
REFLEX_LIB_ROOT ?= $(if $(REFLEX_ROOT),$(REFLEX_ROOT)/lib)
REFLEX_UNICODE_ROOT ?= $(if $(REFLEX_ROOT),$(REFLEX_ROOT)/unicode)
LOCAL_REFLEX := $(firstword $(wildcard $(REFLEX_ROOT)/bin/win64/Reflex.exe $(REFLEX_ROOT)/bin/reflex $(REFLEX_ROOT)/bin/reflex.exe))
LOCAL_BISON := $(firstword $(wildcard $(USER_HOME)/Bison/win_bison.exe $(USER_HOME)/Bison/bison $(USER_HOME)/Bison/bison.exe))
REFLEX ?= $(if $(REFLEX_EXE),$(REFLEX_EXE),$(if $(LOCAL_REFLEX),$(LOCAL_REFLEX),reflex))
BISON ?= $(if $(BISON_EXE),$(BISON_EXE),$(if $(LOCAL_BISON),$(LOCAL_BISON),bison))

GTEST_PKG_ROOT ?= $(ROOT_DIR)/packages/Microsoft.googletest.v140.windesktop.msvcstl.static.rt-dyn.1.8.1.3
GMOCK_PKG_ROOT ?= $(ROOT_DIR)/packages/googletestmock.v.141.1.0.3/build/native

NATE_LIB := $(OUT_DIR)/$(LIB_PREFIX)Nate$(STATIC_LIB_EXT)
MATHPARSER_LIB := $(OUT_DIR)/$(LIB_PREFIX)MathParser$(STATIC_LIB_EXT)

export ROOT_DIR CONFIG PLATFORM HOST_OS COMPILER OUT_DIR OBJ_ROOT
export CXX AR EXE_EXT LIB_PREFIX STATIC_LIB_EXT CXX_STANDARD
export COMMON_CXXFLAGS COMMON_ARFLAGS COMMON_LDFLAGS COMMON_DEFINES
export REFLEX_ROOT REFLEX_INCLUDE REFLEX_LIB_ROOT REFLEX_UNICODE_ROOT REFLEX BISON
export GTEST_PKG_ROOT GMOCK_PKG_ROOT NATE_LIB MATHPARSER_LIB

LIB_TARGETS := utf8 MathParser Nate
APP_TARGETS := Out NateCompiler
TEST_TARGETS := MathParser-Test NateTest
ALL_TARGETS := $(LIB_TARGETS) $(APP_TARGETS) $(TEST_TARGETS)

.PHONY: all libs apps tests clean env-check help $(ALL_TARGETS)

ifeq ($(NEEDS_MSVC_ENV),yes)
.PHONY: msvc-env-build

all libs apps tests env-check $(ALL_TARGETS): msvc-env-build

msvc-env-build:
	@if not exist "$(subst /,\,$(VSWHERE))" (echo Visual Studio locator not found. Set CXX or VSWHERE. & exit /B 1)
	@"$(subst /,\,$(ROOT_DIR))\build\msvc-env.cmd" "$(subst /,\,$(VSWHERE))" "$(VCVARS_ARCH)" "$(MAKE)" "$(if $(MAKECMDGOALS),$(MAKECMDGOALS),all)"
else
all: $(ALL_TARGETS)

libs: $(LIB_TARGETS)

apps: $(APP_TARGETS)

tests: $(TEST_TARGETS)

MathParser Nate: utf8
Nate: MathParser
Out NateCompiler: Nate MathParser
MathParser-Test: MathParser
NateTest: Nate MathParser

$(ALL_TARGETS):
	$(MAKE) -C $@

env-check:
ifeq ($(HOST_OS),windows)
	@$(CXX) --version 2>NUL || $(CXX) /? >NUL
	@$(AR) --version 2>NUL || $(AR) /? >NUL
else
	@command -v $(firstword $(CXX))
	@command -v $(firstword $(AR))
endif
endif

clean:
	$(MAKE) -C utf8 clean
	$(MAKE) -C MathParser clean
	$(MAKE) -C Nate clean
	$(MAKE) -C Out clean
	$(MAKE) -C NateCompiler clean
	$(MAKE) -C MathParser-Test clean
	$(MAKE) -C NateTest clean
ifeq ($(HOST_OS),windows)
	if exist "$(subst /,\,$(OBJ_ROOT))" rmdir /S /Q "$(subst /,\,$(OBJ_ROOT))"
else
	rm -rf "$(OBJ_ROOT)"
endif

help:
	@echo Available targets: all libs apps tests clean env-check
	@echo Example: make CONFIG=Debug CXX=clang++ all
	@echo Set REFLEX_ROOT and optionally REFLEX, BISON, GTEST_INCLUDE, and GTEST_LIBS.
